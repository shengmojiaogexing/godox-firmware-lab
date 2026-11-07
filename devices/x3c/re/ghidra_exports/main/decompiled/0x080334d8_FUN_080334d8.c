// 0x080334d8 FUN_080334d8 size=16

undefined4 FUN_080334d8(void)

{
  char *pcVar1;
  undefined4 uVar2;
  
  pcVar1 = DAT_080334e8;
  uVar2 = *(undefined4 *)(DAT_080334e8 + 4);
  do {
    *pcVar1 = '\x01';
  } while (*pcVar1 == '\0');
  return uVar2;
}

