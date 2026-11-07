// 0x08036af4 FUN_08036af4 size=114

void FUN_08036af4(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
                 int param_7,int param_8,int param_9,int param_10,int param_11)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  for (iVar1 = 0; iVar1 < param_9; iVar1 = iVar1 + 1) {
    iVar2 = param_5 + (param_7 * iVar1 >> 8) >> 8;
    iVar3 = param_6 + (param_8 * iVar1 >> 8) >> 8;
    if ((((iVar2 < 0) || (param_2 <= iVar2)) || (iVar3 < 0)) || (param_3 <= iVar3)) {
      *(undefined1 *)(param_11 + iVar1) = 0;
    }
    else {
      *(undefined2 *)(param_10 + iVar1 * 2) =
           *(undefined2 *)(param_1 + (iVar3 * param_4 + iVar2) * 2);
      *(undefined1 *)(param_11 + iVar1) =
           *(undefined1 *)(iVar3 * param_4 + iVar2 + param_1 + param_4 * param_3 * 2);
    }
  }
  return;
}

