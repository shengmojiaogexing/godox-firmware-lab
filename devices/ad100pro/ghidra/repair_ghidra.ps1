# repair_ghidra.ps1 - rebuild the corrupt Ghidra zip from the parallel chunks.
# Three chunks (031/033/035) contain extra appended bytes from aborted retries;
# their first $chunkSize bytes are still the correct sequential range, so we
# truncate them and re-merge, then verify Lead's expected sha256.
$ErrorActionPreference = 'Stop'
# 路径由脚本位置推导（<repo>/devices/ad100pro/ghidra/），不写死本机绝对路径。
$repoRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)   # <repo>
$tools = Join-Path $repoRoot 'tools'
$src   = Join-Path $tools 'ghidra_dl'
$fix   = Join-Path $tools 'ghidra_fix'
$zip   = Join-Path $tools 'ghidra_12.2_PUBLIC_fixed.zip'
$size  = 569712708L
$chunk = 15825353L
$want  = '2c2cd776db3c1f6a097ba418922378ac3649cadd5e844a6f5f088ae628a7be67'

New-Item -ItemType Directory -Force -Path $fix | Out-Null
$n = [int]($size / $chunk)
if ($size % $chunk -ne 0) { $n += 1 }
Write-Host "chunks=$n chunkSize=$chunk expectedSize=$size"
for ($i = 0; $i -lt $n; $i++) {
    $name = 'chunk_{0:d3}.bin' -f $i
    $dst = Join-Path $fix $name
    if (-not (Test-Path $dst)) {
        Copy-Item (Join-Path $src $name) $dst
    }
    $len = (Get-Item $dst).Length
    $exp = [math]::Min($chunk, $size - [long]$i * $chunk)
    if ($len -gt $exp) {
        Write-Host "truncate $name $len -> $exp"
        $fs = [IO.File]::Open($dst, 'Open', 'ReadWrite')
        $fs.SetLength($exp)
        $fs.Close()
    } elseif ($len -ne $exp) {
        Write-Host "WARN $name len=$len expected=$exp"
    }
}
Write-Host 'merging...'
$out = [IO.File]::Open($zip, [IO.FileMode]::Create)
for ($i = 0; $i -lt $n; $i++) {
    $f = Join-Path $fix ('chunk_{0:d3}.bin' -f $i)
    $in = [IO.File]::OpenRead($f)
    $in.CopyTo($out)
    $in.Close()
}
$out.Close()
$final = (Get-Item $zip).Length
Write-Host "merged size=$final expected=$size"
if ($final -ne $size) { Write-Host 'SIZE MISMATCH'; exit 3 }
$h = (Get-FileHash -Algorithm SHA256 -Path $zip).Hash.ToLower()
Write-Host "sha256=$h"
if ($h -ne $want) { Write-Host 'SHA256 MISMATCH'; exit 4 }
Write-Host 'ZIP OK -> extracting'
& tar.exe -xf $zip -C $tools
if ($LASTEXITCODE -ne 0) { Write-Host "tar failed $LASTEXITCODE"; exit 5 }
Get-ChildItem $tools -Directory | Select-Object Name | Format-Table -AutoSize | Out-String
Write-Host 'EXTRACT OK'
