// 0x0802c05c FUN_0802c05c size=90

int FUN_0802c05c(int param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 *puVar2;
  
  FUN_0802d8d6(param_2,4);
  if (param_1 == 0) {
    iVar1 = 0;
  }
  else {
    iVar1 = FUN_0802c55a(param_1);
    if ((iVar1 == 0) && (*(int *)(param_1 + 8) == 0)) {
      iVar1 = 0;
    }
    else {
      iVar1 = 0;
      puVar2 = (undefined4 *)FUN_08017f30(DAT_0802c0b8);
      while ((puVar2 != (undefined4 *)0x0 &&
             (((code *)*puVar2 == (code *)0x0 ||
              (iVar1 = (*(code *)*puVar2)(puVar2,param_1,param_2), iVar1 != 1))))) {
        puVar2 = (undefined4 *)FUN_08017f5e(DAT_0802c0b8,puVar2);
      }
    }
  }
  return iVar1;
}

