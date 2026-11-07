// 0x08003908 FUN_08003908 size=18

void FUN_08003908(uint param_1)

{
  *(uint *)(DAT_0800391c + 4) = *(uint *)(DAT_0800391c + 4) & 0xfffffffc | param_1 & 3;
  return;
}

