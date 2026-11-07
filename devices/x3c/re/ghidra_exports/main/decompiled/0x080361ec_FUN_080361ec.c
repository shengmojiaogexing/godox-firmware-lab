// 0x080361ec FUN_080361ec size=586

void FUN_080361ec(int param_1)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  code *pcVar8;
  int iVar9;
  undefined4 *puVar10;
  undefined1 auStack_74 [5];
  undefined1 local_6f;
  undefined2 local_6e;
  undefined4 local_60;
  undefined1 local_56;
  undefined1 auStack_48 [15];
  undefined1 local_39;
  undefined1 auStack_2c [4];
  undefined1 auStack_28 [16];
  
  iVar1 = DAT_08036378;
  piVar2 = (int *)FUN_08028242(*(undefined4 *)(DAT_08036378 + 4));
  if (*(code **)(param_1 + 0xc) != (code *)0x0) {
    (**(code **)(param_1 + 0xc))(param_1);
  }
  if (((*piVar2 != 0) && (piVar2[1] == 0)) ||
     ((*piVar2 != 0 &&
      ((piVar2[1] != 0 &&
       (((int *)**(int **)(iVar1 + 4))[1] * *(int *)**(int **)(iVar1 + 4) == piVar2[3])))))) {
    while (piVar2[4] != 0) {
      pcVar8 = *(code **)(**(int **)(iVar1 + 4) + 0x34);
      if (pcVar8 != (code *)0x0) {
        (*pcVar8)();
      }
    }
  }
  iVar9 = 0;
  uVar3 = FUN_080282b0(*(undefined4 *)(iVar1 + 4));
  iVar4 = FUN_08031ad0(*(undefined4 *)(param_1 + 4),uVar3);
  if (*(int *)(*(int *)(iVar1 + 4) + 0x14) != 0) {
    iVar9 = FUN_08031ad0(*(undefined4 *)(param_1 + 4));
  }
  if (iVar4 == 0 && iVar9 == 0) {
    iVar5 = FUN_080282e6(*(undefined4 *)(iVar1 + 4));
    iVar6 = FUN_08028248(*(undefined4 *)(iVar1 + 4));
    FUN_08027518(auStack_28,0,0,iVar6 + -1,iVar5 + -1);
    if (*(int *)(param_1 + 0x30) == 0) {
      if (*(int *)(*(int *)(iVar1 + 4) + 0x2c) == 0) {
        FUN_08029a38(auStack_74);
        local_6e = *(undefined2 *)(*(int *)(iVar1 + 4) + 0x2a);
        local_6f = *(undefined1 *)(*(int *)(iVar1 + 4) + 0x29);
        FUN_08029a14(param_1,auStack_74,*(undefined4 *)(param_1 + 4));
      }
      else {
        iVar5 = FUN_0802c05c(*(undefined4 *)(*(int *)(iVar1 + 4) + 0x2c),auStack_2c);
        if (iVar5 == 1) {
          FUN_080287cc(auStack_48);
          local_39 = *(undefined1 *)(*(int *)(iVar1 + 4) + 0x29);
          FUN_08028758(param_1,auStack_48,auStack_28,*(undefined4 *)(*(int *)(iVar1 + 4) + 0x2c));
        }
      }
    }
    else {
      FUN_08029a38(auStack_74);
      iVar5 = *(int *)(iVar1 + 4);
      local_60 = *(undefined4 *)(iVar5 + 0x2c);
      local_56 = *(undefined1 *)(iVar5 + 0x29);
      local_6e = *(undefined2 *)(iVar5 + 0x2a);
      local_6f = *(undefined1 *)(iVar5 + 0x29);
      (**(code **)(param_1 + 0x30))(param_1,auStack_74,auStack_28);
    }
  }
  iVar5 = *(int *)(iVar1 + 4);
  if ((*(byte *)(iVar5 + 0x28) & 1) == 0) {
    if (*(int *)(iVar5 + 0x14) != 0) {
      if (iVar9 == 0) {
        iVar9 = *(int *)(iVar5 + 0x14);
      }
      FUN_0803665c(param_1,iVar9);
    }
    if (iVar4 == 0) {
      iVar4 = *(int *)(*(int *)(iVar1 + 4) + 0x10);
    }
    FUN_0803665c(param_1,iVar4);
  }
  else {
    if (iVar4 == 0) {
      iVar4 = *(int *)(iVar5 + 0x10);
    }
    FUN_0803665c(param_1,iVar4);
    iVar4 = *(int *)(*(int *)(iVar1 + 4) + 0x14);
    if (iVar4 != 0) {
      if (iVar9 == 0) {
        iVar9 = iVar4;
      }
      FUN_0803665c(param_1,iVar9);
    }
  }
  uVar3 = FUN_08028284(*(undefined4 *)(iVar1 + 4));
  FUN_0803665c(param_1,uVar3);
  uVar3 = FUN_08028270(*(undefined4 *)(iVar1 + 4));
  FUN_0803665c(param_1,uVar3);
  iVar9 = DAT_0801cedc;
  piVar7 = *(int **)(iVar1 + 4);
  piVar2 = (int *)FUN_08028242(*(undefined4 *)(DAT_0801cedc + 4));
  puVar10 = *(undefined4 **)(*piVar7 + 0x48);
  if ((code *)puVar10[0xd] != (code *)0x0) {
    (*(code *)puVar10[0xd])(puVar10);
  }
  if (((*piVar2 != 0) && (piVar2[1] != 0)) &&
     (((int *)**(int **)(iVar9 + 4))[1] * *(int *)**(int **)(iVar9 + 4) != piVar2[3])) {
    while (piVar2[4] != 0) {
      pcVar8 = *(code **)(**(int **)(iVar9 + 4) + 0x34);
      if (pcVar8 != (code *)0x0) {
        (*pcVar8)();
      }
    }
  }
  piVar2[4] = 1;
  iVar1 = *(int *)(**(int **)(iVar9 + 4) + 0x18);
  if (((*(uint *)(iVar1 + 0x18) & 1) == 0) || (-1 < *(int *)(iVar1 + 0x18) << 0x1e)) {
    piVar2[5] = 0;
  }
  else {
    piVar2[5] = 1;
  }
  iVar1 = piVar2[5];
  iVar9 = *piVar7;
  if (*(int *)(iVar9 + 0x20) != 0) {
    if (((*(uint *)(iVar9 + 0x1c) & 0x30) == 0) || (-1 < (int)(*(uint *)(iVar9 + 0x1c) << 0x1d))) {
      FUN_0801a19c(iVar9,puVar10[1],*puVar10);
    }
    else {
      FUN_0801cee0(puVar10[1],*puVar10);
    }
  }
  iVar9 = *piVar2;
  if (((iVar9 != 0) && (piVar2[1] != 0)) && ((*(byte *)(*piVar7 + 0x1c) & ~(iVar1 != 0) & 1) == 0))
  {
    if (piVar2[2] == iVar9) {
      piVar2[2] = piVar2[1];
    }
    else {
      piVar2[2] = iVar9;
    }
  }
  return;
}

