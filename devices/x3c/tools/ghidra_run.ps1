# ghidra_run.ps1 -- import + analyze + export the X3-C firmware images with Ghidra headless.
#
# IMPORTANT: this file is ASCII-only. PowerShell 5.1 reads .ps1 as ANSI without a BOM,
# and Ghidra 12.2 cannot run from a non-ASCII install path (jar: URLs with CJK chars break
# log4j/ClassLoader). Therefore:
#   - Ghidra 12.2 PUBLIC : D:\ghidra_12.2_PUBLIC   (moved out of the CJK path on purpose)
#   - JDK 25 (required)  : D:\jdk25
#   - project            : D:\ghidra_project   (project name X3C)
#   - scripts            : D:\ghidra_scripts   (copies of devices\x3c\tools\ghidra_scripts\*.java)
#   - exports            : D:\ghidra_out       (junction -> devices\x3c\re\ghidra_exports)
#   - inputs             : C:\x3c\... (junction -> devices\x3c\firmware), C:\x3c_tools\... (-> tools)
#
# Usage:
#   powershell -NoProfile -ExecutionPolicy Bypass -File D:\...\ghidra_run.ps1 -Main
#   ... -Blob | (no switch = both)
param(
  [switch]$Main,
  [switch]$Blob
)
$ErrorActionPreference = 'Continue'
$ProgressPreference = 'SilentlyContinue'

$env:JAVA_HOME = 'D:\jdk25'
$env:PATH = 'D:\jdk25\bin;' + $env:PATH

$AH = 'D:\ghidra_12.2_PUBLIC\support\analyzeHeadless.bat'
$PROJ = 'D:\ghidra_project'
$PROJNAME = 'X3C'
$SCRIPTS = 'D:\ghidra_scripts'
$FWDIR = 'D:\ghidra_out'
$MAINBIN = 'C:\x3c\X3_C_v1.38_app.bin'
$BLOBBIN = 'C:\x3c_tools\hotshoe_blob.bin'

function Ensure-Setup {
  foreach ($d in @('C:\x3c', 'C:\x3c_tools', 'D:\ghidra_out')) {
    if (-not (Test-Path $d)) { Write-Host "ERROR: junction $d missing (recreate it; see GHIDRA_USAGE.md)"; exit 1 }
  }
  New-Item -ItemType Directory -Force -Path 'D:\ghidra_project', 'D:\ghidra_scripts' | Out-Null
  # refresh script copies from the team-working copy (if present; ASCII-safe no-op otherwise)
  Copy-Item 'D:\ghidra_scripts\*.java' 'D:\ghidra_scripts\' -Force -ErrorAction SilentlyContinue
}

function Extract-Blob {
  if (Test-Path $BLOBBIN) { return }
  $bytes = [IO.File]::ReadAllBytes($MAINBIN)
  $len = 0x0F961C - 0x0F3800
  $out = New-Object byte[] $len
  [Array]::Copy($bytes, 0x0F3800, $out, 0, $len)
  [IO.File]::WriteAllBytes($BLOBBIN, $out)
  Write-Host "extracted hot-shoe blob ($len bytes)"
}

function Run-Headless([string]$tag, [string[]]$extra) {
  $tagDir = Join-Path $FWDIR $tag
  New-Item -ItemType Directory -Force -Path $tagDir | Out-Null
  $outLog = Join-Path $tagDir 'ghidra_stdout.log'
  $errLog = Join-Path $tagDir 'ghidra_stderr.log'
  $argList = @($PROJ, $PROJNAME) + $extra
  Write-Host "=== Ghidra: $tag ==="
  Write-Host ("args: " + ($argList -join ' '))
  $sw = [Diagnostics.Stopwatch]::StartNew()
  $p = Start-Process -FilePath $AH -ArgumentList $argList -Wait -PassThru -NoNewWindow `
        -RedirectStandardOutput $outLog -RedirectStandardError $errLog
  Write-Host ("=== {0} exit={1} elapsed={2:n0}s ===" -f $tag, $p.ExitCode, $sw.Elapsed.TotalSeconds)
  Get-Content $outLog -Tail 30 -ErrorAction SilentlyContinue
  Get-Content $errLog -Tail 30 -ErrorAction SilentlyContinue
  return $p.ExitCode
}

Ensure-Setup
if (-not (Test-Path $AH)) { Write-Host "ERROR: $AH missing"; exit 1 }

$mainArgs = @(
  '-import', $MAINBIN, '-overwrite',
  '-processor', 'ARM:LE:32:Cortex',
  '-loader', 'BinaryLoader',
  '-loader-baseAddr', '0x08004800',
  '-scriptPath', $SCRIPTS,
  '-preScript', 'PreSetup.java', '0x08004800', '131', '0x08004AF9',
  '-postScript', 'ExportAll.java', 'D:\ghidra_out\main',
  '-analysisTimeoutPerFile', '3600'
)
$blobArgs = @(
  '-import', $BLOBBIN, '-overwrite',
  '-processor', 'ARM:LE:32:Cortex',
  '-loader', 'BinaryLoader',
  '-loader-baseAddr', '0x08002000',
  '-scriptPath', $SCRIPTS,
  '-preScript', 'PreSetup.java', '0x08002000', '46', '0x080020CC',
  '-postScript', 'ExportAll.java', 'D:\ghidra_out\hotshoe',
  '-analysisTimeoutPerFile', '1800'
)

if (-not $Main -and -not $Blob) { $Main = $true; $Blob = $true }

if ($Main) { $rc = Run-Headless 'main' $mainArgs }
if ($Blob) { Extract-Blob; $rc = Run-Headless 'hotshoe' $blobArgs }

$manifest = [ordered]@{
  project     = 'D:\ghidra_project'
  projectName = 'X3C'
  ghidra      = 'D:\ghidra_12.2_PUBLIC'
  jdk         = 'D:\jdk25'
  scripts     = 'D:\ghidra_scripts'
  programs    = [ordered]@{
    main    = [ordered]@{ name = 'X3_C_v1.38_app.bin'; base = '0x08004800'; file = 'C:\x3c\X3_C_v1.38_app.bin' }
    hotshoe = [ordered]@{ name = 'hotshoe_blob.bin'; base = '0x08002000'; file = 'C:\x3c_tools\hotshoe_blob.bin' }
  }
}
$manifest | ConvertTo-Json -Depth 6 | Set-Content -Encoding ASCII 'D:\ghidra_out\project.json'
Write-Host 'wrote D:\ghidra_out\project.json'
exit 0
