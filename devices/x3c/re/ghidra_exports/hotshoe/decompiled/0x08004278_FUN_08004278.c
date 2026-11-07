// 0x08004278 FUN_08004278 size=74

void FUN_08004278(void)

{
  byte bVar1;
  byte *pbVar2;
  int iVar3;
  
  if ((((*DAT_080042c4 == '\0') && (iVar3 = FUN_08002fc0(&DAT_48000000,8), iVar3 == 0)) &&
      (iVar3 = FUN_08002fb6(&DAT_48000000,0x80), iVar3 == 0)) &&
     (iVar3 = FUN_08002fb6(&DAT_48000000,4), pbVar2 = DAT_080042c8, iVar3 == 0)) {
    bVar1 = *DAT_080042c8;
    *DAT_080042c8 = bVar1 + 1;
    if (0x13 < (byte)(bVar1 + 1)) {
      *pbVar2 = 0;
      FUN_0800431c();
    }
  }
  return;
}

