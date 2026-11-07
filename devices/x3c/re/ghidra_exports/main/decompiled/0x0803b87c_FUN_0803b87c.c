// 0x0803b87c FUN_0803b87c size=50

void FUN_0803b87c(char *param_1,char *param_2)

{
  uint uVar1;
  char cVar2;
  uint uVar3;
  
  cVar2 = '\0';
  uVar1 = 2;
  for (; *param_1 != '\0'; param_1 = param_1 + 1) {
    cVar2 = cVar2 + '\x01';
    uVar3 = uVar1 + 1 & 0xffff;
    param_2[uVar1] = *param_1;
    uVar1 = uVar3 + 1 & 0xffff;
    param_2[uVar3] = '\0';
  }
  *param_2 = cVar2 * '\x02' + '\x02';
  param_2[1] = '\x03';
  return;
}

