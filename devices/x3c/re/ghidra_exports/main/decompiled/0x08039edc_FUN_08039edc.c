// 0x08039edc FUN_08039edc size=66

void FUN_08039edc(undefined4 *param_1)

{
  undefined2 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  
  param_1 = (undefined4 *)*param_1;
  uVar2 = FUN_08031782(param_1[2]);
  uVar3 = FUN_0802f9bc(*param_1,uVar2,*(undefined2 *)(param_1 + 1));
  param_1[3] = uVar3;
  uVar1 = *(undefined2 *)(param_1 + 1);
  *(undefined2 *)(param_1 + 1) = 0;
  FUN_08039f20(*param_1,uVar2,uVar1,param_1);
  *(undefined2 *)(param_1 + 1) = uVar1;
  puVar4 = (undefined4 *)FUN_080227c6(*param_1,param_1[2]);
  FUN_08032ff8(*puVar4,*(undefined2 *)(param_1 + 1),param_1[3]);
  return;
}

