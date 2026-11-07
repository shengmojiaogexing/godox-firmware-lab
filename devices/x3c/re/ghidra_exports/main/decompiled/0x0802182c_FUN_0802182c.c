// 0x0802182c FUN_0802182c size=110

undefined4 FUN_0802182c(uint param_1)

{
  uint *puVar1;
  undefined4 uVar2;
  uint *puVar3;
  
  puVar1 = DAT_0802189c;
  uVar2 = 3;
  if (param_1 + 0xf8000000 < 0x80000) {
    *DAT_0802189c = *DAT_0802189c | 2;
    puVar1[1] = param_1;
    *puVar1 = *puVar1 | 0x40;
    uVar2 = FUN_08021640(0x80000000);
    *puVar1 = *puVar1 & 0xfffffffd;
  }
  else if (DAT_080218a0 + param_1 < 0x80000) {
    puVar3 = DAT_0802189c + 0x10;
    *puVar3 = *puVar3 | 2;
    puVar1[0x11] = param_1;
    *puVar3 = *puVar3 | 0x40;
    uVar2 = FUN_08021688(0x80000000);
    *puVar3 = *puVar3 & 0xfffffffd;
  }
  return uVar2;
}

