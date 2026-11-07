// 0x08035c78 FUN_08035c78 size=116

void FUN_08035c78(uint param_1,uint param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = (*DAT_08035cec & 0x7ff) >> 8;
  uVar2 = 7 - uVar1;
  if (4 < uVar2) {
    uVar2 = 4;
  }
  if (uVar1 + 4 < 7) {
    uVar1 = 0;
  }
  else {
    uVar1 = uVar1 - 3;
  }
  uVar1 = ((1 << (uVar2 & 0xff)) - 1U & param_2) << (uVar1 & 0xff) |
          (1 << (uVar1 & 0xff)) - 1U & param_3;
  if ((int)param_1 < 0) {
    *(char *)((param_1 & 0xf) + 0xe000ed14) = (char)(uVar1 << 4);
  }
  else {
    *(char *)(param_1 + 0xe000e400) = (char)(uVar1 << 4);
    *(int *)(&DAT_e000e100 + (param_1 >> 5) * 4) = 1 << (param_1 & 0x1f);
  }
  return;
}

