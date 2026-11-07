// 0x08013688 FUN_08013688 size=580

bool FUN_08013688(byte *param_1,uint param_2)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  char cVar6;
  ushort uVar7;
  byte bVar8;
  undefined4 local_50;
  undefined1 local_4c;
  undefined1 local_4b;
  undefined1 local_4a;
  undefined1 local_49;
  char acStack_48 [28];
  undefined4 local_2c;
  undefined1 local_28;
  undefined1 local_27;
  undefined1 local_26;
  undefined1 local_25;
  
  cVar6 = -0x5f;
  bVar8 = 0;
  FUN_080380c4();
  iVar2 = DAT_080138cc;
  bVar1 = *param_1;
  if (bVar1 == 0xc3) {
    cVar6 = -0x3c;
  }
  else if (bVar1 < 0xc4) {
    if (bVar1 != 0xa0) {
      if (bVar1 == 0xc0) {
        cVar6 = -0x3f;
      }
      else if (bVar1 == 0xc1) {
        cVar6 = -0x3e;
      }
    }
  }
  else if (bVar1 == 0xc5) {
    cVar6 = -0x3a;
  }
  else if (bVar1 == 0xc6) {
    cVar6 = -0x39;
  }
  else if (bVar1 == 200) {
    cVar6 = -0x37;
  }
  if (bVar1 == 0xc0) {
    for (uVar4 = 0; uVar4 < param_2; uVar4 = uVar4 + 1 & 0xff) {
      *(byte *)(iVar2 + uVar4) = param_1[uVar4];
    }
  }
  else {
    for (uVar4 = 1; uVar4 < param_2; uVar4 = uVar4 + 1 & 0xff) {
      cVar6 = param_1[uVar4] + cVar6;
    }
    for (uVar4 = 0; uVar4 < param_2; uVar4 = uVar4 + 1 & 0xff) {
      *(byte *)(iVar2 + uVar4) = param_1[uVar4];
    }
    *(char *)(iVar2 + param_2) = cVar6;
    acStack_48[param_2] = cVar6;
    param_2 = param_2 + 1 & 0xff;
  }
  FUN_08035c56(0x28);
  uVar3 = DAT_080138d0;
  FUN_08038400(DAT_080138d0,0);
  iVar2 = DAT_080138d4;
  do {
    *(undefined1 *)(iVar2 + 2) = 0;
    FUN_08038128(&local_50);
    local_50 = 0;
    local_4c = 0;
    local_4b = 1;
    local_4a = 1;
    local_49 = 1;
    FUN_080382c8(uVar3,&local_50);
    FUN_08038224(uVar3,0x40,0);
    FUN_08022e34(DAT_080138d8,0x400);
    local_25 = 2;
    local_28 = 0;
    local_26 = 1;
    local_2c = 0x400;
    local_27 = 0;
    FUN_08022e50(DAT_080138d8,&local_2c);
    for (uVar4 = 0; uVar4 < param_2; uVar4 = uVar4 + 1 & 0xff) {
      uVar7 = 0;
      FUN_080381d6(uVar3,*(undefined1 *)(DAT_080138cc + uVar4));
      do {
        iVar5 = FUN_08038216(uVar3,2);
        if (iVar5 != 0) break;
        uVar7 = uVar7 + 1;
      } while (uVar7 < 20000);
      if (19999 < uVar7) break;
    }
    FUN_08038224(uVar3,0x40,1);
    FUN_0801c118(10);
    *(undefined1 *)(iVar2 + 4) = 2;
    *(undefined1 *)(iVar2 + 2) = 0;
    *(undefined1 *)(iVar2 + 5) = 0;
    uVar7 = 0;
    do {
      if (*(char *)(iVar2 + 4) == '\0') break;
      uVar7 = uVar7 + 1;
      FUN_0801c118(1);
    } while (uVar7 < 200);
    if ((*param_1 == *(byte *)(iVar2 + 5)) || (*(byte *)(DAT_080138cc + 0x48) == *param_1)) break;
    local_25 = 2;
    local_28 = 0;
    local_26 = 0;
    local_2c = 0x400;
    local_27 = 1;
    FUN_08022e50(DAT_080138d8,&local_2c);
    bVar8 = bVar8 + 1;
    FUN_0801c118(500);
  } while (bVar8 < 3);
  *(uint *)(DAT_080138d8 + 0x14) = *(uint *)(DAT_080138d8 + 0x14) | 0x400;
  local_25 = 2;
  local_28 = 0;
  local_26 = 0;
  local_2c = 0x400;
  local_27 = 1;
  FUN_08022e50(DAT_080138d8,&local_2c);
  FUN_080380c4();
  FUN_08035c78(0x28,1,0);
  FUN_08038400(uVar3,1);
  if (2 < bVar8) {
    FUN_080380c4();
  }
  return 2 < bVar8;
}

