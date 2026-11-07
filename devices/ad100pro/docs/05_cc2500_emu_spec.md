# 05 · 虚拟 CC2500 芯片级规格（供 emu/cc2500.py 实现）

> 依据：AD100 Pro V1.2 固件 `firmware/AD100pro_V1.2.bin`（基址 0x08003000）反汇编；
> 本文每条结论后给证据地址。`/*?*/` = 未完全确认。
> 配套：docs/03_rf_protocol.md（协议层）、docs/04_ui_lcd.md（UI 层）。

---

## 0. 顶层结论（实现者速读）

固件把 CC2500 当**纯寄存器/命令 + 硬件包模式**器件使用：

- SPI 模式 0（CPOL=0/CPHA=0），8 位 MSB first。
- 每次传输 = 1 个地址字节 + N 个数据字节（N 可为 0 = 命令选通）。
- **只用过 9 个命令选通**：0x30 SRES、0x33 SCAL、0x34 SRX、0x35 STX、0x36 SIDLE、0x39 SFRX、0x3A SFTX、0x3B SWORRST（另外 0x33 多次）。
- **收包只靠 GDO0 中断**（IOCFG0=0x06 包收到），**不轮询** MARCSTATE/RXBYTES/PKTSTATUS。
- **发包不判定成功**：写 TX FIFO → STX → 固定延时 600 µs → SWORRST，不做任何回读。
- 唯一回读校验：初始化后读 FREQ2(0x0D) 期望 **0x5C**（0x08004B0A–0x08004B2C），失败重试 10 次（每次 10 ms）。
- 组包：PKTLEN=4、PKTCTRL0=0x00 ⇒ **固定 4 字节 payload、CRC 关闭、无地址/无白化**；
  RX FIFO 里**没有长度字节**（固定长度模式），读出的 4 字节就是 payload。

---

## 1. SPI 物理层（逐条还原）

### 1.1 引脚与电平

| 信号 | MCU 引脚 | 方向 | 初值/上下拉 | 证据 |
|---|---|---|---|---|
| CS | PC6 | 输出 | 空闲高（0x08008C8E 置 1） | 0x0800CE38/0x0800CE58 |
| SCK | PB13 | 输出 | 空闲低；MOVER 设为输出后先 BRR 清零 | 0x08008C78 @0x08008CBC |
| MOSI | PB12 | 输出 | 空闲低 | 0x08008C78 @0x08008CD?；0x0800CD7C |
| MISO | PB14 | 输入 | **下拉**（PUPDR=0b10，0x08008CD0 的 `r1 = GPIOB<<19 = 1<<29`） | 0x08008C78 |
| GDO0 | PA15 | 输入/EXTI15 | 包收到（IOCFG0=0x06） | IRQ7 处理 0x08005B0C 检查 PR bit15 |
| GDO2（/*?*/） | PA6 | 输入/EXTI6 | 默认配置（IOCFG2=0x29） | 0x08005B34 检查 PR bit6；0x0800ABE4 读 PA6 |
| RF 模块控制 /*?*/ | PC8 | 输出 | 初值 0 | 0x08008C78 末尾 |

> **注意**：MISO 是**下拉**不是上拉——所以 `while (MISO high)` 形式的“等 CHIP_RDY”循环在
> 从机不驱动时天然通过（0x0800CE38、0x0800CCFC）。若虚拟模型把 MISO 置 1，会**卡死在
> 0x0800CE46 / 0x0800CCFC 的轮询里**。空闲（无 inject）时 MISO 必须读 0。

### 1.2 写字节（`SPI_WriteByte` @0x0800CD7C）

函数入口：`r0 = byte`；长度固定 8 bit，**MSB first**。

