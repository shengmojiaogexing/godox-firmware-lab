// 0x0801915c FUN_0801915c size=48

void FUN_0801915c(int param_1)

{
  undefined1 *puVar1;
  int iVar2;
  
  if ((*(char *)(param_1 + 8) == '\x16') &&
     (iVar2 = FUN_080271ac(*(undefined4 *)(param_1 + 4)), puVar1 = DAT_0801918c, iVar2 == 100)) {
    DAT_0801918c[0xf] = 1;
    FUN_0802e2d2(*(undefined4 *)(puVar1 + 0x90),1);
    FUN_08037ae8(*puVar1);
    return;
  }
  return;
}

