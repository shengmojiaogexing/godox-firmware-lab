// 0x08025990 FUN_08025990 size=108

void FUN_08025990(int param_1)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = FUN_0802ebf4(param_1);
  for (uVar2 = 0; uVar2 < uVar1; uVar2 = uVar2 + 1) {
    FUN_08025990(*(undefined4 *)(**(int **)(param_1 + 8) + uVar2 * 4));
  }
  if ((*(ushort *)(param_1 + 0x2a) & 1) != 0) {
    *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) & 0xfffe;
    FUN_08030958(param_1);
    FUN_0803075e(param_1);
    if (((uVar1 != 0) && (uVar1 = FUN_0802f8ba(param_1,0), uVar1 != 0)) &&
       (uVar1 <= *(uint *)(DAT_080259fc + 4))) {
                    /* WARNING: Could not recover jumptable at 0x080259f8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*DAT_08025a00 + (uVar1 - 1) * 8))
                (param_1,*(undefined4 *)(*DAT_08025a00 + (uVar1 - 1) * 8 + 4));
      return;
    }
  }
  return;
}

