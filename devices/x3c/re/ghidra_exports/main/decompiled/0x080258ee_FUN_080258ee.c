// 0x080258ee FUN_080258ee size=162

undefined4 FUN_080258ee(int param_1,undefined4 param_2,int param_3,undefined4 *param_4)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  
  uVar1 = FUN_080180c2(param_2);
  FUN_0802ed04(param_2,&local_28);
  FUN_080274e4(&local_28,uVar1,uVar1);
  if (param_3 == 2) {
    local_48 = local_28;
    uStack_44 = uStack_24;
    uStack_40 = uStack_20;
    uStack_3c = uStack_1c;
    FUN_0802fb00(param_2,&local_48,0);
    iVar2 = FUN_08017484(&local_38,*(undefined4 *)(param_1 + 8),&local_48);
    if (iVar2 == 0) {
      return 0;
    }
    local_58 = local_38;
    uStack_54 = uStack_34;
    uStack_50 = uStack_30;
    uStack_4c = uStack_2c;
    FUN_0802fb00(param_2,&local_58,0,1);
    iVar2 = FUN_08017484(&local_58,&local_58,&local_28);
    if (iVar2 == 0) {
      return 0;
    }
    *param_4 = local_58;
    param_4[1] = uStack_54;
    param_4[2] = uStack_50;
    param_4[3] = uStack_4c;
  }
  else {
    if (param_3 != 1) {
      return 0;
    }
    iVar2 = FUN_08017484(&local_38,*(undefined4 *)(param_1 + 8),&local_28);
    if (iVar2 == 0) {
      return 0;
    }
    *param_4 = local_38;
    param_4[1] = uStack_34;
    param_4[2] = uStack_30;
    param_4[3] = uStack_2c;
  }
  return 1;
}

