# scripts — 启动脚本

| 脚本 | 作用 |
|---|---|
| [`start_studio.bat`](start_studio.bat) | 启动可视化密钥工坊（studio）并打开浏览器 |
| [`start_x3_sim.bat`](start_x3_sim.bat) | 启动 X3-C 固件模拟机（后端 18765 + 浏览器调试界面） |
| [`stop_x3_sim.bat`](stop_x3_sim.bat) | 停止 X3-C 模拟机后端（只清理本项目进程） |

> 说明：X3-C 模拟机的 PowerShell 实现在 `devices/x3c/emu/start_x3_sim.ps1`，
> 这里的 bat 只是从仓库根目录调用的入口。

其他设备的模拟器/调试器请从各自目录启动，见：

- `devices/ad100pro/README.md` —— AD100 Pro 模拟器 + Web 调试器
- `devices/ad200pro/README.md` —— AD200 Pro 模拟器 + CLI/Web 调试器
