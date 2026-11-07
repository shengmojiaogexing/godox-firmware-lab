import sys,struct,collections
b=open(sys.argv[1],'rb').read()
known={
 'RCC':0x40023800,'FLASH':0x40023c00,'GPIOA':0x40020000,'GPIOB':0x40020400,'GPIOC':0x40020800,'GPIOD':0x40020c00,'GPIOE':0x40021000,'GPIOF':0x40021400,'GPIOG':0x40021800,
 'SYSCFG':0x40013800,'EXTI':0x40013c00,'SPI1':0x40013000,'SPI2':0x40003800,'SPI3':0x40003c00,'I2C1':0x40005400,'I2C2':0x40005800,'I2C3':0x40005c00,
 'USART1':0x40011000,'USART2':0x40004400,'USART3':0x40004800,'UART4':0x40004c00,'UART5':0x40005000,'USART6':0x40011400,
 'TIM2':0x40000000,'TIM3':0x40000400,'TIM4':0x40000800,'TIM5':0x40000c00,'TIM6':0x40001000,'TIM7':0x40001400,'TIM1':0x40010000,'TIM8':0x40010400,
 'DMA1':0x40026000,'DMA2':0x40026400,'ADC1':0x40012000,'ADC2':0x40012100,'ADC3':0x40012200,'DAC':0x40007400,'PWR':0x40007000,'RTC':0x40002800,'IWDG':0x40003000,'WWDG':0x40002c00,
 'SDIO':0x40012c00,'USB':0x50000000,'LTDC':0x40016800,'FMC':0xa0000000,'SAI1':0x40015800,'CRC':0x40023000,'RNG':0x50060800,'HASH':0x50060400,'DCMI':0x50050000,
}
cnt=collections.Counter()
for i in range(0,len(b)-3,4):
    v=struct.unpack_from('<I',b,i)[0]
    for k,a in known.items():
        if v==a: cnt[k]+=1
print('exact base refs:',cnt.most_common())
# range refs: count words in register windows per peripheral
win=collections.Counter()
for i in range(0,len(b)-3,4):
    v=struct.unpack_from('<I',b,i)[0]
    for k,a in known.items():
        if a<=v<a+0x400: win[k]+=1
print('\nwindow refs:',win.most_common())
# vector table end check & count IRQs with non-default
import struct as s
print('\nbytes 0x1d0-0x230:')
for off in range(0x1d0,0x240,16):
    c=b[off:off+16]; print('%06x '%off+' '.join('%02x'%x for x in c))
