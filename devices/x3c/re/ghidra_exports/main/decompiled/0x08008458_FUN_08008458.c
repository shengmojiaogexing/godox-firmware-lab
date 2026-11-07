// 0x08008458 FUN_08008458 size=60

void FUN_08008458(void)

{
  int iVar1;
  
  iVar1 = FUN_08020984(4);
  if (iVar1 == 0) {
    return;
  }
  *DAT_08008494 = 0;
  if (*DAT_08008498 != '\0') {
    *DAT_0800849c = 100;
  }
  *DAT_080084a0 = 1;
  *(undefined1 *)(DAT_080084a4 + 0xf) = 1;
  *DAT_080084a8 = 2000;
  FUN_08020978(4);
  return;
}

