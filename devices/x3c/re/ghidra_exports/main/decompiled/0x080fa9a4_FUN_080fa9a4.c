// 0x080fa9a4 FUN_080fa9a4 size=172

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_080fa9a4(char param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined4 local_18;
  
  local_18 = DAT_080faa50;
  FUN_080f86ee();
  FUN_080f8fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff | 0x40000000;
  FUN_080fa960(10);
  FUN_080f8f0c(&DAT_48000000,0x8000);
  FUN_080fa960(0x32);
  uVar1 = local_18;
  local_18._3_1_ = SUB41(uVar1,3);
  local_18._0_3_ = CONCAT12(param_1 + -0x4e,CONCAT11(param_1,(undefined1)local_18));
  uVar3 = 0;
  do {
    FUN_080fa878(*(undefined1 *)((int)&local_18 + uVar3));
    FUN_080fa960(0x14);
    uVar3 = uVar3 + 1 & 0xff;
  } while (uVar3 < 3);
  iVar2 = FUN_080fa878(0xff);
  FUN_080f8fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
  _DAT_4800000c = _DAT_4800000c | 0x40000000;
  FUN_080f86ac();
  FUN_080f8fca(DAT_080faa54,0x20);
  return iVar2 != 0xb1;
}

