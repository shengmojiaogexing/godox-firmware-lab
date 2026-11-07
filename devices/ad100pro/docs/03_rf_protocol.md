# 03 · 2.4G 无线协议逆向（AD100 Pro V1.2）

> 固件 `firmware/AD100pro_V1.2.bin`，加载基址 **0x08003000**，应用 0x08003000–0x0800F448。
> 工具：`tools\python\python.exe`（capstone 5.0.7，需 `CS_MODE_MCLASS` 才能解 MSR/PRIMASK）。
> 本文所有地址均为固件内绝对地址；`/*?*/` 表示证据不足、待确认。

---

## 1. 结论速览

| 项目 | 结论 | 关键地址/证据 |
|---|---|---|
| 射频芯片 | **TI CC2500 兼容 2.4 GHz 收发器**（寄存器组、TEST2/1/0 魔数、PATABLE、命令选通完全吻合） | 初始化表 0x0800CAE8；TEST2/1/0 = 0x88/0x31/0x0B |
| 接口 | **4 线软件 SPI（bit-bang）**：CS=**PC6**，SCLK=**PB13**，MOSI=**PB12**，MISO=**PB14**（上拉输入） | GPIO 初始化 0x08008C78；读写 0x0800CD7C/0x0800CD40 |
| 中断 | **GDO0 = PA15 = EXTI15** → IRQ7（EXTI4_15）0x08005B0C → 收包 0x0800AA74 | 0x08005B14 `lsls r0,#0x10`（PR bit15）；全固件无 EXTICR 写，EXTI 全为端口 A |
| 另一 GDO | **PA6 = EXTI6** → 同一 IRQ7 处理（TX/状态用途 /*?*/） | 0x08005B34 `lsls r0,#0x19`；0x0800ABE4 读 PA6 |
| 调制/速率 | GFSK，**250 kbps**，载波 26 MHz 晶振 | MDMCFG4=0x1D,MDMCFG3=0x3B（250kbps 精确值）；MDMCFG2=0x73 |
| 信道 | **32 个**，2413.0–2464.5 MHz，逐信道写 FREQ2/1/0；CHANNR 恒 0 | 表 0x0800DAAC（FREQ2[32]）/0x0800DACC（FREQ1[32]）/0x0800DAEC（FREQ0[32]）；设置函数 0x0800B718 |
| 同步字/ID | 16 位 sync word = **50024 / (ID+1)**（ID 0–99） | 0x0800B758；除法 0x080030F0 |
| 包格式 | CC2500 硬件包模式：**固定 4 字节 payload，无 CRC、无地址、无变长** | PKTCTRL0=0x00, PKTLEN=0x04（0x0800CAE8 写 0x06=4、0x08=0） |
| 包类型 | payload[0]=**0xD5** 命令帧（遥控器→灯）/ **0xA9** 属性帧 | 收包分发 0x0800AA74 |
| 属性帧 | `[0xA9][组号: 0x50=全部, 0x09+组][键值≥0xB0][参数]` | 发送 0x0800AA54；解析 0x0800AFC4 |
| 命令帧 | `[0xD5][cmd][arg0][arg1]`，cmd ∈ {0x09,0x19,0x21,0x29,0x80,0x81,0x88,0x89} | 0x0800AA74 @0x0800AAC8；属性/命令处理 0x0800AFC4 |
| 功率 | PATABLE(0x3E) 由 18 字节表 0x0800DA9A 按档位索引写入 | 0x08004AF0 @0x08004B34–0x08004B40 |
| 配对/扫描 | 0x0800ABE4 循环发送 `A9 50 B4 02` 并轮询 PA6，之后进入收包 | 0x0800ABE4；0x0800B9C0 启动序列 |
| 设置存储 | flash 0x0801F000 起若干 100 字节页，页首字节校验 0x5C；ID/信道/功率来自该区 | 0x0800AD44；拷贝 0x0800CE6C |

