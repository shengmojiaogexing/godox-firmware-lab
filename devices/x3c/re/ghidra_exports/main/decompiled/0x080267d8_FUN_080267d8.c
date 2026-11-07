// 0x080267d8 FUN_080267d8 size=154

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_080267d8(undefined4 param_1)

{
  undefined2 uVar1;
  undefined4 uVar2;
  
  uVar1 = FUN_08027fe0();
  uVar2 = DAT_08026878;
  FUN_0802553e(param_1,2,0,0x79,DAT_08026878,uVar1,&DAT_08026874);
  if (*_DAT_0802687c == '\0') {
    uVar1 = FUN_08027fe0();
    uVar2 = DAT_080268b4;
    FUN_0802553e(param_1,2,0,0xeb,DAT_080268b4,uVar1,&DAT_080268a0);
    return (ulonglong)CONCAT24(uVar1,uVar2);
  }
  if (*_DAT_0802687c == '\x01') {
    uVar1 = FUN_08027fe0();
    uVar2 = DAT_08026890;
    FUN_0802553e(param_1,2,0,0xeb,DAT_08026890,uVar1,s_Press_for_2s_0802687f + 1);
    uVar1 = FUN_08027fe0();
    FUN_0802553e(param_1,2,0,0x109,uVar2,uVar1,s_to_unlock_08026894);
  }
  return (ulonglong)CONCAT24(uVar1,uVar2);
}