| 步骤 | 动作 | 指令地址 |
|---|---|---|
| 1 | `GPIOB_BRR = 0x2000` → **SCK 拉低**（每个 bit 开始前都先拉低） | 0x0800CD8C |
| 2 | 测试 byte bit7：置 1 → `GPIOB_BSRR=0x1000`（**MOSI 高**）；否则 `GPIOB_BRR=0x1000`（MOSI 低） | 0x0800CD8E–0x0800CD96 |
| 3 | byte <<= 1（实际 `(b<<25)>>24`，只保留位序） | 0x0800CD98 |
| 4 | 3×nop + `bl 0x0800B874`（=4×nop + bx lr，短延时） | 0x0800CD9C–0x0800CDA2 |
| 5 | `GPIOB_BSRR = 0x2000` → **SCK 拉高** | 0x0800CDA6 |
| 6 | `bl 0x0800B874` 延时 | 0x0800CDA8 |
| 7 | 读 `GPIOB_IDR`，测试 **bit14 (MISO)**，值拼进累加器（状态字节，调用方几乎都丢弃） | 0x0800CDAC–0x0800CDB6 |
| 8 | 循环 8 次；结束后 `GPIOB_BRR=0x2000`（SCK 回低） | 0x0800CDB8–0x0800CDC0 |

⇒ **MOSI 在 SCK 低电平期间建立，SCK 上升沿采样**；**MISO 在 SCK 高电平（上升沿后）被采样**。
⇒ 从机应在**下降沿**（或 SCK 低电平期间）更新 MISO，在上升沿前保持有效 —— **SPI mode 0**。

### 1.3 读字节（`SPI_ReadByte` @0x0800CD40）

入口时 SCK 为低（上一字节结束状态）。每 bit：

| 步骤 | 动作 | 指令地址 |
|---|---|---|
| 1 | `GPIOB_BSRR = 0x2000` → **SCK 拉高** | 0x0800CD4C |
| 2 | nop + `bl 0x0800B874` 延时 | 0x0800CD4E–0x0800CD50 |
| 3 | 读 `GPIOB_IDR` bit14（MISO），累加器左移并入 | 0x0800CD56–0x0800CD5E |
| 4 | `GPIOB_BRR = 0x2000` → **SCK 拉低** | 0x0800CD60 |
| 5 | nop×3 + `bl 0x0800B874` 延时 | 0x0800CD62–0x0800CD68 |

⇒ 采样点仍在**上升沿之后**；从机必须在 SCK 低电平期间就准备好该 bit。

### 1.4 CS 时序与事务边界

- `CS_Low`（0x0800CE38）：`PC6=0` → **忙等直到 MISO==0**（`bmi` 循环，0x0800CE46）。
- `CS_High`（0x0800CE58）：`PC6=1`，无延时。
- 事务内 CS 一直保持低：地址字节 +（0/1/N 个数据字节）→ CS 高。
- 字节之间 SCK 回到低电平，无额外延时；SCK 的**每个上升沿恰好对应一个 bit**（共 8×字节数 个），
  虚拟模型按“上升沿计 bit、每 8 个 bit 为一字节”解码即可，不需要靠 CS 边界判字节。
- 半周期 ≈ 3 nop + `bl`(4 nop) ≈ 十几 cycle@48 MHz ≲ 0.5 µs ⇒ 实际 SCK ≲ 1–2 MHz /*?*/（不影响功能）。

### 1.5 复位脉冲（`0x0800CCBC`，上电序列）

```
PC6=1, delay 30µs
PC6=0, delay 30µs
PC6=1, delay 45µs
PC6=0                     ; 保持低
  等 MISO==0
  命令 0x30 (SRES)        ; 内部再走一遍 CS低→写1字节→CS高
  等 MISO==0
PC6=1
```
⇒ 虚拟模型：CS 的每次 1→0 都开始新事务，但要容忍“CS 已低时又一次写低”（不得重复计入事务）；
SRES 在 CS 上升沿生效，效果 = 寄存器恢复默认 + FIFO 清空 + state=IDLE + GDO0 拉低。

---

## 2. 命令/地址格式与 driver 实际用到的寄存器

### 2.1 地址字节

