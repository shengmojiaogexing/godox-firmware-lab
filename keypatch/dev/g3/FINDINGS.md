# FINDINGS.md — G3（Godox Firmware Updater V2.0）校验逻辑审计：补丁后的 app.bin 能否被接受并刷入

> 审计对象：`vendor/G3_windows.zip` → 唯一文件 `Godox_V2.0_setup.exe`
> （28,330,736 B，SHA256 `83A295938DB6E0D0007745B583DC1CE52F54A594313BAEA4C8CD1D207D5FD495`，
> Inno Setup **6.5.0 unicode** 安装包）
> 方法：纯静态（innoextract 解包 + strings/imports/PE 分析 + Ghidra 12.2 反编译 + Qt metaobject/QML 资源解析）。
> **未运行安装器、未连接/枚举任何真实 USB 设备。**
> 结论时间：2026-11-07。

---

## 0. 结论（TL;DR）

### 0.1 直接回答：把 `.bin` 中间几个字节原位改掉、长度不变，G3 会不会拒绝？

**对 AD100 Pro / AD200 Pro / X3-C 这类设备：不会拒绝，能刷入。**

G3 V2.0 对本地 `.bin`（"打开文件"路径）**没有任何文件内容校验**：

* 不检查文件扩展名之外的东西（文件对话框过滤 `*.bin`，选中后不校验内容）；
* **不检查文件长度**（`Config.xml` 里的 "文件大小不合格 / File size error" 文案在本版本代码里没有调用点）；
* **不检查型号/版本**（G3 从不读取设备当前固件版本，本地文件也没有版本字段）；
* **没有整镜像 CRC / MD5 / SHA / 签名 / 加密校验**；
* 唯一与文件内容有关的、会发给设备的值是**每 1024 字节块的 32 位字节累加和**，由 G3 在刷写时**根据（补丁后的）文件现算**并随块发送（`FUN_00407610` / `SendDataLooper` case 8）。因此补丁字节只改变所在块的校验和，G3 会重算，设备端按收到的数据重算也一致——**不可能成为拒绝来源**。

证据链见 §3、§4、§5。

### 0.1b 三台设备该给哪种文件（结论表）

| 设备（G3 `device_id`） | 官方现行包 | 判定 | 应交付形式 | 依据 |
|---|---|---|---|---|
| **AD100Pro** | `AD100Pro_V1.2.zip` → 50,248 B **裸镜像** | legacy（裸 bin 路径） | **裸 bin**（原位补丁即可） | 官方包无容器 magic；与本地样本 SHA256 相同（§6.3） |
| **AD200Pro** | `AD200Pro_V1.6.zip` → 46,380 B **裸镜像** | legacy（裸 bin 路径） | **裸 bin** | 同上（本地样本与官方包 SHA256 相同） |
| **X3C** | `X3C_V1.35_APP.zip` → 1,021,096 B **裸镜像** | legacy（裸 bin 路径） | **裸 bin** | 官方包无容器 magic；`X3_C_v1.38_app.bin` 同为裸镜像 |

> 交付建议：三款都先给**裸 bin**；同时备一份 `make_container.py` 打包的 container 作为"万一用户设备回 AA55A1"的备用件（见 §6.4、§7）。

### 0.2 唯一的例外：G3 内部存在"V2.0 设备"专用容器格式

G3 在设备连接时会发送 `AA 55 A0` 探测帧（2 次）。**如果设备回 `AA 55 A1`**，G3 会把设备标记为 "V2.0 device"（内部标志 `this+0x9a = 1`），此后**只接受"容器格式"的 .bin**：

```
payload（真正的固件镜像）
MD5(payload)                16 字节
文件名 UTF-8                L 字节（必须包含设备 ID 字符串）
L                           u16 小端
AA 55 66 99 77 88 22 33     8 字节 magic（文件末尾）
```

此时原版 raw .bin 和补丁 raw .bin **都会**在开刷前被拒绝，提示 "选择的固件与设备不匹配 / The selected firmware is not for this device"（错误码 10）。
**但这个路径不适用于上述三款设备的官方固件**：2026-11-07 从 godox.com 实际下载的官方现行包中，AD100Pro V1.2（50248 B）、AD200Pro V1.6（46380 B）、X3C V1.35（1021096 B）**全部是 raw app image，不含容器 magic**（见 §6）。容器格式只出现在最新一代产品（实测 iT32、XR、iT30ProC、V480C 的官方包有 magic；AD100Pro/AD200Pro/X3C/AD200ProII/V100C/P2400 无）。

### 0.3 剩余风险（G3 之外的）

* 设备端 bootloader 是否在写入完成后做整镜像验证（CRC/签名）——G3 协议**不传输**任何整镜像摘要，所以不会因补丁被 G3 拒绝；但若设备固件自身带自校验（镜像内 CRC/签名、启动校验），可能出现"刷入成功但校验失败/不启动"。**这属于固件补丁团队的风险，本审计只能确认 G3 端无此机制。**
* 若拿到的是"V2.0（A1）设备"，必须按 §6 重新打包容器（`make_container.py`），否则 G3 直接拒绝。

### 0.4 不确定项与最小验证方案

唯一无法离线 100% 确认的是：**手里这三台设备当前固件/引导是否会对 `AA 55 A0` 回 `AA 55 A1`**。三种低成本验证（按推荐顺序）：

1. **直接看 G3 行为**（最简单）：连设备 → "打开文件"选测试 .bin → 点"更新"。
   * 若立即弹 **"选择的固件与设备不匹配"**（还没开始进度）→ 设备是 A1/V2.0 设备，需要 §6 容器；
   * 若显示"打开文件成功"并进入进度条 → 走的是旧协议（raw bin），补丁无碍。
2. **USB 抓包确认**（USBPcap + Wireshark，另一台机器/虚拟机）：连接设备后看 G3 是否发出 65 字节 HID report `00 AA 55 A0 …`，以及设备是否回 `AA 55 A1 …`。有 A1 回包 = V2.0 设备。
3. **纯离线佐证**：官方现行包（§6 的 URL）是 raw ⇒ 厂商自己就是用 G3 V2.0 刷 raw 包，说明该设备族走旧协议。

---

