// 0x08012ad8 FUN_08012ad8 size=620

void FUN_08012ad8(void)

{
  byte bVar1;
  undefined4 uVar2;
  undefined1 uVar3;
  int iVar4;
  int iVar5;
  char cVar6;
  uint uVar7;
  uint extraout_r2;
  int extraout_r3;
  ushort uVar8;
  undefined4 local_3c;
  undefined1 local_38;
  undefined1 local_37;
  undefined1 local_36;
  char local_35 [25];
  
  iVar5 = DAT_08012d58;
  local_35[1] = *DAT_08012d44;
  if (*DAT_08012d48 == 2) {
    local_35[2] = *(byte *)(DAT_08012d4c + 0xe) | 2;
  }
  else {
    local_35[2] = *DAT_08012d48;
  }
  if (*DAT_08012d50 == '\x01') {
    local_35[3] = 4;
  }
  else if ((int)((uint)*DAT_08012d54 << 0x18) < 0) {
    local_35[3] = 2;
  }
  else {
    cVar6 = '\0';
    uVar7 = 0;
    do {
      if (*(char *)(DAT_08012d58 + uVar7 * 6) == '\x01') {
        cVar6 = cVar6 + '\x01';
      }
      uVar7 = uVar7 + 1 & 0xff;
    } while (uVar7 < 0x10);
    if (cVar6 == '\0') {
      local_35[3] = 1;
    }
    else {
      local_35[3] = 0;
    }
  }
  local_35[4] = *DAT_08012d5c;
  local_35[5] = *DAT_08012d60;
  local_35[6] = *DAT_08012d64;
  local_35[7] = *DAT_08012d68;
  local_35[8] = 0;
  bVar1 = *DAT_08012d6c;
  if (bVar1 < 0x19) {
    local_35[9] = 1;
  }
  else if (bVar1 < 0x1d) {
    local_35[9] = 2;
  }
  else if (bVar1 < 0x24) {
    local_35[9] = 3;
  }
  else if (bVar1 < 0x33) {
    local_35[9] = 4;
  }
  else if (bVar1 < 0x47) {
    local_35[9] = 5;
  }
  else if (bVar1 < 0x51) {
    local_35[9] = 6;
  }
  else if (bVar1 < 0x6a) {
    local_35[9] = 7;
  }
  else if (bVar1 < 0x88) {
    local_35[9] = 8;
  }
  else {
    local_35[9] = 9;
  }
  if (*DAT_08012d70 != '\0') {
    local_35[9] = local_35[9] | 0x80;
  }
  local_35[10] = *DAT_08012d54;
  local_35[0xb] = *DAT_08012d74;
  if (0x46 < (byte)local_35[0xb] - 0x14) {
    local_35[0xb] = 0x5a;
  }
  local_35[0xc] = *DAT_08012d78;
  local_35[0xd] = *DAT_08012d7c;
  uVar7 = 0;
  do {
    iVar4 = iVar5 + uVar7 * 6;
    if (*(byte *)(iVar4 + 3) >> 6 == 3) {
      cVar6 = *(char *)(iVar5 + uVar7 * 6);
      if (cVar6 == '\0') {
        local_35[uVar7 * 2 + 0xe] = '\x01';
        local_35[uVar7 * 2 + 0xf] = *(char *)(iVar4 + 1);
      }
      else if (cVar6 == '\x01') {
        local_35[uVar7 * 2 + 0xe] = '\0';
        uVar3 = FUN_08008560(*(undefined1 *)(iVar4 + 2));
        *(undefined1 *)(extraout_r3 + 0xe) = uVar3;
        uVar7 = extraout_r2;
      }
    }
    else {
      local_35[uVar7 * 2 + 0xe] = '\x06';
      local_35[uVar7 * 2 + 0xf] = '\0';
    }
    uVar2 = DAT_08012d80;
    uVar7 = uVar7 + 1 & 0xff;
  } while (uVar7 < 5);
  local_35[0x18] = -0x2f;
  uVar7 = 0;
  do {
    local_35[0x18] = local_35[0x18] + local_35[uVar7 + 1];
    uVar7 = uVar7 + 1 & 0xff;
  } while (uVar7 < 0x17);
  FUN_08038400(DAT_08012d80,0);
  FUN_08038128(&local_3c);
  local_3c = 0;
  local_38 = 0;
  local_37 = 1;
  local_36 = 1;
  local_35[0] = '\x01';
  FUN_080382c8(uVar2,&local_3c);
  FUN_08038224(uVar2,0x40,0);
  uVar7 = 0;
  do {
    uVar8 = 0;
    FUN_080381d6(uVar2,local_35[uVar7 + 1]);
    do {
      iVar5 = FUN_08038216(uVar2,2);
      if (iVar5 != 0) break;
      uVar8 = uVar8 + 1;
    } while (uVar8 < 2000);
    uVar7 = uVar7 + 1 & 0xff;
    if (0x17 < uVar7) {
      FUN_0801c118(0x28);
      FUN_080380c4();
      FUN_08035c78(0x28,1,0);
      FUN_08038400(uVar2,1);
      iVar5 = DAT_08012d88;
      uVar7 = 1;
      do {
        *(char *)(iVar5 + uVar7) = local_35[uVar7];
        uVar7 = uVar7 + 1 & 0xff;
      } while (uVar7 < 0x18);
      return;
    }
  } while( true );
}

