// 0x0803a74c FUN_0803a74c size=62

void FUN_0803a74c(int param_1,int param_2)

{
  if (param_2 == 0) {
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xdfffffff;
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | 0x40000000;
  }
  if (param_2 == 1) {
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xbfffffff;
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | 0x20000000;
  }
  if (param_2 == 2) {
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xbfffffff;
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xdfffffff;
  }
  return;
}

