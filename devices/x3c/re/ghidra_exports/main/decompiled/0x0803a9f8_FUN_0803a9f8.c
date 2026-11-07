// 0x0803a9f8 FUN_0803a9f8 size=176

void FUN_0803a9f8(int param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  code *UNRECOVERED_JUMPTABLE;
  
  iVar2 = param_1 + (param_2 & 0x7f) * 0x2c;
  if (param_2 == 0) {
    if (*(char *)(param_1 + 0x2e0) == '\x02') {
      uVar1 = *(uint *)(iVar2 + 0x14);
      if (uVar1 < *(uint *)(iVar2 + 0x30)) {
        uVar1 = *(uint *)(iVar2 + 0x30) - uVar1;
        *(uint *)(iVar2 + 0x30) = uVar1;
        if (*(uint *)(iVar2 + 0x14) <= uVar1) {
          uVar1 = *(uint *)(iVar2 + 0x14);
        }
        FUN_0803afc8(param_1,0,*(undefined4 *)(iVar2 + 0x20),uVar1 & 0xffff);
        return;
      }
      if (((*(uint *)(iVar2 + 0x2c) == uVar1) && (uVar1 <= *(uint *)(iVar2 + 0x34))) &&
         (*(uint *)(iVar2 + 0x34) < (uint)*(ushort *)(param_1 + 0x2e2))) {
        *(undefined4 *)(iVar2 + 0x2c) = 0;
        FUN_0803afc8(param_1,0,0);
        FUN_0803af24(param_1,0,0);
        return;
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*(int *)(param_1 + 4) + 0xc);
      if ((UNRECOVERED_JUMPTABLE != (code *)0x0) && (*(char *)(param_1 + 0x2e4) == '\x03')) {
        (*UNRECOVERED_JUMPTABLE)(param_1);
      }
      *(undefined1 *)(param_1 + 0x2e0) = 5;
      FUN_0803af24(param_1,0,0);
      return;
    }
  }
  else {
    UNRECOVERED_JUMPTABLE = *(code **)(*(int *)(param_1 + 4) + 0x14);
    if ((UNRECOVERED_JUMPTABLE != (code *)0x0) && (*(char *)(param_1 + 0x2e4) == '\x03')) {
                    /* WARNING: Could not recover jumptable at 0x0803aa28. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*UNRECOVERED_JUMPTABLE)(param_1);
      return;
    }
  }
  return;
}

