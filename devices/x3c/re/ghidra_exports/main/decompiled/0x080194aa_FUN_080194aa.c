// 0x080194aa FUN_080194aa size=110

void FUN_080194aa(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
                 int param_7,int param_8,int param_9,int param_10,int param_11)

{
  int iVar1;
  int iVar2;
  undefined2 *puVar3;
  int iVar4;
  
  for (iVar1 = 0; iVar1 < param_9; iVar1 = iVar1 + 1) {
    iVar4 = param_5 + (param_7 * iVar1 >> 8) >> 8;
    iVar2 = param_6 + (param_8 * iVar1 >> 8) >> 8;
    if ((((iVar4 < 0) || (param_2 <= iVar4)) || (iVar2 < 0)) || (param_3 <= iVar2)) {
      *(undefined1 *)(param_11 + iVar1) = 0;
    }
    else {
      puVar3 = (undefined2 *)(param_4 * iVar2 * 3 + iVar4 * 3 + param_1);
      *(undefined2 *)(param_10 + iVar1 * 2) = *puVar3;
      *(undefined1 *)(param_11 + iVar1) = *(undefined1 *)(puVar3 + 1);
    }
  }
  return;
}

