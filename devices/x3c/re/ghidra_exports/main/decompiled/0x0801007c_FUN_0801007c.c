// 0x0801007c FUN_0801007c size=166

uint FUN_0801007c(uint param_1,uint param_2,uint param_3)

{
  uint uVar1;
  int iVar2;
  
  if (param_2 == 1) {
    iVar2 = 0;
  }
  else if (param_2 == 2) {
    iVar2 = 1;
  }
  else if (param_2 == 3) {
    iVar2 = 2;
  }
  else if (param_2 == 4) {
    iVar2 = 3;
  }
  else if (param_2 == 5) {
    iVar2 = 4;
  }
  else if (param_2 < 8) {
    iVar2 = 5;
  }
  else if (param_2 < 10) {
    iVar2 = 6;
  }
  else if (param_2 == 10) {
    iVar2 = 7;
  }
  else if (param_2 == 0xb) {
    iVar2 = 8;
  }
  else if (param_2 < 0xf) {
    iVar2 = 9;
  }
  else if (param_2 < 0x14) {
    iVar2 = 10;
  }
  else if (param_2 < 0x3c) {
    iVar2 = 0xb;
  }
  else {
    iVar2 = 0xc;
  }
  switch(param_1 / 10) {
  case 2:
    uVar1 = (uint)*(byte *)(DAT_08010140 + iVar2);
    break;
  case 3:
    uVar1 = (uint)*(byte *)(DAT_0801013c + iVar2);
    break;
  case 4:
    uVar1 = (uint)*(byte *)(DAT_08010138 + iVar2);
    break;
  case 5:
    uVar1 = (uint)*(byte *)(DAT_08010134 + iVar2);
    break;
  case 6:
    uVar1 = (uint)*(byte *)(DAT_08010130 + iVar2);
    break;
  case 7:
    uVar1 = (uint)*(byte *)(DAT_0801012c + iVar2);
    break;
  case 8:
  case 9:
    uVar1 = (uint)*(byte *)(DAT_0801012c + iVar2);
    break;
  default:
    return 0;
  }
  if (param_3 <= uVar1) {
    uVar1 = param_3;
  }
  return uVar1;
}

