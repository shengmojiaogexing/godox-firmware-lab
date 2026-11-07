// 0x0802b670 FUN_0802b670 size=154

void FUN_0802b670(int param_1)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  
  if (((param_1 != 0) && (iVar1 = FUN_0802ed7a(param_1), iVar1 != 0)) &&
     ((*(byte *)(iVar1 + 0x1c) & 1) == 0)) {
    FUN_0802b8b2(iVar1,0);
    for (piVar2 = (int *)FUN_08017f30(iVar1); piVar2 != (int *)0x0;
        piVar2 = (int *)FUN_08017f5e(iVar1,piVar2)) {
      if (*piVar2 == param_1) {
        if ((*(int **)(iVar1 + 0xc) != (int *)0x0) && (**(int **)(iVar1 + 0xc) != param_1)) {
          uVar3 = FUN_08022204(iVar1);
          iVar4 = FUN_0802ad74(**(undefined4 **)(iVar1 + 0xc),0xf,uVar3);
          if (iVar4 != 1) {
            return;
          }
          FUN_080302d0(**(undefined4 **)(iVar1 + 0xc));
        }
        *(int **)(iVar1 + 0xc) = piVar2;
        if (piVar2 == (int *)0x0) {
          return;
        }
        if (*(code **)(iVar1 + 0x10) != (code *)0x0) {
          (**(code **)(iVar1 + 0x10))(iVar1);
        }
        uVar3 = FUN_08022204(iVar1);
        iVar4 = FUN_0802ad74(**(undefined4 **)(iVar1 + 0xc),0xe,uVar3);
        if (iVar4 == 1) {
          FUN_080302d0(**(undefined4 **)(iVar1 + 0xc));
          return;
        }
        return;
      }
    }
  }
  return;
}

