// 0x08032d60 FUN_08032d60 size=98

uint FUN_08032d60(undefined1 param_1)

{
  ushort *puVar1;
  int *piVar2;
  uint uVar3;
  int iVar4;
  
  puVar1 = DAT_08032dc8;
  piVar2 = DAT_08032dc4;
  iVar4 = *DAT_08032dc4;
  if (iVar4 == 0) {
    puVar1[2] = 0;
    puVar1[3] = 0;
    *puVar1 = 0x6f;
  }
  uVar3 = (uint)*puVar1;
  if ((uVar3 + 1 & 0xc000) == 0) {
    if (*(uint *)(puVar1 + 2) < uVar3 - 0x6e) {
      uVar3 = uVar3 - 0x4f & 0xffffffe0;
      if (uVar3 == 0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      iVar4 = FUN_0802d72c(iVar4,uVar3);
      if (iVar4 == 0) {
        return 0;
      }
      *piVar2 = iVar4;
      *(uint *)(puVar1 + 2) = uVar3;
    }
    uVar3 = (uint)(ushort)(*puVar1 + 1);
    *puVar1 = *puVar1 + 1;
    iVar4 = *piVar2;
    if (iVar4 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    *(undefined1 *)(iVar4 + uVar3 + -0x70) = param_1;
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}

