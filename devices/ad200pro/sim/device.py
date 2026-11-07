"""Virtual AD200 Pro device model: analog inputs, buttons, RF link, charge model.

This is the "physical world" the firmware sees.  The emulator wires it to the
STM32F0 peripherals (GPIO input levels, ADC channels, bit-banged RF slave).
"""


class Device:
    def __init__(self):
        # --- analog world -------------------------------------------------
        self.battery_mv = 14400       # 14.4 V pack (2S Li-ion), mV
        self.cap_voltage = 0.0        # main capacitor, Volts (0 ... 350)
        self.cap_max = 350.0
        self.cap_min = 0.0
        self.temp_c = 28.0
        self.auto_charge = True       # capacitor charges while firmware drives the charger
        self.charge_rate = 40.0       # V per simulated 100 ms when charging enabled
        self.discharge_rate = 500.0   # V per flash event
        self._charging = False
        self.flash_count = 0
        self.ready_led = False

        # --- ADC channel map (channel -> value, 12 bit) -------------------
        # channel mapping is a *guess* of the board wiring; override with set_adc
        self.adc_defaults = {
            0: 0,                    # (unused)
            1: 0,                    # (unused)
            10: 3200,                # VREFINT-ish
        }
        self.adc_override = {}

        # --- GPIO input levels: {port_letter: {pin: 0/1}} -----------------
        self.gpio_inputs = {'A': {}, 'B': {}, 'C': {}, 'D': {}, 'F': {}}
        self.gpio_outputs = {'A': 0, 'B': 0, 'C': 0, 'D': 0, 'F': 0}
        self.gpio_dirs = {'A': 0, 'B': 0, 'C': 0, 'D': 0, 'F': 0}

        # --- buttons ------------------------------------------------------
        self.buttons = {
            'POWER': False, 'MODE': False, 'TEST': False,
            'SET': False, 'UP': False, 'DOWN': False, 'LEFT': False, 'RIGHT': False,
        }
        self.button_pins = {}         # (port,pin) -> button name, filled by config

        # --- RF (2.4 GHz) -------------------------------------------------
        self.rf_rx = []               # list of (channel:int, payload:bytes)
        self.rf_tx = []               # list of payload bytes the firmware "sent"
        self.rf_slave_mode = 'auto'   # 'auto' | 'zero' | 'ff' | 'regmap'
        self.rf_regmap = {}            # address -> byte, for 'regmap' mode
        self.rf_present = True

        # --- bit-banged SPI slave (RF chip / optional peripherals) --------
        self.spi_writes = []           # [(cmd_byte, payload bytes)]
        self.spi_reads = []            # [cmd_byte]

    # ------------------------------------------------------------------ ADC
    def adc_value(self, channel):
        if channel in self.adc_override:
            return int(self.adc_override[channel]) & 0xFFF
        if channel in (0, 3):
            # battery divider: assume 1/6 divider against 3.3V reference
            mv = max(0.0, min(self.battery_mv, 20000))
            return int(mv / 6.0 / 3300.0 * 4095) & 0xFFF
        if channel in (1, 10):
            return int(max(0.0, self.cap_voltage) / 400.0 * 4095) & 0xFFF
        if channel in (2, 11):
            return int((self.temp_c + 40) / 100.0 * 4095) & 0xFFF
        if channel == 12:
            return 1489        # internal 1.2 V reference
        if channel in self.adc_defaults:
            return self.adc_defaults[channel] & 0xFFF
        return 0x800

    def set_adc(self, channel, value):
        self.adc_override[int(channel)] = int(value) & 0xFFF

    # --------------------------------------------------------------- charge
    def set_charge_enabled(self, on):
        self._charging = bool(on)

    def tick(self, elapsed_ms):
        if self.auto_charge and self._charging and self.cap_voltage < self.cap_max:
            self.cap_voltage = min(self.cap_max, self.cap_voltage + self.charge_rate * elapsed_ms / 100.0)

    def flash(self):
        self.cap_voltage = max(self.cap_min, self.cap_voltage - self.discharge_rate)
        self.flash_count += 1

    # ---------------------------------------------------------------- GPIO
    def set_input(self, port, pin, level):
        """force an external pin level; returns True if it changed"""
        d = self.gpio_inputs.setdefault(port, {})
        old = d.get(pin)
        d[pin] = 1 if level else 0
        return old != d[pin]

    def get_input(self, port, pin):
        return self.gpio_inputs.get(port, {}).get(pin, 0)

    def set_output(self, port, value):
        self.gpio_outputs[port] = value & 0xFFFF

    def set_dir(self, port, value):
        self.gpio_dirs[port] = value & 0xFFFF

    # ------------------------------------------------------------ buttons
    def press(self, name, down=True):
        if name in self.buttons:
            self.buttons[name] = bool(down)

    # ---------------------------------------------------------------- RF
    def inject_rf(self, channel, payload):
        self.rf_rx.append((int(channel), bytes(payload)))

    def state(self):
        return {
            'battery_mv': self.battery_mv,
            'cap_voltage': round(self.cap_voltage, 1),
            'cap_max': self.cap_max,
            'temp_c': round(self.temp_c, 1),
            'charging': self._charging,
            'auto_charge': self.auto_charge,
            'flash_count': self.flash_count,
            'buttons': dict(self.buttons),
            'gpio_inputs': {k: dict(v) for k, v in self.gpio_inputs.items()},
            'gpio_outputs': dict(self.gpio_outputs),
            'adc': {str(ch): self.adc_value(ch) for ch in range(16)},
            'rf_rx_pending': len(self.rf_rx),
            'rf_tx_count': len(self.rf_tx),
            'spi_writes': list(self.spi_writes[-20:]),
        }
