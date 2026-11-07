// 0x0803b2ae FUN_0803b2ae size=54

undefined4 FUN_0803b2ae(int param_1)

{
  if ((*(char *)(param_1 + 0x2e4) == '\x02') || (*(char *)(param_1 + 0x2e4) == '\x03')) {
    if (*(char *)(param_1 + 0x2e7) != '\0') {
      *(uint *)(param_1 + 0x2ec) = *(uint *)(param_1 + 0x2ec) | 2;
    }
    FUN_0803aca4(param_1,param_1 + 0x2ec,2);
  }
  else {
    FUN_0803accc();
  }
  return 0;
}

