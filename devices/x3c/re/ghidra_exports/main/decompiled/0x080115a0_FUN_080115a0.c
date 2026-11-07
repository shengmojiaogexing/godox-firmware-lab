// 0x080115a0 FUN_080115a0 size=608

void FUN_080115a0(void)

{
  char cVar1;
  ushort uVar2;
  undefined2 uVar3;
  int iVar4;
  undefined1 *puVar5;
  byte bVar6;
  undefined1 uVar7;
  ushort uVar8;
  undefined4 uVar9;
  uint uVar10;
  uint unaff_r4;
  int iVar11;
  int iVar12;
  uint uVar13;
  uint uVar14;
  undefined8 uVar15;
  
  iVar4 = DAT_08011800;
  bVar6 = *(char *)(DAT_08011800 + 0xd) + 1;
  *(byte *)(DAT_08011800 + 0xd) = bVar6;
  if (1 < bVar6) {
    *(undefined1 *)(iVar4 + 0xd) = 0;
    iVar11 = DAT_08011804;
    if (*(char *)(iVar4 + 8) == '\x01') {
      uVar8 = *(ushort *)(iVar4 + 0x10);
      uVar9 = FUN_0803c384();
      FUN_0803c1c0(uVar9,DAT_08011808);
      uVar15 = FUN_0803c15c();
      FUN_0803bf60((int)uVar15,(int)((ulonglong)uVar15 >> 0x20),0,DAT_0801180c);
      FUN_0803befc();
      uVar7 = FUN_0803c344();
      *(undefined1 *)(iVar4 + 9) = uVar7;
      *(char *)(iVar4 + 0xf) = (char)(uVar8 / 0x51);
      uVar13 = (uint)uVar8 % 0x51;
      if (uVar13 == 0) {
        *(undefined2 *)(iVar4 + 0x12) = 0;
      }
      FUN_08023520(0x36);
      FUN_08023540(0x18);
      FUN_08014814(*(undefined1 *)(iVar4 + 0xf));
      FUN_08010b48();
      FUN_08023520(0x34);
      thunk_FUN_0801c118(500);
      uVar14 = 0;
      iVar12 = DAT_08011804 + -200;
      do {
        uVar10 = FUN_08023480(0x34);
        if (uVar10 < 0x80) {
          uVar8 = 0x4bU - (short)(uVar10 >> 1) & 0xff;
        }
        else {
          uVar8 = (short)((~uVar10 + 1 & 0xff) >> 1) + 0x4b;
        }
        *(ushort *)(iVar12 + uVar14 * 2) = uVar8;
        thunk_FUN_0801c118(5);
        uVar14 = uVar14 + 1 & 0xff;
      } while (uVar14 < 0x40);
      FUN_08010b24();
      FUN_08023520(0x36);
      bVar6 = 1;
      do {
        uVar14 = 0;
        do {
          uVar10 = uVar14 + 1;
          uVar8 = *(ushort *)(iVar12 + uVar14 * 2);
          uVar2 = *(ushort *)(iVar12 + uVar10 * 2);
          if ((uint)uVar2 < (uint)uVar8) {
            *(ushort *)(iVar12 + uVar14 * 2) = uVar2;
            *(ushort *)(iVar12 + uVar10 * 2) = uVar8;
            unaff_r4 = (uint)uVar8;
          }
          uVar14 = uVar10 & 0xff;
        } while (uVar14 < 0x3f);
        bVar6 = bVar6 + 1;
      } while (bVar6 < 0x40);
      uVar14 = 8;
      do {
        unaff_r4 = *(ushort *)(iVar12 + uVar14 * 2) + unaff_r4 & 0xffff;
        uVar14 = uVar14 + 1 & 0xff;
      } while (uVar14 < 0x38);
      uVar14 = unaff_r4 / 0x30 + (uint)*(ushort *)(iVar4 + 0x12);
      *(short *)(iVar4 + 0x12) = (short)uVar14;
      if ((uVar13 == 0x50) &&
         (bVar6 = *(byte *)(iVar4 + 0xf),
         *(short *)(iVar11 + (uint)bVar6 * 2) = (short)((uVar14 & 0xffff) / 0x51), bVar6 == 0x1f)) {
        *(undefined1 *)(iVar4 + 8) = 2;
      }
      *(short *)(iVar4 + 0x10) = *(short *)(iVar4 + 0x10) + 1;
    }
    if (*(char *)(iVar4 + 8) == '\x02') {
      FUN_08023520(0x36);
      FUN_08023540(0x18,8);
      uVar13 = 0;
      iVar12 = DAT_08011804 + 0x40;
      do {
        *(char *)(iVar12 + uVar13) = (char)uVar13;
        uVar13 = uVar13 + 1 & 0xff;
      } while (uVar13 < 0x20);
      uVar13 = 0;
      do {
        bVar6 = (char)uVar13 + 1;
        for (uVar14 = (uint)bVar6; uVar14 < 0x20; uVar14 = uVar14 + 1 & 0xff) {
          if (*(ushort *)(iVar11 + uVar14 * 2) < *(ushort *)(iVar11 + uVar13 * 2)) {
            uVar3 = *(undefined2 *)(iVar11 + uVar13 * 2);
            *(undefined2 *)(iVar11 + uVar13 * 2) = *(undefined2 *)(iVar11 + uVar14 * 2);
            *(undefined2 *)(iVar11 + uVar14 * 2) = uVar3;
            uVar7 = *(undefined1 *)(iVar12 + uVar13);
            *(undefined1 *)(iVar12 + uVar13) = *(undefined1 *)(iVar12 + uVar14);
            *(undefined1 *)(iVar12 + uVar14) = uVar7;
          }
        }
        uVar13 = (uint)bVar6;
      } while (uVar13 < 0x20);
      uVar13 = 0;
      uVar14 = 0;
      uVar10 = 0;
      iVar11 = DAT_08011800 + 0x1a;
      while ((uVar13 < 3 || (uVar14 < 5))) {
        bVar6 = *(byte *)(iVar12 + (0x1f - uVar10));
        if ((bVar6 < 0x10) && (uVar13 < 3)) {
          *(byte *)(iVar11 + uVar13 + uVar14) = bVar6 + 1;
          uVar13 = uVar13 + 1 & 0xff;
        }
        else if ((0xf < bVar6) && (uVar14 < 5)) {
          *(byte *)(iVar11 + uVar13 + uVar14) = bVar6 + 1;
          uVar14 = uVar14 + 1 & 0xff;
        }
        uVar10 = uVar10 + 1 & 0xff;
      }
      *(undefined1 *)(iVar4 + 8) = 0;
      *(undefined1 *)(iVar4 + 9) = 100;
    }
    cVar1 = *(char *)(iVar4 + 8);
    if ((*(char *)(iVar4 + 6) != cVar1) && (*(char *)(iVar4 + 6) = cVar1, cVar1 == '\0')) {
      *(undefined2 *)(iVar4 + 0x10) = 0;
      *(undefined1 *)(iVar4 + 8) = 0;
      FUN_08010b24();
      FUN_08023520(0x36);
      *(undefined2 *)(iVar4 + 0x10) = 0;
      FUN_08023540(0x18,8);
      FUN_08023520(0x36);
      thunk_FUN_0801c118(1000);
      puVar5 = DAT_08011810;
      FUN_08014814(*DAT_08011810);
      *(undefined1 *)(iVar4 + 2) = *puVar5;
      puVar5 = DAT_08011814;
      FUN_08014864(*DAT_08011814);
      *(undefined1 *)(iVar4 + 3) = *puVar5;
      FUN_08023520(0x33);
      thunk_FUN_0801c118(1000);
      FUN_08023520(0x36);
      return;
    }
  }
  return;
}

