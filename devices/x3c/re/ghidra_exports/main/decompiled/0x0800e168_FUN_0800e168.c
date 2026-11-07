// 0x0800e168 FUN_0800e168 size=374

void FUN_0800e168(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 *puVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined *puVar15;
  
  FUN_0802e2d2(param_1,0x10,param_3,param_4,param_1,param_2,param_3,param_4);
  uVar3 = (undefined2)((uint)param_2 >> 0x10);
  uVar2 = FUN_08027f68();
  uVar5 = DAT_0800e2e4;
  uVar6 = CONCAT22(uVar3,uVar2);
  uVar4 = FUN_0802553e(param_1,7,0x2b,0,DAT_0800e2e4,uVar6,&DAT_0800e2e0);
  uVar3 = (undefined2)((uint)uVar6 >> 0x10);
  uVar2 = FUN_08027f68();
  uVar6 = CONCAT22(uVar3,uVar2);
  uVar5 = FUN_0802553e(param_1,7,0x4e,0,uVar5,uVar6,&DAT_0800e2e8);
  uVar3 = (undefined2)((uint)uVar6 >> 0x10);
  uVar2 = FUN_08027f68();
  uVar7 = CONCAT22(uVar3,uVar2);
  uVar6 = FUN_0802553e(param_1,7,0xcc,0,DAT_0800e2f0,uVar7,&DAT_0800e2ec);
  uVar2 = (undefined2)((uint)uVar7 >> 0x10);
  if (*(char *)(DAT_0800e2f4 + 0xb) == '\x01') {
    FUN_0802da22(uVar6,1);
  }
  uVar3 = FUN_08027f68();
  uVar13 = CONCAT22(uVar2,uVar3);
  uVar7 = FUN_0802553e(param_1,7,0xac,0,DAT_0800e2fc,uVar13,&DAT_0800e2f8);
  uVar2 = (undefined2)((uint)uVar13 >> 0x10);
  if (*DAT_0800e300 == '\0') {
    FUN_0802da22(uVar7,1);
  }
  else {
    FUN_0802e2d2(uVar7,1);
  }
  uVar3 = FUN_08027f68();
  uVar13 = CONCAT22(uVar2,uVar3);
  uVar8 = FUN_0802553e(param_1,7,0x8a,0,DAT_0800e304,uVar13,&DAT_0800e2f8);
  uVar3 = (undefined2)((uint)uVar13 >> 0x10);
  uVar2 = FUN_08027f68();
  uVar13 = CONCAT22(uVar3,uVar2);
  uVar9 = FUN_0802553e(param_1,7,0xef,0,DAT_0800e308,uVar13,&DAT_0800e2f8);
  uVar3 = (undefined2)((uint)uVar13 >> 0x10);
  uVar2 = FUN_08027f68();
  uVar13 = CONCAT22(uVar3,uVar2);
  uVar10 = FUN_0802553e(param_1,7,0x10b,0,DAT_0800e30c,uVar13,&DAT_0800e2f8);
  uVar2 = (undefined2)((uint)uVar13 >> 0x10);
  uVar11 = FUN_08027f68();
  puVar15 = &DAT_0800e310;
  uVar14 = CONCAT22(uVar2,(short)uVar11);
  uVar13 = DAT_0800e314;
  uVar12 = FUN_0802553e(param_1,7,299,0);
  puVar1 = DAT_0800e318;
  DAT_0800e318[7] = uVar12;
  puVar1[6] = uVar10;
  puVar1[5] = uVar9;
  puVar1[3] = uVar8;
  puVar1[2] = uVar6;
  puVar1[1] = uVar5;
  *puVar1 = uVar4;
  puVar1[4] = uVar7;
  FUN_0803840c(uVar13,uVar14,puVar15,uVar11);
  return;
}

