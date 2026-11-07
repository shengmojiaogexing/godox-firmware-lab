// 0x080304c2 FUN_080304c2 size=272

void FUN_080304c2(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined1 auStack_44 [16];
  undefined1 auStack_34 [16];
  int local_24;
  int local_20;
  
  iVar4 = *(int *)(param_1 + 4);
  if (iVar4 != 0) {
    iVar1 = FUN_0802f938(iVar4,0);
    iVar2 = FUN_0802f998(iVar4,0);
    iVar3 = FUN_0802fc9e(param_1,0x40000);
    if (iVar3 == 0) {
      iVar3 = FUN_0802f0cc(iVar4);
      param_2 = param_2 + ((*(int *)(iVar4 + 0x14) + iVar1) - iVar3);
      iVar1 = FUN_0802f0dc(iVar4);
      param_3 = param_3 + ((*(int *)(iVar4 + 0x18) + iVar2) - iVar1);
    }
    else {
      param_2 = *(int *)(iVar4 + 0x14) + param_2 + iVar1;
      param_3 = *(int *)(iVar4 + 0x18) + param_3 + iVar2;
    }
    iVar1 = FUN_0802f864(iVar4,0);
    param_2 = param_2 + iVar1;
    param_3 = param_3 + iVar1;
  }
  local_24 = param_2 - *(int *)(param_1 + 0x14);
  local_20 = param_3 - *(int *)(param_1 + 0x18);
  if ((local_24 != 0) || (local_20 != 0)) {
    FUN_080302d0(param_1);
    FUN_0802ed04(param_1,auStack_34);
    iVar1 = 0;
    if (iVar4 != 0) {
      FUN_0802ec40(iVar4,auStack_44);
      iVar1 = FUN_080174c4(auStack_34,auStack_44,0);
      if (iVar1 == 0) {
        FUN_08031248(iVar4);
      }
    }
    *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + local_24;
    *(int *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + local_20;
    *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + local_24;
    *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + local_20;
    FUN_08030460(param_1,local_24,local_20,0);
    if (iVar4 != 0) {
      FUN_0802ad74(iVar4,0x22,param_1);
    }
    FUN_080302d0(param_1);
    if ((iVar4 != 0) &&
       ((iVar2 = FUN_080174c4(param_1 + 0x14,auStack_44,0), iVar1 != 0 || (iVar2 != 0)))) {
      FUN_08031248(iVar4);
    }
  }
  return;
}