## 1. 安装包解包（复现）

innoextract 1.9 官方版**解不动**（Inno Setup 6.5.0 的 setup loader revision 2 是 1.9 之后才支持的）：

```
Warning: Unexpected setup loader revision: 2
Warning: Setup loader checksum mismatch!
Could not determine setup data version!
```

改用 `UserUnknownFactor/innoextract_win` 的 **innoextract 1.12-dev + e561d8c（release `670`）**：
`https://github.com/UserUnknownFactor/innoextract_win/releases/download/670/innoextract670.zip`

```powershell
# 解包（输出到 %TEMP%\g3extract，避免污染工作区；本仓库只保留清单与文本证据）
Expand-Archive -Path 'vendor\G3_windows.zip' -DestinationPath installer -Force
& "$env:TEMP\innoextract670\innoextract.exe" -e --progress=no -d "$env:TEMP\g3extract" `
  "installer\Godox_V2.0_setup.exe"
# 清单：innoextract -l --list-sizes
```

结果：**822 个文件 / 117,417,683 字节**，全部在 `app\` 下。完整清单见：
[installer_manifest.txt](installer_manifest.txt)

关键文件：

| 路径 | 大小 | 说明 |
|---|---|---|
| `app\Godox V2.0.exe` | 652,288 | 主程序（Qt5/MinGW 32 位） |
| `app\Config\Config.xml` | 2,721 | 语言/文案/网络 JSON URL 配置 |
| `app\Config\G3_firmware_20241219.json` | 61,600 | 内置固件目录（与官网 JSON 同结构） |
| `app\Config\logo*.png / ON.png / OFF.png` | — | UI 资源 |
| `app\Qt5Core/Gui/Netdevices/x3c/Qml/Quick/Widgets/Svg…dll` | ~50 MB | Qt 5.15 动态库 |
| `app\libcrypto-1_1.dll / libssl-1_1.dll` | — | 用于 HTTPS 下载 JSON/固件 |
| `app\libgcc_s_dw2-1.dll / libstdc++-6.dll / libwinpthread-1.dll` | — | MinGW 运行库 |
| `app\platforms\qwindows.dll` 等 | — | Qt 插件 |

**没有任何 USB 驱动文件**：HID 访问用 **hidapi 静态链接进主程序**（见 §2）。

## 2. 主程序技术栈与结构

| 项目 | 结论 |
|---|---|
| PE | PE32 i386 GUI（machine 0x14c，subsystem 2），ImageBase 0x400000，GCC MinGW-W64 8.1.0 |
| 框架 | **Qt 5.15**（Qt5Core/Gui/Netdevices/x3c/Qml；QML 前端，`QQmlApplicationEngine`） |
| HID | **hidapi 静态链接**：主程序导出 `hid_enumerate/hid_open/hid_open_path/hid_write/hid_read/hid_read_timeout/hid_send_feature_report/...`（`.edata` 0x45a122 起） |
| XML | tinyxml2（读 `Config/Config.xml`） |
| ZIP | Qt5Gui 的 `QZipReader::extractAll`（官方 zip 包解压到 `Config/Temp/bin/`） |
| 反调试/加壳 | 无；明文 Qt 符号 + MinGW RTTI 名 |

主要类（由 Qt metaobject 解析，见 `parse_metaobject.py`/`meta2.py`）：

* **BackEnd**（metaobject @0x434080）：UI 后端
  * signals: sigOutputMsg, sigUsbConnectStatus, sigProgressValue, siglangageChange, **sig_SetFilePath**, **sig_StartUpdate**, sigSetCurSelectDevice, sigSetDetailMsg, …
  * slots: windowsClose(0x402820), **openFirwareFile(0x403040)**, fromUiEevent(0x402880), slot_ReceiveDeviceID(0x404e20), qml_LoadJsonFile(0x405650), qml_OpenNetFile(0x405d20), qml_TreeViewSelectDevice(0x403ab0), …
* **MainTask**（QThread，metaobject @0x4340b0）：刷写线程
  * signals: StartTask, sig_ChangeUsbConnectStatus, sig_ChangeProgressValue, sig_OutoutMsgToBackend, **sig_ReceiveV20DeviceID**
  * slots: **SendDataLooper(0x4082e0)**, **SetFilePath(0x407540)**, **StartUpdate(0x4075f0)**
  * `run()` = 0x408b30
* **TreeViewModel**（0x434098）：网络固件列表（Qt 树模型）
* **FileDownloader**（QNetworkAccessManager 封装，0x406960）

QML 资源（从 `.rdata` 内嵌 Qt resource 解出，脚本 `extract_qml.py`）：

* `qml/main.qml`（13,664 B，zlib 压缩）
* `qml/Qml_DeviceSelect.qml`（14,572 B，zlib；原注释 `// ModalWindow.qml`）
* `qml/Qml_DownLoadProgress.qml`（2,577 B，明文）

## 3. 文件选择与识别：G3 怎么选择/识别固件文件？

### 3.1 本地文件路径（用户实际使用）

`main.qml`（解出后 L430-441）：

```qml
FileDialog {
    id:fileDialog
    file: StandardPaths.standardLocations(StandardPaths.DesktopLocation)[0]
    nameFilters: ["Firmware Files (*.bin)"]      // 只过滤 *.bin
    onAccepted: { backend.openFirwareFile(fileDialog.file); }
}
```

`BackEnd::openFirwareFile` = **FUN_00403040**（0x403040，反编译见 `re/0x00403040_FUN_00403040.c`）：

1. `QFileInfo(file)` → `fileName()`
2. 与当前选中设备名拼一条日志字符串（0x435138 是分隔符字面量）
3. `FUN_004028d0(this, false, str)` —— 只是**往 UI 输出日志**（带 `[HH:MM:SS] ` 前缀，黑色）
4. `FUN_0041f480(this, fileInfo)` —— 发出 **`sig_SetFilePath`** 信号

**没有**大小/内容/版本/型号检查，没有读文件内容。
`MainTask::SetFilePath`（0x407540）只把 QUrl 转成本地路径存到 `this+8`，仅此而已。

### 3.2 网络文件路径

`main.qml`"网络固件"按钮 → `backend.qml_LoadJsonFile()`（FUN_00405650）→ 下载目录 JSON：