```
bit7 = R/W    1=读，0=写
bit6 = BURST  1=突发（多字节），0=单次
bit5..0 = 地址
```
| 地址 | 含义 | driver 用法 | 证据 |
|---|---|---|---|
| 0x00–0x2E | 配置寄存器 | 逐字节写（单次） | 0x0800CAE8（下表） |
| 0x30–0x3D | 命令选通 | 只写地址字节（无数据） | 见 §2.3 |
| 0x3E | PATABLE | **burst 写 0x7E**，len=1 | 0x0800CAE8 @0x0800CC6A–0x0800CC70 |
| 0x3F | TX FIFO（写 0x7F）/ RX FIFO（读 0xFF） | burst 写 4 / burst 读 4 | 0x0800CD24 / 0x0800CC98 |
| 0x80\|reg | 单寄存器读 | 仅 FREQ2/FREQ1 | 0x0800CCA6 |
| 0xFF | = 0xC0\|0x3F，burst 读 RX FIFO | len=4 | 0x0800CC98 |

### 2.2 初始化配置表（0x0800CAE8，按写入顺序 addr=value）

```
00=29 01=2E 02=06 03=01   ; IOCFG2/1/0, FIFOTHR
04=C3 05=68 06=04 07=00   ; SYNC1/0, PKTLEN=4, PKTCTRL1
08=00 09=00 0A=00 0B=07   ; PKTCTRL0=00(固定长/无CRC/无白化), ADDR, CHANNR, FSCTRL1
0C=00 0D=5C 0E=CE 0F=C4   ; FSCTRL0, FREQ2/1/0 = 2413.0 MHz (26MHz xtal)
10=1D 11=3B 12=73 13=22   ; MDMCFG4..1 = 250kbps, GFSK, 30/32 sync, 4B preamble
14=F8 15=00 16=07 17=30   ; MDMCFG0, DEVIATN=0(/*?*/), MCSM2, MCSM1=0x30
18=08 19=1D 1A=1C 1B=C7   ; MCSM0=0x08(手动校准), FOCCFG, BSCFG, AGCCTRL2
1C=00 1D=B2 1E=6B 1F=87   ; AGCCTRL1/0, WOREVT1/0
20=F8 21=B6 22=10 23=EA   ; WORCTRL, FREND1/0, FSCAL3=EA
24=0A 25=00 26=11 27=41   ; FSCAL2..0, 0x27=41
28=00 29=59 2A=7F 2B=3F   ; 校准/测试组（名称 /*?*/）
2C=88 2D=31 2E=0B         ; TEST2/1/0 = CC2500 标准魔数
PATABLE burst(0x7E) = FE  ; 0x0800CAE8 结尾（随后 SCAL + SFRX，各延时约 1ms）
```
> 实现建议：**照抄即可**（driver 初始化时全量重写，所以默认值只要不影响读回校验）。
> 事后还会单独写：0x0D/0x0E/0x0F（信道设置 0x0800B718）、0x04/0x05（sync=ID 0x0800B758）、
> 0x06=0x04（PKTLEN 重写：0x0800AA74 特殊路径、0x0800C7D4）。

### 2.3 命令选通（driver 实际使用集合）

| 命令 | 名称 | 写入点 | 期望行为 |
|---|---|---|---|
| 0x30 | SRES | 0x0800CCBC | 软复位：寄存器默认、FIFO 清空、IDLE、GDO0 低 |
| 0x33 | SCAL | 0x0800CAE8、0x0800B718、0x0800B758、0x0800AB7C、0x0800ABE4、0x0800C7D4 | 校准；虚拟模型可忽略（保持/回到 IDLE 即可） |
| 0x34 | SRX | 0x0800AA74、0x0800AB7C、0x0800C7D4 | 进入 RX，允许注入收包 |
| 0x35 | STX | 0x0800C204 | 发送 TX FIFO 中 PKTLEN 字节；随后 TXOFF_MODE→IDLE |
| 0x36 | SIDLE | 0x08004AF0、0x0800AB7C、0x0800ABE4、0x0800C7D4 | 回 IDLE |
| 0x39 | SFRX | 0x0800CAE8、0x0800AB7C（else 分支） | **清 RX FIFO / 清 RXBYTES / GDO0 低** |
| 0x3A | SFTX | 0x0800AA74、0x0800AB7C、0x0800C7D4 | 清 TX FIFO |
| 0x3B | SWORRST | 0x0800C204（STX 后 600µs） | 复位 WOR 定时器；可视为无操作 |
| 0x31/0x32/0x38/0x3C/0x3D | SFSTXON/SXOFF/SPWD/SWOR/SNOP | **未使用** | 可忽略 |

