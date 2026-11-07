// 0x08004a58 FUN_08004a58 size=332

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08004a58(void)

{
  int iVar1;
  undefined4 uVar2;
  int extraout_r2;
  uint extraout_r2_00;
  uint extraout_r2_01;
  uint uVar3;
  uint uVar4;
  undefined8 uVar5;
  
  uVar4 = 0xa1;
  FUN_08002f0c(DAT_08004ba4,0x40);
  *DAT_08004ba8 = *DAT_08004ba8 & 0xffff7fff;
  *DAT_08004bac = 1;
  FUN_08004960(0x32);
  uVar5 = FUN_08004878(0xff);
  FUN_08004960(0x14,(int)((ulonglong)uVar5 >> 0x20),(int)uVar5);
  if (extraout_r2 == 0xa0) {
    uVar3 = 1;
    *DAT_08004bb0 = 0xa0;
    do {
      iVar1 = FUN_08004878(0xff);
      DAT_08004bb0[uVar3] = (char)iVar1;
      if (uVar3 != 0) {
        uVar4 = iVar1 + uVar4 & 0xff;
      }
      FUN_08004960(0x14);
      uVar3 = uVar3 + 1 & 0xff;
    } while (uVar3 < 0x18);
    uVar5 = FUN_08004878(0xff);
    FUN_08004960(0x14,(int)((ulonglong)uVar5 >> 0x20),(int)uVar5);
    if (uVar4 == extraout_r2_00) {
      FUN_08004878(0xa0);
      FUN_08007a38();
      *DAT_08004bb4 = 1;
      goto LAB_08004b5e;
    }
    uVar2 = 0xaa;
  }
  else {
    if (extraout_r2 != 0xc6) goto LAB_08004b5e;
    iVar1 = FUN_08004878(0xff);
    FUN_08004960(0x14);
    uVar5 = FUN_08004878(0xff);
    FUN_08004960(0x14,(int)((ulonglong)uVar5 >> 0x20),(int)uVar5);
    if ((iVar1 + 199U & 0xff) == extraout_r2_01) {
      FUN_08004878(0xc6);
      FUN_08004960(0x14);
      uVar2 = DAT_08004ba4;
      _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
      _DAT_4800000c = _DAT_4800000c | 0x40000000;
      FUN_08002f0c(DAT_08004ba4,0x20);
      FUN_08004960(10);
      FUN_08002fca(uVar2,0x20);
      FUN_08002d18();
      disableIRQinterrupts();
      DataSynchronizationBarrier(0xf);
      *(undefined4 *)(DAT_08004bbc + 0xc) = DAT_08004bb8;
      DataSynchronizationBarrier(0xf);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    uVar2 = 0xca;
  }
  FUN_08004878(uVar2);
LAB_08004b5e:
  FUN_08004960(100);
  FUN_08002fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
  _DAT_4800000c = _DAT_4800000c | 0x40000000;
  FUN_080026ac();
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
  _DAT_4800000c = _DAT_4800000c | 0x40000000;
  FUN_080026ac();
  FUN_08002fca(DAT_08004ba4,0x40);
  *DAT_08004bac = 0;
  return;
}

