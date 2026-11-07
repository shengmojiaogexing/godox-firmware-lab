// 0x0803ba5c FUN_0803ba5c size=12

void FUN_0803ba5c(uint param_1)

{
  *(uint *)(DAT_0803ba68 + 4) = *(uint *)(DAT_0803ba68 + 4) & 0xfffffff8 | param_1 & 7;
  return;
}

