// 0x0803a976 FUN_0803a976 size=72

undefined4 FUN_0803a976(int param_1)

{
  if ((*(char *)(param_1 + 0x2e4) == '\x02') || (*(char *)(param_1 + 0x2e4) == '\x03')) {
    if (*(short *)(param_1 + 0x2ce) == 1) {
      *(undefined1 *)(param_1 + 0x2e7) = 0;
      *(uint *)(param_1 + 0x2ec) = *(uint *)(param_1 + 0x2ec) & 0xfffffffd;
      (**(code **)(*(int *)(param_1 + 4) + 8))(param_1);
      FUN_0803acbc(param_1);
    }
  }
  else {
    FUN_0803accc(param_1);
  }
  return 0;
}

