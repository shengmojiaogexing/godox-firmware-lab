// 0x08029de4 FUN_08029de4 size=44

undefined8 FUN_08029de4(int param_1,undefined4 *param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iStack_20;
  undefined4 *puStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  
  iStack_20 = param_1;
  puStack_1c = param_2;
  if ((2 < *(byte *)(param_2 + 6)) &&
     (uStack_18 = param_3, uStack_14 = param_4,
     iVar1 = FUN_08017484(&iStack_20,*param_2,*(undefined4 *)(param_1 + 8)), iVar1 != 0)) {
    if (*(code **)(param_1 + 0x34) != (code *)0x0) {
      (**(code **)(param_1 + 0x34))(param_1);
    }
    (**(code **)(param_1 + 0x54))(param_1,param_2);
  }
  return CONCAT44(puStack_1c,iStack_20);
}

