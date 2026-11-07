// 0x0802ff30 FUN_0802ff30 size=196

void FUN_0802ff30(undefined4 param_1,int param_2,undefined2 *param_3,undefined4 param_4)

{
  byte bVar1;
  undefined2 uVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  
  bVar1 = FUN_0802f9bc(param_1,param_2,0x4e,param_4,param_2,param_3,param_4);
  *(byte *)(param_3 + 8) = bVar1;
  if (2 < bVar1) {
    if (param_2 != 0) {
      uVar3 = FUN_0802f8cc(param_1,param_2);
      if (uVar3 < 3) {
        *(undefined1 *)(param_3 + 8) = 0;
        return;
      }
      if (uVar3 < 0xfd) {
        *(char *)(param_3 + 8) = (char)(uVar3 * *(byte *)(param_3 + 8) >> 8);
      }
    }
    iVar4 = FUN_0802f9bc(param_1,param_2,0x49);
    *(int *)(param_3 + 2) = iVar4;
    if (iVar4 != 0) {
      uVar5 = FUN_0802f9bc(param_1,param_2,0x4d);
      uVar2 = FUN_0801812e(param_1,param_2,uVar5);
      *param_3 = uVar2;
      iVar4 = FUN_0802f9bc(param_1,param_2,0x4a);
      *(int *)(param_3 + 4) = iVar4;
      if (iVar4 != 0) {
        uVar5 = FUN_0802f9bc(param_1,param_2,0x4b);
        *(undefined4 *)(param_3 + 6) = uVar5;
      }
      iVar4 = FUN_0802f9bc(param_1,param_2,0x4c);
      *(byte *)((int)param_3 + 0x11) =
           *(byte *)((int)param_3 + 0x11) & 0xf3 | (iVar4 != 0) << 2 | (iVar4 != 0) << 3;
      if (param_2 != 0) {
        bVar1 = FUN_0802f846(param_1,param_2);
        *(byte *)((int)param_3 + 0x11) = *(byte *)((int)param_3 + 0x11) & 0xfc | bVar1 & 3;
        return;
      }
    }
  }
  return;
}

