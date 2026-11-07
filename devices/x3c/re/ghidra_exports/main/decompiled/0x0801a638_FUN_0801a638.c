// 0x0801a638 FUN_0801a638 size=288

void FUN_0801a638(void)

{
  int iVar1;
  undefined4 uVar2;
  char *pcVar3;
  byte bVar4;
  ushort uVar5;
  int iVar6;
  int iVar7;
  
  pcVar3 = DAT_0801a760;
  uVar2 = DAT_0801a75c;
  iVar1 = DAT_0801a758;
  if (*(char *)(DAT_0801a758 + 3) == '\x01') {
    iVar7 = FUN_08022eb4(DAT_0801a75c,1);
    if (iVar7 == 1) {
      *pcVar3 = '\0';
      *DAT_0801a768 = 4;
      *(undefined2 *)(iVar1 + 0x20) = 0;
    }
    else {
      uVar5 = *(short *)(iVar1 + 0x20) + 1;
      *(ushort *)(iVar1 + 0x20) = uVar5;
      if (5000 < uVar5) {
        *(undefined1 *)(iVar1 + 3) = 0;
      }
    }
  }
  else {
    iVar6 = FUN_08022eb4(DAT_0801a75c,1);
    iVar7 = DAT_0801a764;
    if (((iVar6 == 1) && (iVar6 = FUN_08022eb4(uVar2,0x8000), iVar6 == 0)) &&
       (iVar6 = FUN_08022eb4(uVar2,0x100), iVar6 == 1)) {
      *(undefined1 *)(iVar7 + (uint)*(byte *)(iVar1 + 0x13)) = 1;
      bVar4 = *(char *)(iVar1 + 0x12) + 1;
      *(byte *)(iVar1 + 0x12) = bVar4;
      if (0x32 < bVar4) {
        *pcVar3 = '\x01';
        *(undefined1 *)(iVar1 + 0x12) = 0;
      }
    }
    else {
      *(undefined1 *)(iVar7 + (uint)*(byte *)(iVar1 + 0x13)) = 0;
      *(undefined1 *)(iVar1 + 0x12) = 0;
      iVar7 = FUN_08022eb4(uVar2,0x100);
      if (iVar7 == 1) {
        *pcVar3 = '\0';
      }
      iVar7 = FUN_08022eb4(uVar2,0x8000);
      if (((iVar7 == 1) && (iVar7 = FUN_08022eb4(uVar2,1), iVar7 == 1)) &&
         (iVar7 = FUN_08022eb4(uVar2,0x100), iVar7 == 1)) {
        bVar4 = *(char *)(iVar1 + 0x16) + 1;
        *(byte *)(iVar1 + 0x16) = bVar4;
        if (200 < bVar4) {
          *(undefined1 *)(iVar1 + 0x16) = 0;
          bVar4 = *(char *)(iVar1 + 0x15) + 1;
          *(byte *)(iVar1 + 0x15) = bVar4;
          if (10 < bVar4) {
            *(undefined1 *)(iVar1 + 0x15) = 0;
            *(undefined1 *)(iVar1 + 3) = 1;
          }
        }
      }
      else {
        *(undefined1 *)(iVar1 + 0x15) = 0;
      }
    }
    bVar4 = *(char *)(iVar1 + 0x13) + 1;
    *(byte *)(iVar1 + 0x13) = bVar4;
    if ((0x31 < bVar4) && (*(undefined1 *)(iVar1 + 0x13) = 0, *pcVar3 == '\0')) {
      bVar4 = 0;
      do {
        bVar4 = bVar4 + 1;
      } while (bVar4 < 0x32);
      *(undefined1 *)(iVar1 + 0x14) = 0;
      *(undefined1 *)(iVar1 + 3) = 0;
    }
    if (*pcVar3 != '\0' || *(char *)(iVar1 + 3) != '\0') {
      *DAT_0801a770 = *DAT_0801a76c;
    }
  }
  return;
}

