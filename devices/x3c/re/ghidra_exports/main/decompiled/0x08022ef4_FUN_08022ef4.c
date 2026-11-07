// 0x08022ef4 FUN_08022ef4 size=70

undefined2 FUN_08022ef4(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined2 uVar1;
  undefined4 uVar2;
  
  if ((*(byte *)(DAT_08022f3c + 0x20) & 1) == 0) {
    uVar2 = FUN_08031868(0x12,2);
    uVar1 = FUN_08027de8(uVar2,param_2,param_3);
  }
  else {
    uVar2 = FUN_0803182c(0x12,2);
    uVar1 = FUN_08027de8(uVar2,param_2,param_3);
  }
  return uVar1;
}

