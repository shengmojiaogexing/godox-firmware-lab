# 03 – 函数清单与调用图（AD200 Pro V1.60 固件）

> 本文件由 `tools/an_doc.py` 依据 `analysis/functions.json` + `tools/an_names.py` 生成，可直接重跑。地址均为**运行地址**（镜像基址 `0x08003000`）。
> 命名依据与置信度见 `tools/an_names.py` 内每条注释；`FUNC_xxxxxxxx` 为尚未命名的函数。
> 总数：**323 个函数**（已命名 197）。

## 1. 调用图（自上而下）

```
0x0800cdb0 main                           83 ins
  0x08003598 adc_task                      147 ins
    0x08006748 adc_read_block                 70 ins
    0x080067e8 adc_wait_eoc                   52 ins
    0x0800c768 delay_us                       24 ins
  0x0800389c FUNC_0800389c                  34 ins
    0x08006e9e gpio_write_pin                  6 ins
  0x08003904 FUNC_08003904                 195 ins
    0x0800339c FUNC_0800339c                  11 ins
    0x080033f4 FUNC_080033f4                  19 ins
    0x08006e94 gpio_read_pin                   5 ins
  0x08003bec ui_power_switch_task           55 ins
    0x08006e9e gpio_write_pin                  6 ins
  0x08004500 flash_power_task               77 ins
    0x080045ac flash_charge_state_machine    293 ins
    0x08006e94 gpio_read_pin                   5 ins
    0x08008504 flash_trigger_state_machine   298 ins
    0x08009d58 FUNC_08009d58                  90 ins
  0x08005a04 charge_monitor                 37 ins
    0x080060ac charge_voltage_convert         49 ins
  0x08005f6c measurement_task               81 ins
    0x08003c68 FUNC_08003c68                  13 ins
    0x080042dc FUNC_080042dc                  52 ins
    0x08005c30 FUNC_08005c30                 265 ins
    0x08005ed4 FUNC_08005ed4                  58 ins
    0x08006e9e gpio_write_pin                  6 ins
    0x0800c1f0 rf_status_update               39 ins
  0x08006118 power_level_dispatch           44 ins
    0x08003ca4 FUNC_08003ca4                 110 ins
    0x0800560c level_table_lookup_m           77 ins
    0x08008ad0 level_table_lookup_d           77 ins
    0x08009ff0 level_table_lookup_f           94 ins
    0x0800a354 level_table_lookup_h           77 ins
    0x0800a628 level_table_lookup_j           77 ins
    0x0800b5dc power_level_mode1              77 ins
  0x08006f68 FUNC_08006f68                  37 ins
    0x08006eac get_tick                        3 ins
  0x08006fbc FUNC_08006fbc                  52 ins
    0x08006eac get_tick                        3 ins
  0x08007044 flash_acr_init                 11 ins
    0x08007064 flash_set_latency              13 ins
    0x08007088 flash_prefetch_config          22 ins
  0x08007a30 rtc_clock_disable              14 ins
    0x080070c0 NVIC_DisableIRQ                 7 ins
  0x08008364 tick_counter_task              58 ins
    0x080030f0 udivmod32                      22 ins
    0x08008340 FUNC_08008340                  12 ins
  0x08008818 ui_key_scan_task              299 ins
    0x080038f0 FUNC_080038f0                   7 ins
    0x08004270 ui_key_100                     22 ins
    0x080042a0 ui_key_10                      28 ins
    0x080048e4 ui_key_14                      41 ins
    0x080063a0 adc_result_update              60 ins
    0x08006d3c exti_config                   157 ins
    0x08006e94 gpio_read_pin                   5 ins
    0x08008d08 ui_key_1                       33 ins
    0x08009b7c ui_key_4                       73 ins
    0x0800b810 rf_flush_tx                     4 ins
    0x0800b90c ui_key_20                      46 ins
    0x0800b9ac rf_set_power_level             20 ins
    0x0800b9e4 rf_status_read                 12 ins
    0x0800ba40 rf_rx_enable                    8 ins
    0x0800bc34 ui_key_80                      39 ins
    0x0800c190 ui_key_2                        9 ins
    0x0800c2d8 ui_menu_task                   59 ins
    0x0800c768 delay_us                       24 ins
  0x08009228 display_task                  273 ins
    0x08003fd0 FUNC_08003fd0                 283 ins
    0x08004964 FUNC_08004964                  52 ins
    0x080049e8 FUNC_080049e8                   7 ins
    0x080049f8 FUNC_080049f8                  49 ins
    0x08004a84 FUNC_08004a84                  24 ins
    0x08004c74 FUNC_08004c74                  10 ins
    0x08004c8c FUNC_08004c8c                  21 ins
    0x08004cbc FUNC_08004cbc                  28 ins
    0x08004d04 FUNC_08004d04                  17 ins
    0x08004d2c FUNC_08004d2c                   7 ins
    0x08004d3c FUNC_08004d3c                   9 ins
    0x08004f70 FUNC_08004f70                  85 ins
    0x080051a8 FUNC_080051a8                   9 ins
    0x080055bc FUNC_080055bc                  28 ins
    0x080083e8 FUNC_080083e8                  33 ins
    0x08008c7c FUNC_08008c7c                  46 ins
    0x08008dec FUNC_08008dec                  44 ins
    0x08008f34 FUNC_08008f34                  44 ins
    0x08008fa0 FUNC_08008fa0                  44 ins
    0x0800900c FUNC_0800900c                  44 ins
    0x08009078 FUNC_08009078                  44 ins
    0x08009150 FUNC_08009150                  44 ins
    0x080091bc FUNC_080091bc                  45 ins
    0x0800ccac FUNC_0800ccac                  11 ins
  0x08009698 adc_dma_init                   62 ins
    0x08005608  (no body)
    0x0800641c adc_dma_config                140 ins
    0x08006550 adc_dma_enable                181 ins
  0x08009730 dma1_clock_init                16 ins
    0x080070d4 NVIC_EnableIRQ                  7 ins
    0x080070e8 NVIC_SetPriority                3 ins
  0x08009758 board_init_inputs             154 ins
    0x08006d3c exti_config                   157 ins
    0x08006e9e gpio_write_pin                  6 ins
    0x080070d4 NVIC_EnableIRQ                  7 ins
    0x080070e8 NVIC_SetPriority                3 ins
  0x080098e0 FUNC_080098e0                  14 ins
    0x08005608  (no body)
    0x08006eb8 FUNC_08006eb8                  78 ins
  0x0800993c FUNC_0800993c                  53 ins
    0x08005608  (no body)
    0x08007878 tim_oc_apply                  102 ins
    0x08007990 rtc_init                       75 ins
    0x08007a88 rtc_set_time                   90 ins
    0x08007b50 rtc_get_time                  105 ins
  0x080099c0 FUNC_080099c0                  16 ins
    0x08005608  (no body)
    0x08007d08 tim_timebase_init              24 ins
  0x080099ec FUNC_080099ec                  36 ins
    0x08005608  (no body)
    0x08007cc0 FUNC_08007cc0                  36 ins
    0x08007d08 tim_timebase_init              24 ins
    0x08007e18 tim_output_compare_init       113 ins
  0x08009a4c FUNC_08009a4c                  36 ins
    0x08005608  (no body)
    0x08007cc0 FUNC_08007cc0                  36 ins
    0x08007d08 tim_timebase_init              24 ins
    0x08007e18 tim_output_compare_init       113 ins
  0x08009aac FUNC_08009aac                  55 ins
    0x08005608  (no body)
    0x08007cc0 FUNC_08007cc0                  36 ins
    0x08007d08 tim_timebase_init              24 ins
    0x08007e18 tim_output_compare_init       113 ins
    0x0800806c tim_set_compare                22 ins
    0x080080a2 tim_pwm_config                100 ins
    0x08008172 tim_set_compare2               24 ins
  0x08009b38 ui_factory_reset_task          24 ins
    0x080043e4 FUNC_080043e4                  94 ins
    0x0800587c flash_erase_param_page         12 ins
    0x0800b7f4 flash_erase_params             12 ins
    0x0800b820 flash_store_params             21 ins
    0x0800c740 flash_params_default           18 ins
  0x08009cf0 rf_rx_timeout_counter          26 ins
  0x0800a2cc status_led_task                24 ins
    0x08006e9e gpio_write_pin                  6 ins
    0x0800a260 FUNC_0800a260                  46 ins
  0x0800aaf4 rf_tx_task                     66 ins
    0x08006e94 gpio_read_pin                   5 ins
    0x08006e9e gpio_write_pin                  6 ins
    0x08006f68 FUNC_08006f68                  37 ins
    0x08009228 display_task                  273 ins
    0x0800a934 FUNC_0800a934                  12 ins
    0x0800aa8c rf_apply_settings              34 ins
    0x0800b96c rf_write_pair                  24 ins
    0x0800c768 delay_us                       24 ins
    0x0800c828 delay_ms                       24 ins
    0x0800cb64 rf_write_cmd                    9 ins
  0x0800ac24 boot_reason_check              17 ins
    0x08005930 flash_read_word                 4 ins
  0x0800ac58 settings_load_task            210 ins
    0x080048e4 ui_key_14                      41 ins
    0x0800587c flash_erase_param_page         12 ins
    0x0800590c flash_read_params              13 ins
    0x0800b820 flash_store_params             21 ins
    0x0800ccc8 settings_validate              90 ins
  0x0800b5b0 rf_poll_settings               15 ins
    0x0800aa8c rf_apply_settings              34 ins
  0x0800ba00 charge_enable_task             24 ins
    0x0800cec4 charge_power_on                12 ins
    0x0800cee8 charge_power_off               17 ins
  0x0800ba5c system_reinit_task             89 ins
    0x080038f0 FUNC_080038f0                   7 ins
    0x08005544 gpio_init_all                  41 ins
    0x0800608c FUNC_0800608c                  13 ins
    0x08006e94 gpio_read_pin                   5 ins
    0x08006e9e gpio_write_pin                  6 ins
    0x08006f68 FUNC_08006f68                  37 ins
    0x080070f0 scb_set_sleep                  22 ins
    0x08007804 tim_oc_setup_8000              55 ins
    0x08007878 tim_oc_apply                  102 ins
    0x08007a30 rtc_clock_disable              14 ins
    0x08007a58 rtc_clock_enable               18 ins
    0x08008340 FUNC_08008340                  12 ins
    0x0800848c gpio_power_latch               25 ins
    0x080098e0 FUNC_080098e0                  14 ins
    0x0800990c FUNC_0800990c                  15 ins
    0x0800bb7c reinit_prepare                 32 ins
    0x0800bbe0 exti_disable_group             16 ins
    0x0800bc08 exti_enable_group              18 ins
    0x0800bc98 clock_init_task                49 ins
    0x0800c248 board_init                     41 ins
    0x0800c768 delay_us                       24 ins
    0x0800cbdc FUNC_0800cbdc                  19 ins
    0x0800cc14 exti_config_group              17 ins
    0x0800cec4 charge_power_on                12 ins
    0x0800cee8 charge_power_off               17 ins
  0x0800bc98 clock_init_task                49 ins
    0x080030f0 udivmod32                      22 ins
    0x080070e8 NVIC_SetPriority                3 ins
    0x08007124 rcc_enable_hse_pll            133 ins
    0x08007244 rcc_clock_switch              153 ins
    0x08007398 rcc_delay_short                 3 ins
    0x0800742c rcc_clock_config              451 ins
    0x08007c68 systick_disable                10 ins
    0x08007c84 systick_init                   18 ins
  0x0800bd68 app_state_init                 35 ins
    0x08003af4 FUNC_08003af4                  90 ins
    0x08004860 rf_init                        43 ins
    0x08005a04 charge_monitor                 37 ins
    0x08006e9e gpio_write_pin                  6 ins
    0x08007dd4 tim_timebase_start             11 ins
    0x080084d4 FUNC_080084d4                  19 ins
    0x0800c808 delay_10ms                     12 ins
    0x0800cc64 settings_save                  27 ins
  0x0800c680 rf_wakeup_sequence             33 ins
    0x08006064 FUNC_08006064                  14 ins
    0x0800c828 delay_ms                       24 ins
    0x0800caa2 rf_write_reg8                   5 ins
    0x0800cb64 rf_write_cmd                    9 ins
```

