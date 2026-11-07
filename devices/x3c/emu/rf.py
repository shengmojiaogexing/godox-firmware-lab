"""rf.py -- RF transceiver / SPI device stubs for the X3-C emulator.

The X3-C drives its 2.4 GHz radio over SPI (SPI1 is the most referenced SPI
peripheral in the image).  Until hw-analyst confirms the exact part, this module
provides a programmable SPI-slave stub that:

* captures every byte the firmware sends, grouped into **frames** (either by a
  chip-select GPIO edge or, when no CS is wired yet, by inter-byte idle gaps),
* timestamps frames with the machine's virtual time (instruction count),
* can queue RX bytes to hand back to the firmware on the next SPI transfers,
* exports captured frames as CSV/JSON and prints a hex summary.

Frame formats (channel/iD/group, Godox 2.4 GHz protocol) are *not* imposed here:
the stub records raw bytes so the protocol can be analysed from captures.

Typical use from a scenario::

    from rf import RFTransceiver
    rf = RFTransceiver(machine)
    rf.attach_spi("SPI1")
    rf.attach_cs("GPIOB", 6)            # once the CS pin is known
    machine.run(max_insns=2000000)
    rf.inject_rx([0xAA, 0x55])          # simulate a reply
    print(rf.summary())
    rf.save_csv("devices/x3c/emu/logs/rf_frames.csv")
"""
from __future__ import annotations

import json
import os


class SPIByteDevice:
    """Base class for a device hanging off an SPI bus."""

    def __init__(self, name="device"):
        self.name = name
        self.bytes = bytearray()

    def on_tx_byte(self, b):
        self.bytes.append(b & 0xFF)
        return 0

    def on_cs(self, level):
        pass


class RFTransceiver(SPIByteDevice):
    """SPI RF transceiver stub with frame capture and RX injection."""

    def __init__(self, machine, name="RF", frame_gap=2000,
                 max_frames=100000):
        super().__init__(name)
        self.m = machine
        self.frame_gap = frame_gap        # vtime gap that ends a frame
        self.max_frames = max_frames
        self.frames = []                  # dict(t, end, data, reason)
        self.cur = bytearray()
        self.cur_start = 0
        self.last_byte_t = 0
        self.spi_name = None
        self.cs_pin = None
        self.cs_active_low = True
        self.cs_level = 1
        self.rx_queue = bytearray()
        self.tx_count = 0
        self.rx_count = 0
        self.regs = {}                    # write-then-read style register model
        self.last_reg = None

    # -- wiring --------------------------------------------------------
    def attach_spi(self, machine=None, spi_name="SPI1"):
        m = machine or self.m
        spi = m.spi.get(spi_name)
        if spi is None:
            raise KeyError("unknown SPI %r (have %r)" % (spi_name, list(m.spi)))
        spi.tx_handler = self
        self.spi_name = spi_name
        return spi

    def attach_cs(self, port, pin, active_low=True):
        gpio = self.m.gpio.get(port)
        if gpio is None:
            raise KeyError("unknown GPIO %r" % (port,))
        self.cs_pin = (port, pin)
        self.cs_active_low = active_low

        def listener(p, level):
            if p != pin:
                return
            active = (level == 0) if active_low else (level == 1)
            self.cs_level = level
            if active:
                self._start_frame("cs")
            else:
                self._end_frame("cs")
        gpio.add_listener(listener)
        return self

    # -- SPI slave -----------------------------------------------------
    def on_tx_byte(self, b):
        b &= 0xFF
        self.tx_count += 1
        now = self.m.vtime
        if not self.cur:
            self.cur_start = now
        elif now - self.last_byte_t > self.frame_gap:
            self._end_frame("gap")
            self.cur_start = now
        self.last_byte_t = now
        self.cur.append(b)
        if len(self.cur) >= 512:
            self._end_frame("maxlen")
        if self.rx_queue:
            self.rx_count += 1
            return self.rx_queue.pop(0)
        return 0

    def on_cs(self, level):
        active = (level == 0) if self.cs_active_low else (level == 1)
        if active:
            self._start_frame("cs")
        else:
            self._end_frame("cs")

    def _start_frame(self, why):
        if self.cur:
            self._end_frame(why + "restart")
        self.cur_start = self.m.vtime

    def _end_frame(self, why):
        if not self.cur:
            return
        ent = dict(t=self.cur_start, end=self.m.vtime,
                   data=bytes(self.cur), reason=why)
        self.frames.append(ent)
        if len(self.frames) > self.max_frames:
            del self.frames[:self.max_frames // 2]
        self.cur = bytearray()

    # -- RX injection --------------------------------------------------
    def inject_rx(self, data):
        self.rx_queue.extend(bytes(data))
        return len(self.rx_queue)

    def drain_rx(self):
        data = bytes(self.rx_queue)
        self.rx_queue.clear()
        return data

    # -- reporting / export -------------------------------------------
    def summary(self, n=20):
        lines = ["RF transceiver %s (spi=%s, cs=%s): %d bytes, %d frames"
                 % (self.name, self.spi_name, self.cs_pin, self.tx_count,
                    len(self.frames))]
        for f in self.frames[-n:]:
            hexs = " ".join("%02X" % b for b in f["data"][:32])
            if len(f["data"]) > 32:
                hexs += " ... (%d bytes)" % len(f["data"])
            lines.append("  t=%-8d len=%-3d [%s] %s"
                         % (f["t"], len(f["data"]), f["reason"], hexs))
        return "\n".join(lines)

    def frame_hex(self, idx=-1):
        f = self.frames[idx]
        return " ".join("%02X" % b for b in f["data"])

    def save_csv(self, path):
        os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
        with open(path, "w", encoding="utf-8") as f:
            f.write("index,vtime_start,vtime_end,len,reason,data_hex\n")
            for i, fr in enumerate(self.frames):
                f.write("%d,%d,%d,%d,%s,%s\n"
                        % (i, fr["t"], fr["end"], len(fr["data"]),
                           fr["reason"], fr["data"].hex()))
        return path

    def save_json(self, path):
        os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
        with open(path, "w", encoding="utf-8") as f:
            json.dump([dict(index=i, vtime_start=fr["t"],
                            vtime_end=fr["end"],
                            data_hex=fr["data"].hex(),
                            reason=fr["reason"])
                       for i, fr in enumerate(self.frames)], f, indent=1)
        return path


class DisplaySpiDevice(SPIByteDevice):
    """Passive SPI sniffer for a display controller (records all bytes)."""

    def __init__(self, name="LCD_SPI"):
        super().__init__(name)
        self.frames = []
        self.cur = bytearray()
        self.in_frame = False

    def on_cs(self, level):
        if level == 0:
            self.cur = bytearray()
            self.in_frame = True
        else:
            if self.cur:
                self.frames.append(bytes(self.cur))
            self.in_frame = False

    def on_tx_byte(self, b):
        self.bytes.append(b & 0xFF)
        if self.in_frame:
            self.cur.append(b & 0xFF)
        return 0

    def summary(self, n=10):
        lines = ["SPI device %s: %d bytes, %d frames"
                 % (self.name, len(self.bytes), len(self.frames))]
        for fr in self.frames[-n:]:
            lines.append("  " + " ".join("%02X" % b for b in fr[:32]))
        return "\n".join(lines)
