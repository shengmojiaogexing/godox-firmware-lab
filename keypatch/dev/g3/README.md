# keypatch/dev/g3 — G3 官方升级工具逆向审计（task-4）

本目录是"补丁 bin 能否被 G3 接受并刷入"的审计产物。**结论见 [FINDINGS.md](FINDINGS.md)**（一句话：
AD100Pro / AD200Pro / X3C 走 legacy 裸 bin 路径，G3 V2.0 对本地 .bin 无任何内容校验，原位补丁、长度不变可刷；
只有回 `AA 55 A1` 的 V2.0 设备才需要 container，用 `make_container.py` 打包）。

## 文件

| 文件/目录 | 说明 |
|---|---|
| `FINDINGS.md` | **主交付**：结论、证据（函数地址/反编译/字符串）、三台设备结论表、刷机步骤、风险、最小验证方案 |
| `g3_patch_check.py` | 模拟 G3 端刷写组帧/每块 32-bit 和；验证补丁只影响相关块、无整镜像校验 |
| `make_container.py` | 生成/校验 V2.0 设备专用容器（payload+MD5+文件名+u16L+magic；已用官方 iT32 包验证） |
| `installer_manifest.txt` | innoextract 解包清单（822 文件 / 117,417,683 B） |
| `evidence_patch_check.txt` | 三份固件的 patch check 输出（含每块和与组帧示例） |
| `pe_info.py` / `pe_analyze.py` | PE 结构/导入表/资源/导出分析 |
| `extract_strings.py` | 提取 ASCII/UTF-16 字符串（`strings_ascii.txt`、`strings_utf16.txt`） |
| `extract_qml.py` | 解出 Qt resource 里的 QML（`qml/`） |
| `parse_metaobject.py` / `meta2.py` | 解析 Qt5 metaobject（类名/信号/槽表，用于定位函数） |
| `scan_iap.py` / `scan_iap2.py` | 固件镜像 IAP 常量扫描（辅助，不是结论依据） |
| `qml/` | main.qml / Qml_DeviceSelect.qml / Qml_DownLoadProgress.qml（从 exe 解出） |
| `re/` | 关键函数反编译 C（0x4082e0/0x407990/0x407610/0x408073/…）、`disasm_key_funcs.txt`、functions.csv、strings.csv |
| `installer/` | 从 `vendor/G3_windows.zip` 解出的安装器本体（只读素材，不运行） |

## 复现要点

* 安装器是 Inno Setup **6.5.0**，官方 innoextract 1.9 解不动（loader revision 2）；
  用 innoextract 1.12-dev（UserUnknownFactor/innoextract_win release `670`）。
* Ghidra 12.2 + JDK25（`D:\jdk25`）对 PE 做标准分析，再用 `DumpAddr.java`/`ExportAll.java` 导出（命令见 FINDINGS §9）。
* 所有中间产物可重建，脚本都在本目录；`%TEMP%` 只用于安装包/官方固件下载，不在仓库内。

## 自检

```powershell
tools\python\python.exe keypatch\dev\g3\g3_patch_check.py --patch 0x100 6 `
    devices\ad100pro\firmware\AD100pro_V1.2.bin `
    devices\ad200pro\firmware\AD200pro_V1.60.bin `
    devices\x3c\firmware\X3_C_v1.38_app.bin
# 预期：三个都显示 "RAW app image (legacy path)"、size/CRC/version = NONE、补丁只改变受影响块的 sum
```

注：X3-C 镜像 1,021,468 B = 998 块（>0xFF），块号高 4 位由 cmd 字节高半字节编码（旧协议 cmd `0x04 | ((idx>>4)&0xF0)`），
`g3_patch_check.py` 已按此实现。
