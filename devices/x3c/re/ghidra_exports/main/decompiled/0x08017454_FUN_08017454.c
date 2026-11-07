// 0x08017454 FUN_08017454 size=34

void FUN_08017454(void)

{
  undefined1 *puVar1;
  undefined4 uVar2;
  
  FUN_08017f7c(DAT_08017478,0x44);
  uVar2 = FUN_08033500(DAT_0801747c,0x1c,0);
  puVar1 = DAT_08017480;
  *(undefined4 *)(DAT_08017480 + 8) = uVar2;
  FUN_08018fa8();
  *puVar1 = 0;
  return;
}

