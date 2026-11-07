// 0x08018cbc FUN_08018cbc size=266

void FUN_08018cbc(int param_1,uint param_2,int param_3,uint param_4)

{
  uint uVar1;
  
  switch(param_2) {
  case 0:
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xfffffff8 | param_4 & 7;
    break;
  case 1:
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xffffffc7 | (param_4 & 7) << 3;
    break;
  case 2:
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xfffffe3f | (param_4 & 7) << 6;
    break;
  case 3:
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xfffff1ff | (param_4 & 7) << 9;
    break;
  case 4:
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xffff8fff | (param_4 & 7) << 0xc;
    break;
  case 5:
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xfffc7fff | (param_4 & 7) << 0xf;
    break;
  case 6:
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xffe3ffff | (param_4 & 7) << 0x12;
    break;
  case 7:
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xff1fffff | (param_4 & 7) << 0x15;
    break;
  case 8:
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xf8ffffff | (param_4 & 7) << 0x18;
    break;
  case 9:
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xc7ffffff | (param_4 & 7) << 0x1b;
    break;
  case 10:
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffffff8 | param_4 & 7;
    break;
  case 0xb:
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xffffffc7 | (param_4 & 7) << 3;
    break;
  case 0xc:
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffffe3f | (param_4 & 7) << 6;
    break;
  case 0xd:
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffff1ff | (param_4 & 7) << 9;
    break;
  case 0xe:
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xffff8fff | (param_4 & 7) << 0xc;
    break;
  case 0xf:
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffc7fff | (param_4 & 7) << 0xf;
    break;
  case 0x10:
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xffe3ffff | (param_4 & 7) << 0x12;
    break;
  case 0x11:
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xff1fffff | (param_4 & 7) << 0x15;
    break;
  case 0x12:
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xf8ffffff | (param_4 & 7) << 0x18;
  }
  uVar1 = (param_3 - ((*(uint *)(param_1 + 0x38) & 0x3fffff) >> 0x14)) + 3 & 0xffff;
  if (uVar1 == 1) {
    *(uint *)(param_1 + 0x38) = *(uint *)(param_1 + 0x38) & 0xffffffe0 | param_2 & 0x1f;
    return;
  }
  if (uVar1 == 2) {
    *(uint *)(param_1 + 0x38) = *(uint *)(param_1 + 0x38) & 0xfffffc1f | (param_2 & 0x1f) << 5;
    return;
  }
  if (uVar1 == 3) {
    *(uint *)(param_1 + 0x38) = *(uint *)(param_1 + 0x38) & 0xffff83ff | (param_2 & 0x1f) << 10;
    return;
  }
  if (uVar1 == 4) {
    *(uint *)(param_1 + 0x38) = *(uint *)(param_1 + 0x38) & 0xfff07fff | (param_2 & 0x1f) << 0xf;
  }
  return;
}

