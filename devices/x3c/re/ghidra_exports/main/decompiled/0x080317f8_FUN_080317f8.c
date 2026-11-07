// 0x080317f8 FUN_080317f8 size=46

void FUN_080317f8(void)

{
  char *pcVar1;
  int iVar2;
  
  pcVar1 = DAT_08031828;
  if (*DAT_08031828 == '\0') {
    *DAT_08031828 = '\x01';
    iVar2 = FUN_0802ee16();
    while( true ) {
      if (-1 < (int)((uint)*(ushort *)(iVar2 + 0x2a) << 0x1e)) break;
      *(ushort *)(iVar2 + 0x2a) = *(ushort *)(iVar2 + 0x2a) & 0xfffd;
      FUN_08025990(iVar2);
    }
    *pcVar1 = '\0';
  }
  return;
}

