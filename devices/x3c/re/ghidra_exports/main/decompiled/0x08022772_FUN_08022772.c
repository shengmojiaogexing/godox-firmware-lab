// 0x08022772 FUN_08022772 size=82

undefined4 FUN_08022772(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 local_1c [2];
  
  iVar1 = FUN_080223b4(param_1);
  if (iVar1 == 0) {
    local_1c[0] = 0;
  }
  else {
    uVar2 = FUN_0802faa0(param_1,0x40000);
    uVar3 = FUN_0802f9bc(param_1,0x40000,0x58);
    uVar4 = FUN_0802cd72(iVar1);
    FUN_08033a84(local_1c,uVar4,uVar2,uVar3,0,0x1fffffff,0);
  }
  return local_1c[0];
}

