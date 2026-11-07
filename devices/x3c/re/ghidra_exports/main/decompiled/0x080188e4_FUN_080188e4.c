// 0x080188e4 FUN_080188e4 size=46

void FUN_080188e4(int param_1,int *param_2,undefined4 *param_3,int *param_4)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = DAT_08018914;
  iVar2 = (**(code **)(DAT_08018914 + 4))(param_1,param_4);
  *param_2 = iVar2;
  if (iVar2 == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = (**(code **)(iVar1 + 4))(*param_4 + param_1,0);
  }
  *param_3 = uVar3;
  return;
}