> 命名提醒：CC2500 的 0x38=SPWD、0x39=SFRX、0x3A=SFTX、0x3B=SWORRST、0x3C=SWOR、
> 0x3D=SNOP。不要把 0x39/0x3A/0x3B 错位（见附录 A 的 review）。

---

## 3. 读取/轮询

**固件只做一处寄存器回读校验**（0x08004AF0 @0x08004B0A–0x08004B2C）：

```c
for (i = 0; i <= 10; i++) {
    buf[0] = read_reg(0x0D);     // FREQ2
    buf[1] = read_reg(0x0E);     // FREQ1
    delay_ms(10);
    if (buf[0] == 0x5C) break;   // 期望 0x5C
}
```
⇒ 虚拟芯片 **必须** 让 `读 0x0D` 返回 0x5C（写入后回读也不难），否则初始化多耗约 100 ms（不会死，但会拖慢启动）。

**没有任何** PARTNUM(0x30)/VERSION(0x31)/MARCSTATE(0x35)/RXBYTES(0x3B)/TXBYTES(0x3A)/
PKTSTATUS(0x38)/RSSI(0x34)/LQI(0x33) 的读取 / 轮询（全局仅 0x0800CCA6 两处调用，参数 0x0D/0x0E）。
所以这些寄存器只为“可观测性”实现即可。若实现读回，建议值（CC2500 datasheet，忽略亦可）：

| 寄存器 | 建议值 | 说明 |
|---|---|---|
| 0x30 PARTNUM | 0x80 | CC2500（driver 不读，可选） |
| 0x31 VERSION | 0x03 | /*?*/ 版本随批次，driver 不读 |
| 0x35 MARCSTATE | IDLE=0x01 / RX=0x0D / TX=0x13 | 仅状态字节与调试用 |
| 0x3A TXBYTES | bit7=TXFIFO_UNDERFLOW, bit6=TXFIFO_OVERFLOW, bit5..0=字节数 | 可选 |
| 0x3B RXBYTES | bit7=CRC_OK, bit6=RXFIFO_OVERFLOW, bit5..0=字节数 | CRC 关闭，bit6 有数据=满时 1 /*?*/ |
| 0x38 PKTSTATUS | 无意义（不读） | 可选 |

> 状态字节（地址字节期间 SO 输出）：bit7=CHIP_RDYn（**0=就绪**），bit6..4=STATE(3bit)，
> bit3..0=FIFO_BYTES_AVAILABLE。固件在 CS 低后用 GPIO 直接读 MISO 等低（不是靠 SPI 状态字节），
> 所以只要 MISO 空闲=0 即可；但状态字节按规范实现（bit7=0）更稳。

---

## 4. 收包流程

### 4.1 RX FIFO 数据格式

- 固定包长模式（PKTCTRL0=0x00, PKTLEN=4）⇒ RX FIFO = **4 字节 payload，无长度字节**。
- CRC 关闭（PKTCTRL0.CRC_EN=0）⇒ **无 2 字节 CRC 状态**；driver 也没追加 RSSI/LQI 读取
  （0x0800CC98 固定 len=4）。
- 顺序：先收到的字节先出（payload[0] 先读）。

### 4.2 GDO0

| 项 | 值 | 证据 |
|---|---|---|
| 配置寄存器 | IOCFG0 = 0x02 | 0x0800CAE8 @0x0800CAFC |
| 功能值 | **0x06 = 包收到（PKT_RECEIVED）** | 同上 |
| 极性 | 非反相（bit6=0），**收到包 → 高电平**；RX FIFO 读空 / 离开 RX → 低 | CC2500 语义 |
| 连到 MCU | **PA15 → EXTI15（上升沿）→ NVIC IRQ7** | IRQ7 处理 0x08005B0C @0x08005B14 检查 PR bit15；无 EXTICR 写⇒端口 A |
| 中断处理 | 0x08005B0C：若 PR bit15 且 `[0x2000017C]==1` 且 `[0x200001D4]==0` → 调 **0x0800AA74**；末尾 `0x080072AC(0x8000)` 清 PR | 0x08005B18–0x08005B2E |

### 4.3 固件收包入口（0x0800AA74）

