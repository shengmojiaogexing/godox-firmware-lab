// 0x08025a04 FUN_08025a04 size=276

void FUN_08025a04(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  ushort uVar3;
  undefined4 uVar4;
  undefined4 local_20;
  undefined4 local_1c;
  
  local_20 = param_3;
  local_1c = param_4;
  FUN_0803babc();
  uVar4 = DAT_08025b18;
  FUN_08022e38(DAT_08025b18,0x400);
  local_20 = 0x400;
  local_1c = 0x1010000;
  FUN_08022e50(uVar4,&local_20);
  FUN_08022e38(uVar4,0x400);
  FUN_0801c0fe(1);
  FUN_08022e34(uVar4,0x400);
  FUN_0801c0fe(10);
  FUN_08022e38(uVar4,0x400);
  FUN_0801c0fe(0x78);
  iVar2 = DAT_08025b20;
  iVar1 = DAT_08025b1c;
  if (*(char *)(DAT_08025b1c + 3) == -0x80) {
    FUN_08025c54();
    FUN_0801c0fe(0x14);
    *(undefined4 *)(iVar1 + 0xc) = 0;
  }
  else if (*(char *)(DAT_08025b1c + 3) == -0x7f) {
    FUN_08035a64();
    FUN_0801c0fe(0x14);
    *(int *)(iVar1 + 0xc) = iVar2;
  }
  else {
    FUN_08035a64();
    FUN_0801c0fe(0x14);
    FUN_08025b28();
    FUN_08022e38(uVar4,0x400);
    FUN_0801c0fe(1);
    FUN_08022e34(uVar4,0x400);
    FUN_0801c0fe(10);
    FUN_08022e38(uVar4,0x400);
    FUN_0801c0fe(0x78);
    FUN_0803babc();
    if (*(int *)(iVar1 + 0xc) == iVar2) {
      FUN_08035a64();
    }
    else {
      FUN_08025c54();
    }
    FUN_08013b80();
  }
  FUN_08015b38(0);
  FUN_0801c0fe(0x32);
  FUN_0800cf68();
  uVar4 = FUN_0803c384(*DAT_08025b24);
  FUN_0803c4a4(uVar4,1);
  uVar3 = FUN_0803c344();
  FUN_0801a234((uint)uVar3 % 0xff + 0x37 & 0xff);
  return;
}

