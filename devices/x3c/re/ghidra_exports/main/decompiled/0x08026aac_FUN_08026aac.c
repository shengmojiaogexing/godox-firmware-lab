// 0x08026aac FUN_08026aac size=120

int * FUN_08026aac(undefined4 *param_1)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  int iVar5;
  
  if (param_1[1] != 0) {
    FUN_08026938(*param_1);
  }
  iVar2 = FUN_08018004(DAT_08026b24);
  iVar5 = DAT_08026b28;
  if (iVar2 != 0) {
    uVar3 = FUN_080334d8();
    *(undefined4 *)(iVar5 + 4) = uVar3;
  }
  piVar4 = (int *)FUN_08017f8c(DAT_08026b24);
  if (piVar4 == (int *)0x0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  FUN_0802d764(piVar4,param_1,0x44);
  if ((undefined4 *)*param_1 == param_1) {
    *piVar4 = (int)piVar4;
  }
  bVar1 = *(byte *)((int)piVar4 + 0x42);
  *(byte *)((int)piVar4 + 0x42) = bVar1 & 0xfb | (*(byte *)(iVar5 + 1) & 1) << 2;
  if ((bVar1 & 1) != 0) {
    if ((code *)piVar4[5] != (code *)0x0) {
      iVar5 = (*(code *)piVar4[5])(piVar4);
      piVar4[8] = piVar4[8] + iVar5;
      piVar4[10] = iVar5 + piVar4[10];
    }
    if (((code *)piVar4[1] != (code *)0x0) && (*piVar4 != 0)) {
      (*(code *)piVar4[1])(*piVar4,piVar4[8]);
    }
  }
  FUN_08018fa8();
  return piVar4;
}