```c
void rf_rx_isr(void) {                     /* 0x0800AA74 */
    if (0x20000185 ∈ {2,4,5,7} && [0x200001B2]==0x1A) { ...特殊路径... }  /*?*/
    CC2500_ReadRxFifo((uint8_t*)0x20000214, 4);   /* 0x0800CC98，burst read 0xFF */
    switch (pkt[0]) {
    case 0xD5: ...命令分发（0x0800AAC8 起）...
    case 0xA9: rf_property_handler(pkt[1], pkt[2], pkt[3]);  /* 0x0800AFC4 */
    default: break;
    }
    CC2500_WriteCmd(0x3A);   /* SFTX */
    CC2500_WriteCmd(0x34);   /* SRX 重新进入接收 */
}
```
- 包缓冲固定 **RAM 0x20000214**（4 字节），后续解析全部读该地址。
- **注入时机要求**：在固件处于 RX（已执行过 SRX，`[0x2000017C]==1`、`[0x200001D4]==0`）时注入；
  虚拟模型置 GDO0=1 → 触发 EXTI15/IRQ7 → 固件读 4 字节；读空后 GDO0 应回 0，
  否则下一次收包不会产生新的上升沿（EXTI 是边沿触发）。

---

## 5. 发包流程

```c
/* 0x0800C204：发送 4 字节包 */
0x0800A6CC();                              /* 射频电源/使能（GPIO） */
CC2500_WriteTxFifo(pkt, 4);                /* 0x0800CD24 → burst write 0x7F */
CC2500_WriteCmd(0x35);                     /* STX */
delay_us(0x4B<<3 = 600);                   /* 0x0800CAB8(600) */
CC2500_WriteCmd(0x3B);                     /* SWORRST（收尾，可忽略） */
0x0800A6A4();                              /* 射频电源/使能恢复 */
```
- TX FIFO 写入点：`burst write 0x7F`，长度 4 字节（= PKTLEN）。
- STX 后 **600 µs** 固定延时就认为发送完成；**不读 TXBYTES、不查 GDO2、不等 PKT_SENT**。
- 虚拟模型在 STX 时抓取 `tx_fifo`（应校验长度 == 4，否则 TX FIFO underflow /*?*/），
  供测试断言（Godox 报文：`A9/ D5 ...`）。
- TX 后芯片状态：MCSM1=0x30 的 TXOFF_MODE=00 ⇒ 回 IDLE；固件靠定时器（0x0800C7D4，
  `[0x200001DE]` 倒计时）重新发 SRX 回到接收。虚拟模型可以直接把 state 置回 IDLE，不影响功能。

---

## 6. 注入建议（测试包）

> 前置：固件已跑过 RF 初始化（0x08004AF0 → SRX），并在主循环中。注入后观察
> `emu.cc2500.status()` / 0x20000214 / 相关 RAM 变化或 UI 渲染。

### 6.1 命令帧：`D5 80 00 00`（清模式位）
| 字节 | 含义 | 期望反应 |
|---|---|---|
| 0xD5 | 命令帧类型 | 进入命令分发（0x0800AAC8） |
| 0x80 | cmd=0x80 | `[0x20000480+0]=0`（0x0800AACC→0x0800AB32） |
| 0x00,0x00 | 未用 | 忽略 |
- 观测点：RAM 0x20000480 首字节 = 0。
- 同族：`D5 81 00 00` → 该字节 = 1；`D5 09 00 00` → `[0x200001B2] |= 0x0A`、
  `[0x200001C4] = 100`（0x0800AAF8 起）；`D5 21 00 00` → 清 0x200001AF/1B3/1B0/1AC
  （0x0800AB2C→0x08009BCC）；`D5 29 00 00` → 0x0800B344（GPIO 脉冲约 0.9 s，注意会阻塞）。

