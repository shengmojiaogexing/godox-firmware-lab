// 0x080335bc FUN_080335bc size=226

uint FUN_080335bc(void)

{
  int *piVar1;
  char *pcVar2;
  char cVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined4 uVar9;
  
  pcVar2 = DAT_080336a0;
  if (DAT_080336a0[2] == '\0') {
    DAT_080336a0[2] = '\x01';
    if (*pcVar2 == '\0') {
      pcVar2[2] = '\0';
      uVar4 = 1;
    }
    else {
      iVar5 = FUN_080334d8();
      piVar1 = DAT_080336a4;
      if ((iVar5 == 0) &&
         (iVar6 = *(int *)(pcVar2 + 0x10), *(uint *)(pcVar2 + 0x10) = iVar6 + 1U,
         piVar1 = DAT_080336a4, 100 < iVar6 + 1U)) {
        pcVar2[0x10] = '\0';
        pcVar2[0x11] = '\0';
        pcVar2[0x12] = '\0';
        pcVar2[0x13] = '\0';
        piVar1 = DAT_080336a4;
      }
      do {
        pcVar2[3] = '\0';
        pcVar2[4] = '\0';
        iVar6 = FUN_08017f30(DAT_080336a8);
        *piVar1 = iVar6;
        while (*piVar1 != 0) {
          iVar6 = FUN_08017f5e(DAT_080336a8);
          iVar7 = FUN_08033570(*piVar1);
          if ((iVar7 != 0) && (pcVar2[4] != '\0' || pcVar2[3] != '\0')) break;
          *piVar1 = iVar6;
        }
      } while (*piVar1 != 0);
      uVar4 = 0xffffffff;
      for (iVar6 = FUN_08017f30(DAT_080336a8); iVar6 != 0; iVar6 = FUN_08017f5e(DAT_080336a8,iVar6))
      {
        if (((*(byte *)(iVar6 + 0x14) & 1) == 0) && (uVar8 = FUN_080336ca(iVar6), uVar8 < uVar4)) {
          uVar4 = uVar8;
        }
      }
      iVar5 = FUN_080334c0(iVar5);
      *(int *)(pcVar2 + 0xc) = iVar5 + *(int *)(pcVar2 + 0xc);
      uVar8 = FUN_080334c0(*(undefined4 *)(pcVar2 + 8));
      if (499 < uVar8) {
        uVar8 = (uint)(*(int *)(pcVar2 + 0xc) * 100) / uVar8;
        cVar3 = (char)uVar8;
        pcVar2[1] = cVar3;
        if ((uVar8 & 0xff) < 0x65) {
          cVar3 = 'd' - cVar3;
        }
        else {
          cVar3 = '\0';
        }
        pcVar2[1] = cVar3;
        pcVar2[0xc] = '\0';
        pcVar2[0xd] = '\0';
        pcVar2[0xe] = '\0';
        pcVar2[0xf] = '\0';
        uVar9 = FUN_080334d8();
        *(undefined4 *)(pcVar2 + 8) = uVar9;
      }
      pcVar2[2] = '\0';
    }
  }
  else {
    uVar4 = 1;
  }
  return uVar4;
}