* `Config.xml` 内 `firmware_url` = `https://www.godox.com/Downloads/G3_firmwares.json`
* exe 内另有硬编码默认 URL（0x435f68）：`https://www.godox.com/Downloads/G3_firmware_20241205_v2.json`

JSON 结构（本地内置 `G3_firmware_20241219.json` 与线上 `G3_firmwares.json` 均已核对）：

```
{ "base_url": "/Downloads/G3_V2.0_bins",
  "Firmwares": [ { "device_id": "...", "version": "...", "release_date": "...",
                   "firmware_url": "XXX.zip", "update_contents": {...},
                   "update_contents_url": "...", "remark": {...},
                   "category1/2/3": {...} }, ... ] }
```

**JSON 中没有任何 size / md5 / sha / crc / hash / checksum 字段**（已遍历所有 key；线上 JSON 同样没有）。它只用于 UI 展示与下载 URL 拼接，**不参与本地/下载文件的校验**。

选择网络固件后：下载 zip → `QZipReader::extractAll` 到 `%appdir%\Config\Temp\bin\` → 把解出的 `.bin` 路径交给同一个 `FUN_00403040`/`sig_SetFilePath`（`FUN_00405d20` 反编译，见 `re/0x00405d20_FUN_00405d20.c`）：

* 判断后缀 `.zip`（0x43515e）→ `chop(4)` → 拼 `/Config/Temp/bin/` + 名字 + `.bin`（0x435163）
* 后缀是 `.bin` 直接使用

> **两条路径最终都是同一个 raw .bin 文件、同一个刷写流程**；JSON 只在网络路径里决定"下载哪个文件"，不做任何内容比对。

### 3.3 G3 里有没有"设备表 / 设备 ID → 是否 V2.0"的映射？（重要：没有）

对主程序做过全字符串检索：**`AD100`、`AD200`、`AD100Pro`、`AD200Pro`、`X3C` 等 device_id 在 exe 中一个都不存在**
（`strings_ascii.txt` / `strings_utf16.txt` 与原始二进制三处都查过；exe 里只有默认 JSON URL
`https://www.godox.com/Downloads/G3_firmware_20241205_v2.json` @0x435f68）。

* G3 **没有**硬编码设备型号表，也**没有**"哪些 device_id 属于 V2.0"的列表。
* "是否 V2.0 设备"**完全由运行时握手决定**：连接后发 `AA 55 A0`（两次），
  **只有设备回 `AA 55 A1` 才置 `this+0x9a = 1`**（0x407367-0x40737a），随后才启用容器解析 + 文件名/设备 ID 匹配。
* 设备型号（device_id）只存在于外部 JSON（网络固件列表 UI 用），与刷写校验无关。

因此：**无法从 exe 静态判定某台设备是否 V2.0**；判定只能靠
(a) 官方包格式（raw ⇒ 厂商自己就用 G3 V2.0 走旧协议，见 §6.3）或
(b) 实测连接握手（§0.4 的最小验证方案）。

### 3.4 支持哪些设备？

* 工具本身**不识别型号**：唯一的设备筛选是 `hid_enumerate(0x0483, 0x572B)`（见 §4.1），任何 VID 0x0483 / PID 0x572B 的 HID 设备都会被当成可刷设备。
* 型号信息只存在于 JSON 目录（用于 UI 展示/下载）和"V2.0 容器"路径（文件名必须包含设备 ID 字符串）。
* 内置 JSON（2024-12-19）与线上 JSON 都包含：**AD100Pro（V1.2）、AD200Pro（V1.6）、AD3xx/4xx/6xx/1200、X3C/X3N/X3S/X3F/X3O/X3L 等 70 条**。三款目标设备都在支持列表内。
* 注意 AD200Pro 还有 `AD200Pro_A`（"for Version A" 硬件版本）；网络路径会显示两个条目，但**本地文件路径不检查硬件版本**，选错文件 G3 也不会拦。

## 4. 刷写协议（G3 端，HID）

### 4.1 设备枚举 / 连接

* `FUN_00408073` / `FUN_00408060`（0x408073 / 0x408060，设备轮询）：
  * `hid_enumerate(0x0483, 0x572B)` —— 指令证据：0x408093 `mov dword [esp+4],0x572b`、0x40809b `mov dword [esp],0x483`、0x4080a2 `call hid_enumerate`。
  * 比对 device path，`hid_open_path()` 打开；随后 `FUN_00407ff0` 发送握手。
* 握手：`FUN_00407ff0`（0x407ff0）调用 `FUN_00407610(0xA0, NULL, 0, buf)` 并 **`hid_write` 两次**。
* 响应读取：`MainTask::run()`（0x408b30）里 `startTimer(1)`，定时器回调（vtable 槽 0x4072b0，未被 Ghidra 识别为函数；反汇编在 `re/disasm_key_funcs.txt`）做 `hid_read_timeout(handle, buf, 0x41, 1)`：
  * 校验 `buf[0]==0xAA && buf[1]==0x55`（0x4072e5-0x4072f2）
  * 校验 60 字节和（0x407310-0x40734a）
  * 若 `buf[2] == 0xA1`：**`this+0x9a = 1`（V2.0 标志）**，把响应 payload 前 32 字节拷到 `this+0x9b`，
    当字符串（`QString::fromUtf8`）存到 `this+0xbc`，并发出 `sig_ReceiveV20DeviceID`（0x407367-0x407479）
  * 否则（任何其它 cmd）：置 ack 标志 `this+0xc0/+0xc1`（0x407352-0x407365）

### 4.2 HID report 帧格式（`FUN_00407610` @0x407610，65 字节）

```
offset 0 : 0x00                HID report ID
offset 1 : 0xAA
offset 2 : 0x55
offset 3 : cmd                 (0xA0/0x00/0x02/0x04/0x08)
offset 4 : payload (0..N)
offset 61..64 : BE32( sum(frame[1..60]) )   // 60 字节累加和，uint32 溢出回绕
```

设备→G3 的响应为去掉 report ID 的同构帧：`AA 55 (cmd|0x01) payload … BE32(sum)`。