### 6.2 属性帧：`A9 50 ?? ??`（全局组属性）
| 字节 | 含义 | 期望反应 |
|---|---|---|
| 0xA9 | 属性帧类型 | 进入 0x0800AFC4 |
| 0x50 | 组 = 全部（也接受 9+本机 C.Fn 组号 `[0x2000017D]+9`） | 通过组过滤（失败则直接返回） |
| key | **键值 ≥ 0xB0**；实际键 = key-0xB0 走 0x0800AFEC 跳表 | 落到 0x0800B014 起的各 case |
| value | 参数 | 视 case（功率/模式/参数写 0x2000019C / 0x20000480 等） |
- 推荐先注入 `A9 50 B4 02`：这是固件自己配对时发送的同一帧（0x0800AA54(0x50,0xB4,0x02)），
  用来验证 0xA9 通路；具体 UI 变化需对照 case 体（映射部分 /*?*/，见 docs/03 §9）。
- key 不在 0xB0.. 范围或组不匹配 ⇒ **无副作用**（安全包，可用于“通路是否通”的探针）。

### 6.3 长帧/边界用例
- `D5 00 00 00`（未知命令）→ 只走默认分支，仍会重新 SRX。
- 4 字节 `00 00 00 00` → 默认分支，无副作用。
- 若测试“背靠背收包”：必须等固件执行完一次 0x0800AA74（读空 FIFO、GDO0 回低、重新 SRX）
  后再注入下一包，否则可能被 SFTX/SFRX 或边沿逻辑吞掉。

---

## 7. 虚拟 CC2500 实现检查表（10–20 条，逐条验收）

1. **SPI mode 0**：SCK 空闲低；每 8 个**上升沿**组成一字节；MSB first。`0x0800CD7C`/`0x0800CD40`。
2. CS 由 PC6 驱动：1→0 开始事务，0→1 结束；**CS 已低时再次写低不重复开始事务**（复位序列 0x0800CCBC 会这样）。
3. MISO 空闲（无驱动）读 **0**（MCU 侧 PB14 下拉）；驱动 MISO 期间按 bit 输出，事务结束释放。
4. 状态字节：bit7=CHIP_RDYn=**0**（就绪），bit6..4=STATE，bit3..0=FIFO_BYTES_AVAILABLE；在地址字节期间输出。
5. 地址解码：bit7=读，bit6=burst，bit5..0=地址；0x3E/0x3F 分别是 PATABLE/TX/RX FIFO。
6. 配置寄存器 0x00–0x2E 写后能正确回读；**读 0x0D 必须返回 0x5C**（0x08004B22 校验）。
7. PATABLE：`burst write 0x7E` len=1 → patable[0]=value（初始化写 0xFE；运行时按功率表写）。
8. 命令选通（仅地址字节、无数据字节）：SRES/SRX/STX/SIDLE/SFRX/SFTX 生效；SWORRST/SCAL 可视为无操作/回 IDLE。
9. **命令值映射不得错位**：0x38=SPWD、0x39=**SFRX**、0x3A=**SFTX**、0x3B=SWORRST、0x3D=SNOP。
10. SRES（0x30）：寄存器回默认、TX/RX FIFO 清空、state=IDLE、**GDO0=0 且清 PA15**。
11. SFRX（0x39）：清 RX FIFO、清 RXBYTES、GDO0 回 0。
12. SFTX（0x3A）：清 TX FIFO（不动 RX FIFO）。
13. SRX（0x34）：state=RX；此后注入的包才应产生 GDO0。
14. STX（0x35）：把 TX FIFO 中 4 字节作为一包记录（tx_packets），清 TX FIFO，state 回 IDLE（或短暂 TX 也可）。
15. 收包注入：RX FIFO = **payload 4 字节（无长度字节/无 CRC）**；GDO0 **0→1 上升沿**；触发 EXTI15 + NVIC IRQ7。
16. GDO0 在 RX FIFO 被读空后回 **0**（保证下一包有新的上升沿）。
17. EXTI/NVIC 触发必须走 core 的 **`emu.request_irq(16+7)`**（IRQ7=exception 23）或置 `pending_gdo0`；
    只写 `exti.pending/scb.pending` 不会让 run loop 进入中断（见附录 A-1）。
18. 注入的 4 字节落到固件 RAM **0x20000214**，随后 0xD5/0xA9 分发能跑通（用 §6 包验证）。
19. 发包观测：固件每发一包 = TX FIFO 4 字节 + STX；能抓到 `A9 50 B4 02`（配对）等。
20. 复位后不要卡住固件：`MISO==0` 恒成立；FREQ2 回读 0x5C 后 0x08004AF0 不再重试。

