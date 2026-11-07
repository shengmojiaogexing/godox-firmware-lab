// 0x08010c8c FUN_08010c8c size=1510

void FUN_08010c8c(void)

{
  bool bVar1;
  bool bVar2;
  int *piVar3;
  undefined1 *puVar4;
  ushort uVar5;
  short sVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  undefined4 uVar11;
  uint uVar12;
  byte bVar13;
  uint *puVar14;
  byte unaff_r6;
  int iVar15;
  byte bVar16;
  uint uVar17;
  byte bVar18;
  byte bVar19;
  byte bVar20;
  
  uVar17 = 0;
  bVar2 = false;
  *DAT_08011094 = 1;
  iVar7 = FUN_08022eb4(DAT_08011098,1);
  if (iVar7 == 0) {
    FUN_0801c0fe(2000);
  }
  puVar14 = DAT_0801109c;
  *DAT_0801109c = *DAT_0801109c | 0x1000;
  FUN_08022e34(DAT_0801109c + -5,(int)puVar14 >> 0x15);
  FUN_08022e34(DAT_080110a0,4);
  *(uint *)(DAT_08011098 + 0x14) = *(uint *)(DAT_08011098 + 0x14) & 0xffffbfff;
  bVar16 = 0;
  bVar18 = 0;
  bVar13 = 0;
  bVar19 = 0;
  bVar20 = 0;
  uVar12 = 0;
  FUN_080359b4();
  while (iVar7 = DAT_080110ac, bVar16 < 0x46) {
    bVar16 = bVar16 + 1;
    iVar7 = FUN_08022eb4(DAT_08011098,0x100);
    if ((iVar7 == 0) && (bVar18 = bVar18 + 1, 0x41 < bVar18)) {
      uVar12 = uVar12 | 0x40;
    }
    FUN_08018c88(DAT_080110a4,1);
    FUN_08018e46(DAT_080110a4,1);
    FUN_0801c118(500);
    FUN_0801c31c(2);
    uVar8 = ((uint)*DAT_080110a8 * 1000) / (uint)DAT_080110a8[1];
    uVar9 = uVar8 & 0xffff;
    *(short *)(DAT_080110ac + 0x24) = (short)uVar8;
    if (uVar9 < 0x28b) {
      if ((0x276 < uVar9) && (bVar13 = bVar13 + 1, 0x41 < bVar13)) {
        uVar12 = uVar12 | 0x80;
      }
    }
    else {
      bVar19 = bVar19 + 1;
      bVar13 = bVar13 + 1;
      if (0x41 < bVar19) {
        uVar12 = uVar12 | 0x10;
      }
      if (0x41 < bVar13) {
        uVar12 = uVar12 | 0x80;
      }
    }
    iVar7 = FUN_08022eb4(DAT_08011098,1);
    if (((iVar7 == 1) && (iVar7 = FUN_08022eb4(DAT_08011098,0x100), iVar7 == 1)) &&
       (bVar20 = bVar20 + 1, 4 < bVar20)) {
      uVar12 = uVar12 | 0x20;
    }
    FUN_08012890();
    FUN_0801c0fe(5);
  }
  *(undefined1 *)(DAT_080110ac + 1) = 0;
  bVar1 = false;
  *DAT_08011094 = 1;
  piVar3 = DAT_080110b0;
  iVar10 = *DAT_080110b0;
  *(int *)(iVar7 + 0x28) = iVar10;
  if (((int)(uVar12 << 0x19) < 0) && (iVar10 != -1)) {
    FUN_080218e8();
    FUN_0802182c(piVar3);
    FUN_080217b0();
  }
  if (*DAT_080110b4 << 5 < 0) {
    if ((uVar12 & 0x60) != 0) {
      FUN_0803ba4c();
      FUN_0801c0fe(500);
      FUN_0803ba4c();
      FUN_0801c0fe(500);
      FUN_0803ba4c();
      FUN_0801c0fe(500);
      FUN_0803ba4c();
      FUN_0801c0fe(500);
      FUN_0803ba4c();
      FUN_0801c0fe(500);
    }
    FUN_0801bc08(DAT_080110b8);
  }
  FUN_08022e38(DAT_0801109c + -5,0x200);
  FUN_08022e38(DAT_080110a0,4);
  switch((int)uVar12 >> 4) {
  case 2:
  case 6:
  case 10:
  case 0xb:
    *DAT_080110bc = 0;
    *(undefined1 *)(iVar7 + 0xf) = 1;
    if ((int)(uVar12 << 0x1b) < 0) {
      puVar14 = DAT_0801109c + -5;
      FUN_08022e38(puVar14,0x200);
      FUN_08022e38(DAT_080110a0,4);
      FUN_0803babc();
      iVar10 = (int)puVar14 >> 0x14;
      FUN_08022e38(puVar14,iVar10);
      FUN_0801c0fe(1);
      FUN_08022e34(puVar14,iVar10);
      FUN_0801c0fe(10);
      FUN_08022e38(puVar14,iVar10);
      FUN_0801c0fe(0x78);
      FUN_08025a04();
      *(undefined1 *)(iVar7 + 2) = 0;
      FUN_080374b0();
      FUN_08005fb4();
      *(undefined1 *)(iVar7 + 0xe) = 1;
      *(undefined2 *)(iVar7 + 0x1e) = 100;
      FUN_0801a504();
      FUN_080067e8();
      bVar2 = true;
    }
    else {
      FUN_0800cfe4();
      FUN_08011888();
      disableIRQinterrupts();
      FUN_0800a874();
      FUN_08022e34(DAT_080110a0,4);
      FUN_08022e38(DAT_08011098,0x4000);
      FUN_080353a0();
      enableIRQinterrupts();
      FUN_0801bd04(DAT_080110c0,1);
      FUN_0800830c();
      *(undefined2 *)(iVar7 + 0x1c) = 0;
      FUN_080393c4();
      *DAT_08011094 = 1;
      *DAT_080110c4 = 1;
      *DAT_080110c8 = 0;
      FUN_0803ba4c();
      do {
        *DAT_080110cc = 0;
        FUN_08036074(1);
        FUN_08036020(0);
        FUN_0801c118(100);
        uVar11 = DAT_080110a4;
        FUN_08018c88(DAT_080110a4,1);
        FUN_08018e46(uVar11,1);
        FUN_0801c0fe(5);
        FUN_0801c31c(2);
        uVar8 = ((uint)*DAT_080110a8 * 1000) / (uint)DAT_080110a8[1];
        *(short *)(iVar7 + 0x24) = (short)uVar8;
        if (0x289 < (uVar8 & 0xffff)) break;
        iVar10 = FUN_08022eb4(DAT_08011098,1);
      } while (iVar10 == 1);
      disableIRQinterrupts();
      FUN_080202bc(0);
      enableIRQinterrupts();
      FUN_080354e4();
      FUN_080353a0();
    }
    bVar13 = 0;
    uVar8 = uVar12;
    while( true ) {
      do {
      } while (*DAT_080110d0 == '\0');
      *DAT_080110d0 = '\0';
      FUN_0803ba4c();
      uVar17 = uVar17 + 1;
      if ((DAT_080110d4 < uVar17) && (uVar17 = 0, 2 < *DAT_080110bc)) {
        *(undefined1 *)(iVar7 + 3) = 1;
      }
      uVar11 = DAT_080112cc;
      if ((int)(uVar12 << 0x1b) < 0) {
        FUN_080335bc();
        if (uVar12 != uVar8) {
          *(undefined1 *)(iVar7 + 0xe) = 1;
          FUN_08022e34(DAT_08011098,0x4000);
          *DAT_080110d8 = 1;
          puVar14 = DAT_0801109c + -5;
          FUN_08022e38(puVar14,0x200);
          FUN_08022e38(DAT_080110a0,4);
          FUN_0803babc();
          iVar10 = (int)puVar14 >> 0x14;
          FUN_08022e38(puVar14,iVar10);
          FUN_0801c0fe(1);
          FUN_08022e34(puVar14,iVar10);
          FUN_0801c0fe(10);
          FUN_08022e38(puVar14,iVar10);
          FUN_0801c0fe(0x78);
          FUN_08025a04();
          *(undefined1 *)(iVar7 + 2) = 0;
          FUN_080374b0();
          FUN_080353a0();
          FUN_080393c4();
          bVar13 = 0;
          *DAT_08011094 = 1;
          FUN_080359b4();
          FUN_08005fb4();
          FUN_0801a504();
          if (!bVar2) {
            FUN_080067e8();
          }
          *(undefined2 *)(iVar7 + 0x1e) = 100;
          uVar8 = uVar12;
        }
        if (*(short *)(iVar7 + 0x1e) != 0) {
          FUN_080061fc();
          uVar11 = FUN_0803c384(*DAT_080110dc);
          FUN_0803c4a4(uVar11,1);
          uVar5 = FUN_0803c344();
          FUN_0801a234((uint)uVar5 % 0xff + 0x37 & 0xff);
          sVar6 = *(short *)(iVar7 + 0x1e) + -1;
          *(short *)(iVar7 + 0x1e) = sVar6;
          if (sVar6 == 0) {
            FUN_08006064();
            FUN_0801a234(0x24);
          }
        }
        if (*(char *)(iVar7 + 0x10) != '\0') {
          *(undefined1 *)(iVar7 + 0xe) = 0;
          if ((int)(uVar12 << 0x18) < 0) {
            FUN_080067d4();
          }
          goto LAB_080111b6;
        }
        FUN_08007940();
        FUN_08005d64();
        bVar1 = false;
      }
      else {
        FUN_08018c88(DAT_080112cc,1);
        FUN_08018e46(uVar11,1);
        FUN_0801c0fe(5);
        FUN_0801c31c(2);
        uVar9 = ((uint)*DAT_080112d0 * 1000) / (uint)DAT_080112d0[1];
        *(short *)(iVar7 + 0x24) = (short)uVar9;
        if ((uVar9 & 0xffff) < 0x28b) {
          unaff_r6 = 0;
        }
        else {
          unaff_r6 = unaff_r6 + 1;
          if (3 < unaff_r6) {
            unaff_r6 = 0;
            uVar12 = uVar12 | 0x10;
          }
        }
        bVar1 = true;
      }
      iVar10 = FUN_08022eb4(DAT_080112d4,1);
      if (iVar10 == 0) {
        bVar13 = bVar13 + 1;
      }
      else {
        bVar13 = 0;
      }
      if (3 < bVar13) break;
      FUN_08012890();
    }
    *(undefined1 *)(iVar7 + 1) = 1;
    break;
  default:
    *(undefined1 *)(iVar7 + 1) = 1;
    break;
  case 4:
    *(undefined1 *)(iVar7 + 1) = 1;
    bVar1 = true;
  case 0xc:
  case 0xd:
  case 0xe:
  }
LAB_080111b6:
  *(undefined1 *)(iVar7 + 0xf) = 0;
  iVar10 = DAT_080112d8;
  if (*(char *)(iVar7 + 1) != '\0') {
    if (bVar1) {
      FUN_08022e38(DAT_080112d8,0x200);
      FUN_08022e38(DAT_080112dc,4);
      FUN_0800cfe4();
      FUN_0803babc();
      iVar15 = iVar10 >> 0x14;
      FUN_08022e38(iVar10,iVar15);
      FUN_0801c0fe(1);
      FUN_08022e34(iVar10,iVar15);
      FUN_0801c0fe(10);
      FUN_08022e38(iVar10,iVar15);
      FUN_0801c0fe(0x78);
      FUN_08025a04();
      FUN_080359b4();
      FUN_0801956c();
      while (*(ushort *)(iVar7 + 0x18) < 800) {
        FUN_080335bc();
        FUN_0803ba4c();
      }
    }
    puVar4 = DAT_080112e0;
    *DAT_080112e0 = 2;
    enableIRQinterrupts();
    uVar17 = 0;
    do {
      iVar10 = FUN_08014634();
      FUN_0803ba4c();
      uVar17 = uVar17 + 1 & 0xff;
      if (uVar17 % 5 == 0) {
        FUN_080147dc();
        FUN_0801c0fe(100);
      }
      else if (10 < uVar17) break;
    } while (iVar10 != 0);
    FUN_08013b80();
    FUN_0801bd04(DAT_080112e4,1);
    FUN_08013cdc();
    FUN_08037470();
    uVar11 = DAT_080112d4;
    bVar13 = 0;
    do {
      iVar10 = FUN_08022eb4(uVar11,0x100);
      if (iVar10 != 0) break;
      FUN_0803ba4c();
      FUN_0801c0fe(1);
      bVar13 = bVar13 + 1;
    } while (bVar13 < 0xc9);
    if (((int)(uVar12 << 0x1b) < 0) && (200 < bVar13)) {
      *puVar4 = 0;
      FUN_08014634();
      FUN_08025a04();
    }
    else {
      FUN_08010c00();
    }
  }
  *(undefined1 *)(iVar7 + 0x11) = 0;
  return;
}

