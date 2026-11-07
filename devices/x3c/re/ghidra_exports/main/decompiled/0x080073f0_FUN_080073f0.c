// 0x080073f0 FUN_080073f0 size=186

void FUN_080073f0(void)

{
  int iVar1;
  byte *pbVar2;
  int iVar3;
  uint uVar4;
  
  *DAT_080074ac = 0x15;
  *DAT_080074b0 = 0;
  iVar1 = DAT_080074b4;
  uVar4 = 0;
  do {
    *(undefined1 *)(iVar1 + uVar4 * 6) = 0;
    iVar3 = iVar1 + uVar4 * 6;
    *(undefined1 *)(iVar3 + 1) = 0x50;
    *(undefined1 *)(iVar3 + 2) = 0x1e;
    *(undefined1 *)(iVar3 + 3) = 0;
    *(undefined1 *)(iVar3 + 4) = 0xb2;
    *(undefined1 *)(iVar3 + 5) = 0;
    uVar4 = uVar4 + 1 & 0xff;
  } while (uVar4 < 0x10);
  *(undefined1 *)(iVar1 + 3) = 0xc0;
  *DAT_080074b8 = 0;
  *DAT_080074bc = 0;
  *DAT_080074c0 = 1;
  *DAT_080074c4 = 5;
  *DAT_080074c8 = 0x50;
  *DAT_080074cc = 5;
  pbVar2 = DAT_080074d0;
  *DAT_080074d0 = 0x80;
  *DAT_080074d4 = 0x80;
  *DAT_080074d8 = 0;
  *DAT_080074e0 = *(undefined2 *)(DAT_080074dc + (*pbVar2 & 0x7f) * 2);
  *DAT_080074e8 = (uint)*DAT_080074e4;
  *DAT_080074ec = 1;
  *DAT_080074f0 = 0x50;
  *DAT_080074f4 = 0;
  *DAT_080074f8 = 1;
  *DAT_080074fc = 0;
  *DAT_08007500 = 0;
  *DAT_08007504 = 100;
  *DAT_08007508 = 0;
  *DAT_0800750c = 0;
  *DAT_08007510 = 0;
  *DAT_08007514 = 0x18;
  *DAT_08007518 = 0;
  *DAT_0800751c = 1;
  *DAT_08007520 = 1;
  return;
}

