// 0x0801ae74 FUN_0801ae74 size=50

undefined4 FUN_0801ae74(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = *(undefined4 *)(*(int *)(param_1 + 4) + 0x24);
  FUN_0803aef0(param_1,0x81,3,0x40);
  FUN_0803aef0(param_1,1,3,0x40);
  FUN_0803af24(param_1,1,uVar1,0x40);
  return 0;
}

