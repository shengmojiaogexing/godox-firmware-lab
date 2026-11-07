// 0x08012424 FUN_08012424 size=266

int FUN_08012424(void)

{
  uint *puVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  
  puVar1 = DAT_08012530;
  *DAT_08012530 = *DAT_08012530 & 0xffffffef;
  *puVar1 = *puVar1 | 0x20;
  *puVar1 = *puVar1 & 0xfffffff7;
  puVar6 = DAT_08012534;
  *DAT_08012534 = *DAT_08012534 & 0xfffffffb;
  FUN_0800ced0(4);
  *puVar6 = *puVar6 | 4;
  FUN_080147f4(DAT_08012538,0x4000,0);
  puVar6 = DAT_08012530 + -5;
  FUN_080147f4(puVar6,0x40,0,1);
  FUN_080147f4(puVar6,0x800,0);
  FUN_080147f4(puVar6,0x1000,0);
  uVar2 = DAT_0801253c;
  FUN_080147f4(DAT_0801253c,4,0,1);
  FUN_080147f4(uVar2,8,0,1);
  FUN_080147f4(uVar2,0x10,0,1);
  FUN_080147f4(uVar2,0x20,0,1);
  *puVar1 = *puVar1 | 8;
  *puVar1 = *puVar1 & 0xffffffdf;
  FUN_0801be5c();
  *puVar1 = *puVar1 | 0x20;
  *puVar1 = *puVar1 & 0xffffffdf;
  iVar3 = FUN_0801be5c();
  *puVar1 = *puVar1 | 0x20;
  *puVar1 = *puVar1 & 0xffffffdf;
  uVar4 = FUN_0801be5c();
  *puVar1 = *puVar1 | 0x20;
  *puVar1 = *puVar1 & 0xffffffdf;
  uVar5 = FUN_0801be5c();
  *puVar1 = *puVar1 | 0x20;
  *puVar1 = *puVar1 | 0x10;
  return (uVar5 | (uVar4 | iVar3 << 8) << 8) << 8;
}

