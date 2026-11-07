// 0x0802d624 FUN_0802d624 size=152

int FUN_0802d624(uint param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  
  iVar1 = DAT_0802d6bc;
  if (param_1 == 0) {
    iVar3 = 0;
  }
  else {
    iVar3 = -1;
    uVar2 = 0;
    do {
      iVar4 = DAT_0802d6bc + uVar2 * 8;
      if ((*(byte *)(iVar4 + 6) & 1) == 0) {
        uVar5 = (uint)*(ushort *)(iVar4 + 4);
        if (param_1 <= uVar5) {
          if (uVar5 == param_1) {
            *(undefined1 *)(DAT_0802d6bc + uVar2 * 8 + 6) = 1;
            return *(int *)(iVar1 + uVar2 * 8);
          }
          if (iVar3 < 0) {
            iVar3 = (int)(char)uVar2;
          }
          else if (uVar5 < *(ushort *)(DAT_0802d6bc + iVar3 * 8 + 4)) {
            iVar3 = (int)(char)uVar2;
          }
        }
      }
      uVar2 = uVar2 + 1 & 0xff;
    } while (uVar2 < 0x10);
    if (iVar3 < 0) {
      uVar2 = 0;
      while ((*(byte *)(DAT_0802d6bc + uVar2 * 8 + 6) & 1) != 0) {
        uVar2 = uVar2 + 1 & 0xff;
        if (0xf < uVar2) {
          do {
                    /* WARNING: Do nothing block with infinite loop */
          } while( true );
        }
      }
      iVar3 = FUN_0802d72c(*(undefined4 *)(DAT_0802d6bc + uVar2 * 8),param_1);
      if (iVar3 == 0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      iVar4 = iVar1 + uVar2 * 8;
      *(undefined1 *)(iVar4 + 6) = 1;
      *(short *)(iVar4 + 4) = (short)param_1;
      *(int *)(iVar1 + uVar2 * 8) = iVar3;
    }
    else {
      *(undefined1 *)(DAT_0802d6bc + iVar3 * 8 + 6) = 1;
      iVar3 = *(int *)(iVar1 + iVar3 * 8);
    }
  }
  return iVar3;
}

