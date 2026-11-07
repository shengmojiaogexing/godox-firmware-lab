// 0x080398c8 FUN_080398c8 size=32

undefined4 FUN_080398c8(int param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  switch(param_2) {
  case 0:
    uVar1 = *(undefined4 *)(param_1 + 0x34);
    break;
  case 2:
    return *(undefined4 *)(param_1 + 0x38);
  case 4:
    return *(undefined4 *)(param_1 + 0x3c);
  case 6:
    return *(undefined4 *)(param_1 + 0x40);
  case 7:
    return *(undefined4 *)(param_1 + 0x74);
  }
  return uVar1;
}

