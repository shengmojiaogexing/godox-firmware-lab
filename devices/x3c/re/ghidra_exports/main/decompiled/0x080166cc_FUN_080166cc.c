// 0x080166cc FUN_080166cc size=830

byte FUN_080166cc(uint param_1,int param_2)

{
  char cVar1;
  char cVar2;
  char cVar3;
  byte bVar4;
  byte bVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  
  cVar1 = *DAT_08016a18;
  cVar2 = *DAT_08016a1c;
  cVar3 = *DAT_08016a20;
  uVar8 = (uint)*DAT_08016a28;
  iVar9 = DAT_08016a24 + 0x51;
  bVar4 = *DAT_08016a2c;
  bVar5 = 0x50;
  switch(*DAT_08016a14) {
  case 1:
    if ((cVar1 == '\x01') && (cVar2 == '\x01')) {
      bVar5 = *(byte *)(DAT_08016a24 + uVar8 + -200);
    }
    else {
      bVar5 = *(byte *)(DAT_08016a24 + uVar8 + -0xcf);
    }
    uVar8 = bVar5 + param_1 & 0xff;
    if (uVar8 < 0x80) {
      uVar8 = 0;
    }
    else {
      uVar8 = uVar8 - 0x80 & 0xff;
    }
    uVar8 = uVar8 * 0x1400000 >> 0x18;
    if (0x50 < uVar8) {
      uVar8 = 0x50;
    }
    if (cVar3 == '\x01') {
      bVar5 = *(byte *)(DAT_08016a24 + uVar8);
      if (bVar4 < *(byte *)(DAT_08016a24 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    else {
      bVar5 = *(byte *)(iVar9 + uVar8);
      if (bVar4 < *(byte *)(iVar9 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    break;
  case 2:
    uVar6 = *(byte *)(DAT_08016a30 + param_2) + param_1 & 0xff;
    if ((cVar1 == '\x01') && (cVar2 == '\x01')) {
      uVar8 = uVar6 + *(byte *)(DAT_08016a24 + uVar8 + -0x9e) & 0xff;
      if (0x18 < uVar8 - 0x97) {
        uVar8 = uVar8 + 0x2a & 0xff;
      }
    }
    else {
      uVar8 = (uint)*(byte *)(DAT_08016a24 + uVar8 + -0xa5);
      if (uVar8 < 0x80) {
        uVar7 = 0;
      }
      else {
        uVar7 = 0xff - uVar8;
      }
      if (uVar7 == 0) {
        uVar8 = uVar6 + uVar8 & 0xff;
      }
      else if (uVar7 < uVar6) {
        uVar8 = uVar6 + uVar8 & 0xff;
      }
      else {
        uVar8 = 0;
      }
      if (uVar8 < 0x52) {
        uVar8 = 0;
      }
      else {
        uVar8 = uVar8 - 0x52 & 0xff;
      }
    }
    if (uVar8 < 0xc0) {
      if (0x50 < uVar8) {
        if (param_1 < 0xc0) {
          uVar8 = 0x50;
        }
        else {
          uVar8 = 0;
        }
      }
    }
    else {
      uVar8 = 0;
    }
    if (cVar3 == '\x01') {
      bVar5 = *(byte *)(DAT_08016a24 + uVar8);
      if (bVar4 < *(byte *)(DAT_08016a24 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    else {
      bVar5 = *(byte *)(iVar9 + uVar8);
      if (bVar4 < *(byte *)(iVar9 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    break;
  case 3:
    if ((cVar1 == '\x01') && (cVar2 == '\x01')) {
      uVar8 = *(byte *)(DAT_08016a24 + uVar8 + -0xba) + param_1 & 0xff;
      if (uVar8 < 0x1c) {
        uVar8 = 0;
      }
      else {
        uVar8 = uVar8 - 0x1c & 0xff;
      }
    }
    else {
      uVar8 = *(byte *)(DAT_08016a24 + uVar8 + -0xc1) + param_1 & 0xff;
      if (uVar8 < 0x26) {
        uVar8 = 0;
      }
      else {
        uVar8 = uVar8 - 0x26 & 0xff;
      }
    }
    if (uVar8 < 0xc1) {
      if (0x50 < uVar8) {
        uVar8 = 0x50;
      }
    }
    else {
      uVar8 = 0;
    }
    if (cVar3 == '\x01') {
      bVar5 = *(byte *)(DAT_08016a24 + uVar8);
      if (bVar4 < *(byte *)(DAT_08016a24 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    else {
      bVar5 = *(byte *)(iVar9 + uVar8);
      if (bVar4 < *(byte *)(iVar9 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    break;
  case 4:
    if ((cVar1 == '\x01') && (cVar2 == '\x01')) {
      uVar8 = *(byte *)(DAT_08016a24 + uVar8 + -0x90) + param_1 & 0xff;
      if (uVar8 < 0x14) {
        uVar8 = 0;
      }
      else {
        uVar8 = uVar8 - 0x14 & 0xff;
      }
    }
    else {
      uVar8 = *(byte *)(DAT_08016a24 + uVar8 + -0x97) + param_1 & 0xff;
      if (uVar8 < 0x14) {
        uVar8 = 0;
      }
      else {
        uVar8 = uVar8 - 0x14 & 0xff;
      }
    }
    if (cVar3 == '\x01') {
      bVar5 = *(byte *)(DAT_08016a24 + uVar8);
      if (bVar4 < *(byte *)(DAT_08016a24 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    else {
      bVar5 = *(byte *)(iVar9 + uVar8);
      if (bVar4 < *(byte *)(iVar9 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    break;
  case 5:
    if ((cVar1 == '\x01') && (cVar2 == '\x01')) {
      uVar8 = *(byte *)(DAT_08016a24 + uVar8 + -0xac) + param_1 & 0xff;
      if (uVar8 < 0x1e) {
        uVar8 = 0;
      }
      else {
        uVar8 = uVar8 - 0x1e & 0xff;
      }
    }
    else {
      uVar8 = *(byte *)(DAT_08016a24 + uVar8 + -0xb3) + param_1 & 0xff;
      if (uVar8 < 0x1e) {
        uVar8 = 0;
      }
      else {
        uVar8 = uVar8 - 0x1e & 0xff;
      }
    }
    if (0x50 < uVar8) {
      uVar8 = 0x50;
    }
    if (cVar3 == '\x01') {
      bVar5 = *(byte *)(DAT_08016a24 + uVar8);
      if (bVar4 < *(byte *)(DAT_08016a24 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    else {
      bVar5 = *(byte *)(iVar9 + uVar8);
      if (bVar4 < *(byte *)(iVar9 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    break;
  case 7:
    if ((cVar1 == '\x01') && (cVar2 == '\x01')) {
      if (*DAT_08016a34 == '\x05') {
        if (uVar8 == 3) {
          if (param_1 < 0x28) {
            param_1 = param_1 / 0xe + param_1 & 0xff;
          }
          else if (param_1 < 0x3c) {
            param_1 = param_1 / 10 + param_1 & 0xff;
          }
          else if (param_1 < 0x50) {
            param_1 = param_1 + (param_1 >> 3) & 0xff;
          }
          else {
            param_1 = param_1 / 6 + param_1 & 0xff;
          }
        }
        param_1 = *(byte *)(DAT_08016a24 + uVar8 + -0x74) + param_1;
      }
      else {
        if (uVar8 == 3) {
          if (param_1 < 0x5b) {
            if (0x49 < param_1) {
              param_1 = param_1 / 0xf + param_1 & 0xff;
            }
          }
          else {
            param_1 = param_1 + 10 & 0xff;
          }
        }
        param_1 = *(byte *)(DAT_08016a24 + uVar8 + -0x82) + param_1;
      }
      if ((param_1 & 0xff) < 0x17) {
        uVar8 = 0;
      }
      else {
        uVar8 = (param_1 & 0xff) - 0x17 & 0xff;
      }
    }
    else {
      if (*DAT_08016a34 == '\x05') {
        bVar5 = *(byte *)(DAT_08016a24 + uVar8 + -0x7b);
      }
      else {
        bVar5 = *(byte *)(DAT_08016a24 + uVar8 + -0x89);
      }
      uVar8 = bVar5 + param_1 & 0xff;
      if (uVar8 < 0x17) {
        uVar8 = 0;
      }
      else {
        uVar8 = uVar8 - 0x17 & 0xff;
      }
    }
    if (0x50 < uVar8) {
      uVar8 = 0x50;
    }
    if (cVar3 == '\x01') {
      bVar5 = *(byte *)(DAT_08016a24 + uVar8);
      if (bVar4 < *(byte *)(DAT_08016a24 + uVar8)) {
        bVar5 = bVar4;
      }
    }
    else {
      bVar5 = *(byte *)(iVar9 + uVar8);
      if (bVar4 < *(byte *)(iVar9 + uVar8)) {
        bVar5 = bVar4;
      }
    }
  }
  return bVar5;
}

