// 0x0800c880 FUN_0800c880 size=136

void FUN_0800c880(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = DAT_0800c908;
  FUN_08022e34(DAT_0800c908,0x80);
  FUN_0801c118(1);
  FUN_0801c118(1);
  uVar2 = DAT_0800c90c;
  if (param_1 == 0) {
    FUN_08022e34(DAT_0800c90c,2);
  }
  else {
    FUN_08022e38(DAT_0800c90c,2);
  }
  FUN_080147f4(uVar2,2,1,0);
  FUN_0801c118(2);
  FUN_0801c118(2);
  FUN_08022e38(uVar1,0x80);
  FUN_0801c118(4);
  FUN_0801c118(4);
  FUN_08022e34(uVar1,0x80);
  FUN_0801c118(1);
  FUN_0801c118(1);
  FUN_08022e34(uVar2,2);
  FUN_0801c118(1);
  FUN_0801c118(1);
  return;
}

