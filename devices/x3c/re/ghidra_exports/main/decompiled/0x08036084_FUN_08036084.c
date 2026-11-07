// 0x08036084 FUN_08036084 size=22

void FUN_08036084(uint param_1,int param_2)

{
  if (param_2 == 1) {
    *(uint *)(DAT_0803609c + 4) = *(uint *)(DAT_0803609c + 4) | param_1;
    return;
  }
  *(uint *)(DAT_0803609c + 4) = *(uint *)(DAT_0803609c + 4) & ~param_1;
  return;
}

