# 神牛固件「私有密钥」补丁工具（keypatch）

给 **神牛 AD100 Pro / AD200 Pro / AD300 Pro / V1C / V860III-C / V860III-S 闪光灯** 和 **神牛 X3-C / X3-S / X3 Pro（C 版） 引闪器** 的固件加入一把
**你专用的 2.4G 私有密钥**：只有你的引闪器能引闪你的灯；其他人的原厂引闪器
（以及没开密钥的灯）无法引闪开了密钥的灯。

全部是**原位字节补丁**：输出固件与原厂固件长度完全一致，用官方 **G3 升级软件**刷入，
原厂固件不会被修改，随时可以刷回。

> ⚠️ 本项目用于个人设备的功能改造/互操作研究。固件版权归权利人所有；改机有风险，
> 请先读完「风险与限制」和「回滚」再动手。

---

## 0. 端到端刷机清单（从这里开始，2026-11-07 当前版）

### 0.1 准备
- 官方 **G3 V2.0** 升级软件 + USB 数据线；
- 私有密钥固定为 **0xD798**（本机 `keypatch/KEY.txt`；该文件已被 `.gitignore` 忽略，
  仓库里只有模板 `keypatch/KEY.example.txt`——复制成 `KEY.txt` 再改）。**同一套系统里的所有
  灯/引闪器必须共用同一把**；要换密钥就全部一起重刷（当前支持的设备见 `keypatch.py list`，
  灯和引闪器都要打同一把）；
- **救砖方法（已实机验证）**：设备刷坏时 → 先正常开机 → **按一下侧面的旋转拨轮按键** → G3 识别到设备 → 打开原厂文件刷回。

### 0.2 第一套：可用配置（用设备自带菜单切换密钥模式，无需新增 UI）

| 顺序 | 设备 | 刷入文件（`keypatch/out/`） | sha256 | 密钥模式开启 | 恢复正常模式 |
|---|---|---|---|---|---|
| 1 | AD200 Pro V1.60 | `AD200pro_V1.60_keyD798.bin` | `8515e39012af13f6f02827cdd1ce29ac611f6af1d9241a3bc1aaef5137812efe` | **C.Fn 第 8 项 `F8` 设为 ON**（原厂该菜单项语义被复用） | F8 = OFF |
| 2 | AD100 Pro V1.2 | `AD100pro_V1.2_keyD798_id0.bin` | `9d4754e7a5d6ac82d585625a5f26316cb2d0b7a8e6a011dbe765ca109845d546` | ID 菜单选 **0（OFF）** | ID 1–99 |
| 3 | X3-C V1.38 | X3_C_v1.38_app_keyD798_keybtn11.bin（最终版） | 756e0493872cb8f85295363747fdee42bb596e2c75d6d1e7345806e58846304 | **长按 M/C/拨轮 ≈1.5s 切密钥**（任意 ID）；**双击 ⚡ = 广播**（32 频道 × 0–99 全 ID，每频道先私钥帧，节流发送，约 40–50 秒）；**按住 M/C ≈20s = 关机**；短按全原厂；**双击 M/C 是原厂关机，请勿使用** | 再长按一次 M/C 关闭密钥 |
| 4 | V1C V2.0 | `V1C_V2.0_keyD798.bin` | `86d563c8c3bc5816844b79a2c6303a39910c7bf349d757d47c8ce01bd79409cd` | **C.Fn 第 3 页 `LIGHT` 设为 ON**（原厂造型灯菜单项语义被复用；V1C 不显示 F 编号） | LIGHT = `12sec`（出厂默认）或 `OFF` |
| 5 | AD300 Pro V1.30 | `AD300pro_V1.30_keyD798.bin` | `4884b841ee8ba86be334d87ac36995c3d25fa3fed0b2aa086366dd923ff014b0` | **C.Fn 第 8 项 `F8` 设为 ON**（原厂「稳定色温模式」语义被复用） | F8 = OFF（出厂默认） |
| 6 | V1S V1.8 | `V1S_V1.8_keyD798.bin` | `3baa5dc8f76bc3daed5edefeb2d7761041de8c27e6a89b836b7b609f60b037cd` | **C.Fn 第 3 页 `LIGHT` 设为 ON**（原厂造型灯菜单项语义被复用） | LIGHT = `12sec`（出厂默认）或 `OFF` |
| 7 | X3 Pro（C 版）V1.22 | `X3pro_C_v1.22_app_keyD798.bin` | `11f58c71e83192fdf6dabd15d541602f391c9db6991bcbf49daf8db4156e4ae6` | **长按 PA6（GPIOA mask `0x40`）≈1.5 s 切换密钥模式**（无 UI；`KEY_ON` 为 RAM 版，重启回普通模式；原厂 PA6 长按关机被复用，两处 compare 均禁用） | 再长按一次；或重启设备（RAM 版复位） |

可选增强（AD100 最终 UI 版，菜单开关）：`AD100pro_V1.2_keyD798_ui.bin`
sha256 `b446084f9b1d1d87b4e9127b8ecda6537a08f11a42c289d91804831457c5ad31` —
**MENU → C.Fn → 第 7 项「KEY」= ON/OFF**（掉电保存；ID 菜单恢复原厂含义）。先用 id0 版验证整套系统，再换 UI 版。

