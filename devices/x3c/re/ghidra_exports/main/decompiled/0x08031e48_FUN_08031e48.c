// 0x08031e48 FUN_08031e48 size=32

uint FUN_08031e48(int param_1)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = (uint)*(ushort *)(param_1 + 0x2e);
  if ((*(byte *)(param_1 + 0x32) & 1) == 0) {
    return uVar2;
  }
  uVar1 = *(ushort *)(param_1 + 0x2c) / 7;
  return uVar2 - uVar1 * (uVar2 / uVar1);
}

