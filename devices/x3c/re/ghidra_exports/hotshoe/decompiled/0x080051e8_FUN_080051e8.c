// 0x080051e8 FUN_080051e8 size=106

void FUN_080051e8(void)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  byte bVar4;
  
  uVar3 = 0;
  cVar1 = *DAT_08005254;
  if (cVar1 == '\x05') {
LAB_0800521a:
    uVar3 = 0x50;
  }
  else {
    if (cVar1 == '\x15') {
LAB_0800521e:
      uVar3 = 10;
      goto LAB_08005230;
    }
    if (cVar1 == '%') {
LAB_08005222:
      uVar3 = 0xb;
      goto LAB_08005230;
    }
    if (cVar1 != '5') {
      if (cVar1 == '\t') goto LAB_0800521a;
      if (cVar1 == '\x19') goto LAB_0800521e;
      if (cVar1 == ')') goto LAB_08005222;
      if (cVar1 != '9') {
        if (cVar1 == 'I') {
          uVar3 = 0xd;
        }
        else if (cVar1 == 'Y') {
          uVar3 = 0xe;
        }
        goto LAB_08005230;
      }
    }
    uVar3 = 0xc;
  }
LAB_08005230:
  iVar2 = switchD_0800735a::caseD_ad();
  if (iVar2 != 0) {
    uVar3 = uVar3 | 0x80;
  }
  bVar4 = 0;
  do {
    iVar2 = FUN_080049a4(uVar3);
    if (iVar2 == 0) {
      return;
    }
    bVar4 = bVar4 + 1;
  } while (bVar4 < 3);
  return;
}

