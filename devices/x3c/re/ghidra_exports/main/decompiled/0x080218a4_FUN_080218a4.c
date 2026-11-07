// 0x080218a4 FUN_080218a4 size=52

void FUN_080218a4(undefined2 param_1)

{
  undefined4 *puVar1;
  uint *puVar2;
  
  puVar1 = DAT_080218dc;
  *DAT_080218dc = DAT_080218d8;
  *puVar1 = DAT_080218e0;
  puVar2 = DAT_080218dc + 2;
  do {
  } while (-1 < (int)(*puVar2 << 0x16));
  *puVar2 = *puVar2 | 0x10;
  *(undefined2 *)(DAT_080218e4 + 2) = param_1;
  FUN_080217f4(0x100000);
  *puVar2 = *puVar2 & 0xffffffef;
  return;
}

