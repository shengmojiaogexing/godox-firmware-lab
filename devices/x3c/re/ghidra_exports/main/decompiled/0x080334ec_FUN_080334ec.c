// 0x080334ec FUN_080334ec size=14

void FUN_080334ec(int param_1)

{
  undefined1 *puVar1;
  
  puVar1 = DAT_080334fc;
  *DAT_080334fc = 0;
  *(int *)(puVar1 + 4) = param_1 + *(int *)(puVar1 + 4);
  return;
}