### 4.3 状态机（`SendDataLooper` = FUN_004082e0 @0x4082e0，状态字 `this+0xc4`，跳表 @0x43518c）

| state | 动作 | 地址 |
|---|---|---|
| 0 | 进度归零；`this+0x14==0`（未连接）→ 错误 6；文件路径为空 → 错误 7 | 0x40846d |
| 1 | `QFile::setFileName(this+8)` + `QFile::open(ReadOnly)`；失败 → 错误 0 | 0x4084cb |
| 2 | 调 `FUN_00407990` 解析"容器"；**若 `this+0x9a==0` 直接跳过（失败无所谓）**；计算 payload 长度与块数；先发一个 **65 字节全 0** report（0x4082e0 反编译 L78-89） | 0x408508 |
| 3 | 等待 10 个 tick | 0x40857a |
| 4 | `cmd 0x00`（空 payload）→ hid_write；等 ack | 0x4085a4 |
| 5 | 等 ack（`this+0xc0`） | 0x408609 |
| 6 | `cmd 0x02`（空 payload）→ hid_write；等 ack | 0x408625 |
| 7 | 等 ack（`this+0xc1`），计数清零 | 0x4087cc |
| 8 | **按 1024 B 块发送**：发块头（cmd 4 或 8，见 §4.4），再 16×64 B 数据（每包 65 B report）；发满 1024 B 后块号 +1、清 ack、进 9 | 0x40868a |
| 9 | 等 ack（`this+0xc2`）→ 进度 = 块号/总块数；未发完回 8，发完进 10 | 0x40876f |
| 10 | 关文件；输出消息 9（"更新设备成功"） | 0x408440 |

关键代码点：**state 2 的旧协议容忍**（0x408523）：

```asm
0040851e  call 0x407990                     ; 解析容器（raw bin 会返回 0）
00408523  cmp  byte ptr [ebx + 0x9a], 0     ; V2.0 标志？
0040852a  je   0x408802                     ; ==0 → 跳过解析/匹配，直接按整文件 payload 处理
00408530  test al, al                       ; ==1 时：
00408532  je   0x408564                     ;   解析失败 → 错误 10
00408534  lea  eax, [ebx + 0xbc]            ;   设备 ID 字符串
00408542  mov  ecx, 0x459058                ;   解析出的文件名 QString
00408552  call [0x45b970]                   ;   QString::indexOf(deviceID)
0040855b  cmp  eax, -1
0040855e  jne  0x408802                     ;   找不到 → 错误 10
00408564  ... FUN_00420980(0x0A)            ; 消息 10 = "选择的固件与设备不匹配"
```

### 4.4 每 1024 字节块的发送格式（核心：校验和是 G3 现算的）

`state 8` 的反汇编（0x40868a-0x4089e0，节选）：

* 从文件读最多 0x400（1024）字节（`QIODevice::read`）；
* **计算 32 位累加和**：`sum = Σ byte[i] (i<1024)`，**超出文件末尾的字节按 0xFF 计**（0x408700 `add edi,0xFF`；0x40871a 与 payload 长度比较）；
* **旧协议**（`this+0x9a==0`，0x408891 分支）：
  * `cmd = 0x04`；若总块数 > 0xFF，则 `cmd = 0x04 | ((blockIndex >> 4) & 0xF0)`
  * payload 5 字节 = `[blockIndex & 0xFF]` + `BE32(sum)`
  * `FUN_00407610(cmd, payload, 5, buf)` → `hid_write(0x41)`（0x4088e3/0x408903）
* **V2.0**（0x408a2c 分支）：
  * `cmd = 0x08`；payload 8 字节 = `BE32(blockIndex)` + `BE32(sum)`
* 随后 16 次 `hid_write(0x41)` 发送该块 1024 字节（每包 64 字节数据，见 0x408920-0x4089a1）。

> **没有任何"整镜像大小/版本/CRC/签名"随 cmd 0x00/0x02 发送**（两者 payload 长度都是 0）；
> 也**没有**在最后发送"整镜像摘要"——state 10 只是关文件。唯一与文件内容相关的传输值 = 每块 sum，且它由 G3 从"要发送的文件内容"现算。

### 4.5 设备侧旁证：X3-C app 镜像里的 USB/IAP 痕迹（不影响 G3 端结论）

* `X3_C_v1.38_app.bin` 内含 USB 字符串（文件偏移 → VA，基址 0x08004800）：
  * `" Godox"` @file 0x1d80f → VA 0x0802200f
  * `" HID IAP Interface"` @file 0x1d847 → VA 0x08022047
  * `" Godox_IAP"` @file 0x1d8af → VA 0x080220af
* 同镜像 file 0x0e85ca 处有一份 USB 描述符数据，包含 **VID 0x0483 / PID 0x572B**（字节 `83 04 2b 57`），
  与 G3 的 `hid_enumerate(0x0483, 0x572B)` 完全一致 → 证实 G3 的 HID 目标就是该设备的 IAP 接口
  （更准确地说：G3 只按 VID/PID 枚举，完全不做型号识别，见 §3.3）。
* AD100Pro / AD200Pro 的 app 镜像里没有这些字符串（其 IAP/USB 在 bootloader 或另一颗 MCU 上），
  但这不影响刷写路径（G3 只按 VID/PID 找 HID，旧协议数据格式相同）。
* X3-C 镜像内**没有** V2.0 容器 magic `AA55669977882233`，也没有找到 `0xA0/0xA1` 命令处理的明显证据——
  与"X3-C 走旧协议"的判断一致（但设备端 IAP 具体实现无法仅凭 app 镜像 100% 确认）。

## 5. 校验点逐条回答（问题清单）

