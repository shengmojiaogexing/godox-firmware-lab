// 0x0801fffc FUN_0801fffc size=102

void FUN_0801fffc(void)

{
  undefined4 uVar1;
  int iVar2;
  ushort uVar3;
  
  uVar3 = 0;
  if (*DAT_08020064 == '\0') {
    return;
  }
  FUN_080218e8();
  FUN_08021980(DAT_08020068,0x55555555);
  FUN_080217b0();
  *DAT_0802006c = 2;
  FUN_08023b74();
  uVar1 = DAT_08020070;
  while ((iVar2 = FUN_08022eb4(uVar1,0x40), iVar2 == 1 && (uVar3 = uVar3 + 1, uVar3 < 0x1389))) {
    FUN_0801c118(10);
  }
  *DAT_08020074 = 1;
  FUN_0801445c();
  FUN_08023b48();
  *DAT_08020078 = 1;
  FUN_08010c00();
  return;
}

