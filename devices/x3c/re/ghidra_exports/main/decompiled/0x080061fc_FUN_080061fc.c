// 0x080061fc FUN_080061fc size=30

void FUN_080061fc(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_0800621c;
  iVar2 = FUN_080303b8(*(undefined4 *)(DAT_0800621c + 0x38));
  if (iVar2 == 0) {
    return;
  }
  FUN_080315fa(*(undefined4 *)(iVar1 + 0x38),0);
  return;
}

