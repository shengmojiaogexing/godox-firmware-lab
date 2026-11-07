// 0x08027e9c FUN_08027e9c size=54

uint FUN_08027e9c(undefined2 param_1,undefined2 param_2,int param_3)

{
  uint uVar1;
  
  uVar1 = CONCAT22(param_2,param_2) & DAT_08027ed4;
  uVar1 = uVar1 + ((param_3 + 4U >> 3) * ((CONCAT22(param_1,param_1) & DAT_08027ed4) - uVar1) >> 5)
          & DAT_08027ed4;
  return uVar1 & 0xffff | uVar1 >> 0x10;
}

