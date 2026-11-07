// 0x080148a8 FUN_080148a8 size=550

void FUN_080148a8(undefined4 param_1)

{
  bool bVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  uint uVar10;
  uint uVar11;
  
  uVar11 = 0;
  bVar1 = false;
  *DAT_08014ad0 = 2;
  uVar6 = FUN_08010acc(param_1,9,0,0,0x170,0x1c0);
  puVar2 = DAT_08014ad4;
  DAT_08014ad4[4] = 1;
  *DAT_08014ad8 = 1;
  *puVar2 = 0;
  iVar3 = DAT_08014ae0;
  uVar9 = DAT_08014adc;
  uVar10 = 0;
  do {
    if ((uVar10 & 1) == 0) {
      uVar4 = 9;
    }
    else {
      uVar4 = 0xbf;
    }
    uVar5 = 0;
    uVar7 = FUN_08006220(uVar6,1,uVar4,(short)(uVar10 >> 1) * 0xb4,0xa8,0xa6);
    FUN_08031696(uVar7,0x19,0);
    uVar8 = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar7,uVar8,0);
    FUN_08031594(uVar7,0xff,0);
    *(undefined4 *)(iVar3 + uVar10 * 4) = uVar7;
    uVar4 = FUN_08027fb8();
    uVar8 = CONCAT22(uVar5,uVar4);
    FUN_0802553e(uVar7,2,0,0x14,DAT_08014ae4,uVar8,*(undefined4 *)(DAT_08014ad4 + uVar10 * 4 + 0xec)
                );
    uVar4 = (undefined2)((uint)uVar8 >> 0x10);
    if (uVar10 == 7) {
      uVar5 = FUN_08027fb8();
      FUN_0802553e(uVar7,2,0,0x73,uVar9,CONCAT22(uVar4,uVar5),*(undefined4 *)(puVar2 + 0x170));
    }
    else if (uVar10 == 8) {
      uVar5 = FUN_08027fb8();
      FUN_0802553e(uVar7,2,0,0x73,uVar9,CONCAT22(uVar4,uVar5),*(undefined4 *)(puVar2 + 0x174));
    }
    else if ((uVar10 == 3) && (puVar2[3] == '\x01')) {
      uVar5 = FUN_08027fb8();
      FUN_0802553e(uVar7,2,0,100,uVar9,CONCAT22(uVar4,uVar5),*(undefined4 *)(puVar2 + 0x160));
    }
    else if (puVar2[3] == '\0') {
      uVar5 = FUN_08027fb8();
      FUN_0802553e(uVar7,2,0,0x73,DAT_08014aec,CONCAT22(uVar4,uVar5),
                   *(undefined4 *)(DAT_08014ae8 + uVar10 * 4));
    }
    else if (puVar2[3] == '\x01') {
      uVar5 = FUN_08027fb8();
      FUN_0802553e(uVar7,2,0,0x73,uVar9,CONCAT22(uVar4,uVar5),
                   *(undefined4 *)(puVar2 + uVar10 * 4 + 0x154));
    }
    FUN_0802d97a(uVar7,DAT_08014af0,0);
    uVar10 = uVar10 + 1 & 0xff;
  } while (uVar10 < 0xd);
  if ((*DAT_08014af4 == '\x05') || (*DAT_08014af4 == '\a')) {
    FUN_0802da22(*(undefined4 *)(iVar3 + 4),1);
    uVar10 = 0;
    do {
      if (uVar10 != 1) {
        if (bVar1) {
          uVar9 = 0xbf;
        }
        else {
          uVar9 = 9;
        }
        FUN_0802db44(*(undefined4 *)(iVar3 + uVar10 * 4),1,uVar9,(uVar11 >> 1) * 0xb4);
        bVar1 = (bool)(bVar1 ^ 1);
        uVar11 = uVar11 + 1 & 0xff;
      }
      uVar10 = uVar10 + 1 & 0xff;
    } while (uVar10 < 0xd);
  }
  uVar9 = FUN_08010acc(param_1,1,0,0xb2,4,0x5c);
  uVar6 = FUN_08027fb8();
  FUN_0803157c(uVar9,uVar6,0);
  FUN_08031594(uVar9,0xff,0);
  return;
}

