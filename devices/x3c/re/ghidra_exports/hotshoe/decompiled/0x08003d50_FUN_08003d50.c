// 0x08003d50 FUN_08003d50 size=116

void FUN_08003d50(void)

{
  int iVar1;
  undefined4 local_18;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 auStack_10 [12];
  
  local_18 = 0x20000;
  local_12 = 1;
  local_14 = 0;
  local_13 = 0;
  FUN_08002c90(&local_18);
  FUN_08003960(0x10000000);
  FUN_0800384c();
  iVar1 = DAT_08003dc4;
  *(uint *)(DAT_08003dc4 + 0x20) = *(uint *)(DAT_08003dc4 + 0x20) | 0x10000;
  *(uint *)(iVar1 + 0x20) = *(uint *)(iVar1 + 0x20) & 0xfffeffff;
  FUN_080039a0();
  FUN_08003df8();
  FUN_080038f0(2);
  FUN_080039b0();
  FUN_08003cf0(0x1000);
  FUN_080037f4(2);
  FUN_08003c88();
  FUN_08003e54();
  FUN_08003b90(auStack_10);
  FUN_08003a98(auStack_10);
  return;
}

