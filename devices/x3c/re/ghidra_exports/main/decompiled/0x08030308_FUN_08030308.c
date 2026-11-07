// 0x08030308 FUN_08030308 size=54

undefined8 FUN_08030308(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  
  uStack_20 = param_1;
  uStack_1c = param_2;
  uStack_18 = param_3;
  uStack_14 = param_4;
  FUN_0802ed10(param_1);
  iVar1 = FUN_0802830e();
  if (iVar1 != 0) {
    FUN_080273fa(&uStack_20,param_2);
    iVar1 = FUN_0802df20(param_1,&uStack_20);
    if (iVar1 != 0) {
      uVar2 = FUN_0802ed10(param_1);
      FUN_08017e20(uVar2,&uStack_20);
    }
  }
  return CONCAT44(uStack_1c,uStack_20);
}