> **keybtn5 注意**：⚡ 长按=原厂开机/关机（电源键）；关机也可以长按 M/C/拨轮 ≈20 秒（或菜单"电源"图块）；低电量自动关机保留。**无广播**；引闪/设置帧仍会双发（密钥灯 + 普通灯兼容），其它短命令帧保持原厂单发（修掉了休眠前灯滴滴响）。
>
> **先别刷** `AD100pro_V1.2_keyD798.bin`（v4 核心、无 UI 入口）：flag 恒 0 → 只能普通模式；仅作为 UI 版的基础镜像。
>
> **V1C（V2.0，v5）**：开关用 **C.Fn 第 3 页 `LIGHT`（原厂造型灯模式：`12sec`/`OFF`/`ON`）** ——
> 不新增 UI，**LIGHT = ON 就是密钥模式**；LIGHT = `12sec`（出厂默认）或 `OFF` 恢复原厂公式
> （`50024/(ID+1)`），ID 菜单恢复原厂含义。补丁分 7 处：同步字函数 `0x080121BC` 的
> **56 字节整函数改写**（读 LIGHT 标志，只写 SYNC1/SYNC0，**无 SCAL、无延时**）
> + `0x08017944` 注入改值即时生效 hook + 2 个 C.Fn 改值 gate + 2 个造型灯读者屏蔽
> + 1 个无线设置同步保护（不覆盖 `settings[8]`）。
> 输出固件长度不变（88,032 B）。`verify` **36 项 PASS**、`selftest` ALL PASS（4 台）；
> 但 **无整机模拟器、无实机验证**，首次刷入请走安全阶梯
> （见 [`devices/v1c/docs/02_验证与实机清单.md`](../devices/v1c/docs/02_验证与实机清单.md)）。
> **代价**：原厂 C.Fn 的 `LIGHT` 项不再改造型灯模式（造型灯输出固定原厂 `12sec` 档），
> 该菜单项只剩「ON = 密钥开关」语义。
> 上面表里的 V1C sha256 是本轮静态生成（key=0xD798）的结果，换密钥/固件版本会变。
>
> **V860III-C（V1.3）**：与 V1C 同款 **C.Fn 第 3 页 `LIGHT`** 密钥开关 —— LIGHT = `ON`（值 2）
> 即密钥模式；`12sec`（出厂默认）/ `OFF` 恢复原厂公式（`50024/(ID+1)`），ID 菜单保持原厂含义。
> 补丁分 7 处：同步字函数 `0x080122E8` 的 **56 字节整函数改写** + `0x080179C2` 注入改值即时生效
> hook + 2 个 C.Fn 改值 gate + 2 个造型灯读者屏蔽 + 1 个无线设置同步保护；输出长度不变（87,612 B）。
> `verify` **35 项 PASS**、Lead `call_harness.py` 交叉验证 PASS；**无整机模拟器、无实机验证**，
> 首次刷入请走安全阶梯（见
> [`devices/v860iiic/docs/02_验证与实机清单.md`](../devices/v860iiic/docs/02_验证与实机清单.md)）。
> **代价**：造型灯输出固定原厂 `12sec` 档（C.Fn 的 LIGHT 只剩「ON = 密钥开关」语义）。
> 该设备产物的 sha256 随密钥变化、本轮未固定生成，使用前请以 `--verify` 的实际输出为准。
>
> **V860III-S（V1.1）**：与 V1C / V860III-C 同款 **C.Fn 第 3 页 `LIGHT`** 密钥开关 —— LIGHT = `ON`（值 2）
> 即密钥模式；`12sec`（出厂默认）/ `OFF` 恢复原厂公式（`50024/(ID+1)`），ID 菜单保持原厂含义。
> 补丁分 **6 处**：同步字函数 `0x080115B8` 的 **56 字节整函数改写**（读 LIGHT 标志，只写 SYNC1/SYNC0，
> **无 SCAL、无延时**）+ **2 处 2 字节 NOP gate**（`0x08005A74`/`0x0800C5BE`，任意 C.Fn 项改值都立即重设
> 同步字）+ 2 个造型灯读者屏蔽（`0x08003956`/`0x0800397E`）+ 1 个无线设置复位保护（`0x0800D0A2`）；
> 输出长度不变（79,300 B）。`verify` **73 项 PASS**（含 C.Fn handler 级 Unicorn 断言）、
> `selftest`/`call_harness.py` 交叉验证 ALL PASS；**无整机模拟器、无实机验证**，
> 首次刷入请走安全阶梯（见
> [`devices/v860iiis/docs/02_验证与实机清单.md`](../devices/v860iiis/docs/02_验证与实机清单.md)）。
> **代价**：造型灯输出固定原厂 `12sec` 档（C.Fn 的 LIGHT 只剩「ON = 密钥开关」语义）。
> 本机 UI 只有 10 项 C.Fn（无 FEB ACL / TX），不影响 LIGHT/ID。
> 本轮静态生成（key=0xD798）的产物 sha256 为
> `9ef11e8116e07dda30bd08ea407498f373ad3d953effb32258e3104c2ba5753d`，换密钥/固件版本会变。

