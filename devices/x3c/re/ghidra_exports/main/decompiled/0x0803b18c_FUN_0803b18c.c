// 0x0803b18c FUN_0803b18c size=70

undefined4 FUN_0803b18c(int param_1)

{
  if (*(short *)(param_1 + 0x2d2) == 1) {
    if (*(char *)(param_1 + 0x2e4) == '\x02') {
      *(undefined1 *)(param_1 + 0x2e8) = 0;
      FUN_0803aca4(param_1,param_1 + 0x2e8,1);
    }
    else if (*(char *)(param_1 + 0x2e4) == '\x03') {
      FUN_0803aca4(param_1,param_1 + 0x2e9,1);
    }
    else {
      FUN_0803accc();
    }
  }
  else {
    FUN_0803accc();
  }
  return 0;
}

