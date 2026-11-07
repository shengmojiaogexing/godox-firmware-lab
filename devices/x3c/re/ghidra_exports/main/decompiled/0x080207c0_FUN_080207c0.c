// 0x080207c0 FUN_080207c0 size=64

undefined4 FUN_080207c0(int param_1)

{
  int iVar1;
  
  if ((int)((uint)*(byte *)(param_1 + 0x18) << 0x1d) < 0) {
    return 0;
  }
  if ((*(char *)(param_1 + 8) == '#') || (*(char *)(param_1 + 8) == '$')) {
    return 1;
  }
  iVar1 = FUN_0802fc9e(*(undefined4 *)(param_1 + 4),0x4000);
  if (iVar1 == 0) {
    return 0;
  }
  switch(*(undefined1 *)(param_1 + 8)) {
  case 0x11:
  case 0x12:
  case 0x13:
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
  case 0x1a:
  case 0x1b:
  case 0x1e:
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x24:
  case 0x29:
  case 0x2a:
  case 0x2c:
    return 0;
  default:
    return 1;
  }
}

