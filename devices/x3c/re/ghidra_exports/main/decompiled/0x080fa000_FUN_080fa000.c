// 0x080fa000 FUN_080fa000 size=16

undefined4 FUN_080fa000(int param_1,uint param_2)

{
  if ((*(uint *)(param_1 + 8) & param_2) == param_2) {
    return 1;
  }
  return 0;
}

