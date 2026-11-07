# PROTOCOL.md — Godox 2.4 GHz link: firmware evidence (X3-C v1.38) + cross-reference

Firmware: `devices/x3c/firmware/X3_C_v1.38_app.bin`, app link base 0x08004800
(`file_offset = vaddr - 0x08004800`). Cross-reference: `Godox-S3-Trigger`
(GPL-3.0 clean-room ESP32-S3 reimplementation; `components/godox_protocol`,
`components/rf_transmitter`, `components/godox_waveform`).

**Headline:** the radio is a **TI CC2500-class 2.4 GHz GFSK transceiver** driven over a
**bit-banged SPI** (GPIOC0=SCLK, GPIOC1=MOSI, GPIOC14=CSn, GPIOC15=MISO). The **16-bit
"encoded ID" from the clean-room implementation is the hardware SYNC WORD**
(`SYNC1/SYNC0 = 50024/(id+1)`), and the channel plan is a per-channel **FREQ2/FREQ1/FREQ0**
table at **0x08045390**. The packet format from the clean-room project is consistent with
this setup (preamble AA…, double encoded-ID, 0xD5, command). Some payload-builder code is
still to be located (see §6).

---

## 1. Radio chip and physical interface (evidence)

Bit-banged SPI (no hardware SPI on the radio):

| signal | GPIO | evidence |
|---|---|---|
| SCLK | GPIOC bit0 (`0x40020800`, mask 0x0001) | bit-bang writer 0x0802377C toggles mask 1 per bit |
| MOSI | GPIOC bit1 (mask 0x0002) | set/clear per bit in 0x0802377C (MSB first, bits tested via `lsls r0,r4,#0x18/0x19/0x1A…`) |
| CSn | GPIOC bit14 (mask 0x4000) | assert 0x08023A7C (with up-to-200×1 wait loop reading PC15), release 0x08023AB0 |
| MISO | GPIOC bit15 (mask 0x8000) | read in 0x0802354C |

| function | vaddr | file | role |
|---|---|---|---|
| bit-bang byte out | 0x0802377C | 0x1EF7C | SCLK/MOSI, MSB first, 9 `nop` per bit |
| register write (2 bytes) | 0x08023540 | 0x1ED40 | `CS assert; reg; value; CS release` |
| command/strobe byte | 0x08023520 | 0x1ED20 | `CS assert; byte; CS release` |
| register+payload write | 0x08023A52 | 0x1F252 | `CS assert; reg; ptr,len; CS release` |
| byte read | 0x0802354C | 0x1ED4C | bit-bang in on PC15 |
| delay | 0x080232B6 | 0x1EAB6 | software delay (unit TBD ~µs) |

Radio register/command numbers observed in the firmware match the **CC2500 register map**
(strong evidence, not a Godox ASIC):

| register | name (CC2500) | values written in firmware | where |
|---|---|---|---|
| 0x00 | IOCFG2 | **0x0B**, **0x29** | 0x08011A40, 0x08011AD0 |
| 0x02 | IOCFG0 | **0x0C**, **0x06** | 0x08011A38, 0x08011AC8 |
| 0x04 | **SYNC1** | **`50024/(id+1) >> 8`** | 0x08014874 |
| 0x05 | **SYNC0** | **`50024/(id+1) & 0xFF`** | 0x0801487C |
| 0x06 | PKTLEN | **0x3D (61)**, 0x04 | 0x08011A20, 0x08011AA8 |
| 0x08 | PKTCTRL0 | **0x12**, 0x00 | 0x08011A30, 0x08011AC0 |
| 0x0D | **FREQ2** | channel table byte `T[ch]` | 0x0801482A |
| 0x0E | **FREQ1** | `T[33+ch]` | 0x08014836 |
| 0x0F | **FREQ0** | `T[66+ch]` | 0x08014842 |
| 0x12 | MDMCFG2 | **0x33** (= GFSK, 30/32-bit sync), 0x73 | 0x08011A28, 0x08011AB0 |
| 0x17 | MCSM1 | **0x32**, 0x30 | 0x08011A18, 0x08011AB8 |
| 0x3E | PATABLE (burst) | 0x81, 0x46, 0xFF → TX power steps | 0x080128E4, 0x08012908, 0x0801292C |
| 0x7E | PATABLE-burst write (0x3E|burst-bit) or FIFO burst | called with 11-byte payload at 0x08023444 | 0x08023A52 path |

