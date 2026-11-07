// 0x080391c8 FUN_080391c8 size=454

void FUN_080391c8(void)

{
  short sVar1;
  undefined1 *puVar2;
  int iVar3;
  int *piVar4;
  undefined4 uVar5;
  int *piVar6;
  ushort uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  
  iVar3 = DAT_08039398;
  puVar2 = DAT_08039394;
  if (*DAT_08039390 << 9 < 0) {
    if (*DAT_08039390 << 9 < 0) {
      *(undefined1 *)(DAT_08039398 + 0xd) = 0;
      *puVar2 = 0;
    }
  }
  else {
    *(undefined1 *)(DAT_08039398 + 0xd) = 1;
    *puVar2 = 1;
  }
  piVar6 = DAT_080393a8;
  uVar5 = DAT_080393a4;
  uVar9 = DAT_080393a0;
  piVar4 = DAT_0803939c;
  sVar1 = *(short *)(DAT_08039398 + 0x1c);
  if (*(char *)(iVar3 + 0xd) == '\0') {
    if (sVar1 != 0) {
      *(short *)(iVar3 + 0x1c) = sVar1 + -1;
      if ((*piVar4 != 0) && ((ushort)(sVar1 - 1U) < 400)) {
        *(undefined2 *)(iVar3 + 0x1c) = 400;
      }
      if (*(short *)(iVar3 + 0x1c) == 0) {
        disableIRQinterrupts();
        *(undefined1 *)(iVar3 + 0xd) = 1;
        *puVar2 = 1;
        FUN_080393c4();
        FUN_080398fa(uVar9,0);
        FUN_0801bd04(DAT_080393bc,0);
        FUN_080398ba(uVar9,99,0x2f);
        FUN_080398fa(uVar9,1);
        FUN_080398fa(uVar5,0);
        FUN_080398ba(uVar5,9,0x2f);
        FUN_080398fa(uVar5,1);
        enableIRQinterrupts();
      }
    }
  }
  else if (sVar1 != 0) {
    disableIRQinterrupts();
    *(undefined1 *)(iVar3 + 0xd) = 0;
    *puVar2 = 0;
    FUN_08036c54();
    FUN_080398fa(uVar9,0);
    FUN_080398ba(uVar9,99,0x11f);
    FUN_080398fa(uVar9,1);
    FUN_080398fa(uVar5,0);
    FUN_080398ba(uVar5,9,0x11f);
    FUN_080398fa(uVar5,1);
    enableIRQinterrupts();
    FUN_0803a7e4();
    uVar8 = FUN_0803c384(*DAT_080393ac);
    FUN_0803c4a4(uVar8,1);
    uVar7 = FUN_0803c344();
    FUN_0801a234((uint)uVar7 % 0xff + 0x37 & 0xff);
    FUN_080061fc();
    *piVar4 = *piVar6;
    *DAT_080393b4 = (uint)*DAT_080393b0;
    *DAT_080393b8 = *piVar6;
  }
  if (*(short *)(iVar3 + 0x22) != *(short *)(iVar3 + 0x1c)) {
    if (*(short *)(iVar3 + 0x22) == 0) {
      disableIRQinterrupts();
      *(undefined1 *)(iVar3 + 0xd) = 0;
      *puVar2 = 0;
      FUN_08036c54();
      FUN_080398fa(uVar9,0);
      FUN_080398ba(uVar9,99,0x11f);
      FUN_080398fa(uVar9,1);
      FUN_080398fa(uVar5,0);
      FUN_080398ba(uVar5,9,0x11f);
      FUN_080398fa(uVar5,1);
      enableIRQinterrupts();
      uVar9 = FUN_0803c384(*DAT_080393ac);
      FUN_0803c4a4(uVar9,1);
      uVar7 = FUN_0803c344();
      FUN_0801a234((uint)uVar7 % 0xff + 0x37 & 0xff);
      *DAT_080393b8 = *piVar6;
      FUN_080061fc();
      *DAT_080393c0 = 0;
    }
    *(undefined2 *)(iVar3 + 0x22) = *(undefined2 *)(iVar3 + 0x1c);
  }
  return;
}

