// 0x08022e50 FUN_08022e50 size=100

void FUN_08022e50(uint *param_1,ushort *param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  uVar1 = 0;
  for (uVar2 = (uint)*param_2; uVar2 != 0; uVar2 = uVar2 >> 1) {
    if ((uVar2 & 1) != 0) {
      uVar3 = uVar1 << 1;
      uVar4 = 3 << (uVar3 & 0xff);
      *param_1 = *param_1 & ~uVar4;
      *param_1 = (uint)(byte)param_2[3] << (uVar3 & 0xff) | *param_1;
      param_1[1] = param_1[1] & ~(1 << (uVar1 & 0xff));
      param_1[1] = param_1[1] | (uint)(byte)param_2[2] << (uVar1 & 0xff);
      param_1[2] = param_1[2] & ~uVar4;
      param_1[2] = (uint)*(byte *)((int)param_2 + 7) << (uVar3 & 0xff) | param_1[2];
      param_1[3] = param_1[3] & ~uVar4;
      param_1[3] = (uint)*(byte *)((int)param_2 + 5) << (uVar3 & 0xff) | param_1[3];
    }
    uVar1 = uVar1 + 1 & 0xffff;
  }
  return;
}

