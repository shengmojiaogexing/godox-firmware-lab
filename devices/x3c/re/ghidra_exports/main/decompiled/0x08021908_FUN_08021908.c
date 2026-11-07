// 0x08021908 FUN_08021908 size=102

void FUN_08021908(void)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  uint *puVar4;
  
  iVar3 = 0xa5;
  iVar2 = FUN_08021768();
  puVar1 = DAT_08021974;
  if (iVar2 != 0) {
    iVar3 = 0;
  }
  *DAT_08021974 = DAT_08021970;
  *puVar1 = DAT_08021978;
  puVar4 = DAT_08021974 + 2;
  do {
  } while (-1 < (int)(*puVar4 << 0x16));
  *puVar4 = *puVar4 | 0x20;
  *puVar4 = *puVar4 | 0x40;
  iVar2 = FUN_080217f4(0x80000000);
  *puVar4 = *puVar4 & 0xffffffdf;
  if ((iVar2 == 3) && (iVar3 == 0xa5)) {
    *puVar4 = *puVar4 | 0x10;
    *DAT_0802197c = 0xa5;
    FUN_080217f4(0x100000);
    *puVar4 = *puVar4 & 0xffffffef;
  }
  return;
}

