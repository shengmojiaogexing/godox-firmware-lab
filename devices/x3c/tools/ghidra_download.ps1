param(
  [Parameter(Mandatory=$true)][string]$Url,
  [Parameter(Mandatory=$true)][string]$OutFile,
  [Parameter(Mandatory=$true)][long]$Size,
  [int]$Chunks = 36,
  [int]$Parallel = 24,
  [string[]]$Proxies = @('https://gh-proxy.com/'),
  [string]$WorkDir = '',
  [string]$Sha256 = ''
)
$ErrorActionPreference = 'Continue'
$ProgressPreference = 'SilentlyContinue'
if(-not $WorkDir){ $WorkDir = Join-Path (Split-Path -Parent $OutFile) 'ghidra_dl' }
New-Item -ItemType Directory -Force -Path $WorkDir | Out-Null

$chunkSize = [math]::Ceiling($Size / $Chunks)
$jobs = @()
for($i=0; $i -lt $Chunks; $i++){
  $s = [long]$i * $chunkSize
  $e = [math]::Min($s + $chunkSize - 1, $Size - 1)
  if($s -le $e){
    $jobs += [pscustomobject]@{ idx=$i; s=$s; e=$e; exp=($e-$s+1); attempt=0; proc=$null; done=$false; proxy=$Proxies[$i % $Proxies.Count] }
  }
}

function Start-Chunk($job) {
  $f = Join-Path $WorkDir ('chunk_{0:d3}.bin' -f $job.idx)
  $have = if(Test-Path $f){ (Get-Item $f).Length } else { 0 }
  if($have -ge $job.exp){ $job.done = $true; return }
  $tmp = Join-Path $WorkDir ('chunk_{0:d3}.tmp' -f $job.idx)
  Remove-Item $tmp -Force -ErrorAction SilentlyContinue
  $rs = $job.s + $have
  $job.proc = Start-Process -FilePath curl.exe -PassThru -WindowStyle Hidden -ArgumentList @(
    '-s','-L','--fail','--retry','0','--max-time','1200','-r',"$rs-$($job.e)",'-o',$tmp,($job.proxy + $Url)
  )
  $job.attempt++
}

$sw = [Diagnostics.Stopwatch]::StartNew()
$launched = 0
while($true){
  $active = @($jobs | Where-Object { -not $_.done -and $_.proc -ne $null -and -not $_.proc.HasExited })
  $pending = @($jobs | Where-Object { -not $_.done -and $_.proc -eq $null })
  $finishedProc = @($jobs | Where-Object { -not $_.done -and $_.proc -ne $null -and $_.proc.HasExited })

  foreach($job in $finishedProc){
    $f = Join-Path $WorkDir ('chunk_{0:d3}.bin' -f $job.idx)
    $tmp = Join-Path $WorkDir ('chunk_{0:d3}.tmp' -f $job.idx)
    if((Test-Path $tmp) -and (Get-Item $tmp).Length -gt 0){
      $haveBefore = if(Test-Path $f){ (Get-Item $f).Length } else { 0 }
      $in = [IO.File]::OpenRead($tmp)
      $out = [IO.File]::Open($f, [IO.FileMode]::Append)
      $in.CopyTo($out); $out.Close(); $in.Close()
      Remove-Item $tmp -Force -ErrorAction SilentlyContinue
    }
    $have = if(Test-Path $f){ (Get-Item $f).Length } else { 0 }
    $job.proc = $null
    if($have -eq $job.exp){
      $job.done = $true
      Write-Host ("[{0:n0}s] chunk {1} DONE ({2} bytes, attempts {3})" -f $sw.Elapsed.TotalSeconds, $job.idx, $have, $job.attempt)
    } elseif($job.attempt -ge 12){
      Write-Host ("[{0:n0}s] chunk {1} FAILED after {2} attempts, have {3}/{4}" -f $sw.Elapsed.TotalSeconds, $job.idx, $job.attempt, $have, $job.exp)
      $job.done = $true; $job.failed = $true
    } else {
      Write-Host ("[{0:n0}s] chunk {1} retry attempt {2}, have {3}/{4}" -f $sw.Elapsed.TotalSeconds, $job.idx, ($job.attempt+1), $have, $job.exp)
    }
  }

  # refill
  $slots = $Parallel - @($jobs | Where-Object { -not $_.done -and $_.proc -ne $null }).Count
  while($slots -gt 0){
    $nextJob = $jobs | Where-Object { -not $_.done -and $_.proc -eq $null } | Select-Object -First 1
    if(-not $nextJob){ break }
    Start-Chunk $nextJob
    $slots--
  }

  $allDone = @($jobs | Where-Object { -not $_.done }).Count -eq 0
  if($allDone){ break }

  Start-Sleep -Milliseconds 1000
  $tot = 0; foreach($j in $jobs){ $f = Join-Path $WorkDir ('chunk_{0:d3}.bin' -f $j.idx); if(Test-Path $f){ $tot += (Get-Item $f).Length } }
  $el = [math]::Max($sw.Elapsed.TotalSeconds,1)
  Write-Host ("[{0:n0}s] total {1:n1} MB / {2:n1} MB  avg {3:n0} KB/s  done={4}/{5} active={6}" -f $el, ($tot/1MB), ($Size/1MB), ($tot/1KB/$el), @($jobs|Where-Object{$_.done -and -not $_.failed}).Count, $jobs.Count, @($jobs | Where-Object { -not $_.done -and $_.proc -ne $null }).Count)
}

$failedCount = @($jobs | Where-Object { $_.failed }).Count
if($failedCount -gt 0){ Write-Host "FAILED CHUNKS: $(@($jobs | Where-Object {$_.failed} | ForEach-Object {$_.idx}) -join ',')"; exit 2 }

Write-Host 'merging...'
$out = [IO.File]::Open($OutFile, [IO.FileMode]::Create)
foreach($j in $jobs){
  $f = Join-Path $WorkDir ('chunk_{0:d3}.bin' -f $j.idx)
  $in = [IO.File]::OpenRead($f); $in.CopyTo($out); $in.Close()
}
$out.Close()
$final = (Get-Item $OutFile).Length
Write-Host "merged size = $final (expected $Size)"
if($final -ne $Size){ Write-Host 'SIZE MISMATCH'; exit 3 }
if($Sha256){
  $hash = (Get-FileHash -Algorithm SHA256 -Path $OutFile).Hash.ToLower()
  Write-Host "sha256 = $hash (expected $Sha256)"
  if($hash -ne $Sha256.ToLower()){ Write-Host 'SHA256 MISMATCH'; exit 4 }
}
Write-Host ("DOWNLOAD OK in {0:n0}s" -f $sw.Elapsed.TotalSeconds)
