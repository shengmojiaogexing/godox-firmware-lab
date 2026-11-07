// 0x08006c90 FUN_08006c90 size=276

void FUN_08006c90(void)

{
  undefined1 uVar1;
  undefined2 uVar2;
  undefined1 *puVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  
  puVar3 = DAT_08006da4;
  *DAT_08006da4 = 0x1e;
  puVar3[1] = 0x2d;
  puVar3[2] = 0x3c;
  puVar3[3] = 0x4b;
  puVar3[4] = *DAT_08006da8;
  puVar3[5] = *DAT_08006dac;
  if ((*DAT_08006db0 == '\x02') && (*DAT_08006db4 == '\x02')) {
    puVar3[6] = *DAT_08006e20;
  }
  else {
    puVar3[6] = *DAT_08006db4;
  }
  uVar1 = *DAT_08006db8;
  puVar3[7] = uVar1;
  puVar3[8] = *DAT_08006dbc;
  puVar3[9] = *DAT_08006dc0;
  puVar3[10] = *DAT_08006dc4;
  puVar3[0xb] = *DAT_08006dc8;
  puVar3[0xc] = *DAT_08006dcc;
  puVar3[0xd] = *DAT_08006dd0;
  iVar4 = DAT_08006dd4;
  uVar6 = 0;
  do {
    puVar3[uVar6 * 6 + 0xe] = *(undefined1 *)(iVar4 + uVar6 * 6);
    iVar5 = iVar4 + uVar6 * 6;
    puVar3[uVar6 * 6 + 0xf] = *(undefined1 *)(iVar5 + 1);
    puVar3[uVar6 * 6 + 0x10] = *(undefined1 *)(iVar5 + 2);
    puVar3[uVar6 * 6 + 0x11] = *(undefined1 *)(iVar5 + 3);
    puVar3[uVar6 * 6 + 0x12] = *(undefined1 *)(iVar5 + 4);
    puVar3[uVar6 * 6 + 0x13] = *(byte *)(iVar5 + 5) & 1;
    uVar6 = uVar6 + 1 & 0xff;
  } while (uVar6 < 0x10);
  puVar3[0x72] = *DAT_08006dd8;
  puVar3[0x73] = *DAT_08006ddc;
  uVar2 = *DAT_08006de0;
  puVar3[0x74] = (char)((ushort)uVar2 >> 8);
  puVar3[0x75] = (char)uVar2;
  puVar3[0x76] = *DAT_08006de4;
  puVar3[0x77] = *DAT_08006de8;
  puVar3[0x78] = *DAT_08006dec;
  puVar3[0x7a] = *DAT_08006df0;
  puVar3[0x79] = *DAT_08006df4;
  puVar3[0x7b] = *DAT_08006df8;
  puVar3[0x7c] = *DAT_08006dfc;
  puVar3[0x7e] = uVar1;
  puVar3[0x7f] = *DAT_08006e00;
  puVar3[0x80] = *DAT_08006e04;
  puVar3[0x82] = *DAT_08006e08;
  puVar3[0x83] = *DAT_08006e0c;
  puVar3[0x84] = *DAT_08006e10;
  puVar3[0x87] = 0x5a;
  puVar3[0x85] = *DAT_08006e14;
  puVar3[0x86] = *DAT_08006e18;
  puVar3[0x88] = *DAT_08006e1c;
  return;
}

