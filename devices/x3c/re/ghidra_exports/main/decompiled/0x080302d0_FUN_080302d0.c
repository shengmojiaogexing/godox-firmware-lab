// 0x080302d0 FUN_080302d0 size=56

undefined8 FUN_080302d0(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  
  local_20 = param_1;
  local_1c = param_2;
  local_18 = param_3;
  local_14 = param_4;
  iVar1 = FUN_080180c2(param_1);
  FUN_080273fa(&local_20,param_1 + 0x14);
  local_20 = local_20 - iVar1;
  local_1c = local_1c - iVar1;
  local_18 = local_18 + iVar1;
  local_14 = local_14 + iVar1;
  FUN_08030308(param_1,&local_20);
  return CONCAT44(local_1c,local_20);
}

