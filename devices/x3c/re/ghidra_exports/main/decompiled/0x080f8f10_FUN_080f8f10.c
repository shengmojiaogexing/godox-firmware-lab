// 0x080f8f10 FUN_080f8f10 size=116

void FUN_080f8f10(uint *param_1,ushort *param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar1 = 0;
  do {
    if (((uint)*param_2 & 1 << (uVar1 & 0xff)) != 0) {
      if (((char)param_2[1] == '\x01') || ((char)param_2[1] == '\x02')) {
        param_1[2] = param_1[2] & ~(3 << (uVar1 << 1 & 0xff));
        param_1[2] = (uint)(byte)param_2[2] << (uVar1 << 1 & 0xff) | param_1[2];
        param_1[1] = param_1[1] & ~((uint)*(byte *)((int)param_2 + 3) << (uVar1 & 0xff));
        param_1[1] = param_1[1] | (uint)*(byte *)((int)param_2 + 3) << (uVar1 & 0xff) & 0xffff;
      }
      uVar2 = uVar1 << 1;
      uVar3 = 3 << (uVar2 & 0xff);
      *param_1 = *param_1 & ~uVar3;
      *param_1 = (uint)(byte)param_2[1] << (uVar2 & 0xff) | *param_1;
      param_1[3] = param_1[3] & ~uVar3;
      param_1[3] = (uint)*(byte *)((int)param_2 + 5) << (uVar2 & 0xff) | param_1[3];
    }
    uVar1 = uVar1 + 1;
  } while (uVar1 < 0x10);
  return;
}

