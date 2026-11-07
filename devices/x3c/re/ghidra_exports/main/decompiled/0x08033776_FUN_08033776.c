// 0x08033776 FUN_08033776 size=36

undefined4 FUN_08033776(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined8 uVar3;
  
  uVar1 = FUN_08033760(param_1);
  uVar3 = FUN_080338cc();
  iVar2 = FUN_080338cc((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),param_2 - (int)uVar3);
  FUN_080336e4(uVar1,iVar2 + param_1);
  return uVar1;
}

