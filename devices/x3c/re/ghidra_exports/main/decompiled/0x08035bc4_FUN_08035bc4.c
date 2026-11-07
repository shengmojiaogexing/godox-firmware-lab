// 0x08035bc4 FUN_08035bc4 size=32

void FUN_08035bc4(undefined1 param_1)

{
  *DAT_08035be4 = *DAT_08035be4 & 0xffffffef;
  DAT_6c000000 = 0xf9;
  FUN_0801c118(1);
  DAT_6c000000 = param_1;
  return;
}

