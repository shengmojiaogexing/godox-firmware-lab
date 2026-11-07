"""input.py -- scriptable input injection for the X3-C trigger emulator.

The X3-C hardware is not fully mapped yet, so the module is written around a
small abstraction that can be wired up as the hardware facts land:

* ``GpioInput``   -- drive a GPIO port/pin level (buttons, power, hot-shoe
                     detect, etc.) and optionally pulse the matching EXTI line.
* ``ExtiInput``   -- pend an NVIC IRQ directly (for wake-up / key IRQs).
* ``TouchInput``  -- feed an I2C (or SPI) touch controller with register
                     responses (I2C slave model once the controller is known).
* ``InputScript`` -- a list of timed events (``at`` = instruction/vtime number)
                     that can be replayed in ``Machine.run`` steps.

Example::

    from input import InputController
    inp = InputController(machine)
    inp.map_key("menu", "GPIOB", 5, active_low=True, irq=40)
    inp.press("menu")           # level + EXTI + IRQ
    machine.run(max_insns=500000)
    inp.release("menu")
"""
from __future__ import annotations


class InputEvent:
    def __init__(self, kind, **kw):
        self.kind = kind
        self.kw = kw

    def __repr__(self):
        return "InputEvent(%s, %s)" % (self.kind, self.kw)


class InputController:
    def __init__(self, machine):
        self.m = machine
        self.keys = {}          # name -> dict(port, pin, active_low, irq)
        self.state = {}
        self.events = []        # applied event log
        self.touch = None       # I2C slave model object, if any

    # -- configuration -------------------------------------------------
    def map_key(self, name, port, pin, active_low=True, irq=None):
        self.keys[name] = dict(port=port, pin=pin, active_low=active_low,
                               irq=irq)
        self.state[name] = False

    def map_touch(self, device):
        """device: object with .feed(bytes) / .on_write(reg, value) hooks."""
        self.touch = device

    # -- low level -----------------------------------------------------
    def _gpio(self, port):
        gpio = self.m.gpio.get(port)
        if gpio is None:
            raise KeyError("unknown GPIO port %r" % (port,))
        return gpio

    def set_pin(self, port, pin, level):
        self._gpio(port).set_input(pin, level)
        self.events.append(("gpio", port, pin, level, self.m.vtime))

    def pulse_exti(self, pin, irq=None):
        """Simulate an EXTI edge: set EXTI pending + optional NVIC IRQ."""
        exti = getattr(self.m, "exti", None)
        if exti is not None:
            exti.pr |= (1 << pin)
        if irq is not None:
            self.m.pend_irq(irq)

    def pend_irq(self, irq):
        return self.m.pend_irq(irq)

    # -- key API -------------------------------------------------------
    def press(self, name):
        info = self.keys[name]
        level = 0 if info["active_low"] else 1
        self.set_pin(info["port"], info["pin"], level)
        self.state[name] = True
        if info.get("irq") is not None:
            self.pulse_exti(info["pin"], info["irq"])
        return True

    def release(self, name):
        info = self.keys[name]
        level = 1 if info["active_low"] else 0
        self.set_pin(info["port"], info["pin"], level)
        self.state[name] = False
        return True

    def click(self, name, hold_insns=20000):
        self.press(name)
        self.m.run(max_insns=hold_insns)
        self.release(name)

    def power(self, on=True):
        """Power key: modelled as a long-press style GPIO + IRQ."""
        if "power" in self.keys:
            return self.press("power") if on else self.release("power")
        self.events.append(("power", on, self.m.vtime))
        return True

    # -- touch ---------------------------------------------------------
    def touch(self, x, y, down=True):
        if self.touch is not None and hasattr(self.touch, "inject_touch"):
            self.touch.inject_touch(x, y, down)
        self.events.append(("touch", x, y, down, self.m.vtime))
        return True

    # -- raw -----------------------------------------------------------
    def uart_rx(self, port, data):
        dev = self.m.uart.get(port)
        if dev:
            dev.feed_rx(data)
            return True
        return False

    def spi_rx(self, port, data):
        dev = self.m.spi.get(port)
        if dev:
            dev.feed_rx(data)
            return True
        return False

    def i2c_rx(self, port, data):
        dev = self.m.i2c.get(port)
        if dev:
            dev.feed_rx(data)
            return True
        return False


class TouchController:
    """Minimal I2C touch-controller stub (e.g. FT6236/GT911 style).

    Until the real part is identified it exposes a register file and injects
    touch coordinates through a queue when a coordinate register is read.
    """

    def __init__(self, addr=0x38, width=240, height=240, name="touch"):
        self.addr = addr
        self.width = width
        self.height = height
        self.name = name
        self.pending = []
        self.regs = {}

    def inject_touch(self, x, y, down=True):
        self.pending.append((x, y, down))

    # returns bytes for the master to read; hook as i2c slave read callback
    def read(self, reg, n):
        if self.pending and reg in (0x00, 0x01, 0x02, 0x03, 0x04):
            x, y, down = self.pending[0]
            if reg == 0x01:
                self.pending.pop(0)
                return bytes([1 if down else 0, (x >> 8) & 0xF, x & 0xFF,
                              (y >> 8) & 0xF, y & 0xFF][:n])
            return bytes([0] * n)
        return bytes([0] * n)


class InputScript:
    """A replayable list of input events keyed by instruction time."""

    def __init__(self, controller):
        self.ctl = controller
        self.items = []      # (at_vtime, callable)

    def add(self, at, fn):
        self.items.append((at, fn))
        self.items.sort(key=lambda x: x[0])

    def add_press(self, at, name, hold=None):
        if hold is None:
            self.add(at, lambda: self.ctl.press(name))
        else:
            self.add(at, lambda: self.ctl.press(name))
            self.add(at + hold, lambda: self.ctl.release(name))

    def add_touch(self, at, x, y, down=True):
        self.add(at, lambda: self.ctl.touch(x, y, down))

    def due(self, vtime):
        out = []
        while self.items and self.items[0][0] <= vtime:
            _, fn = self.items.pop(0)
            out.append(fn)
        return out

    def run(self, machine, max_insns, step=50000, log=print):
        """Run the machine, applying scripted events at their vtime."""
        start = machine.vtime
        while machine.vtime - start < max_insns:
            for fn in self.due(machine.vtime):
                fn()
            machine.run(max_insns=step)
            if machine.stop_reason:
                break
        return machine.vtime - start