| 问题 | 结论 | 证据 |
|---|---|---|
| 选择文件时校验？ | **不校验**，只判断能打开 | 0x403040 只 `QFileInfo`+发信号；state 1 才 `QFile::open` |
| 文件大小/长度检查？ | **没有**（"文件大小不合格"文案无调用点） | 全程序 `FUN_00402c90(msgIdx)` 调用点只有 0xB（网络错误）及 MainTask 消息 0/4/5/6/7/8/9/10；无 1/2/3 |
| 版本号检查（不得低于/等于当前）？ | **没有**。G3 从不读取设备当前版本；本地 bin 无版本字段 | 协议只有 A0/A1/00/02/04/08，无版本读写命令 |
| 型号匹配？ | 旧协议：**不匹配**（任何 0483:572B 设备 + 任何 .bin 都能开刷）；V2.0：文件名必须包含设备 ID | 0x408523 分支；0x408534-0x40855e |
| CRC/checksum/签名/加密？ | **无整镜像校验**；只有每块的 32 位累加和（工具现算） | §4.4 |
| 校验发生在何时？ | 旧协议：无；V2.0：**开刷前**解析容器（MD5 校验 payload+文件名匹配） | 0x408508 state 2 |
| 写入后由设备确认？ | 每块/每命令有 ack（`+0xc0/0xc1/0xc2`），但 G3 不比较任何整体校验值 | §4.3 |
| 中间 6 字节改掉、长度不变？ | **旧协议：接受并刷入**；V2.0 容器：拒绝 | §0.1/§0.2 |
| 隐藏参数强制升级？ | **没有**。exe 无命令行参数分支（`GetCommandLineW`/`CommandLineToArgvW` 仅 0x422260 启动代码）；无 force/-f 字符串 | strings 全表检索 + main 逻辑 |

### 5.1 "没有整体 size/CRC/version 校验"的完整调用链证据（裸 bin 路径）

1. **文件 → 刷写线程**：`BackEnd::openFirwareFile`(0x403040) 只 `QFileInfo` + 发 `sig_SetFilePath` →
   `MainTask::SetFilePath`(0x407540) 只把路径存到 `this+8`。**没有打开/读取/检查内容。**
2. **state 0**（0x40846d）：只检查 HID 句柄非空（否则错误 6）和路径非空（否则错误 7）。
3. **state 1**（0x4084cb）：`QFile::open(ReadOnly)`，失败才报错 0。**不检查大小/头/版本。**
4. **state 2**（0x408508）：
   * 调 `FUN_00407990` 解析容器；**`this+0x9a==0`（非 V2.0）时 0x40852a `je 0x408802` 直接跳走**，
     解析失败被完全忽略，`local_68`（footer 长度）保持 0 → **整个文件都算 payload**；
   * 0x408802：`QFile::size()`（0x45b954 虚调用）→ `payload = filesize - 0`；
     `[0x434008] = payload`，`[0x45905c] = (payload+0x3FF)>>10`（块数）。**size 只用于算块数，不做任何比对**；
   * 随后 `rep stosd` 清零 65 字节缓冲并 `hid_write` 一个**全 0 report**（0x408837-0x408859）。
5. **state 4/6**（0x4085a4 / 0x408625）：`FUN_00407610(cmd=0x00 / 0x02, NULL, 0, buf)` —— **payload 长度为 0**，
   即启动/擦除命令不携带任何大小、块数、版本或整镜像摘要。
6. **state 8**（0x40868a）：唯一的"内容相关"计算 = 每 1024 B 块的 `sum`（§4.4），
   该 sum 由 `QIODevice::read` 读到的**当前文件内容**现算（含超出 EOF 按 0xFF 的规则）。
7. **state 10**（0x408440）：`QFileDevice::close()` + 消息 9（成功）。**没有结束命令、没有尾部摘要。**

### 5.1b 长度是否必须等于官方包长度？（问题：支持任意长度）

**G3 对文件长度没有任何要求**（不要求等于官方包大小，也没有 min/max 检查）：

* 长度只被用来算两个数：`payload = filesize - footer`（裸 bin 时 footer=0）和
  `块数 = (payload + 0x3FF) >> 10`（0x408819-0x40882d）；
* 最后一块不足 1024 B 时，**超出的字节按 0xFF 补齐后参与 sum 计算**（0x408700），并照样发送；
* 旧协议块号编码支持最多 4095 块（cmd 高半字节 + 低 8 位，即 ≤ ~4 MB），
  V2.0 用 32-bit 块号（无实际限制）；X3-C 1,021,468 B = 998 块，仍在旧协议范围内；
* 空文件/极短文件也能"发送"（会发 1 个 0xFF 填充块后直接成功）——仅说明无长度校验，不代表设备接受。

> 对本项目：全部是**等长原位补丁**，块数/块边界都不变，因此这一点不构成任何风险。


其他佐证：
* 全程序 `QCryptographicHash`（MD5/SHA）**只被容器解析函数 0x407990 调用**；裸 bin 路径不经过它。
* 全程序 `QFile::size()` 只有 **0x408802 一处**调用点（见上，仅算块数）。
* 不存在任何"版本比较"或 "force/-f/--force" 字符串/分支。

### 5.1c G3 是否"逐块发送全部块"（含全 0xFF 块）？——**是，无跳过/优化/截断**

针对"把注入代码写到原厂镜像尾部 0xFF 空闲区"这一方案，逐问确认（全部来自 0x4082e0 反汇编）：

1. **是否逐块发送全部块（包括内容全 0xFF 的块）？→ 是，没有跳过分支。**
   * 块头/块数据发送在 state 8；每个块固定发 1 个块头 + 16 个数据包（16×64 = 1024 B）：
     块头 `0x408891`-`0x408903`（`FUN_00407610` + `hid_write`），
     16 包循环 `0x408920`-`0x4089a1`（`esi=1..0x40`，`cmp esi,0x41` 循环，`0x408340` 处
     `hid_write` 65 B 包、`add [ebp-0x7c],0x40`、`cmp eax,0x400; jne 0x408920` 继续下一包）。
   * 一包发满 1024 B 后：`0x40837b` `add dword [0x459078],1`（块号 +1）→ 清 ack → 进 state 9。
   * state 9（`0x40876f` → ack 后 `0x408a56`）只比较 **块号 vs 总块数**：
     `0x408aa7` 取总块数 `[0x45905c]`，`0x408ab1` `jae 0x4084fc`（块号 ≥ 总块数 → 结束），
     否则 `0x408ab7` `sub dword [ebx+0xc4],1`（回到 state 8 继续下一块）。
   * **整个发送路径没有任何 `cmp 块内容, 0xFF` / `memcmp` / "全 FF 跳过" 分支**；
     唯一的 `cmp ...,0xff` 是 `0x4088a6` `cmp [0x45905c],0xff`（判断块数 >255 以决定 cmd 高半字节编码，与内容无关）。