> **AD300 Pro（V1.30，C.Fn `F8`）**：密钥开关复用 **C.Fn 第 8 项 `F8`（原厂「稳定色温模式」OFF/ON）**
> —— F8 = `ON`（值 1）即密钥模式；F8 = `OFF`（出厂默认）恢复原厂公式（`50024/(ID+1)`），
> ID 菜单保持原厂含义。补丁分 11 处：同步字函数 `0x0800B548` 的 **56 字节整函数改写**
> （读 F8 标志，只写 SYNC1/SYNC0，**无 SCAL、无延时**）+ 2 个 C.Fn 改值 gate
> （`0x08004677`/`0x08008425` 的 bne→blo，item≥6 即 ID/F8 改值立即重设同步字）
> + 6 处稳定色温读者屏蔽 + 1 处退出路径静默清零阻断；输出长度不变（44,220 B）。
> `verify` **47 项 PASS**、`selftest` ALL PASS、studio UI e2e 14/14 PASS；
> **无整机模拟器、无实机验证**，首次刷入请走安全阶梯（见
> [`devices/ad300pro/README.md`](../devices/ad300pro/README.md) §5）。
> **代价**：原厂「稳定色温模式」功能在 F8=ON 时固定为 OFF 行为；F8=OFF 时与原厂逐指令一致。
> 上面表里的 AD300 sha256 是本轮静态生成（key=0xD798）的结果，换密钥/固件版本会变。
>
> **X3-S（V1.25，keybtn 默认）**：与 X3-C 同平台（AT32F435），关键函数逐字节相同、地址分段平移
> （`send_short`/`send_property` -0x64，其余多数 -0x118）。默认补丁 = X3-C 同款 **keybtn**：
> **长按拨轮/M/C ≈1.5s 切换密钥模式**（`KEY_ON`=RAM `0x20001080`）+ **双发**
> （密钥模式下先发 `SYNC=KEY` 帧，再发当前 ID 普通同步字帧，普通灯也兼容）；入口
> `0x0801184C`/`0x080117E8` B.W + `0x080EE000` 注入，按键扫描 `0x0800CBD0` B.W + `0x080EE400`
> 注入；差异 539 字节，长度不变 1,012,268 B。另保留 **v1 固定 KEY**（`_fixed.bin`，5 字节差异）。
> 容器备用件 `X3S_v1.25_app_key<KEY>_container.bin`。模块 `verify` **17 项 PASS**、
> 整机模拟器（X3-C Unicorn 模型加载 X3-S）**ALL PASS**、启动回归 **ALL PASS**；**无实机验证**。
> 详见 [`devices/x3s/README.md`](../devices/x3s/README.md)。
>
> **X3 Pro（C 版，V1.22）**：开关用**物理按键长按** —— **长按 PA6（GPIOA mask `0x40`）≈1.5 s
> 切换密钥模式**（不新增 UI；`KEY_ON` 在 RAM `0x20001096`，**RAM 版**，重启/换电池回普通模式）。
> 原厂 PA6 长按的关机动作被禁用（**两处** compare：`0x0802830A` / `0x080284F2` 的 `cmp #0x21`
> 都改成 `#0xFF`，各改 1 字节）并复用为密钥开关；
> 电源键（GPIOB0）长按关机不受影响。ID 菜单恢复原厂含义（0..99，不再有 ID=99 特殊语义）。
> 语义：`KEY_ON=1` 时每帧先按私有 KEY 发（引闪密钥灯），再按**当前 ID** 的原厂同步字发
> （兼容同频道同 ID 的原厂灯），发完恢复 KEY；`KEY_ON=0` 时原厂单发。
> 补丁分 **6 个区域 / 7 个改动位置**：按键扫描入口 `0x08027E44` 4 B 改 `B.W` + 同步字函数
> `0x0802F040` 内 **10 字节改写** + `0x0802EDC4`(TX4)/`0x0802EE2C`(TX2) 两个发送入口
> **4 B 改 `B.W`** + **两处 compare** `0x0802830A` / `0x080284F2` 各改 1 B（禁用原厂长按
> 动作 A/B）+ `0x081C0000` 空闲区 **356 B 注入**；输出长度不变（1,941,020 B），与原厂差异
> **370 B**。`verify` **41 项 PASS**（静态 + Unicorn 调用级：同步字/
> 双发/单发/按键计数 5 用例/原厂自证）；**无整机模拟器、无实机验证**，首次刷入请走安全阶梯
> （见 [`devices/x3pro/README.md`](../devices/x3pro/README.md) §7.3）。
> **风险**：键位需实机确认（若 PA6 不是想用的键，改 `BTN_MASK`=`0x10`(PB4)/`0x20`(PB5) 重新生成）；
> 原厂 PA6 长按关机被占用；开关是 RAM 版；密钥模式下每次发射多一帧（4B 帧多一次 FIFO+STX；
> 2B 短命令原厂本就连发 3 次，每帧再多一次 500 µs 级发射），HSS/连拍异常优先怀疑这里；
> 无反馈指示，可用同频道同 ID 原厂灯 + TEST 引闪验证。上面表里的 sha256 是本轮静态生成
> （key=0xD798）的结果，换密钥/固件版本会变。

> **X2T-C（V1.01，C.Fn `LIGHT`）**：引闪器 Godox X2T-C（`X2TC_Z02_V1.01.bin`，79,748 B，
> sha256 `d185e410…`）。**原厂文件本身是 G3 V2.0 容器**（payload 79,704 B + MD5/文件名/魔数，
> payload 基址 `0x08005000`）：`apply()` 拆容器 → 原位改 payload → **重算 MD5 封回容器**，
> 输出仍是长度不变、可直接用 G3 刷写的容器（`container_builtin=True`，CLI/studio 的
> 「生成 G3 容器备用件」会**自动跳过**，不要二次套壳）。
> 开关 = **C.Fn 第 4 页 `LIGHT`**：`LIGHT = ON`（值 2）→ 同步字 = 私有 KEY；
> `12sec`（出厂默认）/`OFF` → 原厂 `50024/(ID+1)`，ID 菜单保持原厂含义。
> 补丁 5 处：同步字函数 `0x0801184C` 起 **56 B 整函数改写** + 背光读者屏蔽 `0x08005824`
> + 保存前置 hook（`0x080073B4` → 向量表保留槽 `0x08005010` 注入 28 B）+ 容器 MD5 重算；
> payload 差异 79 B。`verify` **34 项 PASS**（含原厂自证、保存 hook）、`selftest` ALL PASS；
> **无实机验证**，首次刷入请先确认 G3 能识别设备并准备原厂回滚包。
> 详见 [`devices/x2tc/README.md`](../devices/x2tc/README.md)。

### 0.3 每台刷完关注什么

1. 能正常开机、菜单/版本正常；
2. **普通模式**下能正常引闪（引闪器与灯同 CH/ID）；
3. 所有设备都切到**密钥模式**后，引闪应正常；
4. **隔离验证**：把引闪器切回普通模式（例如 ID=1），密钥模式的灯应**不被引闪**——这就是密钥生效的标志；
5. 全部恢复普通模式后，一切应与原厂一致。

