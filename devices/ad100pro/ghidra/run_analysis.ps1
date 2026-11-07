# run_analysis.ps1 - reproduce the Ghidra headless run for the Godox AD100 Pro V1.2 firmware
#
# Usage:
#   pwsh -File run_analysis.ps1
#   (edit the parameters below / pass -GhidraHome / -JdkHome if your paths differ)
#
# The script imports the raw firmware image with the ARM Cortex-M0 language,
# runs the InitFirmware pre-script (vector table -> named Thumb functions),
# auto-analysis, and the ExportAll post-script (decompiled C + TSV exports).

param(
    [string]$GhidraHome = '',
    [string]$JdkHome    = '',
    [string]$Repo       = '',
    [string]$Firmware   = '',
    [long]  $BaseAddr   = 0x08003000,
    [string]$Processor  = 'ARM:LE:32:Cortex',
    [int]   $AnalysisTimeout = 1800
)

$ErrorActionPreference = 'Stop'
# 所有路径均由脚本位置推导（本脚本位于 <repo>/devices/ad100pro/ghidra/），不写死本机绝对路径。
$here     = $PSScriptRoot
$repoRoot = Split-Path -Parent (Split-Path -Parent $here)   # <repo>
if (-not $Repo)       { $Repo       = Split-Path -Parent $here }                        # devices/ad100pro
if (-not $Firmware)   { $Firmware   = Join-Path $Repo 'firmware\AD100pro_V1.2.bin' }
if (-not $GhidraHome) { $GhidraHome = Join-Path $repoRoot 'tools\ghidra_12.2_PUBLIC' }
if (-not $JdkHome)    { $JdkHome    = Join-Path $repoRoot 'tools\jdk\jdk-21.0.13+9' }
$ghidra   = $GhidraHome
$scripts  = Join-Path $Repo 'ghidra\scripts'
$projectD = Join-Path $Repo 'ghidra\project'
$outDir   = Join-Path $Repo 'out\ghidra'
$logFile  = Join-Path $Repo 'ghidra\analyze.log'
$scriptLog= Join-Path $Repo 'ghidra\analyze_scripts.log'

if (-not (Test-Path $ghidra)) { throw "Ghidra not found: $ghidra" }
if (-not (Test-Path (Join-Path $ghidra 'support\analyzeHeadless.bat'))) { throw "analyzeHeadless.bat not found" }
if (-not (Test-Path $Firmware)) { throw "Firmware not found: $Firmware" }
if (-not (Test-Path $JdkHome)) { throw "JDK not found: $JdkHome" }

New-Item -ItemType Directory -Force -Path $projectD, $outDir | Out-Null

$env:JAVA_HOME = $JdkHome
$env:PATH = (Join-Path $JdkHome 'bin') + ';' + $env:PATH
& (Join-Path $JdkHome 'bin\java.exe') -version 2>&1 | ForEach-Object { $_ } | Write-Host

$baseHex = '0x{0:X8}' -f $BaseAddr
$headless = Join-Path $ghidra 'support\analyzeHeadless.bat'

$arguments = @(
    $projectD, 'AD100pro_V12',
    '-import', $Firmware,
    '-processor', $Processor,
    '-loader', 'BinaryLoader',
    '-loader-baseAddr', $baseHex,
    '-loader-blockName', 'FLASH',
    '-scriptPath', $scripts,
    '-preScript', 'InitFirmware.java',
    '-postScript', 'ExportAll.java', $outDir,
    '-analysisTimeoutPerFile', "$AnalysisTimeout",
    '-max-cpu', '4',
    '-deleteProject',
    '-log', $logFile,
    '-scriptlog', $scriptLog
)

Write-Host "Running: $headless $($arguments -join ' ')"
$sw = [Diagnostics.Stopwatch]::StartNew()
$output = & $headless @arguments 2>&1
$output | Tee-Object -FilePath $logFile -Append | Write-Host
$sw.Stop()
Write-Host ("analyzeHeadless finished in {0:n0}s (exit {1})" -f $sw.Elapsed.TotalSeconds, $LASTEXITCODE)
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