Strobes seen as single-byte commands via 0x08023520: **0x33 = SCAL** (0x080117E8, 0x08011968,
0x08014888), **0x35 = STX** (0x08011A8C, 0x080170A8, 0x0801715C), **0x36 = SIDLE**
(23 call sites: 0x08006FA4, 0x08009308, 0x08011588, 0x080116EC, 0x080117B0, 0x0801188C,
0x08011920, 0x08011A10, 0x08011AA0, 0x08012950, 0x08014850, 0x0801488C, 0x080170BC),
**0x39 = SPWD**, **0x3B = SFTX** (0x080170BC/0x0801715C).

Radio init write sequence #1 (`0x08011A18`..`0x08011A48`):
`MCSM1=0x32, PKTLEN=0x3D, MDMCFG2=0x33, PKTCTRL0=0x12, IOCFG0=0x0C, IOCFG2=0x0B`
→ GFSK, 30/32-bit sync detection, max payload 61 bytes, variable/fixed config per PKTCTRL0.
Radio init write sequence #2 (`0x08011AA8`..`0x08011AD8`):
`PKTLEN=0x04, MDMCFG2=0x73, MCSM1=0x30, PKTCTRL0=0x00, IOCFG0=0x06, IOCFG2=0x29`
→ second profile (short packet / different sync mode — likely RX/scan or ACK profile).

---

## 2. Channel plan and channel→frequency table (decoded!)

**Channel set routine: 0x08014814** (file 0xFF14):
```
SIDLE (0x36); delay(1000)
r0 = 0x08045390;            FREQ2 = table[ch]
r0 = 0x08045390 + 0x21;     FREQ1 = table[33+ch]
r0 = 0x08045390 + 0x42;     FREQ0 = table[66+ch]
delay(20); SCAL (0x33); delay(700)
```
Table **0x08045390** (file 0x40B90) = 99 bytes = `FREQ2[33] FREQ1[33] FREQ0[33]`.
CC2500 synthesizer: `f = FREQ24 · f_XTAL / 2^16` with f_XTAL = 26 MHz; decoded values
below use that formula and match the FCC-derived channel plan of the clean-room project.

FREQ2 (33): `5c 5c 5c 5d 5d 5d 5d 5d 5d 5d 5d 5d 5d 5d 5d 5d 5d 5d 5d 5e 5e 5e 5e 5e 5e 5e 5e 5e 5e 5e 5e 5e 5f`
FREQ1 (33): `ce dd ec 00 0e 1d 31 40 56 62 71 87 93 a2 b3 c4 d3 e9 f6 04 1b 27 36 4c 58 67 7d 89 98 ae bb c9 4e`
FREQ0 (33): `c4 89 4e 00 c4 89 3b 00 27 76 3b 62 b1 76 b1 ec b1 d8 27 ec 13 62 27 4e 9d 62 89 d8 9d c4 13 d8 c4`

