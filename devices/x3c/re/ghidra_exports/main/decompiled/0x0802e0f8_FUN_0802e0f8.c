// 0x0802e0f8 FUN_0802e0f8 size=48

ulonglong FUN_0802e0f8(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  
  uVar1 = FUN_0802f9bc(param_1,param_2,0x5b,param_4,param_2,param_3,param_4);
  uVar3 = uVar1 & 0xff;
  uVar2 = FUN_0802f9bc(param_1,param_2,0x16);
  if ((uVar1 & 0xff) == 0) {
    uVar3 = 1;
  }
  return CONCAT44(uVar2,uVar3) & 0xffffffff000000ff;
}

