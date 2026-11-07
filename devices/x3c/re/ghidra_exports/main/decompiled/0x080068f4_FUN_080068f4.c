// 0x080068f4 FUN_080068f4 size=774

void FUN_080068f4(void)

{
  char cVar1;
  char cVar2;
  ushort uVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  undefined1 *puVar7;
  int iVar8;
  int iVar9;
  byte bVar10;
  int iVar11;
  uint uVar12;
  
  iVar6 = DAT_08006c04;
  iVar5 = DAT_08006bfc;
  if (*(char *)(DAT_08006bfc + 4) != *DAT_08006c00) {
    *(char *)(DAT_08006bfc + 4) = *DAT_08006c00;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 5) != *DAT_08006c08) {
    *(char *)(iVar5 + 5) = *DAT_08006c08;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  puVar7 = DAT_08006c14;
  cVar1 = *DAT_08006c10;
  if ((*DAT_08006c0c == '\x02') && (cVar1 == '\x02')) {
    if (*(char *)(DAT_08006bfc + 6) != *DAT_08006c8c) {
      *(char *)(iVar5 + 6) = *DAT_08006c8c;
      *(undefined2 *)(iVar6 + 2) = 0x78;
    }
  }
  else if (*(char *)(DAT_08006bfc + 6) != cVar1) {
    *(char *)(iVar5 + 6) = cVar1;
    *(undefined2 *)(iVar6 + 2) = 0x78;
    *puVar7 = 10;
  }
  cVar2 = *DAT_08006c18;
  if (*(char *)(iVar5 + 7) != cVar2) {
    *(char *)(iVar5 + 7) = cVar2;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 8) != *DAT_08006c1c) {
    *(char *)(iVar5 + 8) = *DAT_08006c1c;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 9) != *DAT_08006c20) {
    *(char *)(iVar5 + 9) = *DAT_08006c20;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 10) != *DAT_08006c24) {
    *(char *)(iVar5 + 10) = *DAT_08006c24;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0xb) != *DAT_08006c28) {
    *(char *)(iVar5 + 0xb) = *DAT_08006c28;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0xc) != *DAT_08006c2c) {
    *(char *)(iVar5 + 0xc) = *DAT_08006c2c;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0xd) != *DAT_08006c30) {
    *(char *)(iVar5 + 0xd) = *DAT_08006c30;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  iVar8 = DAT_08006c34;
  uVar12 = 0;
  do {
    iVar9 = iVar5 + uVar12 * 6;
    cVar4 = *(char *)(iVar8 + uVar12 * 6);
    if (*(char *)(iVar9 + 0xe) != cVar4) {
      *(char *)(iVar9 + 0xe) = cVar4;
      *(undefined2 *)(iVar6 + 2) = 0x78;
    }
    iVar11 = iVar8 + uVar12 * 6;
    if (*(char *)(iVar9 + 0xf) != *(char *)(iVar11 + 1)) {
      *(char *)(iVar9 + 0xf) = *(char *)(iVar11 + 1);
      *(undefined2 *)(iVar6 + 2) = 0x78;
    }
    if (*(char *)(iVar9 + 0x10) != *(char *)(iVar11 + 2)) {
      *(char *)(iVar9 + 0x10) = *(char *)(iVar11 + 2);
      *(undefined2 *)(iVar6 + 2) = 0x78;
    }
    if (*(char *)(iVar9 + 0x11) != *(char *)(iVar11 + 3)) {
      *(char *)(iVar9 + 0x11) = *(char *)(iVar11 + 3);
      *(undefined2 *)(iVar6 + 2) = 0x78;
    }
    if (*(char *)(iVar9 + 0x12) != *(char *)(iVar11 + 4)) {
      *(char *)(iVar9 + 0x12) = *(char *)(iVar11 + 4);
      *(undefined2 *)(iVar6 + 2) = 0x78;
    }
    bVar10 = *(byte *)(iVar11 + 5) & 1;
    if (*(byte *)(iVar9 + 0x13) != bVar10) {
      *(byte *)(iVar9 + 0x13) = bVar10;
      *(undefined2 *)(iVar6 + 2) = 0x78;
    }
    uVar12 = uVar12 + 1 & 0xff;
  } while (uVar12 < 0x10);
  if (*(char *)(iVar5 + 0x72) != *DAT_08006c38) {
    *(char *)(iVar5 + 0x72) = *DAT_08006c38;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x73) != *DAT_08006c3c) {
    *(char *)(iVar5 + 0x73) = *DAT_08006c3c;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  uVar3 = *DAT_08006c40;
  if ((ushort)*(byte *)(iVar5 + 0x74) != uVar3 >> 8) {
    *(char *)(iVar5 + 0x74) = (char)(uVar3 >> 8);
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x75) != (char)uVar3) {
    *(char *)(iVar5 + 0x75) = (char)uVar3;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x76) != *DAT_08006c44) {
    *(char *)(iVar5 + 0x76) = *DAT_08006c44;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x77) != *DAT_08006c48) {
    *(char *)(iVar5 + 0x77) = *DAT_08006c48;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x78) != *DAT_08006c4c) {
    *(char *)(iVar5 + 0x78) = *DAT_08006c4c;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x7a) != *DAT_08006c50) {
    *(char *)(iVar5 + 0x7a) = *DAT_08006c50;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x79) != *DAT_08006c54) {
    *(char *)(iVar5 + 0x79) = *DAT_08006c54;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x7b) != *DAT_08006c58) {
    *(char *)(iVar5 + 0x7b) = *DAT_08006c58;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x7c) != *DAT_08006c5c) {
    *(char *)(iVar5 + 0x7c) = *DAT_08006c5c;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x7d) != cVar1) {
    *(char *)(iVar5 + 0x7d) = cVar1;
    *(undefined2 *)(iVar6 + 2) = 0x78;
    *puVar7 = 10;
  }
  if (*(char *)(iVar5 + 0x7e) != cVar2) {
    *(char *)(iVar5 + 0x7e) = cVar2;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x7f) != *DAT_08006c60) {
    *(char *)(iVar5 + 0x7f) = *DAT_08006c60;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x80) != *DAT_08006c64) {
    *(char *)(iVar5 + 0x80) = *DAT_08006c64;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x81) != *DAT_08006c68) {
    *(char *)(iVar5 + 0x81) = *DAT_08006c68;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x83) != *DAT_08006c6c) {
    *(char *)(iVar5 + 0x83) = *DAT_08006c6c;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x82) != *DAT_08006c70) {
    *(char *)(iVar5 + 0x82) = *DAT_08006c70;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar5 + 0x85) != *DAT_08006c74) {
    *(char *)(iVar5 + 0x85) = *DAT_08006c74;
    *(undefined2 *)(iVar6 + 2) = 0x1e;
  }
  if (*(char *)(iVar5 + 0x86) != *DAT_08006c78) {
    *(char *)(iVar5 + 0x86) = *DAT_08006c78;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  cVar1 = *DAT_08006c7c;
  if (*(char *)(iVar5 + 0x88) != cVar1) {
    *(char *)(iVar5 + 0x88) = cVar1;
    *(undefined2 *)(iVar6 + 2) = 0x78;
  }
  if (*(char *)(iVar6 + 1) != cVar1) {
    *(char *)(iVar6 + 1) = cVar1;
    *puVar7 = 10;
    *DAT_08006c80 = *DAT_08006c80 ^ 0x1000;
  }
  if (*(short *)(iVar6 + 2) == 0x78) {
    *DAT_08006c88 = (uint)*DAT_08006c84;
  }
  return;
}

