// 0x08026b84 FUN_08026b84 size=104

void FUN_08026b84(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  
  *(undefined2 *)(param_2 + 0x2c) = 0;
  *(undefined2 *)(param_2 + 0x32) = 0x87;
  *(undefined2 *)(param_2 + 0x34) = 0x2d;
  *(undefined2 *)(param_2 + 0x2e) = 0x87;
  *(undefined2 *)(param_2 + 0x30) = 0x10e;
  *(ushort *)(param_2 + 0x3c) = *(ushort *)(param_2 + 0x3c) & 0xfff9;
  *(undefined2 *)(param_2 + 0x36) = 0x8000;
  *(ushort *)(param_2 + 0x3c) = *(ushort *)(param_2 + 0x3c) | 8;
  *(undefined2 *)(param_2 + 0x38) = 0;
  *(undefined2 *)(param_2 + 0x3a) = 100;
  *(ushort *)(param_2 + 0x3c) = *(ushort *)(param_2 + 0x3c) & 0xfffe;
  *(undefined2 *)(param_2 + 0x3e) = 0x2d0;
  uVar1 = FUN_080334d8();
  *(undefined4 *)(param_2 + 0x40) = uVar1;
  *(undefined2 *)(param_2 + 0x44) = *(undefined2 *)(param_2 + 0x30);
  FUN_0802da22(param_2,2);
  FUN_0802e2d2(param_2,0x310);
  FUN_08031290(param_2,0xd);
  return;
}

