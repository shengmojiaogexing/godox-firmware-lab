// 0x08025618 FUN_08025618 size=502

void FUN_08025618(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 *puVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined2 uVar9;
  
  uVar9 = 0;
  uVar3 = FUN_08010acc(0,9,0,0,0x170,0x1c0,param_3,param_4);
  uVar4 = FUN_08027ac8();
  FUN_0803157c(uVar3,uVar4,0);
  FUN_08031594(uVar3,0xff,0);
  uVar2 = FUN_08027fb8();
  FUN_0802553e(uVar3,2,0,0x1e,DAT_08025814,CONCAT22(uVar9,uVar2),&DAT_08025810);
  uVar9 = 0;
  uVar5 = FUN_08006220(uVar3,1,9,0x78,0x15e,0x50);
  uVar4 = FUN_08027d38(0x27,0x27);
  FUN_0803157c(uVar5,uVar4,0);
  FUN_08031594(uVar5,0xff,0);
  FUN_08031696(uVar5,0xf,0);
  uVar4 = FUN_08027d38(100,100);
  FUN_0803157c(uVar5,uVar4,0x20);
  uVar2 = FUN_08027fb8();
  uVar4 = CONCAT22(uVar9,uVar2);
  FUN_0802553e(uVar5,7,0xc,0,DAT_08025820,uVar4,&DAT_08025818);
  uVar9 = (undefined2)((uint)uVar4 >> 0x10);
  uVar2 = FUN_08027fb8();
  uVar4 = DAT_08025828;
  uVar6 = FUN_0802553e(uVar5,7,0x126,0,DAT_08025828,CONCAT22(uVar9,uVar2),&DAT_08025824);
  uVar9 = 0;
  uVar7 = FUN_08006220(uVar3,1,9,0xd2,0x15e,0x50);
  uVar8 = FUN_08027d38(0x27,0x27);
  FUN_0803157c(uVar7,uVar8,0);
  FUN_08031594(uVar7,0xff,0);
  FUN_08031696(uVar7,0xf,0);
  uVar8 = FUN_08027d38(100,100);
  FUN_0803157c(uVar7,uVar8,0x20);
  uVar2 = FUN_08027fb8();
  uVar8 = CONCAT22(uVar9,uVar2);
  FUN_0802553e(uVar7,7,0xc,0,DAT_08025834,uVar8,s_English_0802582c);
  uVar9 = (undefined2)((uint)uVar8 >> 0x10);
  uVar2 = FUN_08027fb8();
  uVar4 = FUN_0802553e(uVar7,7,0x126,0,uVar4,CONCAT22(uVar9,uVar2),&DAT_08025824);
  puVar1 = DAT_08025838;
  *DAT_08025838 = uVar6;
  puVar1[1] = uVar4;
  FUN_0802d97a(uVar5,DAT_0802583c,0);
  FUN_0802d97a(uVar7,DAT_0802583c,0);
  if (*(char *)((int)DAT_08025838 + -0xe1) == '\0') {
    FUN_0802da22(uVar4,1);
    FUN_0802e2d2(uVar6,1);
  }
  else {
    FUN_0802da22(uVar6,1);
    FUN_0802e2d2(uVar4,1);
  }
  puVar1 = DAT_08025838;
  DAT_08025838[-0x14] = uVar5;
  puVar1[-0x13] = uVar7;
  FUN_0802832e(uVar3);
  return;
}