## 2. 中断处理链

| 向量 | 入口 | 名称 | 直接调用 |
|---|---|---|---|
| NMI | 0x0800a25c | `NMI_Handler` |  |
| HardFault | 0x08008200 | `HardFault_Handler` |  |
| SVCall | 0x0800b81c | `SVCall_Handler` |  |
| PendSV | 0x0800a90c | `PendSV_Handler` |  |
| SysTick | 0x0800bc8c | `SysTick_Handler` | systick_tick_inc systick_dispatch |
| IRQ0_WWDG | 0x080030e6 | `default_handler` |  |
| IRQ1_PVD | 0x080030e6 | `default_handler` |  |
| IRQ2_RTC | 0x0800ac0c | `RTC_IRQHandler` | rtc_clear_flags |
| IRQ3_FLASH | 0x080030e6 | `default_handler` |  |
| IRQ4_RCC | 0x080030e6 | `default_handler` |  |
| IRQ5_EXTI0_1 | 0x080053f0 | `EXTI0_1_IRQHandler` | FUNC_08005ae8 exti_clear_pending rf_rx_enable |
| IRQ6_EXTI2_3 | 0x08005478 | `EXTI2_3_IRQHandler` | exti_clear_pending |
| IRQ7_EXTI4_15 | 0x08005484 | `EXTI4_15_IRQHandler` | FUNC_08005ae8 exti_clear_pending gpio_read_pin FUNC_0800a954 rf_rx_enable FUNC_0800c798 |
| IRQ8_TSC | 0x080030e6 | `default_handler` |  |
| IRQ9_DMA1_Ch1 | 0x080044d0 | `DMA1_Ch1_IRQHandler` | dma1_ch1_task |
| IRQ10_DMA1_Ch2_3 | 0x080030e6 | `default_handler` |  |
| IRQ11_DMA1_Ch4_7 | 0x080030e6 | `default_handler` |  |
| IRQ12_ADC | 0x080030e6 | `default_handler` |  |
| IRQ13_TIM1_BRK_UP_TRG_COM | 0x0800be1c | `TIM1_BRK_UP_TRG_COM_IRQHandler` | gpio_write_pin tim_irq_handler FUNC_08009550 |
| IRQ14_TIM1_CC | 0x080030e6 | `default_handler` |  |
| IRQ15_TIM2 | 0x0800be58 | `TIM2_IRQHandler` | tim_irq_handler rf_rx_timeout_task |
| IRQ16_TIM3 | 0x080030e6 | `default_handler` |  |
| IRQ17_TIM6 | 0x080030e6 | `default_handler` |  |
| IRQ18_TIM7 | 0x080030e6 | `default_handler` |  |
| IRQ19_TIM14 | 0x0800bde0 | `TIM14_IRQHandler` | FUNC_08004818 tim_irq_handler FUNC_08009c14 FUNC_0800ae40 FUNC_0800c1a8 |
| IRQ20_TIM15 | 0x080030e6 | `default_handler` |  |
| IRQ21_TIM16 | 0x080030e6 | `default_handler` |  |
| IRQ22_TIM17 | 0x080030e6 | `default_handler` |  |
| IRQ23_I2C1 | 0x080030e6 | `default_handler` |  |
| IRQ24_I2C2 | 0x080030e6 | `default_handler` |  |
| IRQ25_SPI1 | 0x080030e6 | `default_handler` |  |
| IRQ26_SPI2 | 0x080030e6 | `default_handler` |  |
| IRQ27_USART1 | 0x080030e6 | `default_handler` |  |
| IRQ28_USART2 | 0x080030e6 | `default_handler` |  |
| IRQ29_USART3 | 0x080030e6 | `default_handler` |  |
| IRQ30_CEC | 0x080030e6 | `default_handler` |  |
| IRQ31_USB | 0x080030e6 | `default_handler` |  |

## 3. 全部函数（按地址）

