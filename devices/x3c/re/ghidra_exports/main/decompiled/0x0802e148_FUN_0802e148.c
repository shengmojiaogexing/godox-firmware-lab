// 0x0802e148 FUN_0802e148 size=208

undefined4 * FUN_0802e148(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  int *piVar4;
  undefined4 *puVar5;
  int iVar6;
  
  uVar1 = FUN_0802225c(param_1);
  puVar2 = (undefined4 *)FUN_0802d5c4(uVar1);
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    FUN_0802d8d6(puVar2,uVar1);
    *puVar2 = param_1;
    puVar2[1] = param_2;
    if (param_2 == 0) {
      iVar6 = FUN_0802821c();
      if (iVar6 == 0) {
        FUN_0802d6e4(puVar2);
        return (undefined4 *)0x0;
      }
      if (*(int *)(iVar6 + 0xc) == 0) {
        puVar5 = (undefined4 *)FUN_0802d5c4(4);
        *(undefined4 **)(iVar6 + 0xc) = puVar5;
        *puVar5 = puVar2;
        *(undefined4 *)(iVar6 + 0x24) = 1;
      }
      else {
        iVar3 = *(int *)(iVar6 + 0x24) + 1;
        *(int *)(iVar6 + 0x24) = iVar3;
        iVar3 = FUN_0802d72c(*(int *)(iVar6 + 0xc),iVar3 * 4);
        *(int *)(iVar6 + 0xc) = iVar3;
        *(undefined4 **)(iVar3 + (*(int *)(iVar6 + 0x24) + -1) * 4) = puVar2;
      }
      puVar2[5] = 0;
      puVar2[6] = 0;
      iVar6 = FUN_08028248();
      puVar2[7] = iVar6 + -1;
      iVar6 = FUN_080282e6(0);
      puVar2[8] = iVar6 + -1;
    }
    else {
      if (*(int *)(param_2 + 8) == 0) {
        FUN_0802dee0(param_2);
      }
      piVar4 = *(int **)(param_2 + 8);
      if (*piVar4 == 0) {
        puVar5 = (undefined4 *)FUN_0802d5c4(4);
        **(undefined4 **)(param_2 + 8) = puVar5;
        *puVar5 = puVar2;
        *(undefined4 *)(*(int *)(param_2 + 8) + 4) = 1;
      }
      else {
        iVar6 = piVar4[1];
        piVar4[1] = iVar6 + 1;
        iVar6 = FUN_0802d72c(**(undefined4 **)(param_2 + 8),(iVar6 + 1) * 4);
        **(int **)(param_2 + 8) = iVar6;
        *(undefined4 **)(iVar6 + (*(int *)(*(int *)(param_2 + 8) + 4) + -1) * 4) = puVar2;
      }
    }
  }
  return puVar2;
}