> 与用户仓库 `Godox-S3-Trigger\components\godox_protocol` 的关系：**同一套空中协议**，
> 但用户实现的是“比特级/波形级”模型（10/12 字节帧），AD100 Pro 端是 **CC2500 硬件包模式**：
> - 用户模型 `godox_short_frame()`：`AA AA AA AA | IDhi IDlo IDhi IDlo | D5 | cmd`
> - 本固件实测：CC2500 自动发 4 字节前导（0xAA 图案）+ 16 位 sync(=编码 ID) + **4 字节 payload**。
>   payload 起始字节正是 **0xD5 / 0xA9**，与用户模型第 8 字节一致；用户模型中的“4×0xAA + ID×2”对应
>   空中的“前导 + 同步字（其观察帧把 sync/前导展开成了字节）”，不是 payload。
> - 用户 `godox_encode_id()`：`50024/(id+1)` —— **与固件 0x0800B758 完全一致**（交叉验证成立）。

---

## 2. CC2500 软件 SPI 驱动（地址与实现）

### 2.1 引脚

| 信号 | MCU 引脚 | 方向 | 证据 |
|---|---|---|---|
| CS（片选） | **PC6** | 输出，低有效 | 0x0800CE38/0x0800CE58 用 `GPIOC(0x48000800)` 的 bit6 |
| SCLK | **PB13** | 输出 | 0x0800CD7C 用 `GPIOB(0x48000400)` bit13（0x2000）做 BSRR/BRR |
| MOSI（SI） | **PB12** | 输出 | 0x0800CD7C 用 bit12（0x1000） |
| MISO（SO） | **PB14** | 输入+**下拉**（PUPDR=10，空闲读 0） | 0x0800CD40 读 GPIOB_IDR bit14（`lsls #0x11`）；0x08008C78 设 PUPDR=01 |
| GDO0 | PA15 | EXTI15 输入 | IRQ7 处理 0x08005B0C 检查 PR bit15；无 EXTICR ⇒ 端口 A |
| GDO2 | PA6 | EXTI6/查询输入 | 0x08005B0C 检查 PR bit6；0x0800ABE4 直接读 PA6 |
| 射频模块控制 | PC8（输出，初值 0）/*?*/ | 输出 | 0x08008C78 末尾配置 PC8=输出并置 0 |

GPIO 初始化函数：**0x08008C78**（`RF_GPIO_Init`，被 0x08004AF0 首先调用）。

### 2.2 底层函数

| 地址 | 功能 |
|---|---|
| 0x0800CE38 | `CS_Low()`：PC6=0，然后等待 MISO(PB14)==0 |
| 0x0800CE58 | `CS_High()`：PC6=1 |
| 0x0800CD7C | `SPI_WriteByte(b)`：8 位 MSB first；每位先 SCLK↓、置 MOSI、SCLK↑、延时、SCLK↓，并同时读回 MISO 拼字节 |
| 0x0800CD40 | `SPI_ReadByte()`：8 位 MSB first，SCLK 打 8 个脉冲采样 MISO |
| 0x0800CDF4 | `CC2500_WriteCmd(cmd)`：CS 低→写 1 字节命令→CS 高 |
| 0x0800CD32 | `CC2500_WriteReg(reg, val)`：内部即 `0x0800CE0C(reg, &val, 1)`（写单寄存器） |
| 0x0800CCA6 | `CC2500_ReadReg(reg)`：`0x0800CDC8(0x80|reg, &v, 1)`（读单寄存器，返回状态字节被丢弃，返回值为数据 /*?*/ 实际返回 r0=地址字节时的状态 /*?*/） |
| 0x0800CDC8 | `CC2500_BurstRead(addr, buf, len)`：CS 低→写地址→循环读 len 字节→CS 高 |
| 0x0800CE0C | `CC2500_BurstWrite(addr, buf, len)`：CS 低→写地址→循环写 len 字节→CS 高 |
| 0x0800CD24 | `CC2500_WriteTxFifo(buf, len)`：`BurstWrite(0x7F, ...)`（0x7F=0x40|0x3F，TX FIFO 突发写） |
| 0x0800CC98 | `CC2500_ReadRxFifo(buf, len)`：`BurstRead(0xFF, ...)`（0xFF=0xC0|0x3F，RX FIFO 突发读） |
| 0x0800CCBC | 上电/复位序列：PC6 脉冲→等 MISO 低→`SRES(0x30)`→等 MISO 低→CS 高 |
| 0x0800CA38 / 0x0800CAB8 / 0x0800CA68 | 短延时（n×约 1 µs，循环 15 个 nop） |
| 0x0800CA98 | 毫秒延时（内部 n × `0x0800CA38(990)`） |
| 0x0800CAE8 | `CC2500_Init()`：42 个寄存器写表 + PATABLE + SCAL + SFRX |
| 0x08004AF0 | `RF_Init()`：GPIO 初始化→延时 20 ms→复位→配置→读回 FREQ2 校验 0x5C→写功率→SCAL→设置信道→进入收包 |

