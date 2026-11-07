// 0x08023494 FUN_08023494 size=136

void FUN_08023494(void)

{
  undefined4 uVar1;
  int iVar2;
  byte bVar3;
  
  uVar1 = DAT_0802351c;
  bVar3 = 0;
  FUN_08022e38(DAT_0802351c,0x4000);
  thunk_FUN_0801c118(0x19);
  FUN_08022e34(uVar1,0x4000);
  thunk_FUN_0801c118(0x19);
  FUN_08022e38(uVar1,0x4000);
  thunk_FUN_0801c118(0x3c);
  FUN_08022e34(uVar1,0x4000);
  do {
    thunk_FUN_0801c118(1);
    bVar3 = bVar3 + 1;
    if (200 < bVar3) break;
    iVar2 = FUN_08022eb4(uVar1,0x8000);
  } while (iVar2 == 1);
  FUN_0802377c(0x30);
  bVar3 = 0;
  do {
    thunk_FUN_0801c118(1);
    bVar3 = bVar3 + 1;
    if (200 < bVar3) break;
    iVar2 = FUN_08022eb4(uVar1,0x8000);
  } while (iVar2 == 1);
  FUN_08022e38(uVar1,0x4000);
  return;
}

