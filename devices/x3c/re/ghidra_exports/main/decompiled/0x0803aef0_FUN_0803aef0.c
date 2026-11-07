// 0x0803aef0 FUN_0803aef0 size=336

void FUN_0803aef0(int *param_1,uint param_2,byte param_3,undefined4 param_4)

{
  uint uVar1;
  byte *pbVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = *param_1;
  uVar1 = param_2 & 0x7f;
  if ((int)(param_2 << 0x18) < 0) {
    iVar3 = uVar1 * 0xb + 3;
    *(undefined1 *)((int)param_1 + uVar1 * 0x2c + 0xe) = 0;
  }
  else {
    iVar3 = uVar1 * 0xb + 0x5b;
    *(undefined1 *)((int)param_1 + uVar1 * 0x2c + 0x16e) = 1;
  }
  pbVar2 = (byte *)(param_1 + iVar3);
  *(undefined4 *)(pbVar2 + 8) = param_4;
  pbVar2[3] = param_3;
  uVar1 = 0;
  if ((*pbVar2 == 0) && (iVar3 = *(int *)(pbVar2 + 8), iVar3 != 0x40)) {
    if (iVar3 == 0x20) {
      uVar1 = 1;
    }
    else if (iVar3 == 0x10) {
      uVar1 = 2;
    }
    else if (iVar3 == 8) {
      uVar1 = 3;
    }
  }
  if (pbVar2[2] == 0) {
    *(uint *)(iVar4 + 0x81c) = *(uint *)(iVar4 + 0x81c) | 1 << *pbVar2;
    if (*pbVar2 == 0) {
      *(uint *)(iVar4 + 0x900) = *(uint *)(iVar4 + 0x900) & 0xfffff800 | uVar1;
    }
    else {
      iVar3 = iVar4 + (uint)*pbVar2 * 0x20;
      *(uint *)(iVar3 + 0x900) =
           *(uint *)(iVar3 + 0x900) & 0xfffff800 | *(ushort *)(pbVar2 + 8) & 0x7ff;
    }
    iVar3 = iVar4 + (uint)*pbVar2 * 0x20;
    *(uint *)(iVar3 + 0x900) = *(uint *)(iVar3 + 0x900) & 0xfff3ffff | (pbVar2[3] & 3) << 0x12;
    iVar3 = iVar4 + (uint)*pbVar2 * 0x20;
    *(uint *)(iVar3 + 0x900) = *(uint *)(iVar3 + 0x900) & 0xfc3fffff | (*pbVar2 & 0xf) << 0x16;
    iVar3 = iVar4 + (uint)*pbVar2 * 0x20;
    *(uint *)(iVar3 + 0x900) = *(uint *)(iVar3 + 0x900) | 0x10000000;
    iVar4 = iVar4 + (uint)*pbVar2 * 0x20;
    *(uint *)(iVar4 + 0x900) = *(uint *)(iVar4 + 0x900) | 0x8000;
    return;
  }
  *(uint *)(iVar4 + 0x81c) = *(uint *)(iVar4 + 0x81c) | (1 << *pbVar2) << 0x10;
  if (*pbVar2 == 0) {
    *(uint *)(iVar4 + 0xb00) = *(uint *)(iVar4 + 0xb00) & 0xfffff800 | uVar1;
  }
  else {
    iVar3 = iVar4 + (uint)*pbVar2 * 0x20;
    *(uint *)(iVar3 + 0xb00) =
         *(uint *)(iVar3 + 0xb00) & 0xfffff800 | *(ushort *)(pbVar2 + 8) & 0x7ff;
  }
  iVar3 = iVar4 + (uint)*pbVar2 * 0x20;
  *(uint *)(iVar3 + 0xb00) = *(uint *)(iVar3 + 0xb00) & 0xfff3ffff | (pbVar2[3] & 3) << 0x12;
  iVar3 = iVar4 + (uint)*pbVar2 * 0x20;
  *(uint *)(iVar3 + 0xb00) = *(uint *)(iVar3 + 0xb00) | 0x10000000;
  iVar4 = iVar4 + (uint)*pbVar2 * 0x20;
  *(uint *)(iVar4 + 0xb00) = *(uint *)(iVar4 + 0xb00) | 0x8000;
  return;
}