### 2.3 `CC2500_Init()`（0x0800CAE8）寄存器表

| 地址 | 寄存器（CC2500） | 值 | 说明 |
|---|---|---|---|
| 0x00 | IOCFG2 | 0x29 | GDO2 配置（/*?*/ 具体信号名待确认） |
| 0x01 | IOCFG1 | 0x2E | GDO1 配置（/*?*/） |
| 0x02 | IOCFG0 | **0x06** | GDO0 = 包收到（Packet received），即 EXTI15 中断源 |
| 0x03 | FIFOTHR | 0x01 | RX FIFO 阈值 |
| 0x04 | SYNC1 | **0xC3** | 同步字高字节；ID=0 时 sync=0xC368=50024 |
| 0x05 | SYNC0 | **0x68** | 同步字低字节；运行时由 ID 更新（见 §4） |
| 0x06 | PKTLEN | **0x04** | 固定 4 字节 payload |
| 0x07 | PKTCTRL1 | 0x00 | 无地址/无附加状态 |
| 0x08 | PKTCTRL0 | **0x00** | 固定长度、**CRC 关闭**、无白化 |
| 0x09 | ADDR | 0x00 | |
| 0x0A | CHANNR | 0x00 | 通道号不用（直接写 FREQ） |
| 0x0B | FSCTRL1 | 0x07 | 中频 |
| 0x0C | FSCTRL0 | 0x00 | |
| 0x0D | FREQ2 | **0x5C** | 2413.0 MHz（26 MHz 晶振） |
| 0x0E | FREQ1 | **0xCE** | |
| 0x0F | FREQ0 | **0xC4** | |
| 0x10 | MDMCFG4 | 0x1D | DRATE_E=13；通道带宽 |
| 0x11 | MDMCFG3 | 0x3B | DRATE_M=59 ⇒ **250 kbps** |
| 0x12 | MDMCFG2 | 0x73 | GFSK，30/32 同步，无 Manchester |
| 0x13 | MDMCFG1 | 0x22 | 4 字节前导，CHANSPC_E=2 |
| 0x14 | MDMCFG0 | 0xF8 | CHANSPC_M=248 ⇒ 200 kHz（CHANNR 未用） |
| 0x15 | DEVIATN | 0x00 | 实测值；按 CC2500 公式频偏≈1.6 kHz，可疑 /*?*/（可能依赖 FSCAL 或厂商自定） |
| 0x16 | MCSM2 | 0x07 | |
| 0x17 | MCSM1 | 0x30 | TXOFF_MODE=IDLE, RXOFF_MODE=RX |
| 0x18 | MCSM0 | 0x08 | 手动校准 |
| 0x19 | FOCCFG | 0x1D | |
| 0x1A | BSCFG | 0x1C | |
| 0x1B | AGCCTRL2 | 0xC7 | |
| 0x1C | AGCCTRL1 | 0x00 | |
| 0x1D | AGCCTRL0 | 0xB2 | |
| 0x1E | WOREVT1 | 0x6B | |
| 0x1F | WOREVT0 | 0x87 | |
| 0x20 | WORCTRL | 0xF8 | |
| 0x21 | FREND1 | 0xB6 | |
| 0x22 | FREND0 | 0x10 | |
| 0x23 | FSCAL3 | 0xEA | 与 SmartRF 250 kbps 推荐值一致 |
| 0x24 | FSCAL2 | 0x0A | |
| 0x25 | FSCAL1 | 0x00 | |
| 0x26 | FSCAL0 | 0x11 | |
| 0x27–0x2B | RCCTRL/TEST 类（/*?*/） | 0x41,0x00,0x59,0x7F,0x3F | 与 CC2500 校准寄存器常用值吻合 |
| 0x2C | TEST2 | **0x88** | CC2500 魔数 |
| 0x2D | TEST1 | **0x31** | |
| 0x2E | TEST0 | **0x0B** | |
| — | PATABLE 0x3E | 表 0x0800DA9A[idx] | 见 §6 |
| — | 命令 0x33 | SCAL（校准）后延时 1 ms |
| — | 命令 0x39 | SFRX（清 RX FIFO）后延时 1 ms |