2. **发给设备的是文件原始字节吗？→ 是，逐字节原样发送。**
   * 数据包填充循环：`0x408949` 取数据字节位置 `[0x459074]`，若 `< payload 长度 [0x434008]` 则
     `0x40898e-0x408996` `movzx eax, byte [文件读缓冲+offset]` → `mov byte [报文的第 esi 字节], al`
     —— **无变换、无加密、无字节序处理**（只有块号和校验和用 BE）。
   * 只有"超出 payload 长度"的尾部字节才写 `0xFF`（`0x408930` `mov byte [ebx+esi+0x18],0xff`；
     以及 sum 计算循环 `0x408700` `add edi,0xff`），这是 EOF 填充，不是内容过滤。
3. **有没有"只发送到某个长度/只发非 FF 范围"的逻辑？→ 没有。**
   * payload 长度 `[0x434008]` 只用于两处：`块数 [0x45905c] = (payload+0x3FF)>>10`（`0x408819-0x40882d`）
     和 EOF 后的 0xFF 填充；发送循环固定每块 1024 B、块数固定迭代完。
   * 因此 G3 侧不会因为"注入区是 0xFF 空闲区"而跳过它；G3 会把注入区当普通 payload 原样发送。
   * 设备端是否因块内容/长度而跳过写入，**G3 侧不可见**（G3 只按块发送、按 ack 推进）；若设备端
     bootloader 对全 FF 块做"跳过/不擦"优化，那属于设备端行为，需真机/固件分析确认（G3 侧无此逻辑）。
4. **容器路径下**：payload 长度 = `filesize - footer`（footer = L + 26，§6.1），块数按该长度算；
   只要注入区位于 footer 之前（正常情形），就落在 payload 内并会被发送。X3-C 1,021,468 B = 998 块，
   注入区 file 0x0E9800 = 块号 0x3A6（第 935 块，块号从 0 起），旧协议 cmd = `4 | ((0x3A6>>4)&0xF0) = 0x34`，
   payload = `[0xA6][BE32(sum)]` → 与普通块完全相同地发送。

### 5.2 本地文件路径 vs 网络下载路径：写入前行为是否一致？

**一致。** 两条路径在"开始刷写"之前汇合到同一处：

| 步骤 | 本地文件 | 网络文件 |
|---|---|---|
| 入口 | `main.qml` FileDialog → `backend.openFirwareFile(file)` | `Qml_DeviceSelect` 选固件 → 下载 zip → `QZipReader::extractAll` 到 `Config/Temp/bin/` |
| 汇合 | **`BackEnd::openFirwareFile`(0x403040)** → `sig_SetFilePath` | 同样调用 **`FUN_00403040`(0x403040)** → `sig_SetFilePath`（见 0x405d20 反编译 L346 附近） |
| 刷写 | `MainTask::SetFilePath` → `SendDataLooper` 状态机 | **同一个**状态机 |
| 容器解析 | state 2 同一个 `FUN_00407990` | 同一个 |
| per-block sum | state 8 同一个算法（从文件现算） | 同一个 |

差别只在"文件从哪来"：网络路径多了"下载 + 解 zip"两步，JSON 的 `version/device_id` 只用于 UI 展示与选取下载 URL，
**不参与**任何比对；本地路径完全不读 JSON。两者最终交给刷写线程的都是一个 `.bin` 文件路径。


## 6. "V2.0 容器"格式（若设备是 A1/V2.0）

### 6.1 解析实现 `FUN_00407990`（0x407990）

```
if filesize < 10: fail
tail10 = read(filesize-10, 10)          // [L_lo][L_hi][AA 55 66 99 77 88 22 33]
if tail10[2..9] != AA 55 66 99 77 88 22 33: fail
L = u16le(tail10[0..1])
if filesize < L + 26: fail
expected = read(filesize-L-26, 16)       // MD5
md5 = MD5( read(0, filesize-L-26) )      // 分块，每块 ≤0x2000 字节
if md5 != expected: fail
name = read(filesize-L-10, L)            // QString::fromUtf8
*out_footer = L + 26                     // payload = filesize - (L+26)
return success
```
（算法已实测：官方 `iT32_V1.18.bin` 的 16 字节摘要 == `MD5(payload)`；见 §6.3）

### 6.2 与 §4.3 的配合

* state 2：payload 长度 = `QFile::size() - footer`，块数 = `ceil(payload/1024)`；
* 只有 payload 被刷入，footer（MD5+文件名+L+magic）**不会**被写入设备；
* 设备 ID 匹配：`QString(文件名).indexOf(设备 ID) != -1`。

### 6.3 实测官方包（2026-11-07 从 godox.com 下载，URL 前缀 `https://www.godox.com/Downloads/G3_V2.0_bins/`）

| 包 | .bin 大小 | 末尾 magic | 类型 |
|---|---:|---|---|
| `AD100Pro_V1.2.zip` | 50,248 | 无 | **raw**（与本地 `AD100pro_V1.2.bin` SHA256 相同：`3a3d3b91…`） |
| `AD200Pro_V1.6.zip` | 46,380 | 无 | **raw**（与本地 `AD200pro_V1.60.bin` SHA256 相同：`fc0f77f1…`） |
| `X3C_V1.35_APP.zip` | 1,021,096 | 无 | **raw** |
| `AD200ProII_V1.03.zip` | 372,004 | 无 | raw |
| `V100C_V1.11.zip` | 1,007,224 | 无 | raw |
| `P2400_GODOX_V2.7.zip` | 86,340 | 无 | raw |
| `iT32_V1.18.zip` | 921,640 | **有**（offset 0xe1020） | **容器**（MD5 验证通过，name=`iT32_V1.18.bin`，L=14） |
| `XR_V1.00.zip` | 62,338 | 有 | 容器 |
| `iT30ProC_V1.02.zip` | 688,172 | 有 | 容器 |
| `V480C_V1.03.zip` | 753,705 | 有 | 容器 |

> 结论：**AD100 Pro / AD200 Pro / X3-C 属于 raw（旧协议）一族**——厂商现行 G3 V2.0 官方包本身就是 raw .bin。

