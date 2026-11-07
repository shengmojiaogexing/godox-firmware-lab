// 0x08025c54 FUN_08025c54 size=570

void FUN_08025c54(void)

{
  undefined4 in_r3;
  undefined4 local_28;
  
  local_28 = in_r3;
  if (*(char *)(DAT_08025e90 + 2) == '\x01') {
    FUN_080143c0(1);
    FUN_0801c0fe(5);
    FUN_080143c0(0x11);
    FUN_080143c0(0x38);
    FUN_0801c0fe(0x32);
    local_28 = 0x6f010000;
    FUN_08014418(0x2a,&local_28,4);
    local_28 = 0xbf010000;
    FUN_08014418(0x2b,&local_28,4);
    local_28 = CONCAT22(local_28._2_2_,0xbf01);
    FUN_08014418(0x44,&local_28,2);
    local_28 = local_28 & 0xffffff00;
    FUN_08014418(0x35,&local_28,1);
    local_28._0_1_ = 0x75;
    FUN_08014418(0x3a,&local_28,1);
    local_28 = CONCAT22(local_28._2_2_,0xff);
    FUN_08014418(0x51,&local_28,2);
    local_28._0_1_ = 0xff;
    FUN_08014418(99,&local_28,1);
    local_28 = CONCAT31(local_28._1_3_,0x20);
    FUN_08014418(0x53,&local_28,1);
    local_28 = 0xbf010000;
    FUN_08014418(0x30,&local_28,4);
    FUN_080143c0(0x13);
  }
  else {
    FUN_080143c0(1);
    FUN_0801c0fe(5);
    local_28._0_2_ = 0x5a5a;
    FUN_08014418(0xc0,&local_28,2);
    local_28 = CONCAT22(local_28._2_2_,0x5a5a);
    FUN_08014418(0xc1,&local_28,2);
    local_28._0_1_ = 1;
    FUN_08014418(0xe4,&local_28,1);
    local_28._0_1_ = 8;
    FUN_08014418(0xb0,&local_28,1);
    local_28._0_2_ = 6;
    FUN_08014418(0xb1,&local_28,2);
    local_28._0_2_ = 0xa5a5;
    FUN_08014418(0xc0,&local_28,2);
    local_28 = CONCAT22(local_28._2_2_,0xa5a5);
    FUN_08014418(0xc1,&local_28,2);
    FUN_080143c0(0x11);
    FUN_080143c0(0x38);
    FUN_0801c0fe(0x32);
    local_28 = 0x6f010000;
    FUN_08014418(0x2a,&local_28,4);
    local_28 = 0xbf010000;
    FUN_08014418(0x2b,&local_28,4);
    local_28 = CONCAT22((short)(local_28 >> 0x10),0xff00);
    FUN_08014418(0x44,&local_28,2);
    local_28 = local_28 & 0xffffff00;
    FUN_08014418(0x35,&local_28,1);
    local_28._0_1_ = 0x75;
    FUN_08014418(0x3a,&local_28,1);
    local_28 = CONCAT31(local_28._1_3_,0x20);
    FUN_08014418(0x53,&local_28,1);
    local_28 = 0xbf010000;
    FUN_08014418(0x30,&local_28,4);
    FUN_080143c0(0x13);
    local_28 = CONCAT22(local_28._2_2_,0xff);
    FUN_08014418(0x51,&local_28,2);
  }
  return;
}

