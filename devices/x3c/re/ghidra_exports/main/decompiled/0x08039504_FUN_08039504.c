// 0x08039504 FUN_08039504 size=862

void FUN_08039504(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  iVar3 = FUN_0802ee0a(param_2);
  uVar2 = DAT_08039868;
  iVar1 = DAT_08039864;
  if (iVar3 == 0) {
    FUN_0802da9c(param_2,*(undefined4 *)(DAT_08039864 + 0xc),0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 8,0x10000);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x10,uVar2);
    return;
  }
  iVar3 = FUN_0802e132(param_2,DAT_0803986c);
  if (iVar3 != 0) {
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x18,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 8,0x10000);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x10,uVar2);
    return;
  }
  iVar3 = FUN_0802e132(param_2,DAT_08039870);
  if (iVar3 != 0) {
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x20,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x28,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xd0,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x58,0x20);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xd8,0x20);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xa0,4);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 200,0x20);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x38,1);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x60,0x80);
    return;
  }
  iVar3 = FUN_0802e132(param_2,DAT_08039874);
  if (iVar3 != 0) {
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x118,0);
    return;
  }
  iVar3 = FUN_0802e132(param_2,DAT_08039878);
  if (iVar3 != 0) {
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x30,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xb0,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xa0,4);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xa8,8);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x28,0x20000);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xb0,0x20000);
    return;
  }
  iVar3 = FUN_0802e132(param_2,DAT_0803987c);
  if (iVar3 != 0) {
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x30,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xb0,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xa0,4);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xa8,8);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x28,0x20000);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xb0,0x20000);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xf0,0x30000);
    iVar3 = DAT_08039880;
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 200,DAT_08039880);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xd0,0x30000);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xd8,iVar3);
    return;
  }
  iVar3 = FUN_0802e132(param_2,DAT_08039884);
  if (iVar3 != 0) {
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x48,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xb0,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xe8,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x60,0x80);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xa0,4);
    iVar3 = DAT_08039888;
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x28,DAT_08039888);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xb0,0x20000);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x60,DAT_08039888 + 0x7f);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xf0,0x30000);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x50,0x30000);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x110,0x30000);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x60,DAT_08039880 + 0x60);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xd8,iVar3);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xd8,0x20000);
    return;
  }
  iVar3 = FUN_0802e132(param_2,DAT_0803988c);
  if (iVar3 != 0) {
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x18,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xe0,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x90,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x98,0);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xa0,4);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xa8,8);
    FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x28,0x40000);
    return;
  }
  iVar3 = FUN_0802e132(param_2,DAT_08039890);
  if (iVar3 == 0) {
    return;
  }
  FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x100,0);
  FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x100,0x20000);
  FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0x108,0x20000);
  FUN_0802da9c(param_2,*(int *)(iVar1 + 0xc) + 0xf0,0x30000);
  return;
}

