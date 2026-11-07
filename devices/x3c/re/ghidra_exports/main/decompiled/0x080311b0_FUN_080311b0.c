// 0x080311b0 FUN_080311b0 size=64

void FUN_080311b0(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 local_20;
  undefined4 local_1c;
  
  local_20 = param_3;
  local_1c = param_4;
  FUN_080317f8(param_1);
  local_20 = 0;
  local_1c = 0;
  iVar1 = FUN_0802ee0a(param_1);
  iVar3 = param_1;
  while (iVar2 = iVar1, iVar2 != 0) {
    FUN_08036e80(param_1 + 0x14,iVar3,&local_20,param_2);
    iVar1 = FUN_0802ee0a(iVar2);
    iVar3 = iVar2;
  }
  return;
}

