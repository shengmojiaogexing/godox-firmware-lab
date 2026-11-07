// 0x08019848 FUN_08019848 size=8

uint FUN_08019848(int param_1)

{
  return *(uint *)(param_1 + 4) & 0xfffffffc;
}

