// 0x08004db0 FUN_08004db0 size=72

void FUN_08004db0(uint *param_1,uint *param_2)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  uint uVar4;
  
  if ((((uint)param_1 | (uint)param_2) & 3) == 0) {
    while( true ) {
      uVar4 = *param_2;
      param_2 = param_2 + 1;
      if ((uVar4 + 0xfefefeff & ~uVar4 & 0x80808080) != 0) break;
      *param_1 = uVar4;
      param_1 = param_1 + 1;
    }
    while( true ) {
      *(char *)param_1 = (char)uVar4;
      if ((uVar4 & 0xff) == 0) break;
      uVar4 = uVar4 >> 8;
      param_1 = (uint *)((int)param_1 + 1);
    }
  }
  else {
    do {
      pcVar2 = (char *)((int)param_2 + 1);
      uVar4 = *param_2;
      pcVar3 = (char *)((int)param_1 + 1);
      *(char *)param_1 = (char)uVar4;
      if ((char)uVar4 == '\0') {
        return;
      }
      param_2 = (uint *)((int)param_2 + 2);
      cVar1 = *pcVar2;
      param_1 = (uint *)((int)param_1 + 2);
      *pcVar3 = cVar1;
    } while (cVar1 != '\0');
  }
  return;
}

