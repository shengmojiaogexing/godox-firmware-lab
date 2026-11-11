# AD300 Pro 逆向文档索引

本目录存放 AD300 Pro V1.30 的静态逆向资料。**注：本设备没有整机 Unicorn 模拟器**，
本轮只做到「静态 + Unicorn 调用级」验证。

| 文档 / 脚本 | 内容 |
|---|---|
| [`../README.md`](../README.md) | 设备档案总览：固件速查、同步字函数、C.Fn `F8` 语义、11 处补丁点、风险与产物 |
| [`../firmware/README.md`](../firmware/README.md) | 原厂固件放置位置、sha256、版本自查 |
| [`keypatch/dev/ad300/NOTES.md`](../../../keypatch/dev/ad300/NOTES.md) | 逆向工作笔记：定位过程、`scan*.py` 脚本说明、settings/开关/持久化证据 |
| [`keypatch/dev/ad300/AUDIT.md`](../../../keypatch/dev/ad300/AUDIT.md) | 独立复核报告（逐项 PASS/FAIL + 证据） |
| [`keypatch/dev/ad300/audit_cfn.py`](../../../keypatch/dev/ad300/audit_cfn.py) | 一键复跑全部静态证据的脚本 |

## 关键结论（速查）

- 基址 `0x08003000`；初始 SP=`0x200008F8`，Reset=`0x080030D5`。
- 同步字函数 `0x0800B548`，56 B（下一函数 `0x0800B580`）；50024 字面量唯一且在 `+0x34`。
- `settings` 基址 `0x2000048C`，C.Fn 共 8 项；item 6 = ID（0..99），item 7 = `F8`（OFF/ON）。
- 密钥开关 = `settings[7]` = RAM `0x20000493`：**F8=ON(1) → 私有 KEY；OFF(0) → 50024/(ID+1)**。
- 补丁：56 B 整函数改写 + 2 个改值即时生效 gate（bne→blo）+ 6 处稳定色温读者屏蔽 +
  1 处退出路径静默清零阻断；输出长度不变（44,220 B）。

复跑：

```powershell
tools\python\python.exe keypatch\dev\ad300\audit_cfn.py
tools\python\python.exe keypatch\keypatch.py patch --device ad300 --key 0xD798 --verify
```