| ch (idx) | FREQ24 | bytes F2 F1 F0 | f (MHz) | S3 `half_mhz` | Δ |
|---:|---:|---|---:|---:|---:|
| 1 (0) | 6082244 | 5c ce c4 | 2413.000 | 4826 | 0 |
| 2 (1) | 6086025 | 5c dd 89 | 2414.500 | 4829 | 0 |
| 3 (2) | 6089806 | 5c ec 4e | 2416.000 | 4832 | 0 |
| 4 (3) | 6094848 | 5d 00 00 | 2418.000 | 4836 | 0 |
| 5 (4) | 6098628 | 5d 0e c4 | 2419.500 | 4839 | 0 |
| 6 (5) | 6102409 | 5d 1d 89 | 2421.000 | 4842 | 0 |
| 7 (6) | 6107451 | 5d 31 3b | 2423.000 | 4846 | 0 |
| 8 (7) | 6111232 | 5d 40 00 | 2424.500 | 4849 | 0 |
| 9 (8) | 6116903 | 5d 56 27 | **2426.750** | 4852 (2426.0) | +0.75 |
| 10 (9) | 6120054 | 5d 62 76 | 2428.000 | 4856 | 0 |
| 11 (10) | 6123835 | 5d 71 3b | 2429.500 | 4859 | 0 |
| 12 (11) | 6129506 | 5d 87 62 | **2431.750** | 4862 (2431.0) | +0.75 |
| 13 (12) | 6132657 | 5d 93 b1 | 2433.000 | 4866 | 0 |
| 14 (13) | 6136438 | 5d a2 76 | 2434.500 | 4869 | 0 |
| 15 (14) | 6140849 | 5d b3 b1 | **2436.250** | 4872 (2436.0) | +0.25 |
| 16 (15) | 6145260 | 5d c4 ec | 2438.000 | 4876 | 0 |
| 17 (16) | 6149041 | 5d d3 b1 | 2439.500 | 4879 | 0 |
| 18 (17) | 6154712 | 5d e9 d8 | **2441.750** | 4882 (2441.0) | +0.75 |
| 19 (18) | 6157863 | 5d f6 27 | 2443.000 | 4886 | 0 |
| 20 (19) | 6161644 | 5e 04 ec | 2444.500 | 4889 | 0 |
| 21 (20) | 6167315 | 5e 1b 13 | **2446.750** | 4892 (2446.0) | +0.75 |
| 22 (21) | 6170466 | 5e 27 62 | 2448.000 | 4896 | 0 |
| 23 (22) | 6174247 | 5e 36 27 | 2449.500 | 4899 | 0 |
| 24 (23) | 6179918 | 5e 4c 4e | **2451.750** | 4902 (2451.0) | +0.75 |
| 25 (24) | 6183069 | 5e 58 9d | 2453.000 | 4906 | 0 |
| 26 (25) | 6186850 | 5e 67 62 | 2454.500 | 4909 | 0 |
| 27 (26) | 6192521 | 5e 7d 89 | **2456.750** | 4912 (2456.0) | +0.75 |
| 28 (27) | 6195672 | 5e 89 d8 | 2458.000 | 4916 | 0 |
| 29 (28) | 6199453 | 5e 98 9d | 2459.500 | 4919 | 0 |
| 30 (29) | 6205124 | 5e ae c4 | **2461.750** | 4922 (2461.0) | +0.75 |
| 31 (30) | 6208275 | 5e bb 13 | 2463.000 | 4926 | 0 |
| 32 (31) | 6212056 | 5e c9 d8 | 2464.500 | 4929 | 0 |
| 33 (32) | 6246084 | 5f 4e c4 | 2478.000 | — | unused/test |

Notes:
* The firmware table is authoritative: **every 3rd channel (9,12,15,…,30) is shifted
  +0.25/+0.75 MHz** vs the rounded FCC table used by `godox_protocol.c`
  (`half_mhz = {4826,4829,…}`). Ch8 (2424.5) agrees with the clean-room field test.
  Recommend updating `godox_channel_plan()` with the exact FREQ24 values above.
* Channels are set by writing **FREQ2/1/0** directly per channel (no `CHANNR` write was
  found), i.e. one table entry per channel, then `SCAL` + settle delay.
* 33 entries: index = channel−1 for CH1–CH32; last entry (2478.0 MHz) is out-of-plan.