---

## 附录 A：对现有 `emu/cc2500.py` 的实现审查（供 Lead 修）

> 已通读 `emu/cc2500.py`（lead 版本）与 `emu/core.py` 的 GPIO/IRQ 接口。以下按严重程度排序。

- **A-1（阻断）GDO0 不会真正触发 IRQ7**：core 的 run loop 用队列派发中断
  （`core.py:_dispatch_due_irq` → `_irq_queue` → `_try_enter_irq`），只有 `request_irq(16+7)` 入队才会进入
  handler。`cc2500._set_gdo0`/`_trigger_exti15` 只写 `emu.exti.pending`、`emu.scb.pending`，**没入队**，
  所以注入收包后 0x0800AA74 不会执行。修法：
  ```python
  self.pending_gdo0 = True            # 供 core._poll_cc2500 消费
  self.emu.request_irq(16 + 7, source='GDO0')
  self.emu.exti.pending |= (1 << 15)  # handler 会读 EXTI PR bit15
  ```
  （`_poll_cc2500` 里已有正确实现，但要求 `pending_gdo0` 为真。）
- **A-2（中）命令选通常量错位**：0x38 被标成 ST_SWOR、0x39 标成 ST_SPWD、0x3A 标成 ST_SFRX、0x3B 标成 ST_SFTX。
  实际行为影响：固件的 **SFRX(0x39) 不清 RX FIFO**、**SFTX(0x3A) 反而清 RX FIFO**。
  后果：连续注入/背靠背收包时下一包可能被 0x0800AA74 末尾的 SFTX 清掉；初始化 0x0800CAE8
  末尾的 SFRX 也无法清掉旧包。请改为 0x39=SFRX、0x3A=SFTX、0x3B=SWORRST。
- **A-3（低）状态字节 bit7 反了**：`_status_byte()` 返回 `0x80 | ...` ⇒ CHIP_RDYn=1（未就绪）。
  固件只用 GPIO 轮询 MISO（下拉=0）不会死，但状态字节应在就绪时 bit7=0；STATE 字段应取
  `marcstate & 7` 放 bits6..4。建议 `(state & 7) << 4 | (avail & 0xF)`。
- **A-4（低）SRES 的 reset() 会清掉日志/统计**：`reset()` 重建 `tx_packets/spi_writes/log`，SRES 后调试信息丢失。
  建议 reset 时保留注入历史（至少 `tx_packets` 与 log）。
- **A-5（低）`_begin_transaction` 未主动驱动 MISO=0**：当前依赖 MCU 侧下拉寄存器恰好为 0；
  为稳健起见 CS 1→0 时应 `self._set_miso(0)`（模拟 CHIP_RDYn=0）。
- **A-6（提示）core.py 对 `cc2500.on_pins` 调用了两次**（`on_gpio_pins` 内两段重复）：
  由于 `on_pins` 用 `new != self.cs` 去重，功能无碍，但建议删掉重复段减少混淆。
- **A-7（提示）`inject_rx(append_status=True)` 与固件不符**：driver 固定读 4 字节，
  多出的 RSSI/LQI 会滞留在 FIFO 使 GDO0/可用字节数异常；测试注入请保持默认 `False`。
- 其余（地址解码 0x3F/0x7F/0xFF、burst 写 FIFO、读回 FREQ2、PATABLE burst、注入 API）实现正确，可直接复用。

---

## 附录 B：待确认

1. 状态字节 STATE 字段的精确取值（可按 MARCSTATE&7 近似，固件不读）。
2. GDO2=PA6 的真实信号（IOCFG2=0x29 默认值；EXTI6 处理只做 50µs 延时+读 PA6+状态机，疑为 CHIP_RDYn/其它 /*?*/）。
3. EXTI15 的边沿配置写入点（全固件无 EXTICR 写；EXTI IMR/RTSR/FTSR 由 0x080072C4 系列配置，具体哪次调用使能线 15 未逐一确认，但 handler 行为明确要求“上升沿”）。
4. 0x0800A6CC/0x0800A6A4 的 GPIO 具体端口位（TX 前后电源/偏置切换，/*?*/）。
