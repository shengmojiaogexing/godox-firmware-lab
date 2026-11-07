// 0x0802df20 FUN_0802df20 size=260

undefined4 FUN_0802df20(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  
  local_28 = param_1;
  local_24 = param_2;
  local_20 = param_3;
  local_1c = param_4;
  iVar1 = FUN_0802fc9e(param_1,1);
  if (iVar1 == 0) {
    iVar1 = FUN_0802ee16(param_1);
    uVar2 = FUN_0802ed10(iVar1);
    iVar3 = FUN_080282b0(uVar2);
    if ((((iVar3 == iVar1) || (iVar3 = FUN_080282c4(uVar2), iVar3 == iVar1)) ||
        (iVar3 = FUN_08028284(uVar2), iVar3 == iVar1)) ||
       (iVar3 = FUN_08028270(uVar2), iVar3 == iVar1)) {
      iVar1 = FUN_0802fcac(param_1,0x80000);
      if (iVar1 == 0) {
        iVar1 = FUN_080180c2(param_1);
        FUN_080273fa(&local_28,param_1 + 0x14);
        local_28 = local_28 - iVar1;
        local_24 = local_24 - iVar1;
        local_20 = local_20 + iVar1;
        local_1c = local_1c + iVar1;
        iVar1 = FUN_08017484(param_2,param_2,&local_28);
        if (iVar1 == 0) {
          return 0;
        }
      }
      FUN_0802fb00(param_1,param_2,1,0);
      for (iVar1 = FUN_0802ee0a(param_1); iVar1 != 0; iVar1 = FUN_0802ee0a(iVar1)) {
        iVar3 = FUN_0802fc9e(iVar1,1);
        if (iVar3 != 0) {
          return 0;
        }
        iVar3 = FUN_0802fcac(iVar1,0x80000);
        if (iVar3 == 0) {
          local_28 = *(int *)(iVar1 + 0x14);
          local_24 = *(int *)(iVar1 + 0x18);
          local_20 = *(int *)(iVar1 + 0x1c);
          local_1c = *(int *)(iVar1 + 0x20);
          FUN_0802fb00(iVar1,&local_28,1,0);
          iVar3 = FUN_08017484(param_2,param_2,&local_28);
          if (iVar3 == 0) {
            return 0;
          }
        }
      }
      uVar2 = 1;
    }
    else {
      uVar2 = 0;
    }
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}