> 判据：0x2C/0x2D/0x2E = 0x88/0x31/0x0B 是 CC2500 数据手册的标准 TEST 魔数（CC1101 不是这组），
> 且 FREQ 换算出的 2413.0 MHz 与 Godox X 系统 CH1 一致（26 MHz 晶振）。

### 2.4 复位/启动序列（0x0800CCBC）

```
CS 脉冲(PC6 高→低→高)              // 0x08007426 写 PC6
等 MISO 低（PB14==0，读 GPIOB_IDR）
CS 低 → 写 0x30(SRES) → 等 MISO 低 → CS 高
```
`RF_Init()`（0x08004AF0）流程：
```
0x08008C78()                       // GPIO 初始化
delay_us(20000)                    // 0x0800CA38(0x4E20)
0x0800CCBC()                       // 硬复位
0x0800CAE8()                       // 寄存器配置 + SCAL/SFRX
读 FREQ2(0x0D)/FREQ1(0x0E) 到 0x20000214，最多 10 次等 FREQ2==0x5C  // 校验 SPI 通路
SIDLE(0x36)
PATABLE = 0x0800DA9A[power_idx]    // power_idx = [0x2000020E]
SCAL(0x33); delay 1 ms
0x0800B758([0x20000480+5])         // 按 ID 设置 sync word
0x0800AB7C()                       // 设置信道并进入 RX
```

---

## 3. 信道表（32 信道，0x0800DAAC / 0x0800DACC / 0x0800DAEC）

`0x0800B718(ch)` 中 `ch` 为 0–31，分别取三张表的第 ch 字节写入 FREQ2/FREQ1/FREQ0，
然后延时 20 µs、SCAL、延时 1 ms。

计算式：`f = (FREQ2<<16 | FREQ1<<8 | FREQ0) × 26 MHz / 2^16`

