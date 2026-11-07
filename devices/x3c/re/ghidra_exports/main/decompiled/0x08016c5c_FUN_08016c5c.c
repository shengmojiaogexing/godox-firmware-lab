// 0x08016c5c FUN_08016c5c size=226

void FUN_08016c5c(void)

{
  char cVar1;
  short sVar2;
  undefined4 uVar3;
  int iVar4;
  char *pcVar5;
  undefined4 uVar6;
  ushort uVar7;
  int iVar8;
  
  uVar3 = DAT_08016d40;
  iVar8 = FUN_08039916(DAT_08016d40,1);
  uVar6 = DAT_08016d4c;
  pcVar5 = DAT_08016d48;
  iVar4 = DAT_08016d44;
  if (iVar8 == 0) {
    return;
  }
  cVar1 = *(char *)(DAT_08016d44 + 0xe);
  if (cVar1 == '\0') {
    if (*DAT_08016d48 == '\0') {
      if (*(char *)(DAT_08016d44 + 10) != '\0') {
        iVar8 = FUN_08022eb4(DAT_08016d4c,0x40);
        if (iVar8 == 0) {
          FUN_080398fa(uVar3,0);
          *pcVar5 = '\x01';
          FUN_0801445c();
          if (*pcVar5 == '\n') {
            *(short *)(iVar4 + 0x16) = *(short *)(iVar4 + 0x16) + 1;
          }
          else {
            *(undefined2 *)(iVar4 + 0x16) = 0;
          }
          *(undefined1 *)(iVar4 + 10) = 0;
          FUN_08023b48();
          FUN_080398fa(uVar3,1);
        }
        else {
          sVar2 = *(short *)(iVar4 + 0x14);
          *(short *)(iVar4 + 0x14) = sVar2 + 1;
          if (500 < (ushort)(sVar2 + 1U)) {
            *(undefined2 *)(iVar4 + 0x14) = 0;
            FUN_08023b48();
            *(short *)(iVar4 + 0x16) = *(short *)(iVar4 + 0x16) + 1;
            *(undefined1 *)(iVar4 + 10) = 0;
          }
          *(undefined1 *)(iVar4 + 0xd) = 0;
        }
      }
    }
    else {
      uVar7 = *(short *)(DAT_08016d44 + 0x1a) + 1;
      *(ushort *)(DAT_08016d44 + 0x1a) = uVar7;
      if (500 < uVar7) {
        *(undefined2 *)(iVar4 + 0x1a) = 0;
        iVar8 = FUN_08022eb4(uVar6,0x40);
        if (iVar8 != 0) {
          cVar1 = *pcVar5;
          *pcVar5 = cVar1 + -1;
          if (cVar1 == '\x01') {
            FUN_08023b74();
            *(undefined1 *)(iVar4 + 10) = 1;
            *(undefined1 *)(iVar4 + 0xd) = 0;
          }
        }
      }
    }
    if (10 < *(ushort *)(iVar4 + 0x16)) {
      *(undefined2 *)(iVar4 + 0x16) = 0;
      *(undefined1 *)(iVar4 + 10) = 0;
      *pcVar5 = '\0';
    }
  }
  else {
    uVar7 = *(short *)(DAT_08016d44 + 0x18) + 1;
    *(ushort *)(DAT_08016d44 + 0x18) = uVar7;
    if (100 < uVar7) {
      *(undefined2 *)(iVar4 + 0x18) = 0;
      *(char *)(iVar4 + 0xe) = cVar1 + -1;
    }
  }
  FUN_08039910(uVar3,1);
  return;
}

