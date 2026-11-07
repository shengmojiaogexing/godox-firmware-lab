// 0x08018a20 FUN_08018a20 size=58

undefined4 FUN_08018a20(char *param_1,int param_2)

{
  char cVar1;
  undefined4 uVar2;
  
  uVar2 = 0;
  if (param_2 == 0x23) {
    cVar1 = *param_1;
    if (cVar1 == '\0') {
      uVar2 = 1;
      *param_1 = '\x01';
    }
    else if (cVar1 == '\x01') {
      *param_1 = '\0';
    }
    else if (cVar1 == '\x02') {
      *param_1 = '\0';
      uVar2 = 1;
    }
  }
  if (*param_1 == '\x01') {
    if (param_2 == 0x20) {
      *param_1 = '\x02';
    }
    uVar2 = 1;
  }
  return uVar2;
}

