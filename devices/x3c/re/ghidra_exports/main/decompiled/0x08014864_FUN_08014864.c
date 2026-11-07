// 0x08014864 FUN_08014864 size=66

void FUN_08014864(int param_1)

{
  ulonglong uVar1;
  
  uVar1 = 0xc368 / (ulonglong)(param_1 + 1);
  FUN_08023540(4,(uint)uVar1 >> 8);
  FUN_08023540(5,(char)uVar1);
  thunk_FUN_0801c118(0x14);
  FUN_08023520(0x33);
  thunk_FUN_0801c118(1000);
  FUN_08023520(0x36);
  thunk_FUN_0801c118(700);
  return;
}