> **V1C / V860III-C / V860III-S 额外注意**：密钥开关是 C.Fn 第 3 页的 **`LIGHT`** 项。原厂 LIGHT 控制造型灯模式
> （`12sec`/`OFF`/`ON`）；复用后造型灯输出固定原厂 `12sec` 档，该菜单项只剩「ON = 密钥开关」。
> 刷完请顺手确认 C.Fn 里能看到并切换 LIGHT，且造型灯行为与预期一致。

### 0.4 回滚（恢复原厂）

| 设备 | 原厂文件 |
|---|---|
| X3-C | `devices/x3c/firmware/X3_C_v1.38_app.bin` |
| X3 Pro（C 版） | `devices/x3pro/firmware/X3pro_C_v1.22_app.bin` |
| AD100 Pro | `devices/ad100pro/firmware/AD100pro_V1.2.bin` |
| AD200 Pro | `devices/ad200pro/firmware/AD200pro_V1.60.bin` |
| AD300 Pro | `devices/ad300pro/firmware/AD300pro_V1.30.bin` |
| V1C | `devices/v1c/firmware/V1C_V2.0.bin` |
| V860III-C | `devices/v860iiic/firmware/V860IIIC_V1.3.bin` |
| V860III-S | `devices/v860iiis/firmware/V860IIIS_V1.1.bin` |
| X2T-C | `devices/x2tc/firmware/X2TC_Z02_V1.01.bin`（原厂文件即 G3 容器） |

G3 若检测不到设备：先正常开机，**按一下侧面旋转拨轮**再插 USB 试。

### 0.5 可选诊断包（不影响正式使用；测完可刷回）

| 用途 | 文件 | sha256 | 怎么看 |
|---|---|---|---|
| **键位探针（备用）** | `X3_C_v1.38_app_keyD798_keyprobe.bin` | `847491e6fef1e4de69e58e53a43bfc9533e524cb4b3d83de5cac275fcfebeb7e` | 引闪器 ID=5、灯普通模式同 ID；长按三个物理键，看**闪 1/2/3 下**分别对应哪个键 |
| **最终手势版（keybtn11，全 ID 广播）** | X3_C_v1.38_app_keyD798_keybtn11.bin | 756e0493872cb8f85295363747fdee42bb596e2c75d6d1e7345806e58846304 | **长按 M/C 1.5s = 密钥开关**；**双击 ⚡ = 广播**（32 频道 × 0–99 全 ID + 每频道私钥帧，节流约 40–50 秒）；按住 M/C 20s = 关机；⚡ 长按 = 原厂电源；双击 M/C 会触发原厂关机，勿用 |
| 广播轻量版（备用） | `X3_C_v1.38_app_keyD798_keybtn8.bin` | `e0947e9e901a848e86b0f6b394b8f51bfbca2d0249365fef0fff95bc26ce8585` | 双击广播只发 32 频道 ×（私钥帧 + 当前 ID 普通帧）共 64 帧；若全 ID 版仍关机可先试这个 |
| 触摸链路探针 | `X3_C_v1.38_app_keyD798_touchprobe_test.bin` | `c59380c22ea9a621b90cd5d127f11a1c4fb661489f97d657c693de461a8f1415` | 摸屏幕 → 同 ID 原厂灯闪一下 = 触摸链路成立 |

### 0.6 正在做 / 待定

- 按键版 remap：**长按拨轮 = 切密钥、长按 ⚡ = 广播**（先出键位探针确认三个键的对应关系）；
- 触摸 UI：已定位触摸 ISR（IRQ18 → `0x0800510C`，坐标写 `0x20000168/0x2000016A`）；探针确认后做"坐标区域触发"（摸锁区域=密钥、摸设置页签=广播）；
- AD100 的 C.Fn「KEY」菜单项、AD200 的 F8 ON/OFF 菜单项（最终 UI 版）；
- **V1C 实机验证**：v5（C.Fn 第 3 页 **`LIGHT`** 开关）已就绪，`verify` 36 项 PASS、`selftest` ALL PASS，
  但整机模拟器未建立、实机未刷入；待验证项：C.Fn 里 LIGHT 的显示/持久化、造型灯行为、
  无线设置同步不会清 LIGHT（安全阶梯见
  [`devices/v1c/docs/02_验证与实机清单.md`](../devices/v1c/docs/02_验证与实机清单.md)）；
- X3-C 磁贴 UI：该固件 UI 框架不允许"页面建完后新增控件"（必崩），暂缓。

### 0.7 风险提示

- 无实机验证的版本（尤其 UI/磁贴相关）不要长期使用；每步刷前先确认救砖方法可用；
- 开密钥后，**原厂引闪器和其他未开密钥的灯无法引闪你的灯**（这是功能目的）；
- 换密钥 = 同一套系统里的所有设备一起重刷；只改密钥不影响其它功能。

---

## 1. 原理（一句话版）

神牛 2.4G 链路用 TI CC2500 兼容芯片，固件把"引闪 ID"换算成 **16 位硬件同步字**
（`SYNC1/SYNC0 = 50024 / (ID+1)`）。同步字对不上，芯片在硬件层就解不出包。

本工具把这条公式改成 **"同步字 = 你的私有密钥"**：

- 你的灯（已打补丁）把私有密钥写进 CC2500 的同步字寄存器 → 只认这个同步字；
- 你的引闪器（已打补丁）用同一个同步字发帧 → 能引闪；
- 别人的原厂引闪器只能发 `50024/(ID+1)` 那 100 个公开值 → 灯不会响应。

16 位同步字不是密码学认证（SDR 抓包理论上可克隆），它挡的是"正常原厂设备误触发"，
不是专业攻击。

---

## 2. 当前版本：v2「可开关 + 兼容普通灯」（可用）；v3「UI 菜单版」开发中

