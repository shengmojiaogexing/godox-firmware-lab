# 一键启动 X3-C 固件模拟机（后端 + 浏览器预览）
# 用法: powershell -NoProfile -ExecutionPolicy Bypass -File devices\x3c\emu\start_x3_sim.ps1
$ErrorActionPreference = 'Stop'

$root   = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
$py     = Join-Path $root 'tools\python\python.exe'
$server = Join-Path $root 'devices\x3c\emu\web_server.py'
$port   = 18765
$url    = "http://127.0.0.1:$port/"
$pidFile = Join-Path $env:TEMP 'x3sim.pid'

function Test-Listening([int]$p) {
    $c = Get-NetTCPConnection -LocalPort $p -State Listen -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($c) { return $c.OwningProcess }
    return $null
}

function Get-CmdLine([int]$procId) {
    try {
        $p = Get-CimInstance Win32_Process -Filter "ProcessId=$procId" -ErrorAction Stop
        return [string]$p.CommandLine
    } catch { return '' }
}

Write-Host '=============================================' -ForegroundColor DarkGray
Write-Host '  Godox X3-C v1.38 固件调试模拟机  一键启动' -ForegroundColor Cyan
Write-Host '=============================================' -ForegroundColor DarkGray

if (-not (Test-Path $py))     { Write-Host "[错误] 找不到 Python: $py" -ForegroundColor Red; exit 1 }
if (-not (Test-Path $server)) { Write-Host "[错误] 找不到后端: $server" -ForegroundColor Red; exit 1 }

# --- 1) 后端: 已在运行且健康则复用, 否则启动 (先精确清理本项目的陈旧实例)
$existing = Test-Listening $port
$needStart = $true
if ($existing) {
    $cmd = Get-CmdLine $existing
    if ($cmd -like '*web_server.py*') {
        try {
            $h = Invoke-RestMethod -Uri "${url}api/health" -TimeoutSec 3
            if ($h.ok) {
                Write-Host "[后端] 已在运行且健康 (PID $existing), 直接复用" -ForegroundColor Green
                $needStart = $false
            }
        } catch { }
        if ($needStart) {
            Write-Host "[后端] 发现无响应的旧实例 (PID $existing), 清理后重启" -ForegroundColor Yellow
            taskkill /PID $existing /T /F | Out-Null
            Start-Sleep -Milliseconds 800
        }
    } else {
        Write-Host "[错误] 端口 $port 被其他程序占用 (PID $existing): $cmd" -ForegroundColor Red
        Write-Host "       请先关闭该程序, 或改用其他端口。" -ForegroundColor Yellow
        exit 1
    }
}
if ($needStart) {
    Write-Host "[后端] 启动中 ... (端口 $port)" -ForegroundColor Yellow
    $log = Join-Path $env:TEMP 'x3sim-backend.log'
    $p = Start-Process -FilePath $py `
         -ArgumentList @($server, '--port', "$port") `
         -WorkingDirectory (Join-Path $root 'devices\x3c\emu') `
         -WindowStyle Hidden `
         -RedirectStandardOutput $log `
         -RedirectStandardError "$log.err" `
         -PassThru
    $p.Id | Set-Content -Path $pidFile -Encoding ascii
    $startedNew = $true

    # 等待 HTTP 就绪
    $ok = $false
    for ($i = 0; $i -lt 120; $i++) {
        Start-Sleep -Milliseconds 500
        try {
            $h = Invoke-RestMethod -Uri "${url}api/health" -TimeoutSec 2
            if ($h.ok) { $ok = $true; break }
        } catch { }
    }
    if (-not $ok) {
        Write-Host "[错误] 后端 60 秒内未就绪, 日志: $log / $log.err" -ForegroundColor Red
        exit 1
    }
    Write-Host "[后端] 已就绪 (PID $($p.Id)), 日志: $log" -ForegroundColor Green
}

# --- 2) 浏览器预览
Write-Host '[预览] 打开浏览器 ...' -ForegroundColor Yellow
& cmd.exe /c start "" $url
Write-Host ''
Write-Host "  模拟机地址: $url" -ForegroundColor Cyan
Write-Host '  启动后会自动运行约 800 万条指令绘制开机画面, 随后暂停。' -ForegroundColor DarkGray
Write-Host '  界面: LCD 屏幕 / 寄存器 / 反汇编(断点) / 内存 / 运行控制' -ForegroundColor DarkGray
Write-Host '  停止: 双击 scripts\stop_x3_sim.bat' -ForegroundColor DarkGray
if ($startedNew) { Write-Host "  后端 PID 已记录: $pidFile" -ForegroundColor DarkGray }
Write-Host ''
