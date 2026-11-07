// 0x0803bd3c FUN_0803bd3c size=80

void FUN_0803bd3c(byte *param_1)

{
  uint *puVar1;
  
  *(uint *)((uint)*param_1 * 8 + -0x60000000) =
       (uint)(param_1[1] | param_1[2]) | (uint)param_1[3] | (uint)*(ushort *)(param_1 + 4) |
       (uint)*(ushort *)(param_1 + 6) | (uint)*(ushort *)(param_1 + 8) |
       (uint)*(ushort *)(param_1 + 10) | (uint)*(ushort *)(param_1 + 0xc) |
       (uint)*(ushort *)(param_1 + 0xe) | (uint)*(ushort *)(param_1 + 0x10) |
       (uint)*(ushort *)(param_1 + 0x12) | *(uint *)(param_1 + 0x14);
  if (param_1[2] == 8) {
    puVar1 = (uint *)((uint)*param_1 * 8 + -0x60000000);
    *puVar1 = *puVar1 | 0x40;
  }
  return;
}

