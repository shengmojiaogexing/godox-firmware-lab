// 0x08025b28 FUN_08025b28 size=270

void FUN_08025b28(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined4 local_20;
  undefined4 local_1c;
  
  local_20 = param_3;
  local_1c = param_4;
  FUN_0801bd04(0x300000,1);
  FUN_0801bd04(DAT_08025c38,1);
  FUN_0801bd04(DAT_08025c38 + 1,1);
  FUN_0801bd04(DAT_08025c38 + 2,1);
  uVar1 = DAT_08025c3c;
  FUN_08022e38(DAT_08025c3c,0x3c);
  local_20 = 0x3c;
  local_1c = 0x1010000;
  FUN_08022e50(uVar1,&local_20);
  uVar1 = DAT_08025c40;
  FUN_08022e38(DAT_08025c40,0x4400);
  local_20 = 0x4400;
  local_1c = 0x1010000;
  FUN_08022e50(uVar1,&local_20);
  uVar2 = DAT_08025c44;
  FUN_08022e38(DAT_08025c44,0x1878);
  local_20 = 0x1878;
  local_1c = 0x1010000;
  FUN_08022e50(uVar2,&local_20);
  uVar2 = DAT_08025c48;
  FUN_08022e38(DAT_08025c48,4);
  local_20 = 4;
  local_1c = 0x1010000;
  FUN_08022e50(uVar2,&local_20);
  FUN_08022e38(uVar1,0x800);
  local_20 = 0x800;
  local_1c = 0x1010000;
  FUN_08022e50(uVar1,&local_20);
  iVar4 = FUN_08012424(0xa1);
  iVar3 = DAT_08025c4c;
  *(int *)(DAT_08025c4c + 0xc) = iVar4;
  if (iVar4 == DAT_08025c50) {
    *(undefined1 *)(iVar3 + 3) = 0x81;
  }
  else {
    *(undefined1 *)(iVar3 + 3) = 0x80;
  }
  return;
}

