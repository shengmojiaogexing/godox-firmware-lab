// 0x080049a4 FUN_080049a4 size=172

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_080049a4(char param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined4 local_18;
  
  local_18 = DAT_08004a50;
  FUN_080026ee();
  FUN_08002fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff | 0x40000000;
  FUN_08004960(10);
  FUN_08002f0c(&DAT_48000000,0x8000);
  FUN_08004960(0x32);
  uVar1 = local_18;
  local_18._3_1_ = SUB41(uVar1,3);
  local_18._0_3_ = CONCAT12(param_1 + -0x4e,CONCAT11(param_1,(undefined1)local_18));
  uVar3 = 0;
  do {
    FUN_08004878(*(undefined1 *)((int)&local_18 + uVar3));
    FUN_08004960(0x14);
    uVar3 = uVar3 + 1 & 0xff;
  } while (uVar3 < 3);
  iVar2 = FUN_08004878(0xff);
  FUN_08002fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
  _DAT_4800000c = _DAT_4800000c | 0x40000000;
  FUN_080026ac();
  FUN_08002fca(DAT_08004a54,0x20);
  return iVar2 != 0xb1;
}

