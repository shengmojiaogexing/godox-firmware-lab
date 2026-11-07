// 0x08023a52 FUN_08023a52 size=52

void FUN_08023a52(undefined4 param_1,int param_2,uint param_3)

{
  uint uVar1;
  
  FUN_08023a7c();
  FUN_0802377c(param_1);
  for (uVar1 = 0; uVar1 < param_3; uVar1 = uVar1 + 1) {
    FUN_0802377c(*(undefined1 *)(param_2 + uVar1));
  }
  FUN_08022e38(DAT_08023abc,0x4000);
  return;
}

