# Godox X2T-C（X2TC_Z02）—— 引闪器固件逆向 / C.Fn 密钥开关

> **原厂固件不入库**：`devices/x2tc/firmware/*.bin` 已被 `.gitignore` 忽略，仅本机保存。
> 本目录记录该设备的逆向结论、C.Fn 开关实现与离线验证方法。

## 1. 固件身份

| 项 | 值 |
|---|---|
| 文件名 | `X2TC_Z02_V1.01.bin` |
| 大小 | **79,748 B**（本身是 **G3 V2.0 容器**：payload 79,704 + MD5 16 + 文件名 18 + u16 + 8 B 魔数） |
| 容器 sha256 | `d185e4101ad61422f2ac2bc9ee7ade1e6c245c6002361dfd50ed5f9e7b90210b` |
| payload sha256 | `f12927c54209ef258ae2246f9375063baad973ef0c857821169143aab2dde438` |
| 内嵌文件名 | `X2TC_Z02_V1.01.bin`（含设备 ID `X2TC`，G3 用它做型号匹配） |
| 设备 | Godox X2T-C（Canon 版 2.4 GHz 引闪器） |
| 镜像基址 | **`0x08005000`**（前 20 KB 为 bootloader，不在升级文件里） |
| 向量表 | MSP=`0x20001158`，Reset=`0x080050D1` → 基址 0x08005000 |
| 参数页 | `0x0801F000`（payload 之外，由设备/G3 保留） |
| 资源/字模 | payload 尾部 `0x08018000+`（8×8 点阵字模、信道表 `0x08018514` 等） |

> 关键点：**这是 `keypatch` 里第一台「原厂文件本身即 G3 V2.0 容器」的设备**。
> `keypatch/devices/x2tc.py` 的 `apply()` 会拆容器 → 原位改 payload → 重算 MD5 封回容器，
> 输出长度不变，仍是可直接用 G3 刷写的容器。

## 2. 同步字函数（核心补丁点）

| 项 | 值 |
|---|---|
| 函数 | `void rf_set_sync(uint8_t id)` @ **`0x0801184C`** |
| 补丁空间 | **56 字节**（`0x0801184C..0x08011883`，下一函数 `0x08011884`） |
| 原厂逻辑 | `sync = 50024 / (id + 1)` → `WriteReg(4, sync>>8)` → `WriteReg(5, sync&0xFF)` → delay(20µs) → `strobe(0x33)`（SCAL）→ delay(1ms) |
| 50024 字面量 | 全镜像只有一处：`0x08011880`（= 函数 +0x34，与补丁区布局完全对齐） |
| 辅助函数 | `__aeabi_uidiv=0x080050EC`、`CC2500_WriteReg=0x08015CCE`、`delay_us=0x08015A2C`、`strobe=0x08015DB8` |
| 调用点 | `0x08007D9A`（RF 初始化）、`0x0800F8AC`（信道/ID 变化后应用）、`0x08011748`（菜单改 ID） |
| 其它写 SYNC1/0 的地方 | 只有 CC2500 初始化表 `0x08015A4C`（写默认 0xC3/0x68，随后被本函数覆盖） |

补丁后语义（与 V1C / V860III 一致的 **C.Fn 开关**）：

```c
flag = *(uint8_t *)0x200009C6;        // C.Fn LIGHT（原厂背光时间）
if (flag == 2) sync = KEY;            // LIGHT = ON  -> 私有密钥模式
else           sync = 50024 / (id+1); // 12sec / OFF -> 原厂公式
WriteReg(4, sync >> 8); WriteReg(5, sync & 0xFF); return;   // 去掉 SCAL + 延时
```

## 3. C.Fn 菜单 / 设置区（为什么复用 LIGHT）

| 项 | 结论 |
|---|---|
| 菜单设置字节数组 | `settings` @ **`0x200009B8`**（16 项，item n = base+n） |
| 关键 item | **5=CH（0..31）、6=ID（0..99）、14=LIGHT、15=LCD** |
| 开关字节 | `settings[14]` = **`0x200009C6`**，值 `0=12sec / 1=OFF / 2=ON`（出厂 0） |
| C.Fn 最大值表 | RAM **`0x20000092`**（16 项，index14=2）。其内容来自压缩块：开机用 `0x08005228` 的 RLE 解压器把 `0x0801871C` 起的压缩数据还原到 `0x20000000`（0x180 B）；解出的 `0x20000092` 处 16 字节 = `02 01 01 09 01 1f 63 01 63 02 01 08 01 03 02 06` |
| 持久化 | 参数记录打包函数 `0x080073B4` → 记录缓冲 `0x20000028` → Flash 参数页 `0x0801F000`；开机 `0x0801004C` 找魔数 0x55AA 记录并解包（`0x08015FB4`）回 RAM（包含 `settings[14]`） |
| C.Fn 改值 | 滚轮增量处理 `0x0800DBC4` / 减量处理 `0x08007A38`：按 item 索引直接读写 `settings[idx]` 并按 `0x20000092` 最大值回绕（item 15=LCD 是钳位不回绕） |
| 背光读者 | `0x08005824` 的 `ldrb r0,[r0,#0xe]` 读 settings[14] 决定背光时间 → 补丁把它改成 `movs r0,#0`，背光固定原厂 12sec 档（LIGHT 只剩密钥开关语义） |
| 改值即时生效 | C.Fn 改值后约 2 s 自动保存；补丁在保存前置 hook（`0x080073B4` 入口 → 注入 `0x08005010`）里按当前 ID 重设同步字，使 LIGHT 切换在保存时立即生效；重启后 RF 初始化也会调用同步字函数 |

