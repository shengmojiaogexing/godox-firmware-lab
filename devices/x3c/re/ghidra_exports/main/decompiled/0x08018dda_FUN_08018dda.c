// 0x08018dda FUN_08018dda size=44

uint FUN_08018dda(int param_1,int param_2)

{
  uint uVar1;
  
  uVar1 = 0;
  if (param_2 == 0) {
    return *(uint *)(param_1 + 0x3c) & 0xffff;
  }
  if (param_2 == 1) {
    return *(uint *)(param_1 + 0x40) & 0xffff;
  }
  if (param_2 == 2) {
    return *(uint *)(param_1 + 0x44) & 0xffff;
  }
  if (param_2 == 3) {
    uVar1 = *(uint *)(param_1 + 0x48) & 0xffff;
  }
  return uVar1;
}

