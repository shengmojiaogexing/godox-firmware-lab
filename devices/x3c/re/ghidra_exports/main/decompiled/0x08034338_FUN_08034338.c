// 0x08034338 FUN_08034338 size=528

void FUN_08034338(int param_1)

{
  byte bVar1;
  char *pcVar2;
  char *pcVar3;
  int iVar4;
  char cVar5;
  int iVar6;
  undefined4 uVar7;
  uint uVar8;
  uint uVar9;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  
  pcVar2 = DAT_08034548;
  if ((DAT_08034548[0xe] == '\0') && (*(char *)(param_1 + 8) == '\f')) {
    FUN_0802c5fc();
    cVar5 = FUN_0802c608();
    pcVar2[0xd] = cVar5;
    iVar6 = FUN_080303b8(*(undefined4 *)(pcVar2 + 0x34));
    iVar4 = DAT_08034550;
    pcVar3 = DAT_0803454c;
    if (iVar6 == 0) {
      cVar5 = pcVar2[0xd];
      if (cVar5 == '\x02') {
        if (((*DAT_0803454c == '\0') && (pcVar2[8] != 0)) && ((byte)pcVar2[8] < 0x11)) {
          if (pcVar2[0x10] == '\0') {
            FUN_080315fa(*(undefined4 *)(pcVar2 + 0x78),0);
            FUN_080315fa(*(undefined4 *)(pcVar2 + 0x44),0);
            FUN_080315fa(*(undefined4 *)(pcVar2 + 0x48),0);
            FUN_080315fa(*(undefined4 *)(pcVar2 + 0x4c),0);
            FUN_0803118e(*(undefined4 *)(pcVar2 + 0x74),1);
            return;
          }
          pcVar2[0xd] = '\0';
        }
      }
      else if (cVar5 == '\x04') {
        iVar6 = FUN_080303b8(*(undefined4 *)(pcVar2 + 0x6c));
        if ((iVar6 == 0) && (*pcVar3 == '\0')) {
          uVar8 = (uint)(byte)pcVar2[8];
          if (((uVar8 != 0) && ((pcVar2[0x11] != '\x01' && (uVar8 < 0x10)))) &&
             (pcVar2[0xe] == '\0')) {
            while ((uVar8 < 0x10 && (*(byte *)(iVar4 + uVar8 * 6 + 3) >> 6 == 0))) {
              uVar8 = uVar8 + 1 & 0xff;
            }
            if (uVar8 < 0x10) {
              *pcVar2 = pcVar2[8] - 1;
              uVar7 = FUN_08010acc(*(undefined4 *)(pcVar2 + 0x5c),0,0,0x198,0x170,0x198);
              *(undefined4 *)(pcVar2 + 0x6c) = uVar7;
              FUN_0800d504(uVar7,uVar8);
              pcVar2[8] = (char)uVar8 + '\x01';
              pcVar2[0xe] = '\x03';
              FUN_080269c8(&local_68);
              local_68 = *(undefined4 *)(pcVar2 + 0x6c);
              local_48 = FUN_0802fc68();
              local_40 = 0;
              local_3c = 300;
              local_64 = DAT_08034554;
              local_4c = DAT_08034558;
              local_44 = local_48;
              FUN_08026aac(&local_68);
              FUN_080269c8(&local_68);
              local_68 = *(undefined4 *)(pcVar2 + 0x68);
              local_48 = 0;
              local_44 = 0;
              local_40 = 0xfffffe68;
              local_3c = 300;
              local_64 = DAT_08034554;
              local_4c = DAT_08034558;
              FUN_08026aac(&local_68);
            }
          }
        }
      }
      else if (((cVar5 == '\b') &&
               (iVar6 = FUN_080303b8(*(undefined4 *)(pcVar2 + 0x6c)), iVar6 == 0)) &&
              (*pcVar3 == '\0')) {
        bVar1 = pcVar2[8];
        if (((bVar1 != 0) && (pcVar2[0x11] != '\x01')) &&
           ((uVar8 = bVar1 - 2, uVar8 < 0xf && (pcVar2[0xe] == '\0')))) {
          while (uVar9 = uVar8 & 0xff, *(byte *)(iVar4 + uVar9 * 6 + 3) >> 6 == 0) {
            uVar8 = uVar9 - 1;
          }
          if (uVar9 < 0x10) {
            *pcVar2 = bVar1 - 1;
            uVar7 = FUN_08010acc(*(undefined4 *)(pcVar2 + 0x5c),0,0,0xfffffe68,0x170,0x198);
            *(undefined4 *)(pcVar2 + 0x6c) = uVar7;
            FUN_0800d504(uVar7,uVar9);
            pcVar2[8] = (char)uVar8 + '\x01';
            pcVar2[0xe] = '\x03';
            FUN_080269c8(&local_68);
            local_68 = *(undefined4 *)(pcVar2 + 0x6c);
            local_48 = FUN_0802fc68();
            local_40 = 0;
            local_3c = 300;
            local_64 = DAT_08034554;
            local_4c = DAT_08034558;
            local_44 = local_48;
            FUN_08026aac(&local_68);
            FUN_080269c8(&local_68);
            local_68 = *(undefined4 *)(pcVar2 + 0x68);
            local_48 = 0;
            local_44 = 0;
            local_40 = 0x198;
            local_3c = 300;
            local_64 = DAT_08034554;
            local_4c = DAT_08034558;
            FUN_08026aac(&local_68);
          }
        }
      }
    }
  }
  return;
}

