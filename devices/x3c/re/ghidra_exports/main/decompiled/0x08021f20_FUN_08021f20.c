// 0x08021f20 FUN_08021f20 size=16

int FUN_08021f20(int param_1)

{
  return (*(int *)(param_1 + 0xc) * 2 + 3U & 0xfffffffc) + 0x14;
}

