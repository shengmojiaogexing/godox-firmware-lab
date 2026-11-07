// 0x0802ad00 FUN_0802ad00 size=34

undefined4 FUN_0802ad00(int param_1)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = *(char *)(param_1 + 8);
  if ((((cVar1 == '\x15') || (cVar1 == '\x14')) || (cVar1 == '\x16')) ||
     (((cVar1 == '\x18' || (cVar1 == '\x17')) || (cVar1 == '\x19')))) {
    uVar2 = FUN_0802ad6c();
    return uVar2;
  }
  return 0;
}

