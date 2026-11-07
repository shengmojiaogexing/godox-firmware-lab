// 0x080112e8 FUN_080112e8 size=178

void FUN_080112e8(void)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int local_20;
  
  bVar1 = true;
  local_20 = 0;
  disableIRQinterrupts();
  FUN_08021812(DAT_0801139c,&local_20,2);
  iVar3 = DAT_080113a8;
  iVar2 = DAT_080113a4;
  if (local_20 == DAT_080113a0) {
    uVar4 = 0;
    do {
      *(uint *)(iVar2 + 4) = 0xd - uVar4;
      FUN_08021778(*(uint *)(iVar2 + 4) & 0xff);
      if (*(char *)(iVar3 + 0x87) == 'Z') {
        bVar1 = false;
        break;
      }
      uVar4 = uVar4 + 1 & 0xff;
    } while (uVar4 < 0xe);
    if (bVar1) {
      FUN_0802170c();
      FUN_0801c0fe(0x1e);
      FUN_080073f0();
      *(undefined4 *)(iVar2 + 4) = 0;
      FUN_080219e4();
    }
    else if (*(int *)(iVar2 + 4) == 0xd) {
      FUN_0802170c();
      FUN_0801c0fe(0x1e);
      *(undefined4 *)(iVar2 + 4) = 0;
      FUN_080219e4();
    }
  }
  else {
    FUN_0802170c();
    FUN_0801c0fe(0x1e);
    FUN_080073f0();
    *DAT_080113ac = 1;
    *(undefined4 *)(iVar2 + 4) = 0;
    FUN_08006c90();
    FUN_080219e4();
  }
  FUN_08026390();
  FUN_08006c90();
  enableIRQinterrupts();
  return;
}

