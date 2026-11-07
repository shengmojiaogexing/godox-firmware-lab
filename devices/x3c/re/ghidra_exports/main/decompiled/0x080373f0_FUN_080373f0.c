// 0x080373f0 FUN_080373f0 size=124

undefined4 FUN_080373f0(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 auStack_34 [2];
  short local_32;
  short local_30;
  undefined1 auStack_24 [14];
  undefined1 local_16;
  
  local_32 = 0;
  local_16 = 1;
  FUN_0801aff4(auStack_24,auStack_34,0x14,5);
  if ((local_32 == -0x165e) && (local_30 == 0x165d)) {
    uVar1 = 0;
  }
  else {
    iVar2 = FUN_08037738(DAT_0803746c,0xb5c);
    if (iVar2 == 0) {
      local_16 = 1;
      iVar2 = FUN_0801aff4(auStack_24,auStack_34,0x14,5);
      if (iVar2 == 0) {
        if ((local_32 == -0x165e) && (local_30 == 0x165d)) {
          uVar1 = 0;
        }
        else {
          uVar1 = 0xffffffff;
        }
      }
      else {
        uVar1 = 0xffffffff;
      }
    }
    else {
      uVar1 = 0xffffffff;
    }
  }
  return uVar1;
}

