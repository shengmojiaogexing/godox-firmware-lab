// 0x0801cd3c FUN_0801cd3c size=222

void FUN_0801cd3c(undefined4 param_1,int *param_2,int *param_3,undefined4 param_4,undefined1 param_5
                 )

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *local_5c [2];
  undefined2 local_54;
  undefined1 local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  undefined4 uStack_30;
  int *piStack_2c;
  int *piStack_28;
  undefined4 local_24;
  
  uStack_30 = param_1;
  piStack_2c = param_2;
  piStack_28 = param_3;
  local_24 = param_4;
  FUN_0802d8d6(local_5c,0x1c);
  local_5c[0] = &local_40;
  local_54 = (undefined2)local_24;
  local_44 = param_5;
  iVar1 = param_2[1];
  iVar5 = param_3[1];
  iVar2 = param_2[3];
  iVar6 = param_3[3];
  iVar7 = *param_2;
  iVar3 = *param_3;
  iVar4 = param_2[2];
  iVar8 = param_3[2];
  local_38 = param_2[2];
  local_3c = param_2[1];
  local_34 = param_3[1] + -1;
  local_40 = iVar7;
  if (iVar1 <= iVar5) {
    FUN_08029de4(param_1,local_5c);
  }
  local_3c = param_3[3] + 1;
  local_34 = param_2[3];
  if (iVar2 >= iVar6) {
    FUN_08029de4(param_1,local_5c);
  }
  local_40 = *param_2;
  local_38 = *param_3 + -1;
  if (iVar5 < iVar1) {
    local_3c = param_2[1];
  }
  else {
    local_3c = param_3[1];
  }
  if (iVar2 < iVar6) {
    local_34 = param_2[3];
  }
  else {
    local_34 = param_3[3];
  }
  if (iVar7 <= iVar3) {
    FUN_08029de4(param_1,local_5c);
  }
  local_40 = param_3[2] + 1;
  local_38 = param_2[2];
  if (iVar8 <= iVar4) {
    FUN_08029de4(param_1,local_5c);
  }
  return;
}

