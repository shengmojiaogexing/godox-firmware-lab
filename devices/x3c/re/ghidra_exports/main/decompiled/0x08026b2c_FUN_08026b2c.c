// 0x08026b2c FUN_08026b2c size=84

uint FUN_08026b2c(uint param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  
  uVar2 = 0;
  while( true ) {
    iVar3 = DAT_08026b80 + uVar2 * 10;
    uVar1 = (uint)*(ushort *)(iVar3 + 2);
    if (uVar1 == 0) {
      return 0xffffffff;
    }
    if (*(byte *)(DAT_08026b80 + uVar2 * 10) + 0x622 == param_1) break;
    if ((((uVar1 == param_1) || ((int)*(char *)(iVar3 + 4) + uVar1 == param_1)) ||
        ((int)*(char *)(iVar3 + 5) + uVar1 == param_1)) ||
       (uVar1 + (int)*(char *)(iVar3 + 6) == param_1)) {
      return uVar2;
    }
    uVar2 = uVar2 + 1 & 0xff;
  }
  return uVar2;
}