| CH | FREQ2 | FREQ1 | FREQ0 | 频率(MHz) | 用户仓库表(半 MHz) |
|---|---|---|---|---|---|
| 1 | 5C | CE | C4 | 2413.000 | 4826 ✓ |
| 2 | 5C | DD | 89 | 2414.500 | 4829 ✓ |
| 3 | 5C | EC | 4E | 2416.000 | 4832 ✓ |
| 4 | 5D | 00 | 00 | 2418.000 | 4836 ✓ |
| 5 | 5D | 0E | C4 | 2419.500 | 4839 ✓ |
| 6 | 5D | 1D | 89 | 2421.000 | 4842 ✓ |
| 7 | 5D | 31 | 3B | 2423.000 | 4846 ✓ |
| 8 | 5D | 40 | 00 | 2424.500 | 4849 ✓ |
| 9 | 5D | 56 | 27 | 2426.750 | 4852 (2426.0) ✗0.75 |
| 10 | 5D | 62 | 76 | 2428.000 | 4856 ✓ |
| 11 | 5D | 71 | 3B | 2429.500 | 4859 ✓ |
| 12 | 5D | 87 | 62 | 2431.750 | 4862 (2431.0) ✗0.75 |
| 13 | 5D | 93 | B1 | 2433.000 | 4866 ✓ |
| 14 | 5D | A2 | 76 | 2434.500 | 4869 ✓ |
| 15 | 5D | B3 | B1 | 2436.250 | 4872 (2436.0) ✗0.25 |
| 16 | 5D | C4 | EC | 2438.000 | 4876 ✓ |
| 17 | 5D | D3 | B1 | 2439.500 | 4879 ✓ |
| 18 | 5D | E9 | D8 | 2441.750 | 4882 (2441.0) ✗0.75 |
| 19 | 5D | F6 | 27 | 2443.000 | 4886 ✓ |
| 20 | 5E | 04 | EC | 2444.500 | 4889 ✓ |
| 21 | 5E | 1B | 13 | 2446.750 | 4892 (2446.0) ✗0.75 |
| 22 | 5E | 27 | 62 | 2448.000 | 4896 ✓ |
| 23 | 5E | 36 | 27 | 2449.500 | 4899 ✓ |
| 24 | 5E | 4C | 4E | 2451.750 | 4902 (2451.0) ✗0.75 |
| 25 | 5E | 58 | 9D | 2453.000 | 4906 ✓ |
| 26 | 5E | 67 | 62 | 2454.500 | 4909 ✓ |
| 27 | 5E | 7D | 89 | 2456.750 | 4912 (2456.0) ✗0.75 |
| 28 | 5E | 89 | D8 | 2458.000 | 4916 ✓ |
| 29 | 5E | 98 | 9D | 2459.500 | 4919 ✓ |
| 30 | 5E | AE | C4 | 2461.750 | 4922 (2461.0) ✗0.75 |
| 31 | 5E | BB | 13 | 2463.000 | 4926 ✓ |
| 32 | 5E | C9 | D8 | 2464.500 | 4929 ✓ |

结论：**信道规划与用户仓库（XProII FCC 表）基本一致**，但 AD100 Pro V1.2 在 CH9/12/15/18/21/24/27/30
上高 0.25–0.75 MHz（固件表为准）。交叉验证成功。

---

## 4. 配对 / ID 学习（sync word = 编码 ID）

`0x0800B758(id)`：

```c
static uint16_t encode_id(uint8_t id) {          // 0x0800B758
    return 50024u / (id + 1);                    // 0x080030F0 = __aeabi_uidiv
}
void rf_set_id(uint8_t id) {                     // 0x0800B758
    uint16_t sync = encode_id(id);               // r0 = 0xC368, r1 = id+1
    CC2500_WriteReg(0x04, sync >> 8);            // SYNC1
    CC2500_WriteReg(0x05, sync & 0xFF);          // SYNC0
    delay_us(20);
    CC2500_WriteCmd(0x33);                       // SCAL
    delay_ms(1);
}
```
- `0xC368 = 50024`（ID=0 的 sync），与用户仓库 `godox_encode_id()` 完全一致。
- 调用点：RF 初始化（0x08004AF0）、按键/菜单修改 ID 后（0x08008A5C @0x08008AC6、0x08008D00 @0x08008FE2、
  0x08004880）。ID 值存放于设置区 `RAM_20000480[5]`（对应 flash 设置字节 config[0x1D]）。
- **配对过程**（0x0800B9C0 启动 → 0x0800ABE4）：
  1. 延时 10 ms，读 PA6；延时 10 ms，再读 PA6；
  2. 0x0800BDC4(0) 清状态；
  3. 循环：发送属性帧 `[A9][50][B4][02]`（0x0800AA54(0x50,0xB4,0x02)），然后轮询 PA6/看门狗；
  4. 结束后调用 0x0800AB7C 进入接收。
  - `0x50`=“全部组”属性帧；`0xB4` 键、`0x02` 值（/*?*/ 语义可能是“请求配对/握手”）。

