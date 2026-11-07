// 0x0802b080 FUN_0802b080 size=12

char * FUN_0802b080(int param_1)

{
  char *pcVar1;
  
  pcVar1 = (char *)(param_1 + 1);
  if (*pcVar1 == ':') {
    pcVar1 = (char *)(param_1 + 2);
  }
  return pcVar1;
}

