// 0x0802081c FUN_0802081c size=256

int FUN_0802081c(int param_1)

{
  byte bVar1;
  int *piVar2;
  code *pcVar3;
  int iVar4;
  int iVar5;
  
  piVar2 = (int *)FUN_0802c5fc();
  if (piVar2 != (int *)0x0) {
    pcVar3 = *(code **)(*piVar2 + 8);
    if (pcVar3 != (code *)0x0) {
      (*pcVar3)(*piVar2,*(undefined1 *)(param_1 + 8));
    }
    if ((int)((uint)*(byte *)(param_1 + 0x18) << 0x1e) < 0) {
      return 1;
    }
    if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
      return 0;
    }
  }
  piVar2 = (int *)FUN_0802ed54(*(undefined4 *)(param_1 + 4),0);
  iVar4 = 0;
  while (piVar2 != (int *)0x0) {
    if (((*piVar2 != 0) && (bVar1 = *(byte *)(piVar2 + 2), (char)bVar1 < '\0')) &&
       ((bVar1 == 0x80 || ((bVar1 & 0x7f) == *(byte *)(param_1 + 8))))) {
      *(int *)(param_1 + 0xc) = piVar2[1];
      (*(code *)*piVar2)(param_1);
      if ((int)((uint)*(byte *)(param_1 + 0x18) << 0x1e) < 0) {
        return 1;
      }
      if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
        return 0;
      }
    }
    iVar4 = iVar4 + 1;
    piVar2 = (int *)FUN_0802ed54(*(undefined4 *)(param_1 + 4),iVar4);
  }
  iVar4 = FUN_0802eb88(0,param_1);
  if (iVar4 == 0) {
    piVar2 = (int *)0x0;
  }
  else {
    piVar2 = (int *)FUN_0802ed54(*(undefined4 *)(param_1 + 4),0);
  }
  iVar5 = 0;
  do {
    if ((piVar2 == (int *)0x0) || (iVar4 != 1)) {
      if ((iVar4 == 1) &&
         ((*(int *)(*(int *)(param_1 + 4) + 4) != 0 && (iVar5 = FUN_080207c0(param_1), iVar5 != 0)))
         ) {
        *(undefined4 *)(param_1 + 4) = *(undefined4 *)(*(int *)(param_1 + 4) + 4);
        iVar5 = FUN_0802081c(param_1);
        iVar4 = 1;
        if (iVar5 != 1) {
          return 0;
        }
      }
      return iVar4;
    }
    if (((*piVar2 != 0) && (bVar1 = *(byte *)(piVar2 + 2), -1 < (int)((uint)bVar1 << 0x18))) &&
       ((bVar1 == 0 || (bVar1 == *(byte *)(param_1 + 8))))) {
      *(int *)(param_1 + 0xc) = piVar2[1];
      (*(code *)*piVar2)(param_1);
      if ((int)((uint)*(byte *)(param_1 + 0x18) << 0x1e) < 0) {
        return 1;
      }
      if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
        return 0;
      }
    }
    iVar5 = iVar5 + 1;
    piVar2 = (int *)FUN_0802ed54(*(undefined4 *)(param_1 + 4),iVar5);
  } while( true );
}

