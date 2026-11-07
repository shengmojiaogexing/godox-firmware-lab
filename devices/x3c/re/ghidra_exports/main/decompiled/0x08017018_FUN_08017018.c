// 0x08017018 FUN_08017018 size=104

void FUN_08017018(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar1 = DAT_08017080;
  if (*(char *)(DAT_08017080 + 2) == '\0') {
    *DAT_08017084 = 1;
    FUN_08012540();
    iVar2 = DAT_08017088;
    if (*(char *)(iVar1 + 3) == '\x02') {
      uVar3 = 0;
      do {
        if (*(char *)(iVar2 + uVar3 * 6) == '\x01') {
          FUN_0801184c(uVar3 + 10 & 0xff,0xb9,0x30);
        }
        uVar3 = uVar3 + 1 & 0xff;
      } while (uVar3 < 5);
    }
  }
  FUN_0801c118(1000);
  FUN_080118b0(9);
  FUN_0801c118(500);
  if ((int)((uint)*(byte *)(iVar1 + 4) << 0x18) < 0) {
    FUN_080118b0(10);
    return;
  }
  return;
}

