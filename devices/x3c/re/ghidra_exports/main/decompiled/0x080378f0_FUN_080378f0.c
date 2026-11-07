// 0x080378f0 FUN_080378f0 size=202

undefined4 FUN_080378f0(void)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  int *piVar6;
  undefined4 in_r3;
  int iVar7;
  uint uVar8;
  undefined4 local_18;
  
  iVar1 = DAT_080379bc;
  *(undefined1 *)(DAT_080379bc + 0x10) = 0;
  *(undefined4 *)(iVar1 + 0x14) = 0;
  *(undefined2 *)(iVar1 + 0x18) = 0;
  if ((*(short *)(iVar1 + 0xe) == 0) &&
     (local_18 = in_r3, iVar3 = FUN_08037348(&local_18), iVar3 == 0)) {
    *(undefined4 *)(iVar1 + 4) = local_18;
  }
  iVar3 = DAT_080379c8;
  piVar2 = DAT_080379c0;
  if (((uint)DAT_080379c0 & 3) == 0) {
    uVar4 = 0xfffffff8;
    if ((((*DAT_080379c0 == DAT_080379c4) && (iVar7 = DAT_080379c0[3], iVar7 != 0)) &&
        (0x3c00 < (uint)DAT_080379c0[5])) && ((uint)DAT_080379c0[5] < 0xa000)) {
      piVar6 = DAT_080379c0 + 6;
      *(int **)(DAT_080379c8 + 8) = piVar6;
      iVar7 = (int)piVar6 + piVar2[4] + iVar7 * 4;
      *(int *)(iVar3 + 4) = iVar7;
      if ((*(char *)(iVar7 + 0x30) == '\x16') && (*(char *)(iVar7 + 0x31) == 'X')) {
        *(int *)(iVar1 + 0x14) = iVar7;
        *(short *)(iVar1 + 0x18) = (short)piVar2[5];
        uVar5 = 0;
        do {
          uVar8 = 0xffffffff;
          if ((uint)piVar2[3] <= uVar5) {
LAB_080379a4:
            if ((*(short *)(iVar1 + 0xe) == 0) && ((int)uVar8 < 0)) {
              uVar8 = 0;
            }
            if (-1 < (int)uVar8) {
              *(undefined1 *)(iVar1 + 0x10) = 1;
            }
            return 0;
          }
          if ((*(uint *)(*(int *)(iVar3 + 8) + uVar5 * 4) & 0xffffff00) ==
              (*(uint *)(iVar1 + 4) & 0xffffff00)) {
            if ((*(ushort *)(iVar1 + 0xc) < *(ushort *)(iVar7 + 0x3e)) ||
               (*(short *)(iVar1 + 0xe) == 0)) {
              uVar8 = uVar5;
            }
            goto LAB_080379a4;
          }
          uVar5 = uVar5 + 1;
        } while( true );
      }
    }
  }
  else {
    uVar4 = 0xfffffff9;
  }
  return uVar4;
}

