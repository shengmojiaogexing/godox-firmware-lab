// 0x0801905c FUN_0801905c size=208

void FUN_0801905c(void)

{
  byte bVar1;
  char *pcVar2;
  int iVar3;
  undefined4 *puVar4;
  uint uVar5;
  int iVar6;
  undefined4 uVar7;
  
  pcVar2 = DAT_0801912c;
  iVar3 = FUN_080334c0(*(undefined4 *)(DAT_0801912c + 4));
  pcVar2[1] = pcVar2[1] ^ 1;
  puVar4 = (undefined4 *)FUN_08017f30(DAT_08019130);
  while (puVar4 != (undefined4 *)0x0) {
    *pcVar2 = '\0';
    bVar1 = *(byte *)((int)puVar4 + 0x42);
    if (-((int)((uint)bVar1 << 0x1d) >> 0x1f) != (uint)(byte)pcVar2[1]) {
      uVar5 = bVar1 & 0xfffffffb | ((byte)pcVar2[1] & 1) << 2;
      *(char *)((int)puVar4 + 0x42) = (char)uVar5;
      if (((-1 < (int)(uVar5 << 0x1c)) && ((int)puVar4[0xc] < 1)) && (-1 < puVar4[0xc] + iVar3)) {
        if (((bVar1 & 1) == 0) && ((code *)puVar4[5] != (code *)0x0)) {
          iVar6 = (*(code *)puVar4[5])(puVar4);
          puVar4[8] = puVar4[8] + iVar6;
          puVar4[10] = iVar6 + puVar4[10];
        }
        if ((code *)puVar4[2] != (code *)0x0) {
          (*(code *)puVar4[2])(puVar4);
        }
        *(byte *)((int)puVar4 + 0x42) = *(byte *)((int)puVar4 + 0x42) | 8;
      }
      iVar6 = puVar4[0xc] + iVar3;
      puVar4[0xc] = iVar6;
      if (-1 < iVar6) {
        if ((int)puVar4[0xb] < iVar6) {
          puVar4[0xc] = puVar4[0xb];
        }
        iVar6 = (*(code *)puVar4[7])(puVar4);
        if (puVar4[9] != iVar6) {
          puVar4[9] = iVar6;
          if ((code *)puVar4[1] != (code *)0x0) {
            (*(code *)puVar4[1])(*puVar4);
          }
        }
        if ((int)puVar4[0xb] <= (int)puVar4[0xc]) {
          FUN_08018fd4(puVar4);
        }
      }
    }
    if (*pcVar2 == '\0') {
      puVar4 = (undefined4 *)FUN_08017f5e(DAT_08019130,puVar4);
    }
    else {
      puVar4 = (undefined4 *)FUN_08017f30(DAT_08019130);
    }
  }
  uVar7 = FUN_080334d8();
  *(undefined4 *)(pcVar2 + 4) = uVar7;
  return;
}