### 6.4 若遇到 A1/V2.0 设备：重新打包

`make_container.py`：

```powershell
tools\python\python.exe keypatch\dev\g3\make_container.py `
    patched_app.bin out\patched_app_container.bin --name "AD100Pro_V1.2.bin" --device-id AD100Pro
tools\python\python.exe keypatch\dev\g3\make_container.py --verify out\patched_app_container.bin
```

**嵌入文件名与设备 ID 的精确规则（来自 G3 反编译，非猜测）：**

* 设备 ID 来源：G3 收到 `AA 55 A1` 响应后，把响应帧 **payload 的前 32 字节**（接收缓冲 +0x5c..+0x7b）
  拷到 `this+0x9b`（0x407373-0x4073b6），然后调用
  `QString::fromUtf8_helper(ptr=this+0x9b, len=strnlen(ptr, 32))`（0x40741e-0x40742c，
  IAT 0x45b7f0 = `QString::fromUtf8_helper`）→ 即**payload 里第一个 NUL 之前的 UTF-8 字符串，最长 32 字节**，
  **原样使用：不做大小写转换、不 trim、不去空格**。
* 写回 `this+0xbc`（0x407435-0x40743b），并发出 `sig_ReceiveV20DeviceID`（0x407474 调 0x4209c0）；
  BackEnd 的 `slot_ReceiveDeviceID`(0x404e20) 只是接收它做 UI 搜索。
* 匹配：容器解析出的文件名 QString 上调用
  `QString::indexOf(deviceID, from=0, cs)`，**cs = 1 = `Qt::CaseSensitive`**（0x40853a `mov [esp+8],1`；
  this = 解析出的文件名 0x459058，参数 = `this+0xbc` 设备 ID）。
  → **嵌入文件名必须原样包含设备 ID 子串（区分大小写）**。
* 因此：如果设备上报的 ID 就是 JSON/catalog 的 `device_id`（`AD100Pro` / `AD200Pro` / `X3C`），
  那么用官方包内文件名最稳：`AD100Pro_V1.2.bin`、`AD200Pro_V1.6.bin`、`X3C_V1.35_APP.bin`（都包含对应 device_id）。
* 若设备上报格式不同（例如带空格 `AD100 Pro`、带版本 `AD100Pro V1.2`、或小写），
  必须以 USB 抓包看到的 A1 payload 为准，文件名严格照抄该子串（建议同时准备几个候选文件名测试）。
  这也是本审计唯一无法离线确认的字符串（只有真实设备会告诉我们）。

## 7. 用户设备类型判断与文件选择（交付前必读）

### 7.1 决策流程

```
连接设备，打开 G3 → 显示"连接设备成功"
   │
   ├─ 选裸 bin（补丁件）→ 点"更新"
   │      ├─ 直接开始进度条 → ✅ 旧协议，裸 bin 可用（本文结论：三台设备默认走这里）
   │      └─ 立即弹"选择的固件与设备不匹配" → 设备是 V2.0(A1) 型
   │              → 改用 make_container.py 打包的 container 再刷（§6.4）
   │
   └─ （可选）USBPcap 抓包：G3 连接后发 00 AA 55 A0 …，设备若回 AA 55 A1 … 即 V2.0 型
```

判定要点：
* **报错时机**：V2.0 拒绝发生在 **state 2（开刷前、还没发过任何数据块）**，日志会立刻出现
  "选择的固件与设备不匹配"；旧协议会在打印"打开文件成功"后进入 0→100% 进度。
* **官方包反证**：设备对应的官方 zip 里如果是裸 bin（本三台都是），该设备族就是旧协议（§6.3）。
* EXE 里没有设备表可查（§3.3），所以不要试图从 G3 版本/型号字符串判断。

### 7.2 交付形式

* **首选**：裸补丁 bin（原位补丁、长度不变）。
* **备用**：`make_container.py --name "<官方包内文件名，含 device_id>"` 打出的 container；
  与裸 bin 一起交付，用户按 §7.1 的提示二选一。

### 7.3 推荐刷机步骤（旧协议 / raw bin）

1. 断电，按官方说明让灯/引闪器进入升级模式（一般是按住某键上电进入 bootloader；以说明书为准——AD100Pro 说明书只说用 G3 打开固件文件）。
2. 用 USB 线连接电脑，打开 G3（`Godox V2.0.exe`）。
3. 等 G3 显示"连接设备成功"（HID 0x0483:0x572B 枚举成功）。
4. "打开文件" → 选**补丁后的 .bin**（注意：不要选 .zip；本地只接受 .bin）。
5. 点"更新"，等进度到 100% 显示"更新设备成功"。
6. 断电重启设备，检查版本/功能。

风险提示：
* 刷写过程中**不要拔线/断电**（擦除后中断可能变砖；G3 有 5000 tick 超时/错误处理但不保证可恢复）。
* 若同一设备之前已被刷成"V2.0 固件"（会回 A1），再刷 raw 补丁包会被拒 → 用 §6.4 重新打包。
* 设备侧自校验（镜像内签名/CRC）可能使补丁固件"刷入成功但启动失败"——固件团队需检查镜像尾部/头部的自校验字段（AD200Pro 镜像尾部 0xC8 填充块、AD100Pro 尾部代码区值得复核）。

## 8. B 计划（若 G3 拒绝）可行性

1. **自写 USB HID 刷写工具（最可行）**
   * 协议已被完整还原：VID/PID 0x0483:0x572B、65 字节 report、`AA 55 cmd payload BE32(sum)`、cmd 0x00/0x02/0x04/0x08/0xA0、1024 B 块、每块 sum。
   * 用 python + hidapi 即可实现（`hid`/`hidapi` 包），把 §4.4 的组帧算法 + 状态机照搬；风险是设备侧对时序/重试的细节要求（G3 用 1 ms 定时器轮询 + 5000 tick 超时）。
   * 优点：完全绕过 G3 的任何校验（本来也没有）。
2. **SWD 直刷（备选）**
   * AD100 Pro / AD200 Pro / X3-C 都是 STM32 类 MCU（X3-C 是 STM32F4 类，App 基址 0x08004800；AD100/AD200 基址 0x08003000），有 SWD 引脚即可用 ST-Link 直写 Flash。
   * 需要拆机、找 SWD 焊盘（通常需要短接/解锁读保护），且要处理 option bytes；比 HID 方案风险大，仅在 HID 路线失败时考虑。
3. **降级/旧版 G3**：无必要（G3 本身不校验）；若设备是 A1 型，旧版 G3 不认识 A0 握手反而会用旧协议——可作为备选（未验证旧版 G3 是否兼容这三款设备的 bootloader）。

---

## 9. 复现与脚本

| 脚本 | 作用 |
|---|---|
| `pe_info.py` / `pe_analyze.py` | 安装包/程序 PE 结构、导入表、资源、导出 |
| `extract_strings.py` | ASCII/UTF-16 字符串 + VA 映射（`re/strings_ascii.txt`, `re/strings_utf16.txt`） |
| `extract_qml.py` | 从 `.rdata` Qt resource 解出 QML（zlib + 明文） |
| `parse_metaobject.py` / `meta2.py` | 解析 Qt5 metaobject：类名/信号/槽/属性表（用于定位 BackEnd/MainTask 函数） |
| `scan_iap.py` / `scan_iap2.py` | 在固件镜像里扫 IAP 协议常量（辅助） |
| `g3_patch_check.py` | **模拟 G3 端组帧/每块校验和**，证明补丁不影响 G3 决策 |
| `make_container.py` | 生成/校验 V2.0 容器（需要时） |

Ghidra 复现（PE 自动分析 + 反编译导出）：

```powershell
$env:JAVA_HOME='D:\jdk25'; $env:PATH="D:\jdk25\bin;$env:PATH"
copy "<解包>\app\Godox V2.0.exe" "$env:TEMP\g3audit\GodoxV2.exe"
& D:\ghidra_12.2_PUBLIC\support\analyzeHeadless.bat "$env:TEMP\g3proj" G3PE `
    -import "$env:TEMP\g3audit\GodoxV2.exe" -overwrite -scriptPath D:\ghidra_scripts `
    -postScript "ExportAll.java $env:TEMP\g3export"
