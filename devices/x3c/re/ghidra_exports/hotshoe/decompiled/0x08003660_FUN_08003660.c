// 0x08003660 FUN_08003660 size=188

void FUN_08003660(void)

{
  byte bVar1;
  byte *pbVar2;
  byte bVar3;
  int iVar4;
  int iVar5;
  
  pbVar2 = DAT_08003728;
  bVar3 = *DAT_0800371c;
  bVar1 = *DAT_08003720;
  if (bVar1 == 1) {
    iVar4 = 0;
  }
  else if (bVar1 == 2) {
    iVar4 = 1;
  }
  else if (bVar1 == 3) {
    iVar4 = 2;
  }
  else if (bVar1 == 4) {
    iVar4 = 3;
  }
  else if (bVar1 == 5) {
    iVar4 = 4;
  }
  else if (bVar1 < 8) {
    iVar4 = 5;
  }
  else if (bVar1 < 10) {
    iVar4 = 6;
  }
  else if (bVar1 == 10) {
    iVar4 = 7;
  }
  else if (bVar1 == 0xb) {
    iVar4 = 8;
  }
  else if (bVar1 < 0xf) {
    iVar4 = 9;
  }
  else if (bVar1 < 0x14) {
    iVar4 = 10;
  }
  else if (bVar1 < 0x3c) {
    iVar4 = 0xb;
  }
  else {
    iVar4 = 0xc;
  }
  if ((bVar3 < 0x50) && (bVar3 != 0x46)) {
    if (bVar3 == 6) {
      iVar5 = DAT_08003724 + -0xd;
    }
    else if (bVar3 == 0x32) {
      iVar5 = DAT_08003724 + -0x1a;
    }
    else if (bVar3 == 0x28) {
      iVar5 = DAT_08003724 + -0x27;
    }
    else if (bVar3 == 0x1e) {
      iVar5 = DAT_08003724 + -0x34;
    }
    else {
      if (bVar3 != 0x14) goto LAB_080036ea;
      iVar5 = DAT_08003724 + -0x41;
    }
    bVar3 = *(byte *)(iVar5 + iVar4);
  }
  else {
LAB_080036ea:
    bVar3 = *(byte *)(DAT_08003724 + iVar4);
  }
  *DAT_08003728 = bVar3;
  if (*pbVar2 < *DAT_0800372c) {
    *DAT_0800372c = *pbVar2;
  }
  return;
}

