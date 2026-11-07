// 0x08027b22 FUN_08027b22 size=64

void FUN_08027b22(undefined4 *param_1,undefined2 param_2,uint param_3)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  puVar1 = param_1;
  if (((uint)param_1 & 3) != 0) {
    puVar1 = (undefined4 *)((int)param_1 + 2);
    *(undefined2 *)param_1 = param_2;
    param_3 = param_3 - 1;
  }
  uVar2 = CONCAT22(param_2,param_2);
  for (; 0x10 < param_3; param_3 = param_3 - 0x10) {
    *puVar1 = uVar2;
    puVar1[1] = uVar2;
    puVar1[2] = uVar2;
    puVar1[3] = uVar2;
    puVar1[4] = uVar2;
    puVar1[5] = uVar2;
    puVar1[6] = uVar2;
    puVar1[7] = uVar2;
    puVar1 = puVar1 + 8;
  }
  for (; param_3 != 0; param_3 = param_3 - 1) {
    *(undefined2 *)puVar1 = param_2;
    puVar1 = (undefined4 *)((int)puVar1 + 2);
  }
  return;
}

