// 0x0802fdd2 FUN_0802fdd2 size=166

void FUN_0802fdd2(int param_1,int param_2,undefined2 *param_3,undefined4 param_4)

{
  byte bVar1;
  char cVar2;
  undefined1 uVar3;
  undefined2 uVar4;
  uint uVar5;
  undefined4 uVar6;
  
  bVar1 = FUN_0802f9bc(param_1,param_2,0x46,param_4,param_2,param_3,param_4);
  *(byte *)((int)param_3 + 0xf) = bVar1;
  if (2 < bVar1) {
    if (param_2 != 0) {
      uVar5 = FUN_0802f8cc(param_1,param_2);
      if (uVar5 < 3) {
        *(undefined1 *)((int)param_3 + 0xf) = 0;
        return;
      }
      if (uVar5 < 0xfd) {
        *(char *)((int)param_3 + 0xf) = (char)(uVar5 * *(byte *)((int)param_3 + 0xf) >> 8);
      }
    }
    *param_3 = 0;
    param_3[1] = 0x100;
    *(int *)(param_3 + 2) = ((*(int *)(param_1 + 0x1c) - *(int *)(param_1 + 0x14)) + 1) / 2;
    *(int *)(param_3 + 4) = ((*(int *)(param_1 + 0x20) - *(int *)(param_1 + 0x18)) + 1) / 2;
    cVar2 = FUN_0802f9bc(param_1,param_2,0x48);
    *(char *)(param_3 + 7) = cVar2;
    if (cVar2 != '\0') {
      uVar6 = FUN_0802f9bc(param_1,param_2,0x47);
      uVar4 = FUN_0801812e(param_1,param_2,uVar6);
      param_3[6] = uVar4;
    }
    if (param_2 != 0) {
      uVar3 = FUN_0802f846(param_1,param_2);
      *(undefined1 *)(param_3 + 8) = uVar3;
      return;
    }
  }
  return;
}

