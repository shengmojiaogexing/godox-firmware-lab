// 0x0803a204 FUN_0803a204 size=292

/* WARNING: Removing unreachable block (ram,0x08018ab8) */

void FUN_0803a204(int param_1)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  
  if (param_1 == 0) {
    FUN_0801be28(1);
    FUN_0801bd04(DAT_0803a310,1);
    FUN_08018ad8(0x1f2c);
    FUN_08018ae4(8000);
    FUN_08018af0(0x1f54);
    FUN_08018ac8(0);
    puVar1 = DAT_08018ac4;
    *DAT_08018ac4 = *DAT_08018ac4 | 2;
    *puVar1 = *puVar1 & 0xfffffffe | 1;
    return;
  }
  iVar2 = *DAT_0803a2fc;
  iVar3 = iVar2 - DAT_0803a300;
  if (iVar2 == DAT_0803a300) {
    iVar2 = 4;
  }
  else if (iVar2 < DAT_0803a300) {
    if (iVar2 == DAT_0803a308) {
      iVar2 = 3;
    }
    else if (iVar2 < DAT_0803a308) {
      if (iVar2 + DAT_0803a30c == 0) {
        iVar2 = 1;
      }
      else {
        if (iVar2 + DAT_0803a30c + DAT_0803a304 != 0) {
          return;
        }
        iVar2 = 0;
      }
    }
    else {
      iVar2 = (iVar2 - DAT_0803a308) + DAT_0803a304;
      if (iVar2 == 0) {
        iVar2 = 2;
      }
      else {
        if (iVar2 + DAT_0803a304 != 0) {
          return;
        }
        iVar2 = 5;
      }
    }
  }
  else if (iVar3 == DAT_0803a314) {
    iVar2 = 9;
  }
  else if (iVar3 < DAT_0803a314) {
    if (iVar3 + DAT_0803a304 == 0) {
      iVar2 = 7;
    }
    else {
      if (iVar3 + DAT_0803a304 + DAT_0803a304 != 0) {
        return;
      }
      iVar2 = 6;
    }
  }
  else {
    iVar2 = (iVar3 - DAT_0803a314) + DAT_0803a304;
    if (iVar2 == 0) {
      iVar2 = 8;
    }
    else {
      if (iVar2 + DAT_0803a304 != 0) {
        return;
      }
      iVar2 = 0xb;
    }
  }
  *DAT_0801be24 = *DAT_0801be24 & 0xffff0fff | iVar2 << 0xc;
  return;
}

