// 0x08025524 FUN_08025524 size=26

undefined4 FUN_08025524(int param_1,uint *param_2)

{
  if ((*(uint *)(param_1 + 4) & 0x3fffffff) == *param_2) {
    FUN_08021e54();
    return 1;
  }
  return 0;
}

