// 0x0803bd8c FUN_0803bd8c size=124

void FUN_0803bd8c(byte *param_1,byte *param_2)

{
  *(uint *)((uint)*param_1 * 8 + -0x5ffffffc) =
       *(uint *)(param_1 + 4) | *(int *)(param_1 + 8) << 4 |
       *(int *)(param_1 + 0xc) << 8 | (uint)*(ushort *)(param_1 + 0x10) << 0x10 |
       (uint)*(ushort *)(param_1 + 0x14) << 0x14 | (uint)param_1[0x18] << 0x18 |
       *(uint *)(param_1 + 0x1c);
  if (*(short *)(param_1 + 2) == 0x4000) {
    *(uint *)((uint)*param_2 * 8 + -0x5ffffefc) =
         *(uint *)(param_2 + 4) | *(int *)(param_2 + 8) << 4 |
         *(int *)(param_2 + 0xc) << 8 | (uint)*(ushort *)(param_2 + 0x10) << 0x10 |
         (uint)*(ushort *)(param_2 + 0x14) << 0x14 | (uint)param_2[0x18] << 0x18 |
         *(uint *)(param_2 + 0x1c);
    return;
  }
  *(undefined4 *)((uint)*param_2 * 8 + -0x5ffffefc) = 0xfffffff;
  return;
}

