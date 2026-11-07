// 0x0802b968 FUN_0802b968 size=92

void FUN_0802b968(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  
  *(undefined4 *)(param_2 + 0x2c) = 0;
  *(byte *)(param_2 + 0x4e) = *(byte *)(param_2 + 0x4e) & 0x83 | 3;
  uVar1 = FUN_0802fc5e(param_2);
  *(undefined4 *)(param_2 + 0x38) = uVar1;
  uVar1 = FUN_0802ed88(param_2);
  *(undefined4 *)(param_2 + 0x3c) = uVar1;
  *(undefined2 *)(param_2 + 0x40) = 0;
  *(undefined2 *)(param_2 + 0x4c) = 0x100;
  *(byte *)(param_2 + 0x4e) = *(byte *)(param_2 + 0x4e) | 0x80;
  *(undefined4 *)(param_2 + 0x30) = 0;
  *(undefined4 *)(param_2 + 0x34) = 0;
  *(undefined4 *)(param_2 + 0x44) = 0;
  *(undefined4 *)(param_2 + 0x48) = 0;
  *(undefined1 *)(param_2 + 0x4f) = 0;
  FUN_0802e2d2(param_2,2);
  FUN_0802da22(param_2,0x10000);
  return;
}

