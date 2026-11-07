// 0x08023a28 FUN_08023a28 size=42

void FUN_08023a28(undefined4 param_1,int param_2,uint param_3)

{
  undefined1 uVar1;
  uint uVar2;
  
  FUN_08023a7c();
  FUN_0802377c(param_1);
  for (uVar2 = 0; uVar2 < param_3; uVar2 = uVar2 + 1) {
    uVar1 = FUN_0802354c();
    *(undefined1 *)(param_2 + uVar2) = uVar1;
  }
  FUN_08022e38(DAT_08023abc,0x4000);
  return;
}

