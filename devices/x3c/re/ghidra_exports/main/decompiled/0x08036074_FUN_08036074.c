// 0x08036074 FUN_08036074 size=12

void FUN_08036074(uint param_1)

{
  *DAT_08036080 = *DAT_08036080 & 0xfffffffe | param_1 & 1;
  return;
}

