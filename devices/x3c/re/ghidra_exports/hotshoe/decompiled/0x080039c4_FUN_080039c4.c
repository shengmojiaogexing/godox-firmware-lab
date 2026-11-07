// 0x080039c4 FUN_080039c4 size=46

bool FUN_080039c4(uint param_1)

{
  uint uVar1;
  
  uVar1 = param_1 >> 8;
  if (uVar1 == 0) {
    uVar1 = *DAT_080039f4;
  }
  else if (uVar1 == 1) {
    uVar1 = DAT_080039f4[8];
  }
  else if (uVar1 == 2) {
    uVar1 = DAT_080039f4[9];
  }
  else if (uVar1 == 3) {
    uVar1 = DAT_080039f4[0xd];
  }
  return (uVar1 & 1 << (param_1 & 0xff)) != 0;
}

