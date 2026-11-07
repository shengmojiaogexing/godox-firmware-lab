// 0x08038148 FUN_08038148 size=128

void FUN_08038148(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 local_18;
  undefined4 local_14;
  
  local_18 = param_3;
  local_14 = param_4;
  FUN_0801bd04(0x300000,1);
  FUN_0801bd04(DAT_080381c8,1);
  uVar1 = DAT_080381cc;
  local_14 = 0x1020100;
  local_18 = 8;
  FUN_08022e50(DAT_080381cc,&local_18);
  FUN_08022ec2(uVar1,3,5);
  uVar2 = local_14;
  local_14._0_2_ = CONCAT11(1,(undefined1)local_14);
  local_14._3_1_ = SUB41(uVar2,3);
  local_14._0_3_ = CONCAT12(2,(undefined2)local_14);
  local_18 = 0x20;
  FUN_08022e50(uVar1,&local_18);
  FUN_08022ec2(uVar1,5);
  uVar2 = local_14;
  local_14._0_2_ = CONCAT11(1,(undefined1)local_14);
  local_14._3_1_ = SUB41(uVar2,3);
  local_14._0_3_ = CONCAT12(2,(undefined2)local_14);
  local_18 = 0x10;
  FUN_08022e50(uVar1,&local_18);
  FUN_08022ec2(uVar1,4,5);
  return;
}

