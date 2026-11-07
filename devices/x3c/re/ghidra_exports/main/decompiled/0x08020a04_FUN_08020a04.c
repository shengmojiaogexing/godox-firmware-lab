// 0x08020a04 FUN_08020a04 size=22

void FUN_08020a04(uint param_1,int param_2)

{
  if (param_2 == 1) {
    *DAT_08020a1c = *DAT_08020a1c | param_1;
    return;
  }
  *DAT_08020a1c = *DAT_08020a1c & ~param_1;
  return;
}

