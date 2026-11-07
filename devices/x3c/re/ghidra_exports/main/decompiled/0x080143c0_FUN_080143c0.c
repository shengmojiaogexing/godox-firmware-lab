// 0x080143c0 FUN_080143c0 size=74

void FUN_080143c0(undefined4 param_1)

{
  uint *puVar1;
  
  if (*(int *)(DAT_0801440c + 0xc) == DAT_08014410) {
    FUN_08035bc4(param_1);
    FUN_0801c118(1);
    *DAT_08014414 = *DAT_08014414 | 0x10;
    return;
  }
  puVar1 = DAT_08014414 + -5;
  FUN_08022e34(puVar1,0x10);
  FUN_08025e94(param_1);
  FUN_0801c118(5);
  FUN_08022e38(puVar1,0x10);
  return;
}

