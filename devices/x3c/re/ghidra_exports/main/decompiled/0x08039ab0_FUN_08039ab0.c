// 0x08039ab0 FUN_08039ab0 size=448

void FUN_08039ab0(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_08039c90;
  iVar2 = DAT_08039c78;
  if (param_1 == DAT_08039c70) {
    FUN_0801bd34(0x240000,1);
    FUN_0801bd34(0x240000,0);
    return;
  }
  if (param_1 == 0x40000000) {
    FUN_0801bd34(0x200000);
    FUN_0801bd34(0x200000,0);
    return;
  }
  if (param_1 == DAT_08039c74) {
    FUN_0801bd34(DAT_08039c78,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039c7c) {
    iVar2 = DAT_08039c78 + 1;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039c80) {
    iVar2 = DAT_08039c78 + 2;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039c84) {
    iVar2 = DAT_08039c78 + 3;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039c88) {
    iVar2 = DAT_08039c78 + 4;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039c8c) {
    FUN_0801bd34(DAT_08039c90,1);
    FUN_0801bd34(iVar1,0);
    return;
  }
  if (param_1 == DAT_08039c94) {
    iVar2 = DAT_08039c90 + 0xf;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039c98) {
    iVar2 = DAT_08039c90 + 0x10;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039c9c) {
    iVar2 = DAT_08039c90 + 0x11;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039ca0) {
    iVar2 = DAT_08039c78 + 5;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039ca4) {
    iVar2 = DAT_08039c78 + 6;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039ca8) {
    iVar2 = DAT_08039c78 + 7;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  if (param_1 == DAT_08039cac) {
    iVar2 = DAT_08039c90 + 0x13;
    FUN_0801bd34(iVar2,1);
    FUN_0801bd34(iVar2,0);
    return;
  }
  return;
}

