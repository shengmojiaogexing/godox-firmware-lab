# 一键停止 X3-C 模拟机后端（只关闭本项目的 web_server.py, 不动其它浏览器/服务）
# 用法: powershell -NoProfile -ExecutionPolicy Bypass -File devices\x3c\emu\stop_x3_sim.ps1
$ErrorActionPreference = 'SilentlyContinue'
$port = 18765
$pidFile = Join-Path $env:TEMP 'x3sim.pid'
$killed = @()

function Get-CmdLine([int]$procId) {
    try {
        $p = Get-CimInstance Win32_Process -Filter "ProcessId=$procId"
        return [string]$p.CommandLine
    } catch { return '' }
}

# 1) 端口监听者（且命令行是本项目 web_server.py）
$c = Get-NetTCPConnection -LocalPort $port -State Listen | Select-Object -First 1
if ($c) {
    $cmd = Get-CmdLine $c.OwningProcess
    if ($cmd -like '*web_server.py*') {
        taskkill /PID $c.OwningProcess /T /F | Out-Null
        $killed += $c.OwningProcess
    }
}

# 2) 记录过的 PID
if (Test-Path $pidFile) {
    $pid2 = (Get-Content $pidFile | Select-Object -First 1).Trim()
    if ($pid2 -match '^\d+$') {
        $cmd = Get-CmdLine ([int]$pid2)
        if ($cmd -like '*web_server.py*') {
            taskkill /PID $pid2 /T /F | Out-Null
            if ($killed -notcontains [int]$pid2) { $killed += [int]$pid2 }
        }
    }
    Remove-Item $pidFile -Force
}

if ($killed.Count -gt 0) {
    Write-Host "[已停止] X3-C 模拟机后端 PID: $($killed -join ', ')" -ForegroundColor Green
} else {
    Write-Host '[提示] 没有找到正在运行的 X3-C 模拟机后端 (端口 18765)' -ForegroundColor Yellow
}
Write-Host '       浏览器页面可以直接关闭。' -ForegroundColor DarkGray
