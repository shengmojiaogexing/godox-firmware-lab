// 0x0800d340 FUN_0800d340 size=32

void FUN_0800d340(void)

{
  undefined4 *puVar1;
  int iVar2;
  
  puVar1 = DAT_0800d360;
  FUN_0802b798(*DAT_0800d360);
  iVar2 = DAT_0800d364;
  FUN_0802b584(*puVar1,*(undefined4 *)(DAT_0800d364 + 0x14));
  FUN_0802b584(*puVar1,*(undefined4 *)(iVar2 + 0x18));
  return;
}

