// 0x08015418 FUN_08015418 size=588

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08015418(void)

{
  char *pcVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  bool bVar6;
  
  iVar4 = DAT_08015668;
  pcVar1 = DAT_08015664;
  if (DAT_08015664[3] == '\x01') {
    iVar2 = FUN_080303b8(*(undefined4 *)(DAT_08015664 + 0x54));
    if (iVar2 != 0) {
      FUN_080316cc(*(undefined4 *)(pcVar1 + 0x54),DAT_080156b0,0);
      FUN_0802d33c(*(undefined4 *)(pcVar1 + 0x54),s_Wirelese_Sync_080156b4);
    }
    iVar2 = FUN_080303b8(*(undefined4 *)(pcVar1 + 0x58));
    if (iVar2 != 0) {
      FUN_080316cc(*(undefined4 *)(pcVar1 + 0x58),DAT_080156b0,0);
      FUN_0802d33c(*(undefined4 *)(pcVar1 + 0x58),s_Cancel_080156c4);
    }
    iVar2 = FUN_080303b8(*(undefined4 *)(pcVar1 + 0x5c));
    if (iVar2 != 0) {
      FUN_080316cc(*(undefined4 *)(pcVar1 + 0x5c),DAT_080156b0,0);
      FUN_0802d33c(*(undefined4 *)(pcVar1 + 0x5c),s_Please_confirm_wireless_sync_on_t_080156cc);
    }
    uVar5 = 0;
    do {
      iVar2 = FUN_080303b8(*(undefined4 *)(iVar4 + uVar5 * 4));
      if (iVar2 != 0) {
        uVar3 = FUN_0802ebc8(*(undefined4 *)(iVar4 + uVar5 * 4),1);
        if (uVar5 == 3) {
          FUN_0802db44(uVar3,2,0,100);
        }
        iVar2 = FUN_080303b8(uVar3);
        if (iVar2 != 0) {
          FUN_080316cc(uVar3,DAT_080156b0,0);
          FUN_0802d33c(uVar3,*(undefined4 *)(_DAT_080156fc + uVar5 * 4));
        }
      }
      uVar5 = uVar5 + 1 & 0xff;
    } while (uVar5 < 0xd);
    if (*pcVar1 == '\f') {
      iVar4 = FUN_080303b8(*(undefined4 *)(pcVar1 + 0x84));
      if (iVar4 != 0) {
        FUN_080316cc(*(undefined4 *)(pcVar1 + 0x84),DAT_080156b0,0);
        FUN_0802d33c(*(undefined4 *)(pcVar1 + 0x84),s_Restore_Factory_Setting_080156ff + 1);
      }
      iVar4 = FUN_080303b8(*(undefined4 *)(pcVar1 + 0x88));
      if (iVar4 != 0) {
        uVar3 = FUN_0802ebc8(*(undefined4 *)(pcVar1 + 0x88),0);
        FUN_080316cc(uVar3,DAT_080156b0,0);
        FUN_0802d33c(uVar3,s_Apply_08015718);
      }
      iVar4 = FUN_080303b8(*(undefined4 *)(pcVar1 + 0x8c));
      if (iVar4 != 0) {
        uVar3 = FUN_0802ebc8(*(undefined4 *)(pcVar1 + 0x8c),0);
        FUN_080316cc(uVar3,DAT_080156b0,0);
        FUN_0802d33c(uVar3,s_Cancel_080156c4);
        return;
      }
    }
  }
  else if (DAT_08015664[3] == '\0') {
    iVar2 = FUN_080303b8(*(undefined4 *)(DAT_08015664 + 0x54));
    if (iVar2 != 0) {
      FUN_080316cc(*(undefined4 *)(pcVar1 + 0x54),DAT_0801566c,0);
      FUN_0802d33c(*(undefined4 *)(pcVar1 + 0x54),&DAT_08015670);
    }
    iVar2 = FUN_080303b8(*(undefined4 *)(pcVar1 + 0x58));
    if (iVar2 != 0) {
      FUN_080316cc(*(undefined4 *)(pcVar1 + 0x58),DAT_0801566c,0);
      FUN_0802d33c(*(undefined4 *)(pcVar1 + 0x58),&LAB_08015680);
    }
    iVar2 = FUN_080303b8(*(undefined4 *)(pcVar1 + 0x5c));
    if (iVar2 != 0) {
      FUN_080316cc(*(undefined4 *)(pcVar1 + 0x5c),DAT_0801566c,0);
      FUN_0802d33c(*(undefined4 *)(pcVar1 + 0x5c),&LAB_08015688);
    }
    uVar5 = 0;
    do {
      bVar6 = uVar5 == 7;
      do {
        do {
          if (bVar6) goto LAB_080155e4;
          bVar6 = true;
        } while (uVar5 == 8);
        iVar2 = FUN_080303b8(*(undefined4 *)(iVar4 + uVar5 * 4));
        bVar6 = true;
      } while (iVar2 == 0);
      uVar3 = FUN_0802ebc8(*(undefined4 *)(iVar4 + uVar5 * 4),1);
      if (uVar5 == 3) {
        FUN_0802db44(uVar3,2,0,0x73);
      }
      iVar2 = FUN_080303b8(uVar3);
      if (iVar2 != 0) {
        FUN_080316cc(uVar3,DAT_0801566c,0);
        FUN_0802d33c(uVar3,*(undefined4 *)(_DAT_080156fc + -0x34 + uVar5 * 4));
      }
LAB_080155e4:
      uVar5 = uVar5 + 1 & 0xff;
    } while (uVar5 < 0xd);
    if (*pcVar1 == '\f') {
      iVar4 = FUN_080303b8(*(undefined4 *)(pcVar1 + 0x84));
      if (iVar4 != 0) {
        FUN_080316cc(*(undefined4 *)(pcVar1 + 0x84),DAT_08015720,0);
        FUN_0802d33c(*(undefined4 *)(pcVar1 + 0x84),&LAB_08015724);
      }
      iVar4 = FUN_080303b8(*(undefined4 *)(pcVar1 + 0x88));
      if (iVar4 != 0) {
        uVar3 = FUN_0802ebc8(*(undefined4 *)(pcVar1 + 0x88),0);
        FUN_080316cc(uVar3,DAT_08015720,0);
        FUN_0802d33c(uVar3,&LAB_08015738);
      }
      iVar4 = FUN_080303b8(*(undefined4 *)(pcVar1 + 0x8c));
      if (iVar4 != 0) {
        uVar3 = FUN_0802ebc8(*(undefined4 *)(pcVar1 + 0x8c),0);
        FUN_080316cc(uVar3,DAT_08015720,0);
        FUN_0802d33c(uVar3,&LAB_08015680);
        return;
      }
    }
  }
  return;
}

