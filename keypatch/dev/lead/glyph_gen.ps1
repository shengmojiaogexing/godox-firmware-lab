param(
    [string]$Chars = "AB",
    [int]$Size = 24,
    [string]$FontName = "SimHei",
    [string]$OutDir = "$PSScriptRoot\glyphs"
)
# Render arbitrary characters (incl. CJK) into 1bpp bitmaps using GDI+ TextRenderer.
# Output: ASCII preview, <U+XXXX>.txt, <U+XXXX>.hex, glyphs.h (C arrays).
# Usage:
#   powershell -NoProfile -ExecutionPolicy Bypass -File glyph_gen.ps1 -Chars "XX" -Size 24 -FontName SimHei
Add-Type -AssemblyName System.Drawing
Add-Type -AssemblyName System.Windows.Forms

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

$header = New-Object System.Text.StringBuilder
[void]$header.AppendLine("/* auto-generated 1bpp glyphs, size=$Size, font=$FontName */")
[void]$header.AppendLine("#pragma once")
[void]$header.AppendLine("#define GLYPH_W $Size")
[void]$header.AppendLine("#define GLYPH_H $Size")

for ($i = 0; $i -lt $Chars.Length; $i++) {
    $ch = $Chars.Substring($i, 1)
    $cp = [int][char]$ch

    $tmp = New-Object System.Drawing.Bitmap ($Size * 2), ($Size * 2)
    $g = [System.Drawing.Graphics]::FromImage($tmp)
    $g.Clear([System.Drawing.Color]::White)
    $g.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::SingleBitPerPixelGridFit
    $font = New-Object System.Drawing.Font($FontName, [single]$Size, [System.Drawing.FontStyle]::Regular, [System.Drawing.GraphicsUnit]::Pixel)
    $flags = [System.Windows.Forms.TextFormatFlags]::NoPadding -bor [System.Windows.Forms.TextFormatFlags]::SingleLine
    $rect = New-Object System.Drawing.Rectangle 0, 0, ($Size * 2), ($Size * 2)
    [System.Windows.Forms.TextRenderer]::DrawText($g, $ch, $font, $rect, [System.Drawing.Color]::Black, [System.Windows.Forms.TextFormatFlags]$flags)
    $g.Dispose()

    $minx = $Size * 2; $miny = $Size * 2; $maxx = -1; $maxy = -1
    for ($y = 0; $y -lt ($Size * 2); $y++) {
        for ($x = 0; $x -lt ($Size * 2); $x++) {
            if ($tmp.GetPixel($x, $y).R -lt 128) {
                if ($x -lt $minx) { $minx = $x }
                if ($y -lt $miny) { $miny = $y }
                if ($x -gt $maxx) { $maxx = $x }
                if ($y -gt $maxy) { $maxy = $y }
            }
        }
    }
    if ($maxx -lt 0) { $minx = 0; $miny = 0; $maxx = $Size - 1; $maxy = $Size - 1 }
    $gw = $maxx - $minx + 1
    $gh = $maxy - $miny + 1
    $ox = [math]::Floor(($Size - $gw) / 2) - $minx
    $oy = [math]::Floor(($Size - $gh) / 2) - $miny

    $rows = @()
    $hexRows = @()
    for ($py = 0; $py -lt $Size; $py++) {
        $line = ""
        $word = [uint32]0
        for ($px = 0; $px -lt $Size; $px++) {
            $sx = $px - $ox
            $sy = $py - $oy
            $on = $false
            if ($sx -ge 0 -and $sx -lt ($Size * 2) -and $sy -ge 0 -and $sy -lt ($Size * 2)) {
                $on = ($tmp.GetPixel($sx, $sy).R -lt 128)
            }
            if ($on) {
                $line += "#"
                $word = $word -bor ([uint32](1 -shl (31 - $px)))
            } else {
                $line += "."
            }
        }
        $rows += $line
        $hexRows += ("0x{0:X8}" -f $word)
    }
    $tmp.Dispose()

    Write-Output ("===== char U+{0:X4} {1}x{2} {3} =====" -f $cp, $Size, $Size, $FontName)
    $rows | ForEach-Object { Write-Output $_ }

    Set-Content -Path (Join-Path $OutDir ("u{0:X4}.txt" -f $cp)) -Value $rows -Encoding UTF8
    Set-Content -Path (Join-Path $OutDir ("u{0:X4}.hex" -f $cp)) -Value $hexRows -Encoding UTF8

    [void]$header.AppendLine(("static const unsigned int glyph_u{0:X4}[GLYPH_H] = {{" -f $cp))
    [void]$header.AppendLine("    " + ($hexRows -join ", "))
    [void]$header.AppendLine("};")
}

Set-Content -Path (Join-Path $OutDir "glyphs.h") -Value $header.ToString() -Encoding UTF8
Write-Output ("written to: " + $OutDir)
