#!/usr/bin/env python3
"""Symbol names for the AD200 Pro firmware (task-1 deliverable).

Every entry lists the *evidence* used to choose the name.  Names are
`subsystem_action` style.  Anything not listed stays `FUNC_xxxxxxxx`.

Evidence types:
  VEC   = appears in the vector table at 0x08003000
  CALL  = call graph position (caller/callee names)
  MMIO  = directly touched peripheral registers
  RAM   = global variables / structure offsets touched
  STR   = string / data-table reference
  ION   = instruction-level evidence (bit-bang sequence, constants, ...)

Confidence: [C] confirmed by direct instruction evidence,
            [P] strong inference (marked [推测] in docs).
"""

NAMES = {
    # ---------------------------------------------------------------- startup / runtime
    0x080030C0: 'startup_c',                # [C] sets SP=0x200008B0, calls scatter_load, bx main
    0x080030C8: 'startup_bx_main',          # [C] bx 0x0800CDB0
    0x080030D4: 'reset_handler',            # [C] VEC[1]; calls SystemInit then startup_c
    0x080030E6: 'default_handler',          # [C] VEC[0] = `b .` infinite loop
    0x080030F0: 'udivmod32',                # [C] shift/subtract divide: r0=quot, r1=rem
    0x08003100: 'udivmod32_loop',           # [C] inner loop of udivmod32 (shared entry)
    0x0800311C: 'udiv32',                   # [P] 32-bit divide wrapper (used by u64 math)
    0x08003144: 'crc16_block',              # [P] block checksum used before flash programming
    0x08003548: 'scatter_load',             # [C] walks table 0x0800E3E4 {src,dst,len,fn}
    0x0800BD0C: 'SystemInit',               # [P] VEC[1] first call: RCC CR/CFGR/CFGR2/CFGR3/CR2 reset
    0x0800CDB0: 'main',                     # [C] entry after scatter; copies vector table, main loop

    # ---------------------------------------------------------------- vector handlers
    0x0800A25C: 'NMI_Handler',              # [C] VEC[2]
    0x08008200: 'HardFault_Handler',        # [C] VEC[3]
    0x0800B81C: 'SVCall_Handler',           # [C] VEC[11]
    0x0800A90C: 'PendSV_Handler',           # [C] VEC[14]
    0x0800AC0C: 'RTC_IRQHandler',           # [C] VEC[18] IRQ2 RTC; sets 0x2000020D=1, calls RTC clear
    0x080053F0: 'EXTI0_1_IRQHandler',       # [C] VEC[21] IRQ5; reads EXTI_PR bit0
    0x08005478: 'EXTI2_3_IRQHandler',       # [C] VEC[22] IRQ6; clears EXTI line3
    0x08005484: 'EXTI4_15_IRQHandler',      # [C] VEC[23] IRQ7; EXTI_PR bits 4/5/10
    0x080044D0: 'DMA1_Ch1_IRQHandler',      # [C] VEC[25] IRQ9; calls dma1_ch1_task(0x200002A8)
    0x0800BE1C: 'TIM1_BRK_UP_TRG_COM_IRQHandler',  # [C] VEC[29] IRQ13
    0x0800BE58: 'TIM2_IRQHandler',          # [C] VEC[31] IRQ15
    0x0800BDE0: 'TIM14_IRQHandler',         # [C] VEC[35] IRQ19
    0x0800BC8C: 'SysTick_Handler',          # [C] VEC[15]; calls 0x08007034 + 0x08007CB4
    0x08007034: 'systick_tick_inc',         # [C] ++*(u32*)0x20000100
    0x08007CB4: 'systick_dispatch',         # [P] per-ms housekeeping chain

    # ---------------------------------------------------------------- clock / power
    0x08007044: 'flash_acr_init',           # [C] FLASH_ACR |= 0x10, then latency/prefetch
    0x08007064: 'flash_set_latency',        # [P] FLASH_ACR latency setup
    0x08007088: 'flash_prefetch_config',    # [P] FLASH_ACR prefetch / APB2ENR
    0x080070C0: 'NVIC_DisableIRQ',          # [C] SCS_ICER write
    0x080070D4: 'NVIC_EnableIRQ',           # [C] SCS_ISER write
    0x080070E8: 'NVIC_SetPriority',         # [C] SCS_SHPR2/3 write
    0x080070F0: 'scb_set_sleep',            # [C] SCB_SCR read-modify-write (sleepdeep/sleeponexit)
    0x08007124: 'rcc_enable_hse_pll',       # [P] RCC_CR/CFGR + PWR_CR: start HSE/PLL
    0x08007244: 'rcc_clock_switch',         # [C] FLASH_ACR + RCC_CFGR (SW/SWS) switch, table 0x0800D00C
    0x08007398: 'rcc_delay_short',          # [P] tiny delay used while switching clock
    0x080073A4: 'rcc_get_clock_freq',       # [P] derives Hz from RCC_CFGR
    0x0800742C: 'rcc_clock_config',         # [P] full RCC/PLL/PWR init (451 insns)
    0x08007C68: 'systick_disable',          # [C] SYSTICK_CSR write
    0x08007C84: 'systick_init',             # [C] SYSTICK_CSR/RVR + SCS_CCR/SHPR2; reload from 0x20000104
    0x08007A30: 'rtc_clock_disable',        # [C] RCC_BDCR r/w (clear RTCEN) + NVIC IRQ2 disable
    0x08007A58: 'rtc_clock_enable',         # [C] RCC_BDCR r/w (set RTCEN/LSE) + NVIC IRQ2 enable
    0x08007990: 'rtc_init',                 # [P] RTC TR/DR/CR/ISR init
    0x08007A88: 'rtc_set_time',             # [P] RTC TR/DR write
    0x08007B50: 'rtc_get_time',             # [P] RTC TR/DR read
    0x08007C34: 'rtc_wait_ready',           # [P] poll RTC ISR RSF/INITS
    0x08007954: 'rtc_clear_flags',          # [P] RTC ISR clear + EXTI/PR clear

    # ---------------------------------------------------------------- GPIO / EXTI
    0x08005544: 'gpio_init_all',            # [C] hard-coded MODER/PUPDR/ODR for GPIOA/B/C/D/F
    0x08006D3C: 'exti_config',              # [C] cfg{pinmask,mode,pull,af}: MODER/OTYPER/PUPDR/AFR/EXTICR/IMR/EMR/RTSR/FTSR
    0x08006E94: 'gpio_read_pin',            # [C] return (port->IDR & mask) ? 1 : 0
    0x08006E9E: 'gpio_write_pin',           # [C] mask,level -> BSRR/BRR
    0x08006EAC: 'get_tick',                 # [C] return *(u32*)0x20000100
    0x08006D24: 'exti_clear_pending',       # [C] EXTI_PR = mask
    0x08009758: 'board_init_inputs',        # [C] GPIOA/B/C/F clock + input/output + EXTI lines + NVIC5/7
    0x08009730: 'dma1_clock_init',          # [C] RCC_AHBENR |= DMA1EN, NVIC IRQ9
    0x08005A04: 'charge_monitor',           # [P] charge voltage supervision (0x78/0xF0 compares)
    0x080060AC: 'charge_voltage_convert',   # [P]
    0x0800CEC4: 'charge_power_on',          # [P] EXTI_Config + GPIOB mask 0x800 (PB11)
    0x0800CEE8: 'charge_power_off',         # [P] EXTI_Config + EXTI_IMR update
    0x0800848C: 'gpio_power_latch',         # [P] GPIOB write + 0x800C7C8/0x800CF14
    0x0800CF14: 'power_off_sequence',       # [P] called from 0x800CC64/0x800848C

    # ---------------------------------------------------------------- timers
    0x08007D08: 'tim_timebase_init',        # [C] TIM base struct -> PSC/ARR/CR1 (calls TIM clock enable)
    0x08007D3C: 'tim_clock_enable',         # [C] RCC_APB2ENR/APB1ENR for TIMx
    0x08007E18: 'tim_output_compare_init',  # [P] CCMR/CCER/CCR setup
    0x08007F08: 'tim_irq_handler',          # [C] common handler, arg = TIM device struct (0x20000318/54/CC)
    0x0800806C: 'tim_set_compare',          # [P] CCR1 write
    0x080080A2: 'tim_pwm_config',           # [P] PWM channel config
    0x08008172: 'tim_set_compare2',         # [P] CCR2 write
    0x080081AC: 'tim17_init',               # [C] TIM17(0x40014800) CR1 + clock
    0x0800BE88: 'tim_get_bus_clock',        # [P] derives TIM input clock (RCC CFGR)
    0x0800BF14: 'tim17_pwm_start',          # [P]
    0x08007DD4: 'tim_timebase_start',       # [P] TIM struct -> CR1 CEN
    0x08007DEC: 'tim_stop',                 # [P]
    0x08007804: 'tim_oc_setup_8000',        # [P] configuration helper for TIM device 0x200002E4
    0x08007878: 'tim_oc_apply',             # [P] applies CCER/CCR + EXTI for TIM device 0x200002E4

    # ---------------------------------------------------------------- ADC / DMA
    0x0800356C: 'adc_hw_init',              # [C] ADC CR/CFGR1/CFGR2/SMPR/TR/CCR (0x40012400+0x308)
    0x08003598: 'adc_task',                 # [C] main loop: 4-sample sort/median per channel, store results
    0x08009698: 'adc_dma_init',             # [C] ADC struct 0x2000025C + DMA config; channels 3/10/11/12
    0x0800641C: 'adc_dma_config',           # [C] programs DMA channel for one channel index
    0x08006550: 'adc_dma_enable',           # [P] starts ADC+DMA conversions
    0x080066C4: 'dma_clock_enable',         # [C] RCC_APB2ENR bit for DMA
    0x080068A4: 'dma1_ch1_task',            # [C] DMA1 CH1 transfer complete/error handling
    0x08006748: 'adc_read_block',           # [P] copy/convert 16-byte ADC result block
    0x080067E8: 'adc_wait_eoc',             # [P]
    0x08006858: 'adc_start_conversion',     # [P]
    0x08006B28: 'dma_disable_channel',      # [P]
    0x08006B7C: 'dma_config_channel',       # [P]
    0x080036D0: 'adc_calibrate',            # [P]
    0x080037B2: 'adc_sample_one',           # [P]
    0x08003818: 'adc_read_sequence',        # [P]
    0x080044E0: 'dma_set_memory_addr',      # [P]

    # ---------------------------------------------------------------- internal Flash param storage
    0x0800587C: 'flash_erase_param_page',   # [C] erase page 0x0801F000 (struct{0,0x0801F000,1})
    0x080058A4: 'flash_mass_erase',         # [C] FLASH_CR MER + STRT
    0x080058C8: 'flash_erase_page',         # [C] FLASH_CR PER + AR + STRT
    0x080058EC: 'flash_program_halfword',   # [C] FLASH_CR PG, *(u16*)addr = value
    0x0800590C: 'flash_read_params',        # [C] copy 8 words from flash -> 0x20000464
    0x08005930: 'flash_read_word',          # [C] returns *(u32*)addr
    0x0800593C: 'flash_get_status',         # [C] FLASH_SR decode (WRPRTERR/PGERR/EOP) + clear
    0x08005974: 'flash_write_params',       # [C] program 8 words from 0x20000464
    0x080059B4: 'flash_wait_last_operation',# [C] poll BSY with timeout 50000, clear flags
    0x08006BCC: 'flash_erase_range',        # [C] struct{type,start,pages}; type==1 -> mass erase
    0x08006C6C: 'flash_lock',               # [C] FLASH_CR |= 0x80
    0x08006C80: 'flash_program_record',     # [C] program 1/2/4 halfwords (with checksum)
    0x08006CFC: 'flash_unlock',             # [C] FLASH_KEYR 0x45670123/0xCDEF89AB
    0x0800B820: 'flash_store_params',       # [P] unlock+write params+lock (called by save/factory tasks)
    0x0800B7F4: 'flash_erase_params',       # [P]
    0x08005974: 'flash_write_params',

    # ---------------------------------------------------------------- 2.4G radio (CC2500 style SPI)
    0x08008778: 'rf_bus_init',              # [C] PC6/PC7 + PB12/PB13/PB14 mode & levels
    0x0800CA2C: 'rf_chip_reset',            # [C] CSN pulses, wait MISO, SRES strobe 0x30
    0x0800CAB0: 'rf_spi_read_byte',         # [C] 8 clocks on PB13, sample PB14, MSB first
    0x0800CAEC: 'rf_spi_write_byte',        # [C] shift out MSB on PB12, clock PB13, sample PB14
    0x0800CBA8: 'rf_select',                # [C] CSN(PC6)=0, wait MISO
    0x0800CBC8: 'rf_deselect',              # [C] CSN(PC6)=1
    0x0800CB38: 'rf_spi_transfer',          # [C] select, cmd byte, read len bytes, deselect
    0x0800CB64: 'rf_write_cmd',             # [C] select, 1 command byte, deselect (strobe)
    0x0800CB7C: 'rf_write_cmd_buf',         # [C] select, cmd byte, write len bytes, deselect
    0x0800CAA2: 'rf_write_reg8',            # [C] write single register (addr, value)
    0x0800CA16: 'rf_read_reg8',             # [C] addr|0x80 then read 1 byte
    0x0800CA08: 'rf_read_reg_buf',          # [C] command 0xFF path used by reg read helpers
    0x0800C858: 'rf_chip_config',           # [C] write registers 0x00..0x2E, 0x7E, strobes 0x33/0x39
    0x08004860: 'rf_init',                  # [C] bus init, reset, verify reg0x0D==0x5C, config, calibrate
    0x080048E4: 'rf_set_channel',           # [P] channel/freq register update (PB0 EXTI related)
    0x0800B9AC: 'rf_set_power_level',       # [C] divide level, write regs 4/5, strobes 0x33, delay
    0x0800B96C: 'rf_write_pair',            # [P] write two consecutive registers
    0x0800AA8C: 'rf_apply_settings',        # [P] strobe sequence 0x36/0x33/0x3A/0x34 based on RF state
    0x0800B5B0: 'rf_poll_settings',         # [C] compares 0x20000203 with 0x2000016C, re-applies
    0x0800C680: 'rf_wakeup_sequence',       # [P] strobes 0x36/0x33/0x3A/0x34 after timeout
    0x0800B888: 'rf_rx_timeout_task',       # [C] TIM2 ISR: RX state machine timeout/reset
    0x08009D2C: 'rf_rx_state_reset',        # [P] clears RF RX state bytes
    0x08009E88: 'flash_trigger_pulse',      # [P] PA0/PA7 pulse with length from 0x200001B2
    0x0800C828: 'delay_ms',                 # [C] nested delay (outer count = r0)
    0x0800C768: 'delay_us',                 # [C] nested delay (~1us per unit)

    # ---------------------------------------------------------------- UI (3x3 key matrix)
    0x08008818: 'ui_key_scan_task',         # [C] PB3/4/5 rows x PC10/11/12 columns, debounce, dispatch
    0x08008D08: 'ui_key_1',                 # [P] key matrix id 0x001 handler
    0x0800C190: 'ui_key_2',                 # [P] key matrix id 0x002 handler
    0x08009B7C: 'ui_key_4',                 # [P] key matrix id 0x004 handler
    0x0800B90C: 'ui_key_20',                # [P] key matrix id 0x020 handler
    0x0800BC34: 'ui_key_80',                # [P] key matrix id 0x080 handler
    0x08004270: 'ui_key_100',               # [P] key matrix id 0x100 handler
    0x080042A0: 'ui_key_10',                # [P] key matrix id 0x010 handler
    0x080048E4: 'ui_key_14',                # [P] key matrix id 0x014 handler
    0x08009B38: 'ui_factory_reset_task',    # [P] erase param page + defaults when combo pressed
    0x08003BEC: 'ui_power_switch_task',     # [P] GPIOA mode manipulation
    0x0800C2D8: 'ui_menu_task',             # [P]

    # ---------------------------------------------------------------- application / main loop tasks
    0x08004500: 'flash_power_task',         # [P] calls charge state machines
    0x080045AC: 'flash_charge_state_machine',  # [P] 293 insns, power/charge tables 0x0800E314
    0x08008504: 'flash_trigger_state_machine', # [P] 298 insns, trigger + power level
    0x08005F6C: 'measurement_task',         # [P] ADC data -> math (0x0800D412 table)
    0x08006118: 'power_level_dispatch',     # [P] switch on 1..7 -> per-mode compute
    0x08006188: 'power_level_dispatch_b',   # [P]
    0x08008364: 'tick_counter_task',        # [P] uses udiv + 0x14/0x28 compare
    0x08009228: 'display_task',             # [P] 273 insns, drives state machine / LCD data
    0x0800A2CC: 'status_led_task',          # [P] gpio_write_pin + 0x800A260
    0x0800BA00: 'charge_enable_task',       # [C] toggles charge on/off via 0x800CEC4/0x800CEE8
    0x08009CF0: 'rf_rx_timeout_counter',    # [P] tick counter up to 1800 then clear
    0x0800BD68: 'app_state_init',           # [C] PA1=1, delay 100, RF init, TIM/RTC setup
    0x0800AC24: 'boot_reason_check',        # [C] RCC_CSR reset flags + flash 0x0801F800 magic
    0x0800AC58: 'settings_load_task',       # [C] flash_read_params + defaults + param page erase
    0x0800AAF4: 'rf_tx_task',               # [P] main loop RF transmit
    0x0800BA5C: 'system_reinit_task',       # [C] if 0x200001C2: gpio_init_all, RTC, clock re-init
    0x0800BB7C: 'reinit_prepare',           # [P]
    0x0800BC08: 'exti_enable_group',        # [P] EXTI_Config + EXTI_IMR
    0x0800BBE0: 'exti_disable_group',       # [P]
    0x0800BC98: 'clock_init_task',          # [C] RCC clock config + SysTick init (called at boot and on reinit)
    0x0800C248: 'board_init',               # [C] full init (same set as main init prologue)
    0x0800CC14: 'exti_config_group',        # [P]
    0x0800CB38: 'rf_spi_transfer',
    0x0800C740: 'flash_params_default',     # [P] small helper used by flash tasks
    0x0800CCC8: 'settings_validate',        # [P] 90 insns, checks 0x20000418 fields
    0x0800CC64: 'settings_save',            # [P]
    0x0800CC40: 'settings_flag_set',        # [P]
    0x0800C6D8: 'math_helper_34a4',         # [P]
    0x0800C706: 'ram_clear_block',          # [P]
    0x0800C7C8: 'ram_clear_41c',            # [P]
    0x0800C1F0: 'rf_status_update',         # [P]
    0x0800C354: 'rf_rx_parse',              # [P] 141 insns
    0x0800C4D4: 'rf_rx_handler',            # [P]
    0x0800C554: 'rf_rx_process',            # [P] 106 insns
    0x0800C248: 'board_init',
    0x0800BC34: 'ui_key_80',
    0x0800BE1C: 'TIM1_BRK_UP_TRG_COM_IRQHandler',
    0x0800BE58: 'TIM2_IRQHandler',
    0x0800BDE0: 'TIM14_IRQHandler',
    0x0800B888: 'rf_rx_timeout_task',
    0x0800B9E4: 'rf_status_read',           # [P]
    0x0800BA40: 'rf_rx_enable',             # [P] strobe 0x34/0x39
    0x0800B858: 'rf_tx_start',              # [P]
    0x0800B810: 'rf_flush_tx',              # [P] strobe 0x3A
    0x0800B7F4: 'flash_erase_params',
    0x0800B4EC: 'rf_rx_done',               # [P]
    0x0800B170: 'rf_rx_crc',                # [P]
    0x0800B134: 'muldiv_scale',             # [P] math used for power/energy scaling
    0x0800B5DC: 'power_level_mode1',        # [P]
    0x0800B6A4: 'power_level_mode2',        # [P]
    0x0800B77C: 'power_level_mode3',        # [P]
    0x0800B820: 'flash_store_params',
    0x0800B9AC: 'rf_set_power_level',
    0x0800BB7C: 'reinit_prepare',
    0x0800BC34: 'ui_key_80',
    0x0800CB38: 'rf_spi_transfer',
    0x0800BD68: 'app_state_init',
    0x0800C680: 'rf_wakeup_sequence',
    0x0800C808: 'delay_10ms',               # [P] delay_us(100) wrapper
    0x0800C740: 'flash_params_default',
    0x0800C858: 'rf_chip_config',
    0x0800C828: 'delay_ms',
    0x0800C768: 'delay_us',
    0x0800CAA2: 'rf_write_reg8',
    # main-loop ADC/measurement helpers with 4-sample median filter
    0x08003598: 'adc_task',
    0x080061F8: 'level_table_lookup_a',     # [P] table 0x0800DC4A
    0x0800626C: 'level_table_lookup_b',     # [P] table 0x0800D18A
    0x0800632C: 'level_table_lookup_c',     # [P]
    0x080063A0: 'adc_result_update',        # [P]
    0x08008AD0: 'level_table_lookup_d',     # [P]
    0x08008B98: 'level_table_lookup_e',     # [P]
    0x08009FF0: 'level_table_lookup_f',     # [P]
    0x0800A0E4: 'level_table_lookup_g',     # [P]
    0x0800A354: 'level_table_lookup_h',     # [P]
    0x0800A41C: 'level_table_lookup_i',     # [P]
    0x0800A628: 'level_table_lookup_j',     # [P]
    0x0800A6F0: 'level_table_lookup_k',     # [P]
    0x0800A4F4: 'level_table_lookup_l',     # [P]
    0x0800560C: 'level_table_lookup_m',     # [P]
    0x080056D4: 'level_table_lookup_n',     # [P]
    0x080057B8: 'level_table_lookup_o',     # [P]
}