---

## 3. ID encoding = hardware sync word (confirmed in firmware)

Function **0x08014864** (file 0x10064) — exact clean-room formula in Godox firmware:
```
movw  r1, #0xC368      ; 50024
adds  r0, r0, #1       ; id+1
udiv  r4, r1, r0       ; encoded = 50024 / (id + 1)
lsrs  r1, r4, #8       ; high byte
movs  r0, #4           ; CC2500 SYNC1
bl    0x08023540       ; write reg 4 = encoded >> 8
uxtb  r1, r4
movs  r0, #5           ; CC2500 SYNC0
bl    0x08023540       ; write reg 5 = encoded & 0xFF
delay(20); SCAL(0x33); delay(1000); SIDLE(0x36); delay(700)
```
→ The ID pairs in `godox_protocol.c` (`bytes[4]=bytes[6]=encoded>>8;
bytes[5]=bytes[7]=encoded&0xFF`) are the **hardware SYNC1/SYNC0**, which explains why the
ID is repeated in the clean-room air frame: hardware sync (first pair) is followed by the
payload which repeats the ID field (second pair). `id > 99` rejected by the clean-room code
is consistent with `50024/(id+1)` uniqueness.

---

## 4. Packet format / air frame (cross-referenced)

From `components/godox_protocol` (clean-room, verified against a real V850II and captured
IQ in this workspace):

Short frame (`godox_short_frame`, 10 bytes / 81 bits on air):
```
AA AA AA AA | enc_hi enc_lo | enc_hi enc_lo | D5 | cmd
                                                  cmd = 0x11 PREPARE | 0x09 TEST | 0x21 UNPREPARE
```
Property frame (`godox_property_frame`, 12 bytes / 97 bits):
```
AA AA AA AA | enc_hi enc_lo | enc_hi enc_lo | D5 | cmd(=0x09) | A9 | group | key | value
                                       group = 0x50 for ALL, 0x09+group for A..E (1..5)
```
Bit expansion: bytes MSB-first; last bit repeated once (`bits[80]=bits[79]`,
`bits[96]=bits[95]`) — this is how the clean-room RF path synthesises the GFSK bitstream at
`GODOX_SAMPLE_RATE` (16 MS/s); 403.2 µs loop per `rf_transmitter.h`.

Firmware consistency checks:
* `AA` preamble ×4 = standard CC2500 preamble pattern; **sync = encoded ID** (SYNC1/SYNC0,
  proven above); `PKTLEN` = 0x3D (61) so an 8–9 byte payload + repeated ID fits easily.
* `MDMCFG2 = 0x33` = GFSK, 30/32-bit sync — matches a 2-byte hardware sync word.
* `0xD5` is the payload start marker in the clean-room frames; in the firmware the
  corresponding payload builder was **not yet isolated** (see §6). Note that the byte 0xD5
  appears as an instruction byte in many places and is not a literal pool constant, so the
  packet template is either built in an unreached function or via a small byte array that
  my table scans did not match.
* Property/ID layout implies TX payload = {ID_hi, ID_lo, 0xD5, cmd, [A9, group, key, value]}
  (10–12 bytes) — within PKTLEN=61.

Channel hopping / TX timing:
* Channel change: `SIDLE → FREQ2/1/0 → delay(20) → SCAL → delay(700)` (0x08014814).
* TX strobe `STX (0x35)` appears at 0x08011A8C, 0x080170A8, 0x0801715C; `SFTX (0x3B)` and
  `SPWD (0x39)` at 0x080170BC/0x0801715C; `SIDLE (0x36)` before/after every change.
* A programming/setup path asserts SCAL after ID writes (0x08014888) and waits 1000 units —
  unit of delay 0x080232B6 to be confirmed (µs-scale would make TX setup ~1 ms, ms-scale
  would be seconds; the clean-room timings suggest µs).