# 需要更多函数时：
& D:\ghidra_12.2_PUBLIC\support\analyzeHeadless.bat "$env:TEMP\g3proj" G3PE `
    -process GodoxV2.exe -noanalysis -readOnly -scriptPath D:\ghidra_scripts `
    -postScript DumpAddr.java out.txt 0x4082e0 0x407990
```

`re/` 目录保存了关键函数的反编译文本与反汇编、`functions.csv`/`strings.csv`。

---

## 10. 附录：关键字符串 / 地址速查

| 字符串（VA） | 含义 |
|---|---|
| `Firmware Files (*.bin)` @0x446480（UTF-16，QML 属性表） | 本地文件对话框过滤 |
| `openFirwareFile` @0x4464c8 | QML→BackEnd 槽名 |
| `/Config/Temp/` @0x4350e4、`/Config/Temp/bin/` @0x43514c、`.zip` @0x43515e、`.bin` @0x435163 | 网络包解压路径 |
| `https://www.godox.com/Downloads/G3_firmware_20241205_v2.json` @0x435f68 | 内置 JSON URL |
| `device_id` @0x43636c、`version` @0x436376、`firmware_url` @0x43638b | JSON 字段（无 size/md5） |
| `sig_ReceiveV20DeviceID` @0x44dbf9 附近、`slot_ReceiveDeviceID` @0x44d6d5 | V2.0 设备 ID 信号 |
| `选择的固件与设备不匹配` / `The selected firmware is not for this device` | `Config.xml` 消息 10 |

| 函数 | VA | 作用 |
|---|---|---|
| `BackEnd::openFirwareFile` | 0x403040 | 本地文件入口（无校验） |
| `BackEnd::logMsg` | 0x4028d0 | UI 日志（时间戳+颜色） |
| `MainTask::SetFilePath` | 0x407540 | 保存路径 |
| `MainTask::StartUpdate` | 0x4075f0 | 置启动标志 |
| `MainTask::run` | 0x408b30 | 线程主循环（1 ms 定时器） |
| HID 读回调（未命名） | 0x4072b0 | `hid_read_timeout` + AA55/和校验 + A1 判定 |
| `SendDataLooper` | 0x4082e0 | 状态机（跳表 0x43518c） |
| 帧构造 `FUN_00407610` | 0x407610 | AA55 cmd payload sum |
| 容器解析 `FUN_00407990` | 0x407990 | MD5 + 文件名 + magic |
| 设备轮询 / 打开 | 0x408073 / 0x408060 | hid_enumerate(0x0483,0x572B) / open_path |
| 握手 | 0x407ff0 | cmd 0xA0 ×2 |
| 消息发射（MainTask） | 0x4208c0/0x4208f0/0x420940/0x420980/0x4209c0 | BackEnd 信号 |
| BackEnd/MainTask/TreeModel moc | 0x41f790 / 0x420a00 / 0x4206b0 | 槽分发表 |

---

## 11. 交付影响（给 keypatch 主流程的建议）

1. **三台设备都按裸 bin 交付**（AD100Pro / AD200Pro / X3C 的官方包都是裸镜像，§6.3），
   原位补丁**不需要**改文件名/版本号/CRC，不需要旧版 G3/force 参数，直接"打开文件"刷。
2. 同时用 `make_container.py` 各打一份 container 备用；README 写明 §7.1 的"看报错二选一"流程。
3. 交付自检建议：对每个成品跑 `g3_patch_check.py <bin> --patch <off> 6`，确认
   * 文件长度不变；
   * 不含容器 magic（裸 bin）或 container MD5 正确；
   * 只有受影响块的 sum 变化（G3 会重算）。
4. 固件团队需另行确认设备端是否存在整镜像自校验（§0.3）——这是本项目真正的风险点，**不在 G3**。
5. 若未来拿到 A1/V2.0 型设备（新固件会回 `AA 55 A1`），改完固件后用 container 交付；
   设备 ID 字符串可用 USBPcap 抓 `AA 55 A1` 响应前若干字节确认（或直接用官方包内文件名中的型号段）。