> **v3（用户已确认要做，开发中）**：灯和引闪器的**固件菜单里新增独立开关**（X3-C 快捷面板
> 新增「密钥」「广播」磁贴，汉字自绘字模；AD100 Pro / AD200 Pro 的 C.Fn 菜单新增英文缩写项），
> 引闪器开密钥时按**当前 ID** 双发。详见 `keypatch/DESIGN_TOGGLE.md` 与
> `keypatch/dev/x3c_ui/`、`keypatch/dev/lights_ui/`。
>
> 下面的 v2 是**已经验证、可立即刷**的安全版本；v3 做好之前可以先刷 v2 用起来（v3 出来后重刷即可）。

### 2.1 开关方式（v2：不新增 UI，用现有 ID 菜单）

| 设备 | ID 菜单设置 | 行为 |
|---|---|---|
| 灯 AD100 Pro / AD200 Pro（已打补丁） | **ID = OFF** | **密钥模式**：只接受私有密钥同步字 |
| 灯（已打补丁） | ID = 1..99 | 普通模式：同步字 = `50024/(ID+1)`，和原厂完全一样 |
| 引闪器 X3-C（已打补丁） | **ID = 99** | **密钥模式**：每帧发两遍 —— 先按私有密钥发（引闪密钥灯），再按 ID=99 的普通同步字发（引闪同频道、ID=99 的普通灯） |
| 引闪器（已打补丁） | ID = 0..98 | 普通模式：只发原厂同步字 |
| 引闪器 **X3 Pro（C 版）**（已打补丁） | **长按 PA6 ≈1.5 s** | **切换密钥模式**（`KEY_ON` 为 RAM 版，上电默认普通模式）：开 = 每帧先按私有 KEY 发、再按当前 ID 原厂同步字发（双发）；关 = 原厂单发。ID 菜单恢复原厂 0..99；原厂 PA6 长按关机两处 compare 均禁用 |

> **注意**：V1C（V2.0，v5）**不在**上面这套「ID=OFF」方案里 —— 它用 **C.Fn 第 3 页 `LIGHT`**
> 作密钥开关（LIGHT=ON → 密钥模式；`12sec`（出厂默认）/`OFF` → 原厂公式，ID 菜单恢复原厂含义），
> 见 §0.2 与 [`devices/v1c/README.md`](../devices/v1c/README.md) §5。

### 2.2 推荐用法

- **只用自己的灯**：灯 `ID=OFF`，引闪器 `ID=99`。
- **自己的灯 + 备用/别人的普通灯（同频道、ID=99）**：同上，引闪器会自动双发，
  两种灯一起闪。
- **完全普通模式（兼容原厂引闪器/让别人也能引闪）**：灯 `ID=1..99`，引闪器 `ID=0..98`。

> 灯 ID=OFF 的语义从"无 ID 的普通模式"变成"密钥模式"，这是本方案的取舍；
> 老的原厂灯（未打补丁）不受影响。

---

## 3. 生成固件

前置：本机 Python 已带 capstone/unicorn：`tools\python\python.exe`

```powershell
cd <repo>

# 1) 看支持的设备 / 原厂固件是否就位
.\tools\python\python.exe keypatch\keypatch.py list

# 2) 用随机密钥或口令生成一把密钥（也可以直接用 keypatch\KEY.txt 里已生成的那把；
#    KEY.txt 不在仓库里，复制 keypatch\KEY.example.txt 再改即可）
.\tools\python\python.exe keypatch\keypatch.py keygen
.\tools\python\python.exe keypatch\keypatch.py keygen --passphrase "我的口令"

# 3) 三台设备打同一把密钥 + 离线验证
.\tools\python\python.exe keypatch\keypatch.py patch-all --key 0xD798 --verify

# 4) 总自检（可选）
.\tools\python\python.exe keypatch\selftest.py --key 0xD798
```

成品输出在 `keypatch\out\`：

| 文件 | 设备 | 原厂长度 | 说明 |
|---|---|---|---|
| `AD100pro_V1.2_keyD798.bin` | AD100 Pro V1.2 | 50248 B | 密钥开关版 |
| `AD200pro_V1.60_keyD798.bin` | AD200 Pro V1.60 | 46380 B | 密钥开关版 |
| `AD300pro_V1.30_keyD798.bin` | AD300 Pro V1.30 | 44220 B | C.Fn F8 密钥开关版（F8=ON；**待实机验证**） |
| `X3_C_v1.38_app_keyD798.bin` | X3-C v1.38 | 1021468 B | 密钥开关 + 双发版 |
| `V1C_V2.0_keyD798.bin` | V1C V2.0 | 88032 B | C.Fn LIGHT 密钥开关版（LIGHT=ON；**待实机验证**） |
| `V860IIIS_V1.1_keyD798.bin` | V860III-S V1.1 | 79300 B | C.Fn LIGHT 密钥开关版（LIGHT=ON；**待实机验证**） |
| `X3pro_C_v1.22_app_keyD798.bin` | X3 Pro（C 版）V1.22 | 1941020 B | 长按 PA6 ≈1.5s 密钥开关版（**RAM 版**；`KEY_ON=1` 双发 KEY+当前 ID 普通帧；两处 compare 禁用原厂长按关机；**待实机验证**） |

> 密钥保存在本机 `keypatch\KEY.txt`（已被 gitignore；模板见 `keypatch\KEY.example.txt`）。
> **灯和引闪器必须用同一把**；换密钥后所有设备都要重刷。

---

## 4. 刷机（官方 G3）

> 已对官方 G3 V2.0 做过静态审计（`keypatch/dev/g3/FINDINGS.md`，含反编译地址与证据）：
> G3 对"打开文件"选中的本地 `.bin` **不做任何内容校验**——不查文件长度、不查型号/版本、
> 没有 CRC/MD5/签名。唯一与内容有关的是"每 1024 字节块的 32 位字节和"，由 G3 在发送时
> **从文件现算**，所以原位改字节（长度不变）会被正常接受并刷入。
> AD100 Pro / AD200 Pro / X3-C 的官方更新包都是"裸 app.bin"（无容器），走 legacy 路径。
> **V1C / V860III-S 本轮未实机刷写验证**：先按裸 bin 刷；若 G3 报「固件与设备不匹配」，改用
> `V1C_V2.0_key*_container.bin` / `V860IIIS_V1.1_key*_container.bin`（容器内嵌文件名含设备 ID，G3 才认）。

1. 安装官方 G3（`vendor\G3_windows.zip` 里的 `Godox_V2.0_setup.exe`）。
2. USB 连接设备（灯或引闪器），等 G3 识别。
3. 点"打开文件"→ 选 `keypatch\out\` 里对应设备的 `.bin` → 点"更新"。
4. 看到进度条开始 = 文件被接受；等完成，设备自动重启。
5. 设备分别刷：AD100 Pro、AD200 Pro、**AD300 Pro**、X3-C、V1C、V860III-S（建议先刷引闪器，再刷灯）。

**万一提示"选择的固件与设备不匹配"（还没开始进度条就报错）**：说明这台设备是新一
代"V2.0（A1）"设备，需要容器格式。`keypatch\out\` 里已经准备好 `*_container.bin`
备用件，直接选它刷即可（文件名里已含设备 ID：AD100Pro / AD200Pro / X3C）。也可以自己重新打包：

```powershell
# 例：给 AD100Pro 打包（文件名里必须包含设备 ID 字符串，如 AD100Pro）
.\tools\python\python.exe keypatch\dev\g3\make_container.py `
  keypatch\out\AD100pro_V1.2_keyD798.bin keypatch\out\AD100pro_V1.2_keyD798_container.bin `
  --name AD100Pro_V1.2_keyD798.bin
