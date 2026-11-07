// 0x08031b5c FUN_08031b5c size=168

undefined8 FUN_08031b5c(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  
  iVar1 = DAT_08031c04;
  uStack_28 = param_1;
  uStack_24 = param_2;
  uStack_20 = param_3;
  uStack_1c = param_4;
  for (uVar6 = 0; uVar6 < *(ushort *)(*(int *)(iVar1 + 4) + 0x250); uVar6 = uVar6 + 1) {
    if (*(char *)(*(int *)(iVar1 + 4) + uVar6 + 0x230) == '\0') {
      for (uVar5 = 0; iVar4 = *(int *)(iVar1 + 4), uVar5 < *(ushort *)(iVar4 + 0x250);
          uVar5 = uVar5 + 1) {
        if (((*(char *)(iVar4 + uVar5 + 0x230) == '\0') && (uVar6 != uVar5)) &&
           (iVar4 = FUN_0801755e(iVar4 + uVar6 * 0x10 + 0x30,iVar4 + uVar5 * 0x10 + 0x30),
           iVar4 != 0)) {
          FUN_08017734(&uStack_28,*(int *)(iVar1 + 4) + uVar6 * 0x10 + 0x30,
                       *(int *)(iVar1 + 4) + uVar5 * 0x10 + 0x30);
          uVar2 = FUN_080274b0(&uStack_28);
          iVar4 = FUN_080274b0(*(int *)(iVar1 + 4) + uVar6 * 0x10 + 0x30);
          iVar3 = FUN_080274b0(*(int *)(iVar1 + 4) + uVar5 * 0x10 + 0x30);
          if (uVar2 < (uint)(iVar3 + iVar4)) {
            FUN_0802740c(*(int *)(iVar1 + 4) + uVar6 * 0x10 + 0x30,&uStack_28);
            *(undefined1 *)(*(int *)(iVar1 + 4) + uVar5 + 0x230) = 1;
          }
        }
      }
    }
  }
  return CONCAT44(uStack_24,uStack_28);
}

