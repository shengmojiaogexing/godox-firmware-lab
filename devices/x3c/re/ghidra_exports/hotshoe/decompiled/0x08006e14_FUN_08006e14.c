// 0x08006e14 FUN_08006e14 size=90

void FUN_08006e14(void)

{
  uint *puVar1;
  uint *puVar2;
  uint uVar3;
  
  puVar1 = DAT_08006e70;
  *DAT_08006e70 = *DAT_08006e70 | 0x10000;
  puVar2 = DAT_08006e74;
  uVar3 = 0;
  do {
    if ((int)(*puVar1 << 0xe) < 0) break;
    uVar3 = uVar3 + 1;
  } while (uVar3 < 0x10000);
  if ((int)(*puVar1 << 0xe) < 0) {
    *DAT_08006e74 = *DAT_08006e74 | 0x10;
    *puVar2 = *puVar2 & 0xfffffff8;
    puVar1[1] = puVar1[1] & 0xffffff0f;
    puVar1[1] = puVar1[1] & 0xfffff8ff;
    puVar1[1] = (puVar1[1] & 0xfffffffc) + 1;
    do {
    } while ((puVar1[1] & 0xf) >> 2 != 1);
  }
  return;
}

