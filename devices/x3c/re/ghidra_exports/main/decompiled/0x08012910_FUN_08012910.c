// 0x08012910 FUN_08012910 size=106

void FUN_08012910(void)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = *(char *)(DAT_0801297c + 0xb);
  if (cVar1 == '\0') {
    *(undefined1 *)(DAT_0801297c + 0xb) = 0;
  }
  else {
    *(char *)(DAT_0801297c + 0xb) = cVar1 + -1;
    uVar2 = DAT_08012980;
    if (cVar1 == '\x01') {
      FUN_08022e38(DAT_08012980,0x4000);
      FUN_08022e34(uVar2,1);
      FUN_08022e38(uVar2,2);
      FUN_08023540(0x18,8);
      FUN_08023520(0x36);
      FUN_08023520(0x33);
      thunk_FUN_0801c118(1000);
      FUN_08023520(0x3b);
      FUN_08023540(6,4);
      FUN_08023520(0x36);
      return;
    }
  }
  return;
}

