// 0x080101cc FUN_080101cc size=46

void FUN_080101cc(int param_1)

{
  undefined1 *puVar1;
  undefined1 uVar2;
  
  puVar1 = DAT_080101fc;
  if (*(char *)(param_1 + 8) == '\x1c') {
    uVar2 = FUN_08031e48();
    uVar2 = FUN_0800ad60(uVar2);
    *puVar1 = uVar2;
    return;
  }
  if (*(char *)(param_1 + 8) == '\r') {
    uVar2 = FUN_0800ad60(*(undefined1 *)(*(int *)(param_1 + 4) + 0x2e));
    *puVar1 = uVar2;
  }
  return;
}