---

## 5. 包结构与收发流程

### 5.1 空中格式（CC2500 硬件包）

```
[前导 0xAA × 4]  [sync 16 bit = 50024/(ID+1)]  [payload 4 bytes]  [CRC 无]
```
- 4 字节 payload 无 CRC/地址/白化；协议层靠固定字节 0xD5/0xA9 识别。
- 用户仓库观察到的 10/12 字节“帧”= 前导 + sync（被展开成 2×ID）+ payload 开头（见 §1 对照）。

### 5.2 接收：`0x0800AA74`（由 EXTI15 → IRQ7 0x08005B0C 调用）

```c
void rf_rx_isr(void) {                               // 0x0800AA74
    if (!(state & ...)) {                            // [0x20000185] ∈ {2,4,5,7} 且 [0x200001B2]==0x1A 时走特殊路径
        CC2500_ReadRxFifo(pkt, 4);                   // 0x0800CC98(0x20000214, 4)
        switch (pkt[0]) {
        case 0xD5:                                   // 命令帧：内联 switch(pkt[1])
            rf_cmd_inline(pkt[1], pkt[2], pkt[3]);   // 0x0800AAC8 起
            break;
        case 0xA9:                                   // 属性帧：组在 pkt[1]
            rf_property_handler(pkt[1], pkt[2], pkt[3]);   // 0x0800AFC4
            break;
        default: break;
        }
        CC2500_WriteCmd(0x3A);                       // SFTX
        CC2500_WriteCmd(0x34);                       // SRX 重新进入接收
    }
}
```

0xD5 命令帧（0x0800AAC8 起）分支：

| cmd | 动作 | 证据 |
|---|---|---|
| 0x80 | `RAM_20000480[0]=0`（模式字段清 0） | 0x0800AACC→0x0800AB32 |
| 0x81 | `RAM_20000480[0]=1` | 0x0800AAE6→0x0800AB36 |
| 0x88 | `RAM_2000016A=0` | 0x0800AAEC→0x0800AB32 |
| 0x89 | `RAM_2000016A=1` | 0x0800AAF2→0x0800AB36 |
| 0x09 | 事件位置位 `RAM_200001B2 |= 0x0A`；`RAM_200001C4=100`；0x0800C6F4() | 0x0800AAF8 |
| 0x19 | 同上（/*?*/ 与 0x09 差异待确认） | 0x0800AB1E |
| 0x21 | 0x08009BCC()（清 0x200001AF/1B3/1B0/1AC） | 0x0800AB2C |
| 0x29 | 0x0800B344()（PC? 口线脉冲 60 次，模拟/闪光相关 /*?*/） | 0x0800AB3A |

### 5.3 发送：`0x0800AA54(type, arg1, arg2)`

```c
void rf_send_property(uint8_t group, uint8_t key, uint8_t value) {   // 0x0800AA54
    uint8_t pkt[4];
    pkt[0] = 0xA9;
    pkt[1] = (group == 0x50) ? 0x50 : (group + 9);   // 0x50=全部，其它 9+组号
    pkt[2] = key;
    pkt[3] = value;
    rf_tx_packet(pkt, 4);                            // 0x0800C204
}
void rf_tx_packet(const uint8_t *pkt, int len) {     // 0x0800C204
    0x0800A6CC();                                    // 互斥/电源准备 /*?*/
    CC2500_WriteTxFifo(pkt, 4);                      // 0x0800CD24 -> BurstWrite(0x7F,...,4)
    CC2500_WriteCmd(0x35);                           // STX
    delay_us(0x4B << 3);                             // 600 µs
    CC2500_WriteCmd(0x3B);                           // SWORRST /*?* 或收尾命令
    0x0800A6A4();
}
```
> 与用户仓库 `godox_property_frame()` 对照：`bytes[8]=0xA9`、`bytes[9]=group?9+group:0x50`、
> `bytes[10]=key`、`bytes[11]=value` —— **完全一致**（本固件 payload 即用户帧的 [8..11]）。

