// 0x080171c0 FUN_080171c0 size=84

void FUN_080171c0(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int local_18;
  
  local_18 = 0;
  disableIRQinterrupts();
  FUN_08021812(DAT_08017214,&local_18,2);
  iVar2 = DAT_08017220;
  iVar1 = DAT_0801721c;
  if (local_18 == DAT_08017218) {
    uVar3 = 0;
    do {
      *(uint *)(iVar1 + 4) = 0xd - uVar3;
      FUN_08021778(*(uint *)(iVar1 + 4) & 0xff);
      if (*(char *)(iVar2 + 0x87) == 'Z') break;
      uVar3 = uVar3 + 1 & 0xff;
    } while (uVar3 < 0xe);
  }
  FUN_08026390();
  FUN_08006c90();
  enableIRQinterrupts();
  return;
}

