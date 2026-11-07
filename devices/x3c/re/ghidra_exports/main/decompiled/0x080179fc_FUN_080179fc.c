// 0x080179fc FUN_080179fc size=76

longlong FUN_080179fc(uint param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  
  iVar1 = DAT_08017a48;
  uVar5 = param_1;
  uVar4 = param_3;
  uVar2 = FUN_080334d8();
  iVar3 = FUN_0802c0bc(iVar1,param_1,param_2,param_3,uVar5,param_2,uVar4);
  if (iVar3 == 0) {
    FUN_0802d8d6(iVar1,0x2c);
    *(undefined4 *)(iVar1 + 0x28) = 0x80000000;
    return (ulonglong)uVar5 << 0x20;
  }
  *(undefined4 *)(iVar1 + 0x28) = 0;
  if (*(int *)(iVar1 + 0x1c) == 0) {
    uVar4 = FUN_080334c0(uVar2);
    *(undefined4 *)(iVar1 + 0x1c) = uVar4;
  }
  if (*(int *)(iVar1 + 0x1c) == 0) {
    *(undefined4 *)(iVar1 + 0x1c) = 1;
  }
  return CONCAT44(uVar5,iVar1);
}

