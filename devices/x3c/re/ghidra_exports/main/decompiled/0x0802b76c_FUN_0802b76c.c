// 0x0802b76c FUN_0802b76c size=44

void FUN_0802b76c(int param_1)

{
  byte bVar1;
  
  bVar1 = *(byte *)(param_1 + 0x1c);
  *(byte *)(param_1 + 0x1c) = bVar1 | 8;
  if ((int)((uint)bVar1 << 0x1d) < 0) {
    FUN_0802b70c(param_1);
  }
  else {
    FUN_0802b644(param_1);
  }
  *(byte *)(param_1 + 0x1c) = *(byte *)(param_1 + 0x1c) & 0xf7 | bVar1 & 8;
  return;
}

