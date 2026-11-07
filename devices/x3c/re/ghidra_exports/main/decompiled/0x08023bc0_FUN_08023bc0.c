// 0x08023bc0 FUN_08023bc0 size=936

undefined8 FUN_08023bc0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  byte bVar1;
  bool bVar2;
  char *pcVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  undefined4 uVar9;
  uint uVar10;
  ushort uVar11;
  int iVar12;
  uint uVar13;
  int iVar14;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  
  pcVar3 = DAT_08023f68;
  bVar2 = false;
  pcVar3[0xc] = '\0';
  pcVar3[0xd] = '\0';
  pcVar3[0xe] = '\0';
  pcVar3[0xf] = '\0';
  local_2c = param_3;
  local_28 = param_4;
  FUN_08023f88();
  pcVar3[8] = '\0';
  iVar14 = DAT_08023f6c;
  pcVar3[9] = '\0';
  pcVar3[10] = '\0';
  pcVar3[0xb] = '\0';
  uVar6 = 0;
  do {
    *(uint *)(pcVar3 + 8) = (uint)*(byte *)(uVar6 + iVar14) + *(int *)(pcVar3 + 8);
    uVar6 = uVar6 + 1;
  } while (uVar6 < 0x8000);
  local_30 = param_2;
  if (*(int *)(pcVar3 + 8) == 0x7f8000) {
    uVar9 = 0;
  }
  else {
    pcVar3[2] = '\0';
    FUN_0803ba4c();
    FUN_08022e34(DAT_08023f70,0x1000);
    pcVar3[2] = '\0';
    pcVar3[1] = '\0';
    pcVar3[6] = '\0';
    uVar9 = DAT_08023f74;
    FUN_08022e34(DAT_08023f74,0x400);
    uVar6 = 0;
    local_2c = 0x400;
    local_28 = 0x2010000;
    FUN_08022e50(uVar9,&local_2c);
    *pcVar3 = '\0';
    puVar4 = DAT_08023f78;
    do {
      if (uVar6 % 100 == 0) {
        *puVar4 = 0xc6;
        puVar4[1] = 1;
        FUN_08013688(DAT_08023f78,2);
      }
      uVar6 = uVar6 + 1 & 0xffff;
      if (200 < uVar6) {
        bVar2 = true;
        goto LAB_08023f5a;
      }
      FUN_0801c0fe(1);
      FUN_0803ba4c();
    } while (*pcVar3 == '\0');
    *pcVar3 = '\0';
    pcVar3[7] = '\x01';
    pcVar3[2] = '\0';
    pcVar3[3] = '\x01';
    uVar6 = 0;
    do {
      if (uVar6 % 100 == 0) {
        *puVar4 = 200;
        uVar9 = *(undefined4 *)(pcVar3 + 8);
        puVar4[1] = (char)uVar9;
        puVar4[2] = (char)((uint)uVar9 >> 8);
        puVar4[3] = (char)((uint)uVar9 >> 0x10);
        puVar4[4] = (char)((uint)uVar9 >> 0x18);
        puVar4[5] = 0;
        puVar4[6] = 0x80;
        FUN_08013688(DAT_08023f78,7);
      }
      uVar6 = uVar6 + 1 & 0xffff;
      if (200 < uVar6) {
        bVar2 = true;
        goto LAB_08023f5a;
      }
      FUN_0801c0fe(1);
      FUN_0803ba4c();
    } while (pcVar3[3] != '\0');
    if (pcVar3[2] == '\0') {
      uVar6 = 0;
      do {
        pcVar3[1] = '\0';
        if (uVar6 % 100 == 0) {
          *puVar4 = 0xc5;
          puVar4[1] = 1;
          FUN_08013688(DAT_08023f78,2);
        }
        FUN_0801c0fe(1);
        uVar6 = uVar6 + 1 & 0xffff;
        if (200 < uVar6) {
          bVar2 = true;
          goto LAB_08023f5a;
        }
      } while (pcVar3[1] == '\0');
      pcVar3[7] = '\0';
    }
    FUN_0801c118(500);
    if (pcVar3[2] != '\0') {
      uVar6 = 0;
      *DAT_08023f7c = 0;
      *DAT_08023f80 = 1;
      FUN_08024010();
      FUN_0803ba4c();
      FUN_0803ba4c();
      FUN_0801c118(5);
      FUN_08022e38(DAT_08023f74,0x10);
      pcVar3[2] = '\0';
      do {
        pcVar3[5] = (char)uVar6 * '\x03';
        FUN_0803ba4c();
        iVar14 = uVar6 << 10;
        if (uVar6 == 0x1f) {
          pcVar3[5] = 'd';
        }
        iVar12 = 0;
        uVar13 = 0;
        do {
          *puVar4 = 0xc0;
          uVar7 = uVar13 + uVar6 * 0x10;
          puVar4[1] = (char)uVar7;
          puVar4[2] = (char)((uVar7 & 0xffff) >> 8);
          puVar4[3] = 0x40;
          iVar8 = DAT_08023f6c;
          uVar10 = (uVar7 & 0xff) + ((uVar7 & 0xffff) >> 8) + 0x101;
          uVar7 = 0;
          do {
            bVar1 = *(byte *)(iVar14 + iVar8);
            uVar10 = uVar10 + bVar1 & 0xffff;
            iVar12 = iVar12 + (uint)bVar1;
            puVar4[uVar7 + 4] = bVar1;
            iVar14 = iVar14 + 1;
            uVar7 = uVar7 + 1;
          } while (uVar7 < 0x40);
          puVar4[0x44] = (char)uVar10;
          puVar4[0x45] = (char)(uVar10 >> 8);
          do {
            iVar8 = FUN_08013688(DAT_08023f78,0x46);
            FUN_0803ba4c();
          } while (iVar8 != 0);
          FUN_0801c118(500);
          uVar13 = uVar13 + 1;
        } while (uVar13 < 0x10);
        FUN_0803ba4c();
        FUN_0801c118(500);
        pcVar3[3] = '\x01';
        *puVar4 = 0xc1;
        puVar4[1] = (char)iVar12;
        puVar4[2] = (char)((uint)iVar12 >> 8);
        puVar4[3] = (char)((uint)iVar12 >> 0x10);
        puVar4[4] = (char)((uint)iVar12 >> 0x18);
        FUN_08013688(DAT_08023f78,5);
        uVar11 = 0;
        do {
          if (pcVar3[3] == '\0') goto LAB_08023e76;
          FUN_0801c0fe(1);
          uVar11 = uVar11 + 1;
        } while (uVar11 < 0xc9);
        pcVar3[4] = '\x01';
LAB_08023e76:
        if (pcVar3[4] != '\0') {
          pcVar3[4] = '\0';
          uVar6 = uVar6 - 1;
        }
        FUN_0801c118(500);
        FUN_080335bc();
        uVar6 = uVar6 + 1;
      } while (uVar6 < 0x20);
      *puVar4 = 0xc3;
      puVar4[1] = 1;
      FUN_08013688(DAT_08023f78,2);
      FUN_0801c0fe(100);
      FUN_0803a1f0();
      puVar5 = DAT_08023f80;
      *DAT_08023f80 = 1;
      if (*DAT_08023f84 == -1) {
        *puVar5 = 0;
      }
      uVar6 = 0;
      do {
        pcVar3[1] = '\0';
        if (uVar6 % 100 == 0) {
          *puVar4 = 0xc5;
          puVar4[1] = 1;
          FUN_08013688(DAT_08023f78,2);
        }
        FUN_0801c0fe(1);
        FUN_0803ba4c();
        uVar6 = uVar6 + 1 & 0xffff;
        local_30 = 0;
        if (200 < uVar6) {
          bVar2 = true;
          goto LAB_08023f5a;
        }
      } while (pcVar3[1] == '\0');
      pcVar3[7] = '\0';
    }
    if (pcVar3[7] != '\0') {
      pcVar3[1] = '\0';
      pcVar3[7] = '\0';
      uVar6 = 0;
      do {
        pcVar3[1] = '\0';
        if (uVar6 % 100 == 0) {
          *puVar4 = 0xc5;
          puVar4[1] = 1;
          FUN_08013688(DAT_08023f78,2);
        }
        FUN_0801c0fe(1);
        uVar6 = uVar6 + 1 & 0xffff;
        if (200 < uVar6) {
          bVar2 = true;
          break;
        }
      } while (pcVar3[1] == '\0');
    }
LAB_08023f5a:
    if (bVar2) {
      uVar9 = 1;
    }
    else {
      uVar9 = 0;
    }
  }
  return CONCAT44(local_30,uVar9);
}

