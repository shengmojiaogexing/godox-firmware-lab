// 0x08029a38 FUN_08029a38 size=116

void FUN_08029a38(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined2 uVar1;
  
  FUN_0802d8d6(param_1,0x4c,param_3,param_4,param_4);
  uVar1 = FUN_08027f24();
  *(undefined2 *)(param_1 + 6) = uVar1;
  uVar1 = FUN_08027f24();
  *(undefined2 *)(param_1 + 8) = uVar1;
  uVar1 = FUN_08027a50();
  *(undefined2 *)(param_1 + 0xc) = uVar1;
  *(undefined1 *)(param_1 + 0xe) = 0xff;
  *(undefined1 *)(param_1 + 0x10) = 2;
  uVar1 = FUN_08027a50();
  *(undefined2 *)(param_1 + 0x22) = uVar1;
  uVar1 = FUN_08027a50();
  *(undefined2 *)(param_1 + 0x36) = uVar1;
  *(undefined4 *)(param_1 + 0x18) = DAT_08029aac;
  *(undefined1 *)(param_1 + 5) = 0xff;
  *(undefined1 *)(param_1 + 0x1e) = 0xff;
  *(undefined1 *)(param_1 + 0x34) = 0xff;
  *(undefined1 *)(param_1 + 0x28) = 0xff;
  *(undefined1 *)(param_1 + 0x48) = 0xff;
  *(byte *)(param_1 + 0x29) = (*(byte *)(param_1 + 0x29) & 0xc1) + 0x1e;
  return;
}

