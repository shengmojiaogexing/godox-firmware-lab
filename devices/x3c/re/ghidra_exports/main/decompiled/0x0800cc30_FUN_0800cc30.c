// 0x0800cc30 FUN_0800cc30 size=464

void FUN_0800cc30(void)

{
  undefined4 uVar1;
  byte *pbVar2;
  byte bVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  
  uVar1 = DAT_0800ce04;
  iVar4 = FUN_08022eb4(DAT_0800ce04,0x200);
  uVar6 = (uint)(iVar4 == 0);
  iVar4 = FUN_08022eb4(uVar1,0x100);
  if (iVar4 == 0) {
    uVar6 = uVar6 | 2;
  }
  iVar4 = FUN_08022eb4(DAT_0800ce08,0x8000);
  pbVar2 = DAT_0800ce0c;
  if (iVar4 == 0) {
    uVar6 = uVar6 | 4;
  }
  if (uVar6 == 0) {
    if (DAT_0800ce0c[5] != 0) {
      DAT_0800ce0c[5] = DAT_0800ce0c[5] - 1;
    }
  }
  else if (DAT_0800ce0c[5] != 0) {
    uVar6 = 0;
  }
  uVar5 = (uint)pbVar2[0xc];
  if (((uVar5 == 0) && (uVar6 != 0)) && (*DAT_0800ce10 != '\0')) {
    *DAT_0800ce10 = '\0';
    pbVar2[10] = 1;
  }
  uVar7 = uVar5 & ~uVar6;
  uVar5 = uVar6 & ~uVar5;
  pbVar2[0xc] = (byte)uVar6;
  if (uVar6 != 0) {
    *(undefined4 *)(pbVar2 + 0x14) = *(undefined4 *)(pbVar2 + 0x20);
    *(undefined4 *)(pbVar2 + 0x18) = *(undefined4 *)(pbVar2 + 0x20);
    *(uint *)(pbVar2 + 0x1c) = (uint)*DAT_0800ce14;
    *DAT_0800ce18 = 2000;
  }
  if ((((uVar6 & 1) != 0) && (FUN_08022e38(uVar1,0x4000), (uVar5 & 1) == 0)) &&
     (((pbVar2[1] & 1) == 0 &&
      (bVar3 = pbVar2[0xd], pbVar2[0xd] = bVar3 + 1, (byte)(bVar3 + 1) == 1)))) {
    pbVar2[1] = pbVar2[1] | 1;
    FUN_08017018();
    *DAT_0800ce1c = *DAT_0800ce1c ^ 1;
  }
  if ((((int)((uint)pbVar2[0xc] << 0x1e) < 0) && (-1 < (int)(uVar5 << 0x1e))) &&
     (-1 < (int)((uint)pbVar2[1] << 0x1e))) {
    bVar3 = pbVar2[0xe] + 1;
    pbVar2[0xe] = bVar3;
    if (bVar3 == 1) {
      *pbVar2 = *pbVar2 | 2;
    }
    else if (0x28 < bVar3) {
      pbVar2[1] = pbVar2[1] | 2;
      *pbVar2 = *pbVar2 & 0xfd;
      pbVar2[7] = 2;
      *DAT_0800ce20 = 0;
      if ((int)(uVar6 << 0x1d) < 0) {
        FUN_08036a14();
        FUN_080073f0();
        FUN_08013b80();
        pbVar2[8] = 200;
        do {
          FUN_0803ba4c();
          FUN_080335bc();
        } while (pbVar2[8] != 0);
      }
      FUN_08013cdc();
      FUN_08035fd0();
    }
  }
  if ((((int)((uint)pbVar2[0xc] << 0x1d) < 0) && (-1 < (int)(uVar5 << 0x1d))) &&
     (-1 < (int)((uint)pbVar2[1] << 0x1d))) {
    bVar3 = pbVar2[0xf] + 1;
    pbVar2[0xf] = bVar3;
    if (((bVar3 != 1) && (0x1d < bVar3)) && (pbVar2[1] = pbVar2[1] | 4, *DAT_0800ce24 != '\0')) {
      *DAT_0800ce24 = '\0';
      pbVar2[9] = 1;
    }
  }
  if ((uVar7 & 1) != 0) {
    pbVar2[0xd] = 0;
    pbVar2[1] = pbVar2[1] & 0xfe;
    FUN_08022e34(uVar1,0x4000);
    FUN_080118b0(0x21);
    FUN_0801c118(200);
    FUN_080118b0(0x21);
  }
  if ((int)(uVar7 << 0x1e) < 0) {
    pbVar2[0xe] = 0;
    bVar3 = *pbVar2;
    if ((int)((uint)bVar3 << 0x1e) < 0) {
      *DAT_0800ce28 = *DAT_0800ce28 ^ 1;
    }
    *pbVar2 = bVar3 & 0xfd;
    pbVar2[1] = pbVar2[1] & 0xfd;
  }
  if ((int)(uVar7 << 0x1d) < 0) {
    pbVar2[0xf] = 0;
    pbVar2[1] = pbVar2[1] & 0xfb;
    pbVar2[9] = 0;
  }
  return;
}

