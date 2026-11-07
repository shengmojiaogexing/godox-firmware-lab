// 0x0802db62 FUN_0802db62 size=868

void FUN_0802db62(undefined4 param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int extraout_r2;
  int extraout_r2_00;
  int extraout_r2_01;
  int extraout_r2_02;
  int extraout_r2_03;
  int extraout_r2_04;
  int extraout_r2_05;
  int extraout_r2_06;
  int extraout_r2_07;
  int extraout_r2_08;
  int extraout_r2_09;
  int extraout_r2_10;
  int extraout_r2_11;
  int extraout_r2_12;
  int extraout_r2_13;
  int extraout_r2_14;
  int extraout_r2_15;
  int extraout_r2_16;
  int extraout_r2_17;
  int extraout_r2_18;
  int iVar8;
  int iVar9;
  undefined8 uVar10;
  
  FUN_080317f8(param_1);
  if (param_2 == 0) {
    param_2 = FUN_0802ee0a(param_1);
  }
  iVar8 = 0;
  iVar9 = 0;
  iVar1 = FUN_0802ee0a(param_1);
  iVar2 = FUN_0802f864(iVar1,0);
  iVar3 = FUN_0802f938(iVar1,0);
  iVar4 = FUN_0802f998(iVar1,0);
  iVar5 = FUN_0802f864(param_2,0);
  iVar6 = FUN_0802f938(param_2,0);
  iVar6 = iVar6 + iVar5;
  iVar7 = FUN_0802f998(param_2,0);
  iVar5 = iVar5 + iVar7;
  if (param_3 == 0) {
    iVar7 = FUN_0802f7e0(param_2,0);
    if (iVar7 == 1) {
      param_3 = 3;
    }
    else {
      param_3 = 1;
    }
  }
  switch(param_3) {
  case 1:
    break;
  case 2:
    uVar10 = FUN_0802ecd0(param_2);
    iVar7 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10 / 2);
    iVar6 = (extraout_r2_01 - iVar7 / 2) + iVar6;
    break;
  case 3:
    uVar10 = FUN_0802ecd0(param_2);
    iVar7 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10);
    iVar6 = (extraout_r2_02 - iVar7) + iVar6;
    break;
  case 4:
    uVar10 = FUN_0802ec9c(param_2);
    iVar7 = FUN_0802ed88(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10);
    iVar5 = (extraout_r2_03 - iVar7) + iVar5;
    break;
  case 5:
    uVar10 = FUN_0802ecd0(param_2);
    iVar7 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10 / 2);
    iVar6 = (extraout_r2_04 - iVar7 / 2) + iVar6;
    uVar10 = FUN_0802ec9c(param_2);
    iVar7 = FUN_0802ed88(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10);
    iVar5 = (extraout_r2_05 - iVar7) + iVar5;
    break;
  case 6:
    uVar10 = FUN_0802ecd0(param_2);
    iVar7 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10);
    iVar6 = (extraout_r2_06 - iVar7) + iVar6;
    uVar10 = FUN_0802ec9c(param_2);
    iVar7 = FUN_0802ed88(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10);
    iVar5 = (extraout_r2_07 - iVar7) + iVar5;
    break;
  case 7:
    uVar10 = FUN_0802ec9c(param_2);
    iVar7 = FUN_0802ed88(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10 / 2);
    iVar5 = (extraout_r2_08 - iVar7 / 2) + iVar5;
    break;
  case 8:
    uVar10 = FUN_0802ecd0(param_2);
    iVar7 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10);
    iVar6 = (extraout_r2_09 - iVar7) + iVar6;
    uVar10 = FUN_0802ec9c(param_2);
    iVar7 = FUN_0802ed88(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10 / 2);
    iVar5 = (extraout_r2_10 - iVar7 / 2) + iVar5;
    break;
  case 9:
    uVar10 = FUN_0802ecd0(param_2);
    iVar7 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10 / 2);
    iVar8 = (extraout_r2 - iVar7 / 2) + iVar6;
    uVar10 = FUN_0802ec9c(param_2);
    iVar6 = FUN_0802ed88(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10 / 2);
    iVar9 = (extraout_r2_00 - iVar6 / 2) + iVar5;
  default:
    iVar6 = iVar8;
    iVar5 = iVar9;
    break;
  case 10:
    iVar5 = FUN_0802ed88(param_1);
    iVar5 = -iVar5;
    iVar6 = iVar8;
    break;
  case 0xb:
    uVar10 = FUN_0802fc5e(param_2);
    iVar6 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10 / 2);
    iVar5 = FUN_0802ed88(param_1);
    iVar5 = -iVar5;
    iVar6 = extraout_r2_11 - iVar6 / 2;
    break;
  case 0xc:
    uVar10 = FUN_0802fc5e(param_2);
    iVar6 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10);
    iVar5 = FUN_0802ed88(param_1);
    iVar5 = -iVar5;
    iVar6 = extraout_r2_12 - iVar6;
    break;
  case 0xd:
    iVar5 = FUN_0802ed88(param_2);
    iVar6 = iVar8;
    break;
  case 0xe:
    uVar10 = FUN_0802fc5e(param_2);
    iVar6 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10 / 2);
    iVar5 = FUN_0802ed88(param_2);
    iVar6 = extraout_r2_13 - iVar6 / 2;
    break;
  case 0xf:
    uVar10 = FUN_0802fc5e(param_2);
    iVar6 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10);
    iVar5 = FUN_0802ed88(param_2);
    iVar6 = extraout_r2_14 - iVar6;
    break;
  case 0x10:
    iVar5 = FUN_0802fc5e(param_1);
    iVar6 = -iVar5;
    iVar5 = iVar9;
    break;
  case 0x11:
    iVar5 = FUN_0802fc5e(param_1);
    uVar10 = FUN_0802ed88(param_2);
    iVar7 = FUN_0802ed88(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10 / 2);
    iVar6 = -iVar5;
    iVar5 = extraout_r2_15 - iVar7 / 2;
    break;
  case 0x12:
    iVar5 = FUN_0802fc5e(param_1);
    uVar10 = FUN_0802ed88(param_2);
    iVar7 = FUN_0802ed88(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10);
    iVar6 = -iVar5;
    iVar5 = extraout_r2_16 - iVar7;
    break;
  case 0x13:
    iVar6 = FUN_0802fc5e(param_2);
    iVar5 = iVar9;
    break;
  case 0x14:
    iVar6 = FUN_0802fc5e(param_2);
    uVar10 = FUN_0802ed88(param_2);
    iVar5 = FUN_0802ed88(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10 / 2);
    iVar5 = extraout_r2_17 - iVar5 / 2;
    break;
  case 0x15:
    iVar6 = FUN_0802fc5e(param_2);
    uVar10 = FUN_0802ed88(param_2);
    iVar5 = FUN_0802ed88(param_1,(int)((ulonglong)uVar10 >> 0x20),(int)uVar10);
    iVar5 = extraout_r2_18 - iVar5;
  }
  iVar7 = FUN_0802f7e0(iVar1,0);
  if (iVar7 == 1) {
    iVar7 = FUN_0802efdc(iVar1);
    iVar3 = (iVar7 + ((param_4 + *(int *)(param_2 + 0x14)) - *(int *)(iVar1 + 0x14))) -
            (iVar3 + iVar2);
  }
  else {
    iVar7 = FUN_0802ef1c(iVar1);
    iVar3 = (iVar7 + ((param_4 + *(int *)(param_2 + 0x14)) - *(int *)(iVar1 + 0x14))) -
            (iVar3 + iVar2);
  }
  iVar7 = FUN_0802f0bc(iVar1);
  iVar8 = *(int *)(param_2 + 0x18);
  iVar1 = *(int *)(iVar1 + 0x18);
  FUN_08031572(param_1,1,0);
  FUN_08031318(param_1,iVar6 + iVar3,
               iVar5 + ((((param_5 + iVar8) - iVar1) + iVar7) - (iVar4 + iVar2)));
  return;
}

