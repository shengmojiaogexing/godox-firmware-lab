// 0x08036b66 FUN_08036b66 size=132

void FUN_08036b66(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
                 int param_7,int param_8,int param_9,int param_10,int param_11,int param_12)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short local_28;
  
  piVar1 = (int *)FUN_08018528();
  uVar2 = *(undefined4 *)(*piVar1 + 0x44);
  FUN_0802d926(param_11,param_9);
  for (iVar3 = 0; iVar3 < param_9; iVar3 = iVar3 + 1) {
    iVar4 = param_5 + (iVar3 * param_7 >> 8) >> 8;
    iVar5 = param_6 + (iVar3 * param_8 >> 8) >> 8;
    if ((((iVar4 < 0) || (param_2 <= iVar4)) || (iVar5 < 0)) || (param_3 <= iVar5)) {
      *(undefined1 *)(param_11 + iVar3) = 0;
    }
    else {
      *(undefined2 *)(param_10 + iVar3 * 2) =
           *(undefined2 *)(param_1 + (iVar5 * param_4 + iVar4) * 2);
    }
    if ((param_12 == 6) && (local_28 = (short)uVar2, *(short *)(param_10 + iVar3 * 2) == local_28))
    {
      *(undefined1 *)(param_11 + iVar3) = 0;
    }
  }
  return;
}

