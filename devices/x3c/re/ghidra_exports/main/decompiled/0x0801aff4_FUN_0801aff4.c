// 0x0801aff4 FUN_0801aff4 size=136

undefined4 FUN_0801aff4(short *param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  bool bVar1;
  ushort uVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  
  bVar1 = false;
  *(undefined1 *)((int)param_1 + 0xf) = 0xe9;
  uVar2 = FUN_08019ae0(param_1 + 1,0xe);
  *param_1 = ~uVar2 + 1;
  iVar3 = FUN_08037a36(0x20000000,param_1,0x10);
  if (iVar3 == 0) {
    FUN_08037568(param_3);
    uVar5 = 0;
    do {
      uVar6 = uVar5 + 1;
      if (0x13 < uVar5) goto LAB_0801b068;
      FUN_08037568(param_4);
      iVar3 = FUN_080376b4(0x20000000,param_2,0x10);
      if (iVar3 != 0) {
        return 0xffffffff;
      }
      uVar5 = uVar6;
    } while (((char)param_1[7] != *(char *)(param_2 + 0xf)) ||
            (iVar3 = FUN_08019ae0(param_2,0x10), iVar3 != 0));
    if (*(char *)(param_2 + 0xe) == '\0') {
      bVar1 = true;
    }
LAB_0801b068:
    uVar4 = 0xffffffff;
    if (bVar1) {
      uVar4 = 0;
    }
  }
  else {
    uVar4 = 0xffffffff;
  }
  return uVar4;
}