| 地址 | 名称 | 指令数 | 调用者 | 被调用 | 外设(MMIO) | 关键 RAM | 字符串 |
|---|---|---|---|---|---|---|---|
| `0x080030c0` | `startup_c` | 5 |  | scatter_load |  | 0x200008b0 |  |
| `0x080030c8` | `startup_bx_main` | 2 | scatter_load |  |  |  |  |
| `0x080030d4` | `reset_handler` | 4 |  | SystemInit |  |  |  |
| `0x080030f0` | `udivmod32` | 22 | udiv32 adc_read_sequence FUNC_08003fd0 FUNC_080049f8 FUNC_08004ac8 FUNC_08004f70 FUNC_080051c0 FUNC_08005360 F |  |  |  |  |
| `0x08003100` | `udivmod32_loop` | 15 |  |  |  |  |  |
| `0x0800311c` | `udiv32` | 19 | FUNC_08005060 FUNC_080051c0 FUNC_08005c30 | udivmod32 |  |  |  |
| `0x08003144` | `crc16_block` | 17 | FUNC_080033b8 FUNC_080034a4 flash_program_record |  |  |  |  |
| `0x08003168` | `FUNC_08003168` | 159 | FUNC_08005c30 | FUNC_08003444 FUNC_08003464 FUNC_0800348a FUNC_080034a4 |  |  |  |
| `0x080032bc` | `FUNC_080032bc` | 165 | FUNC_08005c30 | FUNC_08003444 FUNC_08003464 FUNC_0800348a FUNC_080034a4 |  |  |  |
| `0x080032cc` | `FUNC_080032cc` | 100 | FUNC_08005c30 | FUNC_080034a4 |  |  |  |
| `0x0800339c` | `FUNC_0800339c` | 11 | FUNC_08003904 FUNC_08003af4 FUNC_08005c30 | FUNC_080034a4 |  |  |  |
| `0x080033b8` | `FUNC_080033b8` | 24 | FUNC_08005c30 | crc16_block |  |  |  |
| `0x080033f4` | `FUNC_080033f4` | 19 | FUNC_08003904 FUNC_08003af4 |  |  |  |  |
| `0x0800341c` | `FUNC_0800341c` | 19 | FUNC_08005c30 |  |  |  |  |
| `0x08003444` | `FUNC_08003444` | 16 | FUNC_08003168 FUNC_080032bc FUNC_080034a4 |  |  |  |  |
| `0x08003464` | `FUNC_08003464` | 19 | FUNC_08003168 FUNC_080032bc |  |  |  |  |
| `0x0800348a` | `FUNC_0800348a` | 13 | FUNC_08003168 FUNC_080032bc FUNC_080034a4 |  |  |  |  |
| `0x080034a4` | `FUNC_080034a4` | 76 | FUNC_08003168 FUNC_080032bc FUNC_080032cc FUNC_0800339c | crc16_block FUNC_08003444 FUNC_0800348a math_helper_34a4 |  |  |  |
| `0x08003548` | `scatter_load` | 69 | startup_c | startup_bx_main muldiv_scale |  | 0x20000122 0x20000153 0x20000178 0x200001a4 0x200001b2 0x200001d0 0x200001d2 |  |
| `0x0800356c` | `adc_hw_init` | 16 | gpio_init_all |  | ADC_+0x20 ADC_+0x308 ADC_CFGR1 ADC_CFGR2 ADC_CR ADC_SMPR ADC_TR |  |  |
| `0x08003598` | `adc_task` | 147 | FUNC_08003af4 main | adc_read_block adc_wait_eoc delay_us |  | 0x200001ce 0x2000025c 0x20000424 |  |
| `0x080036d0` | `adc_calibrate` | 39 | adc_wait_eoc | get_tick |  |  |  |
| `0x08003722` | `FUNC_08003722` | 53 |  | FUNC_08006548 |  |  |  |
| `0x0800378e` | `FUNC_0800378e` | 12 |  | FUNC_0800654c |  |  |  |
| `0x080037a8` | `FUNC_080037a8` | 4 |  | FUNC_0800654a |  |  |  |
| `0x080037b2` | `adc_sample_one` | 49 | adc_wait_eoc | get_tick |  |  |  |
| `0x08003818` | `adc_read_sequence` | 57 | adc_read_block | udivmod32 get_tick |  | 0x20000104 |  |
| `0x0800389c` | `FUNC_0800389c` | 34 | main | gpio_write_pin |  | 0x200001ce |  |
| `0x080038f0` | `FUNC_080038f0` | 7 | flash_charge_state_machine flash_trigger_state_machine ui_key_scan_task system_reinit_task |  |  | 0x200001ce |  |
| `0x08003904` | `FUNC_08003904` | 195 | main | FUNC_0800339c FUNC_080033f4 gpio_read_pin |  | 0x2000010a 0x200001a0 0x200001a9 0x200001aa 0x200001ba 0x200001bb 0x200001bc 0x200001bd 0x200001c2 0x200001dc |  |
| `0x08003af4` | `FUNC_08003af4` | 90 | app_state_init board_init | FUNC_0800339c FUNC_080033f4 adc_task delay_us |  | 0x20000114 0x200001ba 0x200001bc 0x200001bd 0x200001dc |  |
| `0x08003bec` | `ui_power_switch_task` | 55 | main | gpio_write_pin |  | 0x200001ca 0x200001ce 0x20000418 |  |
| `0x08003c68` | `FUNC_08003c68` | 13 | FUNC_08004358 measurement_task |  |  | 0x200001ce |  |
| `0x08003c88` | `FUNC_08003c88` | 8 | FUNC_08003eec |  |  | 0x200001d0 |  |
| `0x08003ca4` | `FUNC_08003ca4` | 110 | power_level_dispatch | level_table_lookup_b level_table_lookup_c muldiv_scale |  | 0x20000122 0x20000153 0x2000015d 0x20000161 0x20000166 0x20000178 0x200001a0 0x200001a4 0x200001aa 0x200001ab 0x200001b2 0x200001c5 0x200001d0 0x200001d2 |  |
| `0x08003dc4` | `FUNC_08003dc4` | 116 | power_level_dispatch_b | level_table_lookup_a |  | 0x20000122 0x20000153 0x2000015d 0x20000161 0x20000166 0x20000172 0x20000178 0x200001a0 0x200001ab 0x200001b4 0x200001b6 0x200001c5 0x200001c6 0x200001d0 |  |
| `0x08003eec` | `FUNC_08003eec` | 54 |  | FUNC_08003c88 gpio_write_pin delay_us |  | 0x200001a0 0x200001a6 |  |
| `0x08003f78` | `FUNC_08003f78` | 33 |  | gpio_write_pin delay_us |  | 0x200001a0 0x200001a6 |  |
| `0x08003fd0` | `FUNC_08003fd0` | 283 | display_task | udivmod32 FUNC_08008f34 FUNC_08008fa0 FUNC_0800900c FUNC_08009078 FUNC_08009150 FUNC_080091bc FUNC_0800ccac |  | 0x2000011c 0x20000163 0x20000164 0x20000418 |  |
| `0x08004270` | `ui_key_100` | 22 | ui_key_scan_task |  |  | 0x20000150 |  |
| `0x080042a0` | `ui_key_10` | 28 | ui_key_scan_task |  |  | 0x20000150 |  |
| `0x080042dc` | `FUNC_080042dc` | 52 | measurement_task | gpio_read_pin |  | 0x20000134 0x200001a4 0x200001a6 0x200001be |  |
| `0x08004358` | `FUNC_08004358` | 52 | FUNC_08005c30 | FUNC_08003c68 |  | 0x20000126 0x20000132 0x2000019b 0x200001a6 0x200001a7 0x200001a9 0x200001ba 0x200001c9 |  |
| `0x080043e4` | `FUNC_080043e4` | 94 | ui_factory_reset_task flash_store_params |  |  | 0x20000151 0x20000152 0x2000015d 0x20000161 0x20000165 0x20000166 0x2000016c 0x2000016f 0x20000184 0x2000018a 0x20000214 0x20000418 |  |
| `0x080044d0` | `DMA1_Ch1_IRQHandler` | 4 |  | dma1_ch1_task |  | 0x200002a8 |  |
| `0x080044e0` | `dma_set_memory_addr` | 15 | dma_config_channel |  |  |  |  |
| `0x08004500` | `flash_power_task` | 77 | main | flash_charge_state_machine gpio_read_pin flash_trigger_state_machine FUNC_08009d58 |  | 0x20000170 |  |
| `0x080045ac` | `flash_charge_state_machine` | 293 | flash_power_task | FUNC_080038f0 rf_flush_tx rf_set_power_level rf_rx_enable |  | 0x20000150 0x200001d0 0x200001fb 0x20000418 |  |
| `0x08004818` | `FUNC_08004818` | 25 | TIM14_IRQHandler | FUNC_08005ae8 rf_rx_process |  | 0x2000016b 0x20000180 0x20000197 0x200001a0 |  |
| `0x08004860` | `rf_init` | 43 | app_state_init board_init | rf_bus_init rf_apply_settings rf_set_power_level delay_us delay_ms rf_chip_config rf_read_reg8 rf_chip_reset rf_write_reg8 rf_write_cmd |  | 0x20000206 0x20000418 |  |
| `0x080048e4` | `ui_key_14` | 41 | ui_key_scan_task settings_load_task |  |  | 0x20000151 0x20000152 0x2000015d 0x20000161 0x20000165 0x20000166 0x2000016c 0x2000016f 0x20000184 0x2000018a 0x20000418 |  |
| `0x08004964` | `FUNC_08004964` | 52 | FUNC_08008c7c display_task | FUNC_0800ccac |  | 0x2000010a 0x2000011c 0x200001bc |  |
| `0x080049e8` | `FUNC_080049e8` | 7 | display_task | FUNC_0800ccac |  |  |  |
| `0x080049f8` | `FUNC_080049f8` | 49 | FUNC_08008c7c display_task | udivmod32 FUNC_08008e58 FUNC_0800ccac |  | 0x2000011c 0x2000015f |  |
| `0x08004a74` | `FUNC_08004a74` | 7 | FUNC_08004f70 | FUNC_0800ccac |  |  |  |
| `0x08004a84` | `FUNC_08004a84` | 24 | display_task | FUNC_0800ccac |  | 0x2000011c 0x20000150 0x2000015d 0x20000180 |  |
| `0x08004ac8` | `FUNC_08004ac8` | 168 | FUNC_08004f70 | udivmod32 FUNC_08008dec FUNC_08008f34 FUNC_08008fa0 FUNC_0800900c FUNC_08009078 FUNC_0800ccac |  | 0x2000015d 0x20000196 0x200001d0 0x200001d2 0x200001fb 0x20000418 |  |
| `0x08004c74` | `FUNC_08004c74` | 10 | FUNC_08008c7c display_task | FUNC_08008d58 FUNC_0800ccac |  |  |  |
| `0x08004c8c` | `FUNC_08004c8c` | 21 | FUNC_08008c7c display_task | FUNC_0800ccac |  |  |  |
| `0x08004cbc` | `FUNC_08004cbc` | 28 | display_task | FUNC_0800ccac |  | 0x2000011c 0x2000015d 0x2000015e |  |
| `0x08004d04` | `FUNC_08004d04` | 17 | display_task | FUNC_0800ccac |  | 0x2000015d |  |
| `0x08004d2c` | `FUNC_08004d2c` | 7 | FUNC_08008c7c display_task | FUNC_0800ccac |  |  |  |
| `0x08004d3c` | `FUNC_08004d3c` | 9 | display_task | FUNC_08004d58 FUNC_0800534c FUNC_0800ccac |  | 0x2000015d |  |
| `0x08004d58` | `FUNC_08004d58` | 137 | FUNC_08004d3c | ram_clear_block |  | 0x2000011c 0x2000015f 0x20000161 0x2000019d 0x2000019e 0x2000019f |  |
| `0x08004f70` | `FUNC_08004f70` | 85 | display_task | udivmod32 FUNC_08004a74 FUNC_08004ac8 FUNC_08005060 FUNC_0800534c FUNC_08005360 FUNC_080090e4 FUNC_08009150 FUNC_080091bc FUNC_0800ccac |  | 0x2000011c 0x20000153 0x2000015d 0x20000165 0x20000166 0x20000172 0x20000174 0x20000178 0x20000184 0x20000202 0x2000020c 0x20000418 |  |
| `0x08005060` | `FUNC_08005060` | 137 | FUNC_08004f70 | udiv32 FUNC_08008ec4 FUNC_080090e4 FUNC_08009150 FUNC_080091bc FUNC_0800ccac |  | 0x2000015d 0x20000165 0x20000166 0x20000184 0x200001d0 0x200001fb |  |
| `0x080051a8` | `FUNC_080051a8` | 9 | display_task | FUNC_080051c0 FUNC_0800ccac |  |  |  |
| `0x080051c0` | `FUNC_080051c0` | 162 | FUNC_080051a8 | udivmod32 udiv32 FUNC_08008dec FUNC_08008f34 FUNC_0800900c FUNC_08009078 FUNC_080090e4 FUNC_08009150 FUNC_080091bc FUNC_0800ccac |  | 0x2000011c 0x20000151 0x20000152 0x2000015f 0x20000165 0x20000166 0x20000184 0x2000018a 0x200001fb |  |
| `0x0800534c` | `FUNC_0800534c` | 9 | FUNC_08004d3c FUNC_08004f70 | FUNC_0800ccac |  |  |  |
| `0x08005360` | `FUNC_08005360` | 23 | FUNC_08004f70 | udivmod32 FUNC_08008dec FUNC_08008f34 FUNC_08008fa0 FUNC_0800900c |  |  |  |
| `0x0800539c` | `FUNC_0800539c` | 29 | FUNC_08005ae8 | gpio_write_pin delay_us |  | 0x200001c9 |  |
| `0x080053f0` | `EXTI0_1_IRQHandler` | 49 |  | FUNC_08005ae8 exti_clear_pending rf_rx_enable | EXTI_PR | 0x2000015d 0x2000016b 0x200001b8 0x200001c2 0x20000418 |  |
| `0x08005478` | `EXTI2_3_IRQHandler` | 4 |  | exti_clear_pending |  |  |  |
| `0x08005484` | `EXTI4_15_IRQHandler` | 54 |  | FUNC_08005ae8 exti_clear_pending gpio_read_pin FUNC_0800a954 rf_rx_enable FUNC_0800c798 | EXTI_PR | 0x20000162 0x20000165 0x20000168 0x2000016b 0x200001c2 |  |
| `0x08005518` | `FUNC_08005518` | 14 | FUNC_0800a260 |  |  | 0x20000122 0x200001a7 0x200001a8 |  |
| `0x08005544` | `gpio_init_all` | 41 | system_reinit_task | adc_hw_init FUNC_0800a90e charge_power_off | GPIOB_MODER GPIOB_ODR GPIOB_PUPDR GPIOC_MODER GPIOC_ODR GPIOC_PUPDR GPIOD_MODER  |  |  |
| `0x080055bc` | `FUNC_080055bc` | 28 | display_task |  |  | 0x2000011c 0x20000141 0x200001be 0x200001bf 0x20000212 |  |
| `0x0800560c` | `level_table_lookup_m` | 77 | power_level_dispatch | level_table_lookup_b level_table_lookup_c muldiv_scale |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001a4 0x200001ab 0x200001b2 0x200001d0 0x200001d2 |  |
| `0x080056d4` | `level_table_lookup_n` | 92 | power_level_dispatch_b | level_table_lookup_a |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001ab 0x200001b4 0x200001b6 0x200001c6 0x200001d0 |  |
| `0x080057b8` | `level_table_lookup_o` | 75 |  | gpio_write_pin delay_us |  | 0x20000166 0x200001a0 0x200001a6 0x200001aa 0x200001ab 0x200001d0 0x200001d2 |  |
| `0x0800587c` | `flash_erase_param_page` | 12 | ui_factory_reset_task settings_load_task | flash_erase_range flash_unlock |  | 0x20000220 0x200004a4 |  |
| `0x080058a4` | `flash_mass_erase` | 13 | flash_erase_range |  | FLASH_CR | 0x20000228 |  |
| `0x080058c8` | `flash_erase_page` | 14 | flash_erase_range |  | FLASH_AR FLASH_CR | 0x20000228 |  |
| `0x080058ec` | `flash_program_halfword` | 11 | flash_program_record |  | FLASH_CR | 0x20000228 |  |
| `0x0800590c` | `flash_read_params` | 13 | settings_load_task |  |  | 0x20000214 0x20000464 |  |
| `0x08005930` | `flash_read_word` | 4 | boot_reason_check |  |  | 0x20000214 |  |
| `0x0800593c` | `flash_get_status` | 23 | flash_wait_last_operation |  | FLASH_SR | 0x20000228 |  |
| `0x08005974` | `flash_write_params` | 24 | flash_store_params | flash_lock flash_program_record flash_unlock |  | 0x20000214 0x20000464 |  |
| `0x080059b4` | `flash_wait_last_operation` | 35 | flash_erase_range flash_program_record | flash_get_status get_tick |  |  |  |
| `0x08005a04` | `charge_monitor` | 37 | app_state_init board_init main | charge_voltage_convert |  | 0x200001ce |  |
| `0x08005a58` | `FUNC_08005a58` | 51 | FUNC_08005ae8 rf_rx_process | exti_clear_pending gpio_write_pin delay_us |  | 0x20000148 0x2000016b 0x200001a0 0x200001aa 0x200001ba 0x200001c9 |  |
| `0x08005ae8` | `FUNC_08005ae8` | 116 | FUNC_08004818 EXTI0_1_IRQHandler EXTI4_15_IRQHandler ui_key_2 | FUNC_0800539c FUNC_08005a58 FUNC_08009560 FUNC_08009e14 delay_us |  | 0x20000122 0x20000148 0x20000150 0x2000015d 0x2000015e 0x20000166 0x2000016b 0x20000180 0x20000184 0x2000018a 0x20000197 0x200001a0 0x200001a1 0x200001a6 0x200001aa 0x200001ba 0x200001fb 0x20000418 |  |
| `0x08005c30` | `FUNC_08005c30` | 265 | measurement_task | udivmod32 udiv32 FUNC_08003168 FUNC_080032bc FUNC_080032cc FUNC_0800339c FUNC_080033b8 FUNC_0800341c FUNC_08004358 |  | 0x20000128 0x20000134 0x20000153 0x2000015d 0x20000166 0x2000016e 0x20000174 0x20000184 0x20000196 0x200001a4 0x200001a6 0x200001d0 0x200001d2 0x200001e6 0x20000418 |  |
| `0x08005ed4` | `FUNC_08005ed4` | 58 | measurement_task | gpio_write_pin |  | 0x2000010a 0x20000134 0x200001a6 0x200001a7 0x200001bc 0x200001be 0x200001bf 0x200001d0 |  |
| `0x08005f6c` | `measurement_task` | 81 | main | FUNC_08003c68 FUNC_080042dc FUNC_08005c30 FUNC_08005ed4 gpio_write_pin rf_status_update |  | 0x20000120 0x20000134 0x200001a0 0x200001a6 0x200001aa 0x200001d0 |  |
| `0x0800603c` | `FUNC_0800603c` | 14 |  | exti_config |  |  |  |
| `0x08006064` | `FUNC_08006064` | 14 | FUNC_0800a954 rf_apply_settings rf_wakeup_sequence | exti_config |  |  |  |
| `0x0800608c` | `FUNC_0800608c` | 13 | FUNC_0800a90e rf_apply_settings system_reinit_task | exti_config |  |  |  |
| `0x080060ac` | `charge_voltage_convert` | 49 | charge_monitor |  |  | 0x200001ce 0x2000020e |  |
| `0x08006118` | `power_level_dispatch` | 44 | main | FUNC_08003ca4 level_table_lookup_m level_table_lookup_d level_table_lookup_f level_table_lookup_h level_table_lookup_j power_level_mode1 |  | 0x20000165 0x2000016e |  |
| `0x08006188` | `power_level_dispatch_b` | 44 | FUNC_08009560 rf_rx_parse | FUNC_08003dc4 level_table_lookup_n level_table_lookup_e level_table_lookup_g level_table_lookup_i level_table_lookup_k power_level_mode2 |  | 0x20000165 0x2000016e |  |
| `0x080061f8` | `level_table_lookup_a` | 43 | FUNC_08003dc4 level_table_lookup_n level_table_lookup_e level_table_lookup_g level_table_lookup_i level_table_ |  |  | 0x20000166 0x20000184 0x20000196 0x200001b6 0x200001c6 0x200001d0 |  |
| `0x0800626c` | `level_table_lookup_b` | 72 | FUNC_08003ca4 level_table_lookup_m level_table_lookup_d level_table_lookup_f level_table_lookup_h level_table_ |  |  | 0x2000015d 0x20000166 0x20000184 0x20000196 0x200001d0 0x200001d2 0x20000418 |  |
| `0x0800632c` | `level_table_lookup_c` | 45 | FUNC_08003ca4 level_table_lookup_m level_table_lookup_d level_table_lookup_f level_table_lookup_h level_table_ |  |  | 0x20000166 0x2000018a 0x20000196 0x200001d0 0x200001d2 |  |
| `0x080063a0` | `adc_result_update` | 60 | ui_key_scan_task |  |  | 0x20000150 |  |
| `0x0800641c` | `adc_dma_config` | 140 | adc_dma_init | udivmod32 | ADC_+0x308 | 0x20000104 |  |
| `0x08006550` | `adc_dma_enable` | 181 | adc_dma_init | dma_clock_enable |  |  |  |
| `0x080066c4` | `dma_clock_enable` | 51 | adc_dma_enable board_init | FUNC_08005608 dma_disable_channel exti_config | RCC_APB2ENR | 0x200002a8 |  |
| `0x08006748` | `adc_read_block` | 70 | adc_task | adc_read_sequence dma_config_channel |  |  |  |
| `0x080067e8` | `adc_wait_eoc` | 52 | adc_task | adc_calibrate adc_sample_one adc_start_conversion |  |  |  |
| `0x08006858` | `adc_start_conversion` | 36 | adc_wait_eoc | get_tick |  |  |  |
| `0x080068a4` | `dma1_ch1_task` | 320 | DMA1_Ch1_IRQHandler |  | DMA_IFCR DMA_ISR |  |  |
| `0x08006b28` | `dma_disable_channel` | 39 | dma_clock_enable |  |  |  |  |
| `0x08006b7c` | `dma_config_channel` | 38 | adc_read_block | dma_set_memory_addr |  |  |  |
| `0x08006bcc` | `flash_erase_range` | 68 | flash_erase_param_page | flash_mass_erase flash_erase_page flash_wait_last_operation | FLASH_CR | 0x20000228 |  |
| `0x08006c6c` | `flash_lock` | 7 | flash_write_params |  | FLASH_CR |  |  |
| `0x08006c80` | `flash_program_record` | 51 | flash_write_params | crc16_block flash_program_halfword flash_wait_last_operation | FLASH_CR | 0x20000228 |  |
| `0x08006cfc` | `flash_unlock` | 12 | flash_erase_param_page flash_write_params |  | FLASH_CR FLASH_KEYR |  |  |
| `0x08006d24` | `exti_clear_pending` | 8 | EXTI0_1_IRQHandler EXTI2_3_IRQHandler EXTI4_15_IRQHandler FUNC_08005a58 FUNC_08008204 exti_disable_group FUNC_ | FUNC_08006d20 | EXTI_PR |  |  |
| `0x08006d3c` | `exti_config` | 157 | FUNC_0800603c FUNC_08006064 FUNC_0800608c dma_clock_enable tim_set_compare FUNC_080084d4 ui_key_scan_task boar |  | EXTI_EMR EXTI_IMR EXTI_RTSR RCC_APB2ENR |  |  |
| `0x08006e94` | `gpio_read_pin` | 5 | FUNC_08003904 FUNC_080042dc flash_power_task EXTI4_15_IRQHandler ui_key_scan_task rf_tx_task FUNC_0800ae40 sys |  |  |  |  |
| `0x08006e9e` | `gpio_write_pin` | 6 | FUNC_0800389c ui_power_switch_task FUNC_08003eec FUNC_08003f78 FUNC_0800539c level_table_lookup_o FUNC_08005a5 |  |  |  |  |
| `0x08006eac` | `get_tick` | 3 | adc_calibrate adc_sample_one adc_read_sequence flash_wait_last_operation adc_start_conversion FUNC_08006eb8 FU |  |  | 0x20000100 |  |
| `0x08006eb8` | `FUNC_08006eb8` | 78 | FUNC_080098e0 FUNC_0800990c | get_tick FUNC_08006f64 |  |  |  |
| `0x08006f68` | `FUNC_08006f68` | 37 | rf_tx_task rf_rx_done system_reinit_task main | get_tick |  |  |  |
| `0x08006fbc` | `FUNC_08006fbc` | 52 | main | get_tick |  |  |  |
| `0x08007034` | `systick_tick_inc` | 5 | SysTick_Handler |  |  | 0x20000100 |  |
| `0x08007044` | `flash_acr_init` | 11 | main | flash_set_latency flash_prefetch_config | FLASH_ACR |  |  |
| `0x08007064` | `flash_set_latency` | 13 | flash_acr_init rcc_clock_switch | udivmod32 NVIC_SetPriority rcc_delay_short systick_init |  |  |  |
| `0x08007088` | `flash_prefetch_config` | 22 | flash_acr_init | NVIC_SetPriority | RCC_APB2ENR |  |  |
| `0x080070c0` | `NVIC_DisableIRQ` | 7 | rtc_clock_disable |  | SCS_ICER |  |  |
| `0x080070d4` | `NVIC_EnableIRQ` | 7 | rtc_clock_enable tim_clock_enable dma1_clock_init board_init_inputs |  | SCS_ISER |  |  |
| `0x080070e8` | `NVIC_SetPriority` | 3 | flash_set_latency flash_prefetch_config rtc_clock_enable tim_clock_enable dma1_clock_init board_init_inputs cl | FUNC_0800a310 |  |  |  |
| `0x080070f0` | `scb_set_sleep` | 22 | system_reinit_task |  | PWR_CR SCB_SCR |  |  |
| `0x08007124` | `rcc_enable_hse_pll` | 133 | clock_init_task | get_tick | PWR_CR RCC_APB1ENR |  |  |
| `0x08007244` | `rcc_clock_switch` | 153 | clock_init_task | get_tick flash_set_latency rcc_get_clock_freq | FLASH_ACR RCC_CFGR RCC_CR | 0x20000104 |  |
| `0x08007398` | `rcc_delay_short` | 3 | flash_set_latency clock_init_task |  |  | 0x20000104 |  |
| `0x080073a4` | `rcc_get_clock_freq` | 44 | rcc_clock_switch | udivmod32 | RCC_CFGR |  |  |
| `0x0800742c` | `rcc_clock_config` | 451 | clock_init_task | get_tick | PWR_CR RCC_CFGR RCC_CR |  |  |
| `0x08007804` | `tim_oc_setup_8000` | 55 | system_reinit_task | get_tick |  |  |  |
| `0x08007878` | `tim_oc_apply` | 102 | FUNC_0800993c system_reinit_task | get_tick | EXTI_+0x24 EXTI_IMR EXTI_RTSR |  |  |
| `0x08007954` | `rtc_clear_flags` | 25 | RTC_IRQHandler | FUNC_08007950 | EXTI_PR |  |  |
| `0x08007990` | `rtc_init` | 75 | FUNC_0800993c | rtc_clock_enable FUNC_0800abd2 |  |  |  |
| `0x08007a30` | `rtc_clock_disable` | 14 | system_reinit_task main | NVIC_DisableIRQ | RCC_BDCR |  |  |
| `0x08007a58` | `rtc_clock_enable` | 18 | rtc_init system_reinit_task | NVIC_EnableIRQ NVIC_SetPriority | RCC_BDCR |  |  |
| `0x08007a88` | `rtc_set_time` | 90 | FUNC_0800993c | rtc_wait_ready FUNC_0800abac FUNC_0800abbc FUNC_0800abd2 |  |  |  |
| `0x08007b50` | `rtc_get_time` | 105 | FUNC_0800993c | rtc_wait_ready FUNC_0800abac FUNC_0800abbc FUNC_0800abd2 |  |  |  |
| `0x08007c34` | `rtc_wait_ready` | 24 | rtc_set_time rtc_get_time | get_tick |  |  |  |
| `0x08007c68` | `systick_disable` | 10 | clock_init_task |  | SCS_SCR |  |  |
| `0x08007c84` | `systick_init` | 18 | flash_set_latency clock_init_task | FUNC_0800a310 | SCS_CCR SCS_SCR SCS_SHPR2 |  |  |
| `0x08007cb4` | `systick_dispatch` | 3 | SysTick_Handler | FUNC_08007c80 |  |  |  |
| `0x08007cc0` | `FUNC_08007cc0` | 36 | FUNC_080094e0 FUNC_080099ec FUNC_08009a4c FUNC_08009aac FUNC_08009ee4 |  |  |  |  |
| `0x08007d08` | `tim_timebase_init` | 24 | FUNC_080094e0 FUNC_080099c0 FUNC_080099ec FUNC_08009a4c FUNC_08009aac FUNC_08009ee4 | tim_clock_enable tim_get_bus_clock |  |  |  |
| `0x08007d3c` | `tim_clock_enable` | 64 | tim_timebase_init | NVIC_EnableIRQ NVIC_SetPriority | RCC_APB2ENR |  |  |
| `0x08007dd4` | `tim_timebase_start` | 11 | FUNC_080094e0 FUNC_08009ee4 app_state_init board_init |  |  |  |  |
| `0x08007dec` | `tim_stop` | 20 | FUNC_08009550 FUNC_08009f64 |  |  |  |  |
| `0x08007e18` | `tim_output_compare_init` | 113 | FUNC_080094e0 FUNC_080099ec FUNC_08009a4c FUNC_08009aac FUNC_08009ee4 | FUNC_0800bf2a FUNC_0800bf40 FUNC_0800c148 FUNC_0800c16a |  |  |  |
| `0x08007f08` | `tim_irq_handler` | 161 | TIM14_IRQHandler TIM1_BRK_UP_TRG_COM_IRQHandler TIM2_IRQHandler | FUNC_08007cbc FUNC_08007cbe FUNC_08007f06 FUNC_080080a0 FUNC_080081a8 FUNC_080081fc FUNC_080081fe |  |  |  |
| `0x0800806c` | `tim_set_compare` | 22 | FUNC_08009aac FUNC_0800a8c4 | exti_config |  |  |  |
| `0x080080a2` | `tim_pwm_config` | 100 | FUNC_08009aac | FUNC_0800bf50 FUNC_0800bfd8 FUNC_0800c058 FUNC_0800c0dc |  |  |  |
| `0x08008172` | `tim_set_compare2` | 24 | FUNC_08009aac | FUNC_080081a6 tim_get_bus_clock |  |  |  |
| `0x080081ac` | `tim17_init` | 30 | FUNC_0800a8c4 | tim17_pwm_start | TIM17_CR1 |  |  |
| `0x08008204` | `FUNC_08008204` | 128 | FUNC_08009560 FUNC_08009e14 flash_trigger_pulse rf_rx_parse rf_rx_handler | exti_clear_pending |  | 0x20000122 0x2000015d 0x20000166 0x20000184 0x2000018a 0x200001a6 0x200001a7 0x200001a8 0x200001a9 0x200001c9 0x200001d0 |  |
| `0x08008340` | `FUNC_08008340` | 12 | tick_counter_task FUNC_0800a260 system_reinit_task |  |  | 0x20000122 0x200001a7 0x200001a8 |  |
| `0x08008364` | `tick_counter_task` | 58 | main | udivmod32 FUNC_08008340 |  | 0x20000122 0x200001a7 0x200001a9 |  |
| `0x080083e8` | `FUNC_080083e8` | 33 | display_task settings_flag_set | gpio_write_pin FUNC_0800843c ram_clear_41c |  |  |  |
| `0x0800843c` | `FUNC_0800843c` | 32 | FUNC_080083e8 | gpio_write_pin ram_clear_41c |  |  |  |
| `0x0800848c` | `gpio_power_latch` | 25 | system_reinit_task | gpio_write_pin ram_clear_41c power_off_sequence |  |  |  |
| `0x080084d4` | `FUNC_080084d4` | 19 | FUNC_08009560 app_state_init board_init rf_rx_parse | exti_config gpio_write_pin |  | 0x20000304 |  |
| `0x08008504` | `flash_trigger_state_machine` | 298 | flash_power_task | FUNC_080038f0 rf_flush_tx rf_set_power_level rf_rx_enable |  | 0x20000150 0x200001d0 0x200001fb 0x20000418 |  |
| `0x08008778` | `rf_bus_init` | 73 | rf_init | gpio_write_pin | GPIOB_BRR GPIOB_MODER GPIOB_PUPDR GPIOC_MODER |  |  |
| `0x08008818` | `ui_key_scan_task` | 299 | main | FUNC_080038f0 ui_key_100 ui_key_10 ui_key_14 adc_result_update exti_config gpio_read_pin ui_key_1 ui_key_4 rf_flush_tx ui_key_20 rf_set_power_level rf_status_read rf_rx_e | GPIOB_BRR | 0x20000150 0x200001d1 0x20000418 |  |
| `0x08008ad0` | `level_table_lookup_d` | 77 | power_level_dispatch | level_table_lookup_b level_table_lookup_c muldiv_scale |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001a4 0x200001ab 0x200001b2 0x200001d0 0x200001d2 |  |
| `0x08008b98` | `level_table_lookup_e` | 92 | power_level_dispatch_b | level_table_lookup_a |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001ab 0x200001b4 0x200001b6 0x200001c6 0x200001d0 |  |
| `0x08008c7c` | `FUNC_08008c7c` | 46 | display_task | FUNC_08004964 FUNC_080049f8 FUNC_08004c74 FUNC_08004c8c FUNC_08004d2c FUNC_08008dec FUNC_08008f34 FUNC_08008fa0 FUNC_08009150 FUNC_0800ccac |  | 0x20000165 0x20000166 0x2000016c 0x2000016f 0x200001bd 0x20000418 |  |
| `0x08008d08` | `ui_key_1` | 33 | ui_key_scan_task |  |  | 0x20000150 0x200001d0 0x200001f4 |  |
| `0x08008d58` | `FUNC_08008d58` | 61 | FUNC_08004c74 | FUNC_0800ccac |  |  |  |
| `0x08008dec` | `FUNC_08008dec` | 44 | FUNC_08004ac8 FUNC_080051c0 FUNC_08005360 FUNC_08008c7c display_task | FUNC_0800ccac |  |  |  |
| `0x08008e58` | `FUNC_08008e58` | 44 | FUNC_080049f8 | FUNC_0800ccac |  |  |  |
| `0x08008ec4` | `FUNC_08008ec4` | 46 | FUNC_08005060 | FUNC_0800ccac |  |  |  |
| `0x08008f34` | `FUNC_08008f34` | 44 | FUNC_08003fd0 FUNC_08004ac8 FUNC_080051c0 FUNC_08005360 FUNC_08008c7c display_task | FUNC_0800ccac |  |  |  |
| `0x08008fa0` | `FUNC_08008fa0` | 44 | FUNC_08003fd0 FUNC_08004ac8 FUNC_08005360 FUNC_08008c7c display_task | FUNC_0800ccac |  |  |  |
| `0x0800900c` | `FUNC_0800900c` | 44 | FUNC_08003fd0 FUNC_08004ac8 FUNC_080051c0 FUNC_08005360 display_task | FUNC_0800ccac |  |  |  |
| `0x08009078` | `FUNC_08009078` | 44 | FUNC_08003fd0 FUNC_08004ac8 FUNC_080051c0 display_task | FUNC_0800ccac |  |  |  |
| `0x080090e4` | `FUNC_080090e4` | 44 | FUNC_08004f70 FUNC_08005060 FUNC_080051c0 | FUNC_0800ccac |  |  |  |
| `0x08009150` | `FUNC_08009150` | 44 | FUNC_08003fd0 FUNC_08004f70 FUNC_08005060 FUNC_080051c0 FUNC_08008c7c display_task | FUNC_0800ccac |  |  |  |
| `0x080091bc` | `FUNC_080091bc` | 45 | FUNC_08003fd0 FUNC_08004f70 FUNC_08005060 FUNC_080051c0 display_task | FUNC_0800ccac |  |  |  |
| `0x08009228` | `display_task` | 273 | rf_tx_task main | FUNC_08003fd0 FUNC_08004964 FUNC_080049e8 FUNC_080049f8 FUNC_08004a84 FUNC_08004c74 FUNC_08004c8c FUNC_08004cbc FUNC_08004d04 FUNC_08004d2c FUNC_08004d3c FUNC_08004f70 FU |  | 0x2000011c 0x2000015d 0x20000162 0x20000165 0x20000166 0x20000169 0x2000016a 0x2000016c 0x2000016d 0x2000016f 0x2000017a 0x200001a8 0x200001bd 0x200001c2 0x200001d0 0x200001d1 0x20000205 0x20000212 0x20000408 0x20000418 |  |
| `0x080094e0` | `FUNC_080094e0` | 42 | FUNC_08009c14 | FUNC_08005608 FUNC_08007cc0 tim_timebase_init tim_timebase_start tim_output_compare_init |  | 0x20000108 0x20000318 |  |
| `0x08009550` | `FUNC_08009550` | 4 | FUNC_08009c14 TIM1_BRK_UP_TRG_COM_IRQHandler | tim_stop |  | 0x20000318 |  |
| `0x08009560` | `FUNC_08009560` | 111 | FUNC_08005ae8 | power_level_dispatch_b gpio_write_pin FUNC_08008204 FUNC_080084d4 FUNC_0800a8c4 FUNC_0800a8f8 delay_us |  | 0x20000122 0x2000015d 0x2000016b 0x200001a6 0x200001a9 0x200001b2 0x200001b4 0x200001b6 0x200001c6 |  |
| `0x08009698` | `adc_dma_init` | 62 | board_init main | FUNC_08005608 adc_dma_config adc_dma_enable |  | 0x2000025c |  |
| `0x08009730` | `dma1_clock_init` | 16 | board_init main | NVIC_EnableIRQ NVIC_SetPriority | RCC_AHBENR |  |  |
| `0x08009758` | `board_init_inputs` | 154 | board_init main | exti_config gpio_write_pin NVIC_EnableIRQ NVIC_SetPriority | RCC_AHBENR |  |  |
| `0x080098e0` | `FUNC_080098e0` | 14 | system_reinit_task board_init main | FUNC_08005608 FUNC_08006eb8 |  | 0x20000248 |  |
| `0x0800990c` | `FUNC_0800990c` | 15 | system_reinit_task | FUNC_08005608 FUNC_08006eb8 |  | 0x20000248 |  |
| `0x0800993c` | `FUNC_0800993c` | 53 | main | FUNC_08005608 tim_oc_apply rtc_init rtc_set_time rtc_get_time |  | 0x200002e4 |  |
| `0x080099c0` | `FUNC_080099c0` | 16 | board_init main | FUNC_08005608 tim_timebase_init |  | 0x200003cc |  |
| `0x080099ec` | `FUNC_080099ec` | 36 | main | FUNC_08005608 FUNC_08007cc0 tim_timebase_init tim_output_compare_init |  | 0x20000318 |  |
| `0x08009a4c` | `FUNC_08009a4c` | 36 | main | FUNC_08005608 FUNC_08007cc0 tim_timebase_init tim_output_compare_init |  | 0x20000354 |  |
| `0x08009aac` | `FUNC_08009aac` | 55 | main | FUNC_08005608 FUNC_08007cc0 tim_timebase_init tim_output_compare_init tim_set_compare tim_pwm_config tim_set_compare2 |  | 0x20000390 |  |
| `0x08009b38` | `ui_factory_reset_task` | 24 | main | FUNC_080043e4 flash_erase_param_page flash_erase_params flash_store_params flash_params_default |  | 0x20000214 0x20000484 |  |
| `0x08009b7c` | `ui_key_4` | 73 | ui_key_scan_task |  |  | 0x20000150 |  |
| `0x08009c14` | `FUNC_08009c14` | 90 | TIM14_IRQHandler | gpio_write_pin FUNC_080094e0 FUNC_08009550 | GPIOF_MODER | 0x2000016f 0x200001c2 0x200001ce 0x20000212 |  |
| `0x08009cf0` | `rf_rx_timeout_counter` | 26 | main |  |  | 0x2000016f 0x200001ce |  |
| `0x08009d2c` | `rf_rx_state_reset` | 12 | FUNC_0800a954 rf_rx_timeout_task | FUNC_08009f64 |  | 0x20000198 0x2000019b 0x2000019c 0x200001a1 |  |
| `0x08009d58` | `FUNC_08009d58` | 90 | flash_power_task |  |  | 0x20000150 | 'ZZZZZZPFF<2(('; 'ZZZZZZPFF<2((' |
| `0x08009e14` | `FUNC_08009e14` | 39 | FUNC_08005ae8 | gpio_write_pin FUNC_08008204 rf_tx_start delay_us |  | 0x20000122 0x20000151 0x200001a9 0x200001b2 |  |
| `0x08009e88` | `flash_trigger_pulse` | 31 | rf_rx_timeout_task | gpio_write_pin FUNC_08008204 delay_us |  | 0x200001a9 0x200001b2 |  |
| `0x08009ee4` | `FUNC_08009ee4` | 45 | rf_tx_start | udivmod32 FUNC_08005608 FUNC_08007cc0 tim_timebase_init tim_timebase_start tim_output_compare_init |  | 0x20000108 0x20000152 0x20000354 |  |
| `0x08009f64` | `FUNC_08009f64` | 4 | rf_rx_state_reset | tim_stop |  | 0x20000354 |  |
| `0x08009f74` | `FUNC_08009f74` | 58 | FUNC_0800a1ec |  |  | 0x200001d0 0x200001d2 |  |
| `0x08009ff0` | `level_table_lookup_f` | 94 | power_level_dispatch | udivmod32 level_table_lookup_b level_table_lookup_c muldiv_scale |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001a0 0x200001a4 0x200001ab 0x200001b2 0x200001cd 0x200001d0 0x200001d2 |  |
| `0x0800a0e4` | `level_table_lookup_g` | 105 | power_level_dispatch_b | udivmod32 level_table_lookup_a |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001a0 0x200001ab 0x200001b4 0x200001b6 0x200001c6 0x200001cd 0x200001d0 |  |
| `0x0800a1ec` | `FUNC_0800a1ec` | 43 |  | gpio_write_pin FUNC_08009f74 delay_us |  | 0x200001a0 0x200001a6 0x200001cd |  |
| `0x0800a260` | `FUNC_0800a260` | 46 | status_led_task | FUNC_08005518 FUNC_08008340 |  | 0x200001e8 0x2000020f |  |
| `0x0800a2cc` | `status_led_task` | 24 | main | gpio_write_pin FUNC_0800a260 |  | 0x200001a0 0x200001aa 0x2000020f |  |
| `0x0800a310` | `FUNC_0800a310` | 30 | NVIC_SetPriority systick_init |  | SCB_SHPR2 |  |  |
| `0x0800a354` | `level_table_lookup_h` | 77 | power_level_dispatch | level_table_lookup_b level_table_lookup_c muldiv_scale |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001a4 0x200001ab 0x200001b2 0x200001d0 0x200001d2 |  |
| `0x0800a41c` | `level_table_lookup_i` | 86 | power_level_dispatch_b | level_table_lookup_a |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001ab 0x200001b4 0x200001b6 0x200001c6 0x200001d0 |  |
| `0x0800a4f4` | `level_table_lookup_l` | 80 |  | gpio_write_pin delay_us |  | 0x20000121 0x20000166 0x200001a0 0x200001a6 0x200001aa 0x200001ab 0x200001d0 0x200001d2 |  |
| `0x0800a5c8` | `FUNC_0800a5c8` | 12 | FUNC_0800a90e rf_apply_settings FUNC_0800c1c4 | gpio_write_pin |  |  |  |
| `0x0800a5e8` | `FUNC_0800a5e8` | 11 | rf_apply_settings | gpio_write_pin |  |  |  |
| `0x0800a608` | `FUNC_0800a608` | 12 | FUNC_0800c1c4 | gpio_write_pin |  |  |  |
| `0x0800a628` | `level_table_lookup_j` | 77 | power_level_dispatch | level_table_lookup_b level_table_lookup_c muldiv_scale |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001a4 0x200001ab 0x200001b2 0x200001d0 0x200001d2 |  |
| `0x0800a6f0` | `level_table_lookup_k` | 92 | power_level_dispatch_b | level_table_lookup_a |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001ab 0x200001b4 0x200001b6 0x200001c6 0x200001d0 |  |
| `0x0800a7d4` | `FUNC_0800a7d4` | 90 |  | gpio_write_pin delay_us |  | 0x20000122 0x20000165 0x200001a0 0x200001a6 0x200001aa 0x200001d0 |  |
| `0x0800a8c4` | `FUNC_0800a8c4` | 20 | FUNC_08009560 rf_rx_parse | FUNC_08005608 tim_set_compare tim17_init |  | 0x20000390 |  |
| `0x0800a8f8` | `FUNC_0800a8f8` | 8 | FUNC_08009560 rf_rx_parse |  |  | 0x20000390 |  |
| `0x0800a90e` | `FUNC_0800a90e` | 13 | gpio_init_all | FUNC_0800608c FUNC_0800a5c8 delay_ms rf_write_cmd |  |  |  |
| `0x0800a934` | `FUNC_0800a934` | 12 | rf_tx_task | FUNC_0800c1c4 |  | 0x20000206 |  |
| `0x0800a954` | `FUNC_0800a954` | 118 | EXTI4_15_IRQHandler | FUNC_08006064 rf_rx_state_reset rf_rx_crc rf_rx_done rf_rx_enable rf_rx_process rf_read_reg_buf rf_write_reg8 rf_write_cmd |  | 0x2000015d 0x2000016e 0x2000016f 0x200001a0 0x200001b2 0x200001cc 0x200001d0 0x200001f4 0x200001fb 0x20000206 0x20000418 |  |
| `0x0800aa8c` | `rf_apply_settings` | 34 | rf_init rf_tx_task rf_poll_settings ui_menu_task | FUNC_08006064 FUNC_0800608c FUNC_0800a5c8 FUNC_0800a5e8 rf_write_pair delay_ms rf_write_cmd |  | 0x20000165 0x2000016c |  |
| `0x0800aaf4` | `rf_tx_task` | 66 | main | gpio_read_pin gpio_write_pin FUNC_08006f68 display_task FUNC_0800a934 rf_apply_settings rf_write_pair delay_us delay_ms rf_write_cmd |  | 0x2000016c 0x20000200 0x20000248 |  |
| `0x0800abac` | `FUNC_0800abac` | 8 | rtc_set_time rtc_get_time |  |  |  |  |
| `0x0800abbc` | `FUNC_0800abbc` | 11 | rtc_set_time rtc_get_time |  |  |  |  |
| `0x0800abd2` | `FUNC_0800abd2` | 26 | rtc_init rtc_set_time rtc_get_time | get_tick |  |  |  |
| `0x0800ac0c` | `RTC_IRQHandler` | 7 |  | rtc_clear_flags |  | 0x2000020d 0x200002e4 |  |
| `0x0800ac24` | `boot_reason_check` | 17 | main | flash_read_word | RCC_CSR | 0x200001bf |  |
| `0x0800ac58` | `settings_load_task` | 210 | main | ui_key_14 flash_erase_param_page flash_read_params flash_store_params settings_validate |  | 0x20000151 0x20000152 0x2000015d 0x20000161 0x20000165 0x20000166 0x2000016c 0x20000184 0x2000018a 0x20000214 0x20000418 0x20000464 |  |
| `0x0800ae40` | `FUNC_0800ae40` | 302 | TIM14_IRQHandler | gpio_read_pin gpio_write_pin ram_clear_block | GPIOC_MODER GPIOC_PUPDR | 0x2000015d 0x20000165 0x20000166 0x2000016f 0x20000184 0x2000018a 0x200001f5 0x20000418 |  |
| `0x0800b134` | `muldiv_scale` | 23 | scatter_load FUNC_08003ca4 level_table_lookup_m level_table_lookup_d level_table_lookup_f level_table_lookup_h | udivmod32 |  | 0x2000015d 0x20000418 |  |
| `0x0800b170` | `rf_rx_crc` | 27 | FUNC_0800a954 | ram_clear_block |  | 0x2000015d 0x20000166 0x2000016e 0x200001a0 |  |
| `0x0800b4ec` | `rf_rx_done` | 74 | FUNC_0800a954 | gpio_write_pin FUNC_08006f68 delay_us |  | 0x200001d0 0x20000248 |  |
| `0x0800b5b0` | `rf_poll_settings` | 15 | main | rf_apply_settings |  | 0x20000165 0x2000016c 0x20000200 |  |
| `0x0800b5dc` | `power_level_mode1` | 77 | power_level_dispatch | level_table_lookup_b level_table_lookup_c muldiv_scale |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001a4 0x200001ab 0x200001b2 0x200001d0 0x200001d2 |  |
| `0x0800b6a4` | `power_level_mode2` | 86 | power_level_dispatch_b | level_table_lookup_a |  | 0x20000122 0x2000015d 0x20000161 0x20000166 0x200001ab 0x200001b4 0x200001b6 0x200001c6 0x200001d0 |  |
| `0x0800b77c` | `power_level_mode3` | 43 |  | gpio_write_pin delay_us |  | 0x200001a0 0x200001a6 0x200001aa 0x200001d0 |  |
| `0x0800b7f4` | `flash_erase_params` | 12 | ui_factory_reset_task |  |  | 0x20000214 |  |
| `0x0800b810` | `rf_flush_tx` | 4 | flash_charge_state_machine flash_trigger_state_machine ui_key_scan_task |  |  | 0x20000214 |  |
| `0x0800b820` | `flash_store_params` | 21 | ui_factory_reset_task settings_load_task | FUNC_080043e4 flash_write_params |  | 0x20000484 |  |
| `0x0800b858` | `rf_tx_start` | 15 | FUNC_08009e14 rf_rx_handler | FUNC_08009ee4 |  | 0x20000151 0x20000168 0x20000199 0x2000019b |  |
| `0x0800b888` | `rf_rx_timeout_task` | 44 | TIM2_IRQHandler | rf_rx_state_reset flash_trigger_pulse |  | 0x20000122 0x20000151 0x2000015d 0x20000168 0x20000199 0x2000019a 0x2000019b 0x200001a1 0x200001a6 |  |
| `0x0800b90c` | `ui_key_20` | 46 | ui_key_scan_task |  |  | 0x20000150 |  |
| `0x0800b96c` | `rf_write_pair` | 24 | rf_apply_settings rf_tx_task | delay_ms rf_write_reg8 rf_write_cmd |  |  | '\\\\\\]]]]]]]]]]]]]]]]^^^^^^^^^^^^^'; ' |
| `0x0800b9ac` | `rf_set_power_level` | 20 | flash_charge_state_machine rf_init flash_trigger_state_machine ui_key_scan_task | udivmod32 delay_ms rf_write_reg8 rf_write_cmd |  |  |  |
| `0x0800b9e4` | `rf_status_read` | 12 | ui_key_scan_task |  |  | 0x20000150 |  |
| `0x0800ba00` | `charge_enable_task` | 24 | main | charge_power_on charge_power_off |  | 0x2000015d 0x200001ce 0x20000418 |  |
| `0x0800ba40` | `rf_rx_enable` | 8 | flash_charge_state_machine EXTI0_1_IRQHandler EXTI4_15_IRQHandler flash_trigger_state_machine ui_key_scan_task |  |  | 0x200001c0 0x200001c2 0x200001c3 |  |
| `0x0800ba5c` | `system_reinit_task` | 89 | main | FUNC_080038f0 gpio_init_all FUNC_0800608c gpio_read_pin gpio_write_pin FUNC_08006f68 scb_set_sleep tim_oc_setup_8000 tim_oc_apply rtc_clock_disable rtc_clock_enable FUNC_ |  | 0x200001c2 0x2000020d 0x20000248 0x200002e4 |  |
| `0x0800bb7c` | `reinit_prepare` | 32 | system_reinit_task |  |  | 0x200001c0 0x200001c2 0x200001c3 0x200001c4 0x20000418 |  |
| `0x0800bbd4` | `FUNC_0800bbd4` | 5 | rf_spi_read_byte rf_spi_write_byte |  |  |  |  |
| `0x0800bbe0` | `exti_disable_group` | 16 | system_reinit_task | exti_clear_pending exti_config |  |  |  |
| `0x0800bc08` | `exti_enable_group` | 18 | system_reinit_task | exti_config | EXTI_IMR |  |  |
| `0x0800bc34` | `ui_key_80` | 39 | ui_key_scan_task |  |  | 0x20000150 0x20000418 |  |
| `0x0800bc8c` | `SysTick_Handler` | 4 |  | systick_tick_inc systick_dispatch |  |  |  |
| `0x0800bc98` | `clock_init_task` | 49 | system_reinit_task main | udivmod32 NVIC_SetPriority rcc_enable_hse_pll rcc_clock_switch rcc_delay_short rcc_clock_config systick_disable systick_init |  |  |  |
| `0x0800bd0c` | `SystemInit` | 38 | reset_handler |  | RCC_CFGR RCC_CFGR2 RCC_CFGR3 RCC_CIR RCC_CR RCC_CR2 |  |  |
| `0x0800bd68` | `app_state_init` | 35 | main | FUNC_08003af4 rf_init charge_monitor gpio_write_pin tim_timebase_start FUNC_080084d4 delay_10ms settings_save |  | 0x20000169 0x2000016a 0x2000016d 0x2000016e 0x2000017a 0x200001c8 0x200003cc |  |
| `0x0800bde0` | `TIM14_IRQHandler` | 18 |  | FUNC_08004818 tim_irq_handler FUNC_08009c14 FUNC_0800ae40 FUNC_0800c1a8 |  | 0x200001b8 0x200001c2 0x200003cc |  |
| `0x0800be1c` | `TIM1_BRK_UP_TRG_COM_IRQHandler` | 21 |  | gpio_write_pin tim_irq_handler FUNC_08009550 |  | 0x20000108 0x200001c2 0x20000318 |  |
| `0x0800be58` | `TIM2_IRQHandler` | 15 |  | tim_irq_handler rf_rx_timeout_task |  | 0x20000109 0x200001c2 0x20000354 |  |
| `0x0800be88` | `tim_get_bus_clock` | 58 | tim_timebase_init tim_set_compare2 |  |  |  |  |
| `0x0800bf14` | `tim17_pwm_start` | 11 | tim17_init |  |  |  |  |
| `0x0800bf2a` | `FUNC_0800bf2a` | 11 | tim_output_compare_init |  |  |  |  |
| `0x0800bf40` | `FUNC_0800bf40` | 8 | tim_output_compare_init |  |  |  |  |
| `0x0800bf50` | `FUNC_0800bf50` | 60 | tim_pwm_config |  |  |  |  |
| `0x0800bfd8` | `FUNC_0800bfd8` | 55 | tim_pwm_config |  |  |  |  |
| `0x0800c058` | `FUNC_0800c058` | 57 | tim_pwm_config |  |  |  |  |
| `0x0800c0dc` | `FUNC_0800c0dc` | 45 | tim_pwm_config |  |  |  |  |
| `0x0800c148` | `FUNC_0800c148` | 17 | tim_output_compare_init |  |  |  |  |
| `0x0800c16a` | `FUNC_0800c16a` | 19 | tim_output_compare_init |  |  |  |  |
| `0x0800c190` | `ui_key_2` | 9 | ui_key_scan_task | FUNC_08005ae8 |  | 0x20000150 |  |
| `0x0800c1a8` | `FUNC_0800c1a8` | 12 | TIM14_IRQHandler |  |  | 0x200001ce |  |
| `0x0800c1c4` | `FUNC_0800c1c4` | 14 | FUNC_0800a934 | FUNC_0800a5c8 FUNC_0800a608 delay_ms FUNC_0800ca94 rf_write_cmd |  | 0x20000206 |  |
| `0x0800c1f0` | `rf_status_update` | 39 | measurement_task | gpio_read_pin |  | 0x20000134 0x200001c8 |  |
| `0x0800c248` | `board_init` | 41 | system_reinit_task | FUNC_08003af4 rf_init charge_monitor dma_clock_enable gpio_write_pin tim_timebase_start FUNC_080084d4 adc_dma_init dma1_clock_init board_init_inputs FUNC_080098e0 FUNC_08 |  | 0x2000011c 0x20000167 0x2000016d 0x200001c8 0x2000025c 0x200003cc |  |
| `0x0800c2d8` | `ui_menu_task` | 59 | ui_key_scan_task | rf_apply_settings |  | 0x20000150 |  |
| `0x0800c354` | `rf_rx_parse` | 141 | rf_rx_process | power_level_dispatch_b gpio_write_pin FUNC_08008204 FUNC_080084d4 FUNC_0800a8c4 FUNC_0800a8f8 delay_us |  | 0x20000122 0x2000015d 0x2000016e 0x200001a0 0x200001a6 0x200001a9 0x200001b2 0x200001b4 0x200001b6 0x200001c6 0x200001d0 |  |
| `0x0800c4d4` | `rf_rx_handler` | 42 | rf_rx_process | gpio_write_pin FUNC_08008204 rf_tx_start delay_us |  | 0x20000151 0x200001a0 0x200001a1 0x200001a9 0x200001b2 |  |
| `0x0800c554` | `rf_rx_process` | 106 | FUNC_08004818 FUNC_0800a954 | FUNC_08005a58 rf_rx_parse rf_rx_handler delay_us |  | 0x20000122 0x20000148 0x20000150 0x2000015d 0x2000015e 0x20000166 0x2000016b 0x20000180 0x20000184 0x2000018a 0x20000197 0x200001a0 0x200001a1 0x200001a6 0x200001ba 0x20000418 |  |
| `0x0800c680` | `rf_wakeup_sequence` | 33 | main | FUNC_08006064 delay_ms rf_write_reg8 rf_write_cmd |  | 0x20000165 0x200001cc |  |
| `0x0800c6d8` | `math_helper_34a4` | 23 | FUNC_080034a4 |  |  |  |  |
| `0x0800c706` | `ram_clear_block` | 13 | FUNC_08004d58 FUNC_0800ae40 rf_rx_crc |  |  |  |  |
| `0x0800c740` | `flash_params_default` | 18 | ui_factory_reset_task |  |  | 0x20000484 |  |
| `0x0800c768` | `delay_us` | 24 | adc_task FUNC_08003af4 FUNC_08003eec FUNC_08003f78 rf_init FUNC_0800539c level_table_lookup_o FUNC_08005a58 FU |  |  |  |  |
| `0x0800c798` | `FUNC_0800c798` | 24 | EXTI4_15_IRQHandler |  |  |  |  |
| `0x0800c7c8` | `ram_clear_41c` | 31 | FUNC_080083e8 FUNC_0800843c gpio_power_latch power_off_sequence |  |  |  |  |
| `0x0800c808` | `delay_10ms` | 12 | app_state_init board_init | delay_us |  |  |  |
| `0x0800c828` | `delay_ms` | 24 | rf_init FUNC_0800a90e rf_apply_settings rf_tx_task rf_write_pair rf_set_power_level FUNC_0800c1c4 rf_wakeup_se |  |  |  |  |
| `0x0800c858` | `rf_chip_config` | 161 | rf_init | delay_ms rf_write_reg8 rf_write_cmd rf_write_cmd_buf |  |  |  |
| `0x0800ca08` | `rf_read_reg_buf` | 6 | FUNC_0800a954 | rf_spi_transfer |  |  |  |
| `0x0800ca16` | `rf_read_reg8` | 9 | rf_init | rf_spi_transfer |  |  |  |
| `0x0800ca2c` | `rf_chip_reset` | 38 | rf_init | gpio_write_pin delay_ms rf_write_cmd | GPIOB_IDR |  |  |
| `0x0800ca94` | `FUNC_0800ca94` | 6 | FUNC_0800c1c4 | rf_write_cmd_buf |  |  |  |
| `0x0800caa2` | `rf_write_reg8` | 5 | rf_init FUNC_0800a954 rf_write_pair rf_set_power_level rf_wakeup_sequence rf_chip_config | rf_write_cmd_buf |  |  |  |
| `0x0800cab0` | `rf_spi_read_byte` | 25 | rf_spi_transfer | FUNC_0800bbd4 | GPIOB_BRR GPIOB_BSRR GPIOB_IDR |  |  |
| `0x0800caec` | `rf_spi_write_byte` | 34 | rf_spi_transfer rf_write_cmd rf_write_cmd_buf | FUNC_0800bbd4 | GPIOB_BRR GPIOB_BSRR |  |  |
| `0x0800cb38` | `rf_spi_transfer` | 18 | rf_read_reg_buf rf_read_reg8 | rf_spi_read_byte rf_spi_write_byte rf_select rf_deselect |  |  |  |
| `0x0800cb64` | `rf_write_cmd` | 9 | rf_init FUNC_0800a90e FUNC_0800a954 rf_apply_settings rf_tx_task rf_write_pair rf_set_power_level FUNC_0800c1c | rf_spi_write_byte rf_select rf_deselect |  |  |  |
| `0x0800cb7c` | `rf_write_cmd_buf` | 18 | rf_chip_config FUNC_0800ca94 rf_write_reg8 | rf_spi_write_byte rf_select rf_deselect |  |  |  |
| `0x0800cba8` | `rf_select` | 10 | rf_spi_transfer rf_write_cmd rf_write_cmd_buf | gpio_write_pin | GPIOB_IDR |  |  |
| `0x0800cbc8` | `rf_deselect` | 6 | rf_spi_transfer rf_write_cmd rf_write_cmd_buf | gpio_write_pin |  |  |  |
| `0x0800cbdc` | `FUNC_0800cbdc` | 19 | system_reinit_task | exti_clear_pending exti_config |  |  |  |
| `0x0800cc14` | `exti_config_group` | 17 | system_reinit_task | exti_config | EXTI_IMR |  |  |
| `0x0800cc40` | `settings_flag_set` | 14 | settings_save | FUNC_080083e8 |  | 0x20000408 |  |
| `0x0800cc64` | `settings_save` | 27 | app_state_init board_init | exti_config gpio_write_pin settings_flag_set power_off_sequence |  |  |  |
| `0x0800ccac` | `FUNC_0800ccac` | 11 | FUNC_08003fd0 FUNC_08004964 FUNC_080049e8 FUNC_080049f8 FUNC_08004a74 FUNC_08004a84 FUNC_08004ac8 FUNC_08004c7 |  |  | 0x20000408 |  |
| `0x0800ccc8` | `settings_validate` | 90 | settings_load_task |  |  | 0x20000151 0x20000152 0x2000015d 0x20000161 0x20000165 0x20000166 0x2000016c 0x2000016f 0x20000184 0x2000018a 0x20000224 0x20000418 0x20000464 |  |
| `0x0800cdb0` | `main` | 83 |  | adc_task FUNC_0800389c FUNC_08003904 ui_power_switch_task flash_power_task charge_monitor measurement_task power_level_dispatch FUNC_08006f68 FUNC_08006fbc flash_acr_init | RCC_APB1ENR RCC_CSR SYSCFG_CFGR1 | 0x20000000 0x200001cf 0x2000020e 0x20000248 0x200002e4 |  |
| `0x0800cec4` | `charge_power_on` | 12 | charge_enable_task system_reinit_task | exti_config |  |  |  |
| `0x0800cee8` | `charge_power_off` | 17 | gpio_init_all charge_enable_task system_reinit_task | exti_config | EXTI_IMR |  |  |
| `0x0800cf14` | `power_off_sequence` | 92 | gpio_power_latch settings_save | gpio_write_pin ram_clear_41c |  |  |  |