```

更省事的方式：打补丁时加 `--container`，会按 `out_name` 自动生成 `*_container.bin`：

```powershell
.\tools\python\python.exe keypatch\keypatch.py patch-all --key 0xD798 --verify --container
.\tools\python\python.exe keypatch\keypatch.py patch --device x3c --key 0xD798 --container
```

> 这些已实机验证过的设备官方包都是裸 bin，正常情况下用裸 bin 即可；容器方案是兜底。
> 首次刷机前建议先确认自己能从官网重新下载同版本原厂固件（回滚用）。

---

## 5. 回滚（恢复原厂）

把原厂固件重新用 G3 刷回去即可：

- AD100 Pro：`devices\ad100pro\firmware\AD100pro_V1.2.bin`
- AD200 Pro：`devices\ad200pro\firmware\AD200pro_V1.60.bin`
- AD300 Pro：`devices\ad300pro\firmware\AD300pro_V1.30.bin`
- X3-C：`devices\x3c\firmware\X3_C_v1.38_app.bin`
- X3 Pro（C 版）：`devices\x3pro\firmware\X3pro_C_v1.22_app.bin`
- V1C：`devices\v1c\firmware\V1C_V2.0.bin`
- V860III-C：`devices\v860iiic\firmware\V860IIIC_V1.3.bin`
- V860III-S：`devices\v860iiis\firmware\V860IIIS_V1.1.bin`

（这些文件在本工作区里就是原厂副本，sha256 见 `keypatch\devices\*.py` 的 `DEVICE["sha256"]`。）

---

## 6. 换密钥 / 后续改功能

- 换密钥：`keypatch.py patch-all --key <新密钥> --verify`，然后把所有设备的新 bin 重新刷一遍。
- 继续改固件功能：`keypatch/INTERFACE.md` 定义了设备模块接口；
  每个设备一个 `python 模块`（`keypatch/devices/<id>.py`），`apply(data, key)` 做原位补丁、
  `verify(patched, key)` 做离线验证。新功能可以：
  1. 用 `devices/x3c/re/ghidra_exports`（X3-C）或 `devices/ad100pro/out`、`devices/ad200pro/analysis`
     里的反汇编/伪 C 找到函数；
  2. 在 `apply()` 里加原位字节补丁（注意保持文件长度不变，G3 最稳）；
  3. 用 Unicorn 调用级测试（参考 `keypatch/dev/lead/*.py`）验证语义，再跑整机模拟器回归。

---

## 7. 验证证据（离线，无实机）

- 四台设备的同步字函数都用 **Unicorn 直接执行补丁后的机器码**验证：
  - 灯：`ID=0 → SYNC=KEY`，`ID=1/37/99 → 原厂 50024/(ID+1)`，与原厂逐位一致；
  - V1C（灯）：**C.Fn 第 3 页 `LIGHT` = ON → SYNC=KEY**；**`12sec`/`OFF` → 原厂
    50024/(ID+1)**（ID=0/1/5/37/99）；只写 SYNC1/0（**无 SCAL、无延时**）；
    注入 hook 对 C.Fn 改值立即重设同步字；与原厂差异共 71 B，全部落在 7 处补丁点内；
  - 引闪器：`ID=99` 时 `SYNC=KEY → 发送 → SYNC=0x01F4 → 再发送`；`ID≠99` 只发一次。
- 整机模拟器回归（AD100 Pro / AD200 Pro / X3-C）：原厂 vs 补丁镜像跑到主循环/数百万条指令，
  PC/寄存器/CC2500 寄存器序列一致，仅预期差异（删掉的 SCAL/延时或同步字/双发）。
  **V1C 没有整机模拟器**，只有调用级验证（见 [`devices/v1c/README.md`](../devices/v1c/README.md) §7）。
- G3 升级工具审计（`keypatch/dev/g3/FINDINGS.md`）：
  - `openFirwareFile`(0x403040) 只取路径、不校验内容；
  - `SetFilePath`(0x407540) 只存路径；
  - state 2 容器解析失败时，`this+0x9a==0`（非 V2.0 设备）直接整文件当 payload；
  - 全程序没有整镜像 CRC/版本/签名检查；唯一内容相关值是每 1024B 块的 32 位字节和，
    由 G3 现算（`SendDataLooper` case 8），补丁后文件自然一致。
- 详见 `keypatch/dev/ad100/NOTES.md`、`keypatch/dev/ad200/NOTES.md`、
  `keypatch/dev/x3c/NOTES.md`；V1C 见 [`devices/v1c/README.md`](../devices/v1c/README.md) §7 与
  [`devices/v1c/docs/02_验证与实机清单.md`](../devices/v1c/docs/02_验证与实机清单.md)。

**没有实机验证**：模拟器不模拟空口同步过滤，最终"别人引闪不了"必须靠真机 CC2500
硬件同步字匹配来保证（这正是原厂 ID 机制本身的工作方式）。

---

## 8. 风险与限制

1. **16 位同步字不是密码学认证**：能挡住原厂设备误触发；专业 SDR 攻击不在防护范围。
2. **无实机验证**：所有结论来自静态分析 + Unicorn/模拟器。首次刷机有风险，
   但有 G3 可回刷原厂固件（前提是设备还能进 USB 升级模式）。
3. **版本绑定**：补丁工具会校验原厂固件 sha256/字节，固件版本不对会拒绝打补丁，
   避免刷错。若你升级了原厂固件，需要重新做补丁（找 Lead/重跑逆向）。
4. **灯的密钥开关按型号不同**：AD100 Pro = ID 菜单 OFF（id0 版）/ C.Fn「KEY」（UI 版）；
   AD200 Pro = C.Fn 第 8 项 `F8`；**V1C = C.Fn 第 3 页 `LIGHT`**（LIGHT=ON 密钥模式、
   `12sec`/`OFF` 原厂公式；原厂造型灯 C.Fn 功能被固定为 12sec 档）。
   普通模式：AD100/V1C 用原厂 ID 1..99，AD200 用 F8=OFF。
5. **删除 SCAL（及延时）**：灯端补丁在改同步字后不再做 `SCAL+1ms`（只改同步字不改频率，
   CC2500 手册上不需要）；AD100（id0 版）保留 20µs 建立时间，**V1C v5 连 20µs 也去掉了**
   （56 B 空间被 LIGHT 开关判断占用）。若真机上出现"偶发首包丢失"，优先怀疑这里（回退方案见各 NOTES）。
6. **引闪器双发**（v2）会增加每次发送的射频时间，属正常现象。

---

## 9. X3-C UI 版（v1.2，当前 X3-C 交付）

> 用户已确认 X3-C 要"UI 版"：快捷面板新增「密钥」「广播」两个磁贴（自绘字模），
> **点击可用**；密钥开关写 RAM `0x20001080`（引闪器双发读它）；广播=32 频道依次发
> 私钥同步字 + 功率属性帧 + 造型灯属性帧 + TEST。

成品：`keypatch/out/X3_C_v1.38_app_keyD798.bin`
- 1,021,468 B，长度不变；sha256 `95aac447c660e026ed496e9bef978a75fa9092391bfe74222925e03b2d7a42ef`
- 差异 1336 B = 核心双发补丁 332 B + UI 补丁 1004 B；
- 原厂代码只改 8 B：`0x080118B0`、`0x0801184C`（核心入口跳转）+ `0x08008236`（磁贴挂载）+ `0x0800FE78`（页 1 轮询跳板）；
- 其余都在 0x080EE000 / 0x080F0000 起的空闲区（原厂为 0xFF）。容器备用件：`X3C_V1.38_app_keyD798_container.bin`。

### 9.1 硬件首验步骤（刷 X3-C 后按序看）

1. 能正常进主界面（不黑屏/不重启）；
2. 快捷面板（声音/造型灯/锁那一页）的"锁"磁贴下方出现两个并排小磁贴：左 `KEY`、右 `BCAST`；
3. **底部页签保持原厂 2 个**（新磁贴是锁下方独立一行，不改页签布局）；
4. **点左磁贴**：应切换密钥模式（RAM 0x20001080 1/0）；再点应切回；
5. **点右磁贴**：32 个频道依次发射（附近灯会依次闪，约 1 秒级）；
6. 若第 4 步点不动：说明 `widget+0x28` 的 bit8 不是"按下"标志 → 只需把轮询函数里
   `movs r6,#8` 换成 1/2/4/0x10 重新生成（单常量），见 `keypatch/dev/x3c_ui/FEASIBILITY.md`。

### 9.2 回滚

- 刷回原厂：`devices/x3c/firmware/X3_C_v1.38_app.bin`。
- 或只回滚两处 4 字节（恢复成原厂指令）：
  - `0x08008236` = `1f f0 d3 fe`（bl 0x08027fe0）
  - `0x0800FE78` = `2d e9 fc 41`（push.w {r2,r3,r4-r8,lr}）
  其余改动都在 0xFF 空闲区，不影响原厂逻辑。

### 9.3 风险

- **无实机验证**：所有结论来自静态反汇编 + Unicorn 调用级 + 整机 boot 回归（与原厂一致）。
  最坏情况是磁贴不显示或点击无效（可回滚）；建议首次刷后用上面 6 步确认。
- 广播帧序列：`set_channel → SYNC=KEY → (0x50,0xD1,功率) → (0x50,0xB1,造型灯) → TEST`；
  功率来自设置表 `0x20000B68[组][+4]&0x7F`，造型灯来自同表 `[+0]`。
  **风险**：`组=0x50(ALL)` 灯端是否接受 `0xD1/0xB1` 未实机验证；若实机功率/造型灯不生效，
  改逐组 `10..14` 的备选版（`FEASIBILITY.md §12.4`，改动很小）。
- 全 ID 档扫 0..99 = 3200 帧 ≈ 十几秒（同步阻塞，期间 UI 会卡），属预期；快速档可配。
- 磁贴标签 `KEY` / `BCAST` 是 ASCII（固件字库缺"密/钥/广/播"字形；汉字 24×24 点阵
  已注入，后续可换成图标+汉字混排）。

### 9.4 成品清单（v1.2.3，已修复实机重启 bug）

| 文件 | 说明 | sha256 |
|---|---|---|
| `X3_C_v1.38_app_keyD798.bin` | **主成品 UI 版**：广播用 group=0x50(ALL) 发功率/造型灯 | `6cd8e8233e4cfbff77350c2dd1340dec16cec2772d765b9134fd2762c322514c` |
| `X3_C_v1.38_app_keyD798_pergroup.bin` | **备选 UI 版**：逐组 0x0A–0x0E(A–E) 发功率/造型灯（若实机不认 ALL 组就用它） | `95c8871701221e42d11a96a21239366e59b9064cd2e52f46cbe4dd0a66b23b9f` |
| `X3_C_v1.38_app_keyD798_core.bin` | **只含核心双发、无任何 UI 补丁**（救砖后安全验证用；密钥模式=ID 菜单选 99） | `af5bd6dfbb6b19d981848adb80a48141b885c078c339e44506281c3a050e39e7` |
| `X3C_V1.38_app_keyD798_container.bin` | 主成品容器兜底（设备若是 V2.0/A1 才用） | — |
| `X3C_V1.38_app_keyD798_pergroup_container.bin` | 备选容器兜底 | — |

**注意：2026-11-07 实机测试事故**——最初的 UI 版（`95aac447…` / `61a14129…`）在 X3-C 上刷入后
**反复重启/自动关机**。根因：`ui_quickpanel_ext` 建磁贴时把 `r6` 当父窗口，但 0x080079E8 尾部处
`r6 = 0x200004C7`（RAM 字节指针），把 RAM 当控件结构写 → 内存破坏 → 看门狗复位。
已在 v1.2.3 修正：父窗口改用 `r5`（快捷面板根窗口，0x08007A06 写入后未再改），颜色暂存改用 `r6`；
新增桩测试护栏（真实 r5/r6 上下文 + canary，防再犯）。**旧 sha 全部作废，请只使用上表版本。**
**请先刷回原厂恢复设备**，再按"安全阶梯"测试：原厂 → `_core.bin`（只核心）→ `X3_C_v1.38_app_keyD798.bin`（v1.2.3 修正版 UI）。

### 9.6 刷机后异常（重启/关机）的恢复

**实机验证可用的救砖方法（2026-11-07 用户实测）**：
1. **先正常开机**（设备即使自动关机/重启，也先让它上电）；
2. **按一下侧面的旋转拨轮（旋钮按键）** —— G3 的 USB 状态变绿/识别到设备；
3. G3 → 打开文件 → 选 **原厂** `devices/x3c/firmware/X3_C_v1.38_app.bin` → 更新，等 100% 成功；
4. 断电重启，设备恢复原厂 V1.38（用户实测确认）。

补充说明：
- bootloader（0x08000000–0x08004800）不在我们补丁范围内，所以只要 G3 能识别到设备就能救回。
- 若上面的方法 G3 还是检测不到：换数据线/USB 2.0 口、重启 G3、拔电池再插 USB 重试；
  仍不行再试"关机 → 按住拨轮键 → 插 USB"；最后才考虑拆机 SWD。
- 恢复后第一次开机请确认版本号（设置 → 设备信息 = V1.38）。

**恢复后的安全阶梯**：原厂 → `_core.bin`（只核心双发、无 UI）→ 修正版 UI；每次只引入一个变量，
每步都先确认能正常开机再继续。

### 9.5 尚未完成（X3-C 后续）

- [ ] 实机验证后按结果微调：按下位 mask、广播 group/功率字段、造型灯方向（都可能一个常量改）；
- [ ] KEY_ON 的 **flash 持久化**（当前 RAM 版，重启复位=关；已定位设置页 0x080F7800 / 保存函数 0x080219E4）。

### 9.7 实机验证记录

| 日期 | 镜像 | 结果 |
|---|---|---|
| 2026-11-07 | 最初 UI 版（含 r6 父窗口 bug，sha `95aac447…`） | ❌ 反复重启/自动关机 → 用"开机后按旋转拨轮 + G3 刷原厂"救回（V1.38） |
| 2026-11-07 | **核心-only**（`_core.bin`，sha `af5bd6df…`，332 B，无 UI） | ✅ **正常开机、界面与原厂一致** → 证明"原位增量补丁固件"能被设备正常接受，之前的重启=UI 补丁 bug |
| 待测 | 修正版 UI（v1.2.3，sha `6cd8e823…`） | 等模拟器截图确认磁贴后再上实机 |

---

## 10. 文件地图

```
keypatch/
├─ keypatch.py           # 命令行工具（list / keygen / patch / patch-all / verify / info）
├─ selftest.py           # 总自检
├─ common.py             # 密钥校验/派生、固件读写
├─ container.py          # G3 V2.0 容器打包/校验库（build/verify，可被 studio 复用）
├─ KEY.example.txt       # 密钥模板（复制为 KEY.txt 使用；KEY.txt 被 gitignore）
├─ INTERFACE.md          # 设备模块接口 + devices 注册 API（二开看这个）
├─ DESIGN_TOGGLE.md      # v2 可开关设计说明
├─ devices/              # 每台设备的补丁模块（ad100 / ad200 / x3c）
│  └─ __init__.py        #   注册表：get() / list_devices()（studio 后端用）
├─ out/                  # 成品固件（含 *_container.bin）
└─ dev/                  # 逆向/验证过程材料
   ├─ ad100/ ad200/ x3c/ # 各设备 NOTES + 可复现脚本/日志
   ├─ g3/                # G3 升级工具审计
   ├─ x3c_ui/ lights_ui/ # 菜单开关/广播的 UI 可行性
   └─ lead/              # Lead 的独立验证/原型（Unicorn harness、广播注入）
```
