// 0x08004a14 FUN_08004a14 size=134

void FUN_08004a14(void)

{
  int iVar1;
  code *UNRECOVERED_JUMPTABLE;
  int iVar2;
  undefined4 *puVar3;
  
  iVar1 = DAT_08004a40;
  puVar3 = (undefined4 *)((int)&DAT_08004a40 + DAT_08004a40);
  iVar2 = DAT_08004a40 + 0x8004a3f;
  if (puVar3 == (undefined4 *)((int)&DAT_08004a40 + DAT_08004a44)) {
    FUN_08004ad8();
  }
  UNRECOVERED_JUMPTABLE = *(code **)(iVar1 + 0x8004a4c);
  if (((uint)UNRECOVERED_JUMPTABLE & 1) != 0) {
    UNRECOVERED_JUMPTABLE = (code *)(iVar2 - (int)UNRECOVERED_JUMPTABLE);
  }
                    /* WARNING: Could not recover jumptable at 0x08004a3e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)
            (*puVar3,*(undefined4 *)((int)&DAT_08004a44 + iVar1),*(undefined4 *)(iVar1 + 0x8004a48))
  ;
  return;
}

