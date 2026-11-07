// 0x0802fd28 FUN_0802fd28 size=170

void FUN_0802fd28(undefined4 param_1,int param_2,undefined2 *param_3,undefined4 param_4)

{
  byte bVar1;
  undefined2 uVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  
  iVar3 = FUN_0802f9bc(param_1,param_2,0x50,param_4,param_2,param_3,param_4);
  *(int *)(param_3 + 2) = iVar3;
  if (iVar3 != 0) {
    bVar1 = FUN_0802f9bc(param_1,param_2,0x53);
    *(byte *)(param_3 + 8) = bVar1;
    if (2 < bVar1) {
      if (param_2 != 0) {
        uVar4 = FUN_0802f8cc(param_1,param_2);
        if (uVar4 < 3) {
          *(undefined1 *)(param_3 + 8) = 0;
          return;
        }
        if (uVar4 < 0xfd) {
          *(char *)(param_3 + 8) = (char)(uVar4 * *(byte *)(param_3 + 8) >> 8);
        }
      }
      uVar5 = FUN_0802f9bc(param_1,param_2,0x52);
      uVar2 = FUN_0801812e(param_1,param_2,uVar5);
      *param_3 = uVar2;
      uVar5 = FUN_0802f9bc(param_1,param_2,0x54);
      *(undefined4 *)(param_3 + 6) = uVar5;
      iVar3 = FUN_0802f9bc(param_1,param_2,0x51);
      *(byte *)((int)param_3 + 0x11) = *(byte *)((int)param_3 + 0x11) & 0xfb | (iVar3 != 0) << 2;
      if (param_2 != 0) {
        bVar1 = FUN_0802f846(param_1,param_2);
        *(byte *)((int)param_3 + 0x11) = *(byte *)((int)param_3 + 0x11) & 0xfc | bVar1 & 3;
        return;
      }
    }
  }
  return;
}

