// 0x0802fcc0 FUN_0802fcc0 size=104

uint FUN_0802fcc0(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  uint uVar2;
  undefined4 local_28;
  byte local_24;
  undefined1 auStack_20 [16];
  
  iVar1 = FUN_0802fc9e(param_1,2);
  if (iVar1 == 0) {
    uVar2 = 0;
  }
  else {
    iVar1 = FUN_0802fcb6(param_1,0x80);
    if (iVar1 == 0) {
      FUN_0802ec02(param_1,auStack_20);
      uVar2 = FUN_0801761a(auStack_20,param_2,0);
      if (uVar2 == 0) {
        uVar2 = 0;
      }
      else {
        iVar1 = FUN_0802fc9e(param_1,0x10000);
        if (iVar1 != 0) {
          local_24 = 1;
          local_28 = param_2;
          FUN_0802ad74(param_1,0x11,&local_28);
          uVar2 = (uint)local_24;
        }
      }
    }
    else {
      uVar2 = 0;
    }
  }
  return uVar2;
}

