// 0x0803be5c FUN_0803be5c size=142

void FUN_0803be5c(int param_1)

{
  undefined1 *puVar1;
  int iVar2;
  undefined1 *puVar3;
  byte bVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  
  puVar3 = DAT_0803bef8;
  iVar2 = DAT_0803bef4;
  iVar7 = DAT_0803bef0;
  puVar1 = DAT_0803beec;
  if (*(char *)(param_1 + 8) == '\x1c') {
    if (*(char *)(DAT_0803bef0 + -0xc9) == '\0') {
      bVar4 = FUN_08031e48();
      uVar6 = (uint)bVar4;
      if (uVar6 == 0) {
        *puVar1 = 1;
        FUN_0802da22(*(undefined4 *)(iVar7 + 4),1);
        return;
      }
      if (uVar6 < 10) {
        *puVar1 = 0;
        *puVar3 = *(undefined1 *)(iVar2 + uVar6);
        FUN_0802e2d2(*(undefined4 *)(iVar7 + 4),1);
        return;
      }
    }
  }
  else if (*(char *)(param_1 + 8) == '\r') {
    iVar7 = (int)*(short *)(*(int *)(param_1 + 4) + 0x2e);
    uVar5 = *(undefined4 *)(DAT_0803bef0 + 4);
    if (iVar7 == 0) {
      *DAT_0803beec = 1;
      FUN_0802da22(uVar5,1);
      return;
    }
    if (iVar7 < 10) {
      *DAT_0803beec = 0;
      *puVar3 = *(undefined1 *)(iVar2 + iVar7);
      FUN_0802e2d2(uVar5,1);
      return;
    }
  }
  return;
}

