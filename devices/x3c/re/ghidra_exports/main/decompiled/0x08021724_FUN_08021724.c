// 0x08021724 FUN_08021724 size=52

void FUN_08021724(undefined2 param_1)

{
  undefined4 *puVar1;
  uint *puVar2;
  
  puVar1 = DAT_0802175c;
  *DAT_0802175c = DAT_08021758;
  *puVar1 = DAT_08021760;
  puVar2 = DAT_0802175c + 2;
  do {
  } while (-1 < (int)(*puVar2 << 0x16));
  *puVar2 = *puVar2 | 0x10;
  *(undefined2 *)(DAT_08021764 + 0x10) = param_1;
  FUN_080217f4(0x100000);
  *puVar2 = *puVar2 & 0xffffffef;
  return;
}

