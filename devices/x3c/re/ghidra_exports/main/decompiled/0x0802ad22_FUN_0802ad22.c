// 0x0802ad22 FUN_0802ad22 size=74

undefined4 FUN_0802ad22(int param_1)

{
  char cVar1;
  
  cVar1 = *(char *)(param_1 + 8);
  if (((((cVar1 == '\x01') || (cVar1 == '\x02')) || (cVar1 == '\x03')) ||
      (((cVar1 == '\x04' || (cVar1 == '\x05')) ||
       ((cVar1 == '\x06' || ((cVar1 == '\a' || (cVar1 == '\b')))))))) ||
     ((cVar1 == '\t' ||
      (((((cVar1 == '\n' || (cVar1 == '\v')) || (cVar1 == '\f')) ||
        ((cVar1 == '\r' || (cVar1 == '\x0e')))) || ((cVar1 == '\x0f' || (cVar1 == '\x10')))))))) {
    return *(undefined4 *)(param_1 + 0x10);
  }
  return 0;
}

