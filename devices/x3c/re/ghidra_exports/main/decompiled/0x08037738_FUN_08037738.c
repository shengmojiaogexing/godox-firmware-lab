// 0x08037738 FUN_08037738 size=148

undefined4 FUN_08037738(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_28;
  
  iVar1 = DAT_080377d0;
  iVar4 = DAT_080377cc;
  local_28 = 0;
  iVar5 = 0;
  iVar6 = DAT_080377d0 + -0x10;
  do {
    FUN_08037700();
    local_28 = iVar4;
    iVar2 = FUN_08037a36(iVar6,&local_28,4);
    if (iVar2 == 0) {
      local_28 = 0x925a;
      iVar2 = FUN_08037a36(iVar1,&local_28,4);
      if ((iVar2 == 0) && (iVar2 = FUN_080379cc(0x20000000,param_1,param_2), iVar2 == 0)) break;
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 5);
  if (iVar2 == 0) {
    local_28 = DAT_080377cc + 2;
    iVar4 = FUN_08037a36(iVar6,&local_28,4);
    if (iVar4 == 0) {
      local_28 = DAT_080377cc + 3;
      iVar4 = FUN_08037a36(iVar6,&local_28,4);
      if (iVar4 == 0) {
        FUN_08037568(0x1e);
        uVar3 = 0;
      }
      else {
        uVar3 = 0xffffffff;
      }
    }
    else {
      uVar3 = 0xffffffff;
    }
  }
  else {
    uVar3 = 0xffffffff;
  }
  return uVar3;
}

