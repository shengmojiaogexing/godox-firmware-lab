// 0x08031384 FUN_08031384 size=46

void FUN_08031384(int param_1,uint param_2)

{
  byte bVar1;
  
  FUN_0802dee0(param_1);
  bVar1 = *(byte *)(*(int *)(param_1 + 8) + 0x20);
  if ((bVar1 & 3) == param_2) {
    return;
  }
  *(byte *)(*(int *)(param_1 + 8) + 0x20) = bVar1 & 0xfc | (byte)param_2 & 3;
  FUN_080302d0(param_1);
  return;
}