### 5.4 属性帧处理 `0x0800AFC4`

- 入参：`r0=组（payload[1]）`、`r1=键（payload[2]）`、`r2=值（payload[3]）`。
- 过滤：若 `组 != 0x50` 且 `组 != [0x2000017D]+9` → 直接返回（[0x2000017D]=本机 C.Fn 组 /*?*/）。
- `r1 -= 0xB0` 后用 `__ARM_common_switch8`（0x0800C8F2，跳表紧跟 BL 之后，表基址 0x0800AFEC）
  分发到 **0x0800B0BE 起的 case 体**；case 覆盖键值 0xB0 以上。
- 键语义（从 case 体可见，/*?*/ 部分待确认）：
  - 修改功率/模式（`RAM_2000019C[组]`、`RAM_20000480[0]`）
  - 修改各组功率/曝光参数、触发 `0x0800A620`/`0x08005D7C`/`0x08003E2C`/`0x08003EB4` 等
  - 复合键 `0x21`/`0x29` 相关处理
  - 具体键码→功能映射需结合用户仓库键表与实机验证（见 §8 待确认）。

---

## 6. 发射功率

- 功率字节表：**0x0800DA9A**，18 字节：
  `00 50 44 C0 84 81 46 93 55 8D C6 69 6E 7F A9 BB FE FF`
- `0x08004AF0`：`idx = [0x2000020E]`，`CC2500_WriteReg(0x3E /*PATABLE*/, table[idx])`，然后 SCAL。
- 用户仓库的功率语义（1/1…1/256，步进 1/10 档）与本表的档位换算关系：/*?*/
  （18 个条目数量与“主功率档+微调”相符，但索引来源 0x2000020E 的写入点尚未完全定位）。
- PA 供电/使能：`0x0800A6CC`/`0x0800A6A4`/`0x0800A6B8` 为 TX/RX 前后的 GPIO 电源/使能切换（/*?*/）。

---

## 7. 相关中断与主循环

| 中断 | 向量地址 | 处理函数 | 作用 |
|---|---|---|---|
| IRQ2 | 0x0800ACF8 | 0x0800ACF8 | 置 `[0x2000021A]=1`，通知 0x20000274 事件（/*?*/ RTC/唤醒） |
| IRQ5 EXTI0_1 | 0x08005A78 | 0x08005A78 | 线 0：按键/充电相关状态机（`[0x20000175]&3`、`[0x20000480+1]`），调用 0x08006014/0x0800B7F0 |
| IRQ6 EXTI2_3 | 0x08005B00 | 0x08005B00 | 清线 3（`0x080072AC(8)`）；线 3 用途 /*?*/ |
| IRQ7 EXTI4_15 | 0x08005B0C | 0x08005B0C | **线 15=GDO0 收包**（0x0800AA74）；线 6=PA6（TX/状态） |
| IRQ9 DMA1_CH1 | 0x08004790 | 0x08004790 | 0x08006E2C(0x200003F8)：ADC/DMA 完成 |
| IRQ13 TIM1 | 0x0800BA7C | 0x0800BA7C | 0x080083E8(0x20000294)：TIM1 到期事件 |
| IRQ15 TIM2 | 0x0800BA8C | 0x0800BA8C | 0x080083E8(0x200002D0)：TIM2 到期事件；0x0800B61C |
| IRQ19 TIM14 | 0x0800BA44 | 0x0800BA44 | 0x0800C1E8/0x08004AA8/0x080099B0/0x080083E8(0x20000348) |
| SysTick | 0x0800B8E4 | 0x0800B8E4 | 1 ms 节拍：0x080075BC（置 [0x200001E1]=1）+ 0x08008194（tick 计数，0x08007434 读取） |

