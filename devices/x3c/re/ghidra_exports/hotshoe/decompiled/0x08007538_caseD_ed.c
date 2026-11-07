// 0x08007538 caseD_ed size=30

undefined2 switchD_0800735a::caseD_ed(void)

{
  uint uVar1;
  
  uVar1 = (uint)DAT_20000168;
  if (0x80 < uVar1) {
    return 5000;
  }
  if (uVar1 < 0x10) {
    uVar1 = 0x10;
  }
  return *(undefined2 *)((int)&DAT_08007c28 + uVar1 * 2);
}

