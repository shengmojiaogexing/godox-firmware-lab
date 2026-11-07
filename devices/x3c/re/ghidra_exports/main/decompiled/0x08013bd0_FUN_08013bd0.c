// 0x08013bd0 FUN_08013bd0 size=256

void FUN_08013bd0(int param_1)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  byte *pbVar4;
  byte bVar5;
  ushort uVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  
  iVar3 = DAT_08013cd0;
  cVar1 = *(char *)(param_1 + 8);
  if (cVar1 == '\x1c') {
    if (*(char *)(DAT_08013cd0 + 0xb) == '\0') {
      uVar9 = *(undefined4 *)(param_1 + 0xc);
      bVar5 = FUN_08027774(*(undefined4 *)(param_1 + 4));
      if (*(char *)(iVar3 + 0x10) == -1) {
        *(byte *)(iVar3 + 0x10) = bVar5;
      }
      else {
        iVar7 = FUN_0802fcb6(*DAT_08013cd4,8);
        pbVar4 = DAT_08013cd8;
        if (iVar7 == 0) {
          iVar7 = (uint)bVar5 - (uint)*(byte *)(iVar3 + 0x10);
          if (iVar7 < 0) {
            iVar8 = *DAT_08013cd8 - 0x14;
            if (-iVar8 == iVar7 || -iVar7 < iVar8) {
              *DAT_08013cd8 = *DAT_08013cd8 + (char)iVar7;
            }
            else {
              *DAT_08013cd8 = 0x14;
            }
          }
          else {
            bVar2 = *DAT_08013cd8;
            *DAT_08013cd8 = (byte)((uint)bVar2 + iVar7);
            if (100 < ((uint)bVar2 + iVar7 & 0xff)) {
              *pbVar4 = 100;
            }
          }
          *(byte *)(iVar3 + 0x10) = bVar5;
          FUN_080316fe(uVar9,(int)((*pbVar4 - 0x14) * 0x104) / 0x50 & 0xffff);
          uVar9 = FUN_0803c384(*pbVar4);
          FUN_0803c4a4(uVar9,1);
          uVar6 = FUN_0803c344();
          FUN_0801a234((uint)uVar6 % 0xff + 0x37 & 0xff);
          return;
        }
        FUN_080316fe(uVar9,(int)((*DAT_08013cd8 - 0x14) * 0x104) / 0x50 & 0xffff);
        uVar9 = FUN_0803c384(*pbVar4);
        FUN_0803c4a4(uVar9,1);
        uVar6 = FUN_0803c344();
        FUN_0801a234((uint)uVar6 % 0xff + 0x37 & 0xff);
        *(byte *)(iVar3 + 0x10) = bVar5;
      }
    }
  }
  else if (cVar1 == '\x01') {
    *(undefined1 *)(DAT_08013cd0 + 0x10) = 0xff;
    *(undefined1 *)(iVar3 + 6) = 1;
  }
  else if (cVar1 == '\b') {
    *(undefined1 *)(DAT_08013cd0 + 6) = 0;
  }
  return;
}

