// 0x08037ae8 FUN_08037ae8 size=470

void FUN_08037ae8(undefined4 param_1)

{
  undefined4 *puVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  undefined4 *puVar6;
  
  puVar1 = DAT_08037ccc;
  FUN_0802b798(*DAT_08037ccc);
  FUN_0802b8b2(*puVar1,0);
  iVar4 = DAT_08037cdc;
  iVar3 = DAT_08037cd8;
  pcVar2 = DAT_08037cd4;
  if (*DAT_08037cd0 != '\0') {
    switch(param_1) {
    case 0:
      uVar5 = 0;
      do {
        if ((*pcVar2 != '\x05') || (uVar5 != 1)) {
          FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + uVar5 * 4));
        }
        uVar5 = uVar5 + 1 & 0xff;
      } while (uVar5 < 0xd);
      FUN_0802b670(*(undefined4 *)(iVar3 + (*(byte *)(DAT_08037cdc + 4) - 1) * 4));
      return;
    case 1:
      if (*DAT_08037ce0 == '\0') {
        puVar6 = (undefined4 *)(DAT_08037cd8 + 0x34);
        FUN_0802b584(*puVar1,*puVar6);
        iVar4 = FUN_0802fc9e(*(undefined4 *)(iVar3 + 0x50),1);
        if (iVar4 == 0) {
          uVar5 = 0;
          do {
            FUN_0802b584(*puVar1,puVar6[uVar5 + 1]);
            uVar5 = uVar5 + 1 & 0xff;
          } while (uVar5 < 6);
        }
        FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + 0x58));
        FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + 0x5c));
        FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + 0x6c));
        return;
      }
      break;
    case 2:
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cd8 + 0x70));
      FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + 0x74));
      FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + 0x78));
      return;
    case 3:
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cd8 + 0x7c));
      FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + 0x80));
      FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + 0x84));
      return;
    case 4:
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cdc + 0x60));
      return;
    case 5:
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cd8 + 0x88));
      iVar4 = FUN_0802fc9e(*(undefined4 *)(iVar3 + 0x90),1);
      if (iVar4 == 0) {
        FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + 0x90));
        return;
      }
      break;
    case 6:
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cd8 + 0x94));
      FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + 0x9c));
      return;
    case 7:
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cdc + 0xbc));
      FUN_0802b584(*puVar1,*(undefined4 *)(iVar4 + 0xc0));
      return;
    case 8:
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cdc + 100));
      return;
    case 9:
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cdc + 0xd4));
    case 10:
      iVar3 = DAT_08037cd8;
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cd8 + 0xa0));
      iVar4 = FUN_0802fc9e(*(undefined4 *)(iVar3 + 0xac),1);
      if (iVar4 == 0) {
        FUN_0802b584(*puVar1,*(undefined4 *)(iVar3 + 0xac));
        return;
      }
      break;
    case 0xb:
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cdc + 0xc4));
      FUN_0802b584(*puVar1,*(undefined4 *)(iVar4 + 200));
      return;
    case 0xc:
      puVar6 = (undefined4 *)(DAT_08037cdc + 0xcc);
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cdc + 0xd0));
      FUN_0802b584(*puVar1,*puVar6);
      return;
    case 0xd:
      FUN_0802b584(*puVar1,*(undefined4 *)(DAT_08037cdc + 0x68));
      return;
    }
  }
  return;
}

