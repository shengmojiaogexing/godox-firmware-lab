// 0x08031e68 FUN_08031e68 size=350

/* WARNING: Removing unreachable block (ram,0x0803678a) */
/* WARNING: Removing unreachable block (ram,0x080367b2) */
/* WARNING: Removing unreachable block (ram,0x080367b4) */

void FUN_08031e68(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int *piVar7;
  int extraout_r3;
  int iVar8;
  undefined4 uVar9;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined1 auStack_68 [4];
  undefined4 local_64;
  undefined4 local_5c;
  undefined1 auStack_58 [16];
  undefined1 auStack_48 [36];
  uint uStack_24;
  undefined1 auStack_20 [12];
  
  iVar4 = FUN_0802acf4(param_2);
  if ((iVar4 == 0x15) || (iVar5 = FUN_0802eb88(DAT_08031edc,param_2), iVar5 == 1)) {
    uVar6 = FUN_0802ad70(param_2);
    if (iVar4 == 0x13) {
      piVar7 = (int *)FUN_0802ad6c(param_2);
      FUN_0802ee0a(uVar6);
      iVar5 = FUN_08022772();
      iVar1 = FUN_0802fc5e(uVar6);
      iVar4 = *piVar7;
      if (*piVar7 <= iVar5 - iVar1) {
        iVar4 = iVar5 - iVar1;
      }
      *piVar7 = iVar4;
    }
    else {
      if (iVar4 == 0x29) {
        iVar4 = FUN_0802ee0a(uVar6);
        iVar5 = FUN_080223b4(iVar4);
        if (iVar5 == 0) {
          return;
        }
        uVar6 = FUN_0802cd72();
        iVar1 = FUN_0802e0f8(iVar5,0,uVar6);
        if (iVar1 == 1) {
          FUN_0803172a(iVar5,0);
        }
        else if (iVar1 == 2) {
          iVar1 = FUN_0802ecd0(iVar4);
          iVar2 = FUN_0802fc5e(iVar5);
          FUN_0803172a(iVar5,(iVar1 - iVar2) / 2);
        }
        else if (iVar1 == 3) {
          iVar1 = FUN_0802ecd0(iVar4);
          iVar2 = FUN_0802fc5e(iVar5);
          FUN_0803172a(iVar5,iVar1 - iVar2);
        }
        iVar1 = FUN_0802faa0(iVar4,0);
        iVar2 = FUN_0802fab2(iVar4,0);
        iVar8 = *(int *)(iVar1 + 8);
        iVar1 = FUN_0802ec9c(iVar4);
        uStack_24 = FUN_0802f9bc(iVar4,0,100);
        FUN_08024eb8(iVar4);
        iVar1 = (iVar1 / 2 - iVar8 / 2) - (uint)*(ushort *)(iVar4 + 0x2e) * (iVar8 + iVar2);
        FUN_08026938(iVar5,DAT_080367f8);
        iVar4 = FUN_0802edcc(iVar5,8,auStack_20 + 0x10,0);
        if (((iVar4 == 1) && (extraout_r3 != iVar1)) || (iVar4 == 0)) {
          FUN_080316f4(iVar5,iVar1,0);
        }
        return;
      }
      if (iVar4 == 0x15) {
        iVar4 = FUN_0802ad70(param_2);
        iVar5 = FUN_0802ee0a(iVar4);
        FUN_08028d04(auStack_48);
        FUN_0802fe78(iVar5,0,auStack_48);
        iVar1 = FUN_0802cd68(iVar4);
        if (iVar1 != 0) {
          uStack_24 = uStack_24 | 0x1000000;
        }
        iVar1 = FUN_0802ad00(param_2);
        uVar6 = *(undefined4 *)(iVar1 + 8);
        iVar2 = FUN_08017484(auStack_58,uVar6,iVar5 + 0x14);
        if (iVar2 != 0) {
          *(undefined1 **)(iVar1 + 8) = auStack_58;
          FUN_0802270c(iVar5,auStack_68);
          local_78 = *(undefined4 *)(iVar4 + 0x14);
          local_74 = *(undefined4 *)(iVar4 + 0x18);
          local_70 = *(undefined4 *)(iVar4 + 0x1c);
          local_6c = local_64;
          iVar5 = FUN_08017484(&local_78,*(undefined4 *)(iVar1 + 8),&local_78);
          if (iVar5 != 0) {
            uVar9 = *(undefined4 *)(iVar1 + 8);
            *(undefined4 **)(iVar1 + 8) = &local_78;
            uVar3 = FUN_0802cd72(iVar4);
            FUN_08028800(iVar1,auStack_48,iVar4 + 0x14,uVar3,0);
            *(undefined4 *)(iVar1 + 8) = uVar9;
          }
          local_78 = *(undefined4 *)(iVar4 + 0x14);
          local_74 = local_5c;
          local_70 = *(undefined4 *)(iVar4 + 0x1c);
          local_6c = *(undefined4 *)(iVar4 + 0x20);
          iVar5 = FUN_08017484(&local_78,*(undefined4 *)(iVar1 + 8),&local_78);
          if (iVar5 != 0) {
            uVar9 = *(undefined4 *)(iVar1 + 8);
            *(undefined4 **)(iVar1 + 8) = &local_78;
            uVar3 = FUN_0802cd72(iVar4);
            FUN_08028800(iVar1,auStack_48,iVar4 + 0x14,uVar3,0);
            *(undefined4 *)(iVar1 + 8) = uVar9;
          }
          *(undefined4 *)(iVar1 + 8) = uVar6;
        }
        return;
      }
    }
  }
  return;
}

