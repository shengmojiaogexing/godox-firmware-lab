// 0x08003af4 FUN_08003af4 size=128

void FUN_08003af4(int param_1,byte *param_2)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  
  iVar2 = DAT_08003b74;
  if (param_1 == 1) {
    if (*(int *)(DAT_08003b74 + 8) << 0x19 < 0) {
      FUN_08003a60(*param_2);
    }
    else {
      param_2[3] = 0;
    }
    FUN_08003a70(param_2[9]);
    uVar7 = (uint)param_2[2] | (uint)*param_2 << 0x10 |
            (uint)param_2[1] << 8 | (uint)param_2[3] << 0x16;
    uVar5 = (uint)param_2[9];
  }
  else {
    if (-1 < *(int *)(DAT_08003b74 + 8) << 0x19) {
      param_2[3] = 0;
    }
    iVar3 = FUN_08003a70(*param_2);
    iVar4 = FUN_08003a70(param_2[1]);
    uVar5 = FUN_08003a70(param_2[2]);
    uVar7 = iVar3 << 0x10 | iVar4 << 8 | uVar5 | (uint)param_2[3] << 0x16;
    uVar5 = FUN_08003a70(param_2[9]);
  }
  bVar1 = param_2[8];
  uVar6 = *(uint *)(param_2 + 4);
  FUN_08003c88();
  *(uint *)(iVar2 + 0x1c) = uVar7 | uVar5 << 0x18 | (uint)bVar1 << 0x1e | uVar6;
  *(undefined4 *)(iVar2 + 0x24) = 0xff;
  return;
}