## 4. 调用图综述

### 层次结构

1. **启动层（无调用者）**：`reset_handler` → `SystemInit` → `startup_c` → `scatter_load` → `main`。
2. **运行时助手层**：`udivmod32` / `udiv32` / `crc16_block` / `delay_us` / `delay_ms` / `get_tick` /
   `gpio_write_pin` / `gpio_read_pin` / `exti_config` 等，被几乎所有上层调用。
3. **驱动层**：
   - 时钟：`rcc_clock_config` / `rcc_clock_switch` / `systick_init`
   - GPIO/EXTI：`gpio_init_all` / `board_init_inputs` / `exti_config`
   - ADC/DMA：`adc_hw_init` / `adc_dma_init` / `adc_dma_config` / `adc_task`
   - 定时器：`tim_timebase_init` / `tim_irq_handler` / `tim14_init` …
   - 内部 Flash：`flash_*` 一组（0x0800587C–0x08006D14）
   - IWDG：`iwdg_init` / `iwdg_refresh` / `iwdg_start`
   - 2.4G：`rf_bus_init` / `rf_spi_*` / `rf_chip_config` / `rf_init`
4. **应用层**（主循环 18 个任务 + 中断服务）：`adc_task`、`key_scan_task`、`flash_power_task`、
   `flash_charge_state_machine`、`flash_trigger_state_machine`、`display_task`、`rf_tx_task`、
   `charge_enable_task`、`system_reinit_task` 等。
5. **中断层**：SysTick(ms 计数)、TIM14(主循环心跳/充电反馈)、TIM2(RF 超时)、TIM1(触发)、
   EXTI0_1 / EXTI4_15(触发输入)、DMA1_CH1(ADC)、RTC。

### 关键事实

- 全镜像 **323 个已识别函数**，反汇编覆盖 35% 的映像字节（其余为数据区/字面量池/未解码填充）。
- 主循环 `main` 的 18 个任务由 **TIM14 中断分频（1/25）** 驱动，SysTick 仅做 1 ms 计时。
- 所有外设通信均为 **GPIO 软件时序**：2.4G = 4 线软 SPI，按键 = 3×3 矩阵，
  充电/触发 = 直连 GPIO 脉冲；没有任何硬件 USART/SPI/I2C 使用。
- 内部 Flash 参数页 **0x0801F000**，参数块 8×u32（RAM 镜像 0x20000464）；
  标志页 0x0801F800 存放魔数 0x0000100E。
- 三个"大函数"（`rcc_clock_config` 451 条、`dma1_ch1_task` 320 条、
  `flash_charge/trigger_state_machine` 293/298 条）是理解固件的重点。

