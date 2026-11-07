// 0x080216a8 FUN_080216a8 size=90

undefined4 FUN_080216a8(undefined1 *param_1,undefined1 param_2)

{
  undefined4 uVar1;
  uint *puVar2;
  
  puVar2 = DAT_08021704;
  uVar1 = 3;
  if (param_1 + -0x8000000 < (undefined1 *)0x80000) {
    *DAT_08021704 = *DAT_08021704 | 1;
    *param_1 = param_2;
    uVar1 = FUN_08021640(0x100000);
    *puVar2 = *puVar2 & 0xfffffffe;
  }
  else if (param_1 + DAT_08021708 < (undefined1 *)0x80000) {
    puVar2 = DAT_08021704 + 0x10;
    *puVar2 = *puVar2 | 1;
    *param_1 = param_2;
    uVar1 = FUN_08021688(0x100000);
    *puVar2 = *puVar2 & 0xfffffffe;
  }
  return uVar1;
}

