// 0x08017184 FUN_08017184 size=52

bool FUN_08017184(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  uVar1 = DAT_080171b8;
  iVar2 = FUN_08022eb4(DAT_080171b8,0x200);
  iVar3 = FUN_08022eb4(uVar1,0x100);
  iVar4 = FUN_08022eb4(DAT_080171bc,0x8000);
  return iVar4 == 0 || (iVar3 == 0 || iVar2 == 0);
}