C.Fn 共 **16 项 = 4 页 × 4 行**（官方 X2T 说明书的 17 项里，X2T-C 没有 AFOPT——AF 辅助灯是机身上的拨动开关）。
`LIGHT` 是第 15 项（index 14）→ **第 4 页第 3 项**。

> 用户操作：`MENU` 进 C.Fn → 翻到 **LIGHT** → `+`/滚轮把值改成 **ON** → 等约 2 秒（自动保存）→ 密钥模式生效。
> 恢复原厂：LIGHT 改回 `12sec`（出厂默认）或 `OFF`。

## 4. 补丁总览（5 处，payload 差异 79 字节）

| # | 地址 | 原厂 | 补丁 | 说明 |
|---|---|---|---|---|
| 1 | `0x0801184C` | 56 B 原厂同步字函数 | 56 B 新函数 | 读 LIGHT 开关；私钥/原厂公式；只写 SYNC1/0 |
| 2 | `0x08005824` | `80 7b`（ldrb r0,[r0,#0xe]） | `00 20`（movs r0,#0） | 背光读者屏蔽，固定 12sec |
| 3 | `0x080073B4` | `70 b5 ff 49` | `fd f7 2c fe`（BL 0x08005010） | 保存前置 hook |
| 4 | `0x08005010` | 28 B 全 0（Cortex-M0 向量表保留槽 vec[4..10]） | 28 B 注入代码 | push 原函数头 → sync(settings[6]) → 回 `0x080073B8` |
| 5 | `0x08018758`（payload 末尾） | 容器 MD5 | 重算后的 MD5 | 容器 MD5 同步更新 |

> Cortex-M0 向量表 vec[4..10] 是 **保留向量槽**，内核不会跳转，正好 28 B 可作代码洞（无任何引用）。

## 5. 离线验证（`keypatch.py patch --device x2tc --key 0xD798 --verify`）

- 容器：长度 79,748 B 不变；G3 V2.0 容器结构/MD5/内嵌文件名合法；
- 补丁点：5 处字节逐字节符合期望；payload 与原厂差异恰为 79 B；
- **Unicorn 调用级**：
  - 补丁版同步字函数：`LIGHT=ON(2)` 时任意 ID → `WriteReg(4, KEY>>8)`、`WriteReg(5, KEY&0xFF)`；
    `LIGHT=0/1` 时 ID=0/1/5/37/99 → 原厂 `50024/(ID+1)`（无 SCAL/延时）；
  - **原厂自证**：同一 harness 跑原厂镜像 → `WriteReg(4/5)` + `delay(20)` + `strobe(0x33)` + `delay(1000)`；
  - 保存前置 hook：flag=0/1/2 时均按 settings[6] 调 sync，最后回到 `0x080073B8`，r0..r3 原样恢复；
- `keypatch/selftest.py` 全设备 **ALL PASS**。

## 6. 刷机 / 回滚 / 验证（无实机验证）

- 产物：`keypatch/out/X2TC_Z02_V1.01_key<KEY>.bin`（key=0xD798 时
  sha256 `ba469e3b7d5089dcf34ce40e2604be0678527b8d8bffcc1b4d04b79acd4fefdb`），
  用官方 **G3 V2.0** 直接刷（它已经是容器，**不要**再勾「生成 G3 容器备用件」）；
- 回滚：刷回 `devices/x2tc/firmware/X2TC_Z02_V1.01.bin`；
- **未实机验证**，首次刷入建议先刷原厂回滚包确认 G3 能识别设备，再刷本补丁；
- 灯端也必须刷同一把密钥的 C.Fn 开关版固件（AD200 Pro / V1C / V860III 等）。

## 7. 复现材料

| 文件 | 说明 |
|---|---|
| `devices/x2tc/re/fwlib.py` | 镜像/容器拆分、常量（BASE/SYNC_FN/设置区/辅助函数）、capstone 反汇编辅助 |
| `devices/x2tc/re/cfn.py` | C.Fn / 设置区 / 补丁点侦察脚本（输出见 `log_cfn.txt`） |
| `devices/x2tc/re/decompress_ram_init.py` | 用 Unicorn 解压 `0x0801871C` 的 RLE 块 → 还原 RAM `0x20000000..0x2000017F`（可见 C.Fn 最大值表） |
| `keypatch/devices/x2tc.py` | 补丁模块（apply/verify，容器 MD5 同步刷新） |
| `keypatch/dev/x2tc/NOTES.md` | 决策记录 / 补丁点证据 / 风险 |