- **没有 RTOS**：SVC(PendSV) = `bx lr` 空实现（0x0800B584、0x0800A918）。
- 主循环 `main()` = **0x0800CF74**：搬 48 项向量表到 RAM 0x20000000、设 SYSCFG、初始化外设，
  然后 1 ms 节拍驱动超级循环（0x0800D010）：
  ```
  等 [0x200001E1]==1 → 清 0；依次调用：
  IWDG 喂狗 0x080074F0(0x20000384) / 0x08008D00（键盘扫描+UI 事件）/
  0x080047CC / 0x080098F8 / 0x080035A8 / 0x0800642C / 0x080038C0 /
  0x0800D098 / 0x080065CC / 0x0800896C / 0x0800B394 / 0x0800A230 /
  0x08003BAC / 0x0800C7D4 / 0x0800B7B0 / 0x080097F8 / 0x0800B80C
  ```
- 看门狗：IWDG，`0x080095CC` 初始化（结构 0x20000384：base=0x40003000，PR=6(/256)，RLR=0xFFF），
  `0x08007440`=启动/初始化，`0x080074F0`/`0x08007544`=喂狗（KR 依次写 0x5555 访问键、0xAAAA 重装）。
  超时 ≈ 26 s（LSI 40 kHz）。

---

## 8. 与 Godox-S3-Trigger 仓库的对照

| 项目 | 用户仓库实现 | AD100 Pro 固件 | 判定 |
|---|---|---|---|
| ID 编码 | `50024/(id+1)` | 0x0800B758 同一公式 | **完全一致** |
| 属性帧 | `[A9][group?9+g:0x50][key][value]` | 0x0800AA54 同构 | **完全一致** |
| 短帧命令 | `[0xD5][cmd]`，cmd=0x11/0x09/0x21 | payload[0]=0xD5 + cmd，命令集 0x09/0x19/0x21/0x29/0x80/0x81/0x88/0x89 | **同族，命令集有差异** |
| 前导/sync | 手工构造 4×0xAA + ID×2 | CC2500 硬件前导 + 16 位 sync=编码 ID | 模型差异（本固件为硬件包模式） |
| 速率/调制 | 波形级 IQ（GFSK 250 kbps） | CC2500 GFSK 250 kbps | **一致** |
| 信道表 | XProII FCC 32 信道 | 0x0800DAAC 表 32 信道，8 个信道频率略有差异 | **基本一致** |
| CRC | 由解码器校验 | **不使用**（PKTCTRL0.CRC_EN=0） | 差异 |
| payload 长度 | 10/12 字节 | 硬件 4 字节 | 差异（层次不同） |

---

## 9. 待确认问题

1. **DEVIATN=0x00**：按 CC2500 公式频偏只有 ~1.6 kHz，与 250 kbps GFSK 不匹配；可能有其它寄存器
   （0x27–0x2B 校准组）或未记录的厂商设置参与，需示波器/SDR 实测确认。
2. **GDO2(PA6) 的确切用途**：EXTI6 处理只做延时+读 PA6+状态机通知；是 TX-done 还是 CHIP_RDYn 待定。
3. **0xD5 命令 0x09 与 0x19 的差异**（0x0800AAF8 与 0x0800AB1E 仅少一条 0x0800C6F4 调用 /*?*/）。
4. **0x0800AFC4 跳表解析**：表基址 0x0800AFEC，count 字节位于 BL 指令末字节（0xFC），按
   `__ARM_common_switch8` 约定 `target = base + 2*table[min(key, count)]`；已定位 default=0x0800B028、
   case 体起点 0x0800B0BE，但**键码→功能**的完整映射未逐一确认。
5. **功率档位表 18 项的语义**（dBm/1-N 档）与 `[0x2000020E]` 写入点。
6. 0x2000020E 附近 RAM 变量（功率、TTL、HSS 状态）之间关系需结合 UI 代码进一步确认。
7. 天线分集/收发切换（PC8，0x0800A6CC 等）的具体时序未逐条确认。
