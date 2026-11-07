// 0x0802fe78 FUN_0802fe78 size=184

void FUN_0802fe78(undefined4 param_1,int param_2,undefined4 *param_3,undefined4 param_4)

{
  byte bVar1;
  undefined1 uVar2;
  undefined2 uVar3;
  uint uVar4;
  undefined4 uVar5;
  
  bVar1 = FUN_0802f9bc(param_1,param_2,0x56,param_4,param_4);
  *(byte *)(param_3 + 9) = bVar1;
  if (bVar1 < 3) {
    return;
  }
  if (param_2 != 0) {
    uVar4 = FUN_0802f8cc(param_1,param_2);
    if (uVar4 < 3) {
      *(undefined1 *)(param_3 + 9) = 0;
      return;
    }
    if (uVar4 < 0xfd) {
      *(char *)(param_3 + 9) = (char)(uVar4 * *(byte *)(param_3 + 9) >> 8);
    }
  }
  uVar3 = FUN_0802fa76(param_1,param_2);
  *(undefined2 *)(param_3 + 3) = uVar3;
  uVar5 = FUN_0802f9bc(param_1,param_2,0x58);
  param_3[6] = uVar5;
  uVar5 = FUN_0802f9bc(param_1,param_2,0x59);
  param_3[5] = uVar5;
  bVar1 = FUN_0802f9bc(param_1,param_2,0x5a);
  *(byte *)(param_3 + 10) = *(byte *)(param_3 + 10) & 0xf8 | bVar1 & 7;
  if (param_2 != 0) {
    bVar1 = FUN_0802f846(param_1,param_2);
    *(byte *)(param_3 + 10) = *(byte *)(param_3 + 10) & 199 | (bVar1 & 7) << 3;
  }
  uVar5 = FUN_0802fa94(param_1,param_2);
  *param_3 = uVar5;
  uVar2 = FUN_0802f9bc(param_1,param_2,0x5b);
  *(undefined1 *)((int)param_3 + 0x26) = uVar2;
  return;
}

