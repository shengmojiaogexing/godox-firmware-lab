// 0x08021980 FUN_08021980 size=90

undefined4 FUN_08021980(undefined4 *param_1,undefined4 param_2)

{
  undefined4 uVar1;
  uint *puVar2;
  
  puVar2 = DAT_080219dc;
  uVar1 = 3;
  if (param_1 + -0x2000000 < (undefined4 *)0x80000) {
    *DAT_080219dc = *DAT_080219dc | 1;
    *param_1 = param_2;
    uVar1 = FUN_08021640(0x100000);
    *puVar2 = *puVar2 & 0xfffffffe;
  }
  else if ((uint)(DAT_080219e0 + (int)param_1) < 0x80000) {
    puVar2 = DAT_080219dc + 0x10;
    *puVar2 = *puVar2 | 1;
    *param_1 = param_2;
    uVar1 = FUN_08021688(0x100000);
    *puVar2 = *puVar2 & 0xfffffffe;
  }
  return uVar1;
}

