// 0x08004da4 FUN_08004da4 size=174

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_08004da4(void)

{
  int iVar1;
  undefined4 in_r3;
  uint uVar2;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 uStack_18;
  
  local_20 = DAT_08004e54;
  local_1c = 10;
  uStack_18 = in_r3;
  FUN_080026ee();
  FUN_08002fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff | 0x40000000;
  FUN_08004960(0x14);
  FUN_08002f0c(&DAT_48000000,0x8000);
  FUN_08004960(0x32);
  uVar2 = 0;
  do {
    FUN_08004878(*(undefined1 *)((int)&local_20 + uVar2));
    FUN_08004960(0x14);
    uVar2 = uVar2 + 1 & 0xff;
  } while (uVar2 < 2);
  FUN_08004878(local_1c);
  FUN_08004960(0x14);
  iVar1 = FUN_08004878(0xff);
  FUN_08002fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
  _DAT_4800000c = _DAT_4800000c | 0x40000000;
  FUN_080026ac();
  FUN_08002fca(DAT_08004e58,0x20);
  return CONCAT44(local_20,(uint)(iVar1 != 0xb9));
}

