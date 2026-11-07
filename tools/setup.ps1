# 神牛固件研究工坊 —— Windows 环境准备脚本
#
# 用法（在仓库根目录）：
#   powershell -NoProfile -ExecutionPolicy Bypass -File tools\setup.ps1
#   powershell -NoProfile -ExecutionPolicy Bypass -File tools\setup.ps1 -UseSystem
#
# 默认：准备/复用仓库内的便携 Python（tools\python），并安装 requirements.txt。
# -System：不下载便携 Python，直接用系统 python 安装依赖。
param(
    [Alias('System')]
    [switch]$UseSystem
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$req = Join-Path $root 'requirements.txt'
$pyDir = Join-Path $PSScriptRoot 'python'
$py = Join-Path $pyDir 'python.exe'

function Write-Step($msg) { Write-Host "[setup] $msg" -ForegroundColor Cyan }

if ($UseSystem) {
    Write-Step '使用系统 Python 安装依赖 ...'
    python -m pip install --upgrade pip
    python -m pip install -r $req
    Write-Step '完成。运行：python studio\server.py --port 18780 --open'
    exit 0
}

if (-not (Test-Path $py)) {
    $ver = '3.12.10'
    $zip = Join-Path $env:TEMP "python-$ver-embed-amd64.zip"
    $url = "https://www.python.org/ftp/python/$ver/python-$ver-embed-amd64.zip"
    Write-Step "下载便携 Python $ver ..."
    Invoke-WebRequest -Uri $url -OutFile $zip
    New-Item -ItemType Directory -Force -Path $pyDir | Out-Null
    Expand-Archive -Path $zip -DestinationPath $pyDir -Force
    Remove-Item $zip -Force
    # 便携版默认禁用 site-packages：打开它，pip 才能安装依赖
    Get-ChildItem $pyDir -Filter 'python*._pth' | ForEach-Object {
        $t = Get-Content $_.FullName -Raw
        $t = $t -replace '#\s*import site', 'import site'
        Set-Content -Path $_.FullName -Value $t -Encoding ascii -NoNewline
    }
    Write-Step '安装 pip ...'
    $getpip = Join-Path $env:TEMP 'get-pip.py'
    Invoke-WebRequest -Uri 'https://bootstrap.pypa.io/get-pip.py' -OutFile $getpip
    & $py $getpip
    Remove-Item $getpip -Force
}

Write-Step '安装/更新依赖（capstone / unicorn / pillow）...'
& $py -m pip install --upgrade pip
& $py -m pip install -r $req
Write-Step "完成。Python: $py"
Write-Step '运行：tools\python\python.exe studio\server.py --port 18780 --open'
