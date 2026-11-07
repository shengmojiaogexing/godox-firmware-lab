// 0x08023a7c FUN_08023a7c size=48

void FUN_08023a7c(void)

{
  undefined4 uVar1;
  int iVar2;
  byte bVar3;
  
  uVar1 = DAT_08023aac;
  bVar3 = 0;
  FUN_08022e34(DAT_08023aac,0x4000);
  do {
    thunk_FUN_0801c118(1);
    bVar3 = bVar3 + 1;
    if (200 < bVar3) {
      return;
    }
    iVar2 = FUN_08022eb4(uVar1,0x8000);
  } while (iVar2 == 1);
  return;
}

