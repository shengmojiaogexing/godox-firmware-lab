// 0x08038238 FUN_08038238 size=120

void FUN_08038238(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_080382bc;
  iVar2 = DAT_080382b4;
  if (param_1 == DAT_080382b0) {
    FUN_0801bd34(DAT_080382b4,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_080382b8) {
    FUN_0801bd34(DAT_080382bc,1);
    FUN_0801bd34(iVar1,0);
    return;
  }
  if (param_1 == DAT_080382c0) {
    iVar2 = DAT_080382bc + 1;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_080382c4) {
    iVar2 = DAT_080382b4 + 1;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  return;
}