* PATABLE writes 0x81/0x46/0xFF (0x080128E4/0x08012908) = output-power steps; exact dBm
  depends on matching network — not determinable from firmware.

---

## 5. Hot-shoe MCU link — NOT the 2.4 GHz protocol (separate, do not confuse)

SPI1 (`0x40013000`, IRQ35) is a **command/response link to the hot-shoe adapter MCU**
(the STM32F0-class image at file 0x0F3800). Evidence: function **0x08013688**
(file 0xEE88):
* frames start with command bytes 0xA0, 0xC0–0xC9 (switch on first byte);
  response header = `cmd + 1` (0xA1, 0xC1…); checksum = `0xA1 + Σpayload`;
* frame buffer copied to RAM **0x20000C2A**; commands written byte-by-byte over SPI1
  (0x080381D6) with CS = **GPIOC bit10** (`0x40020800` mask 0x400) toggled via
  0x08022E34/0x08022E38; per-byte timeout 0x4E20 (20000) polls of SPI1 SR;
* IRQ35 handler 0x08012985 drains SPI1 DR into RAM 0x20000011.
This is the channel used by the "Hot Shoe Upgrading" flow (image2 at file 0x0F3800).

---

## 6. What is still missing (priority for Ghidra/emulator follow-up)

1. **TX packet builder**: find the code that fills the CC2500 TX FIFO with
   `{AA…, enc, enc, D5, cmd, …}` for PREPARE(0x11)/TEST(0x09)/UNPREPARE(0x21) and the
   property frame (A9/group/key/value). Grep candidates: functions 0x08011500–0x08011B00
   (radio config/strobe cluster), 0x080128E4–0x08012950 (PATABLE/config), 0x08016C00–0x08016D60
   (TMR5/6/7 handlers used as TX timing), 0x080170A8–0x0801715C (STX/SFTX/SPWD),
   0x08014814/0x08014864 (channel/ID).
2. **RX path/scan**: byte-read 0x0802354C has no direct BL caller in my scan (likely called
   through a function pointer or via a table) — find it to document RSSI/status decoding.
3. **GDO/IRQ line**: which GPIOC/GPIO pin carries CC2500 GDO0/GDO2 into EXINT
   (IRQ6 EXINT0 / IRQ8 EXINT2 / IRQ23 EXINT9_5 are candidates). This is the radio IRQ.
4. **Timing constants**: upload/hopping intervals; delay unit of 0x080232B6.
5. Confirm CC2500 vs pin-compatible clone (e.g. same register map) — the register set is
   100% CC2500-compatible; only the crystal (26 MHz assumed) and PATABLE calibration matter.

---

## 7. Cross-reference summary vs `Godox-S3-Trigger`

| Item | Firmware (X3-C) | Clean-room project | Verdict |
|---|---|---|---|
| Radio | CC2500-class, GFSK | ESP32-S3 SDR (IQ synthesis) | same air protocol |
| Frequency table | 0x08045390, FREQ24 per ch (exact, §2) | `godox_channel_plan()` `half_mhz[32]` | mostly equal; 8 channels differ (table above) |
| ID encoding | `50024/(id+1)` @0x08014864 → SYNC1/2 | `godox_encode_id()` | **identical formula** |
| Sync/preamble | hardware sync = encoded ID, AA preamble | 4×0xAA + encoded ID ×2 | consistent |
| Frame markers | not yet located (D5/A9) | `godox_short_frame` / `godox_property_frame` | to verify |
| Commands | 0x09 / 0x11 / 0x21 (from S3) | same | to verify in TX builder |
| Groups | S3: 0x50 ALL, 0x09+grp A..E | — | to verify |
| Modem | MDMCFG2=0x33 GFSK 30/32 sync, PKTLEN=61 | 2 Mbps-ish GFSK SDR | consistent |
| Hopping | per-channel FREQ writes + SCAL | scan/auto-detect FFT | different approach, same channels |
