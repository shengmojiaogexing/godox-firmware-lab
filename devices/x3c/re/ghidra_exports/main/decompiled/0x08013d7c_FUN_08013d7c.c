// 0x08013d7c FUN_08013d7c size=288

void FUN_08013d7c(int param_1)

{
  char cVar1;
  int iVar2;
  uint *puVar3;
  byte bVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  
  iVar2 = DAT_08013e9c;
  cVar1 = *(char *)(param_1 + 8);
  if (cVar1 == '\x1c') {
    if (*(char *)(DAT_08013e9c + 0xb) == '\0') {
      bVar4 = FUN_08031e48(*(int *)(param_1 + 4));
      uVar7 = (uint)bVar4;
      if (uVar7 < 5) {
        *(byte *)(iVar2 + 2) = *(byte *)(iVar2 + 2) & 0xf8 | bVar4;
        puVar3 = DAT_08013ea4;
        *DAT_08013ea4 = (uint)*(ushort *)(DAT_08013ea0 + uVar7 * 2);
        *DAT_08013ea8 = *puVar3;
        *DAT_08013eac = *puVar3;
      }
      FUN_0802e2d2(*(undefined4 *)(iVar2 + 0x7c),1);
      if (uVar7 < 2) {
        iVar6 = FUN_080303b8(*(undefined4 *)(iVar2 + 0x7c));
        if (iVar6 != 0) {
          if (*(char *)(iVar2 + 3) == '\0') {
            FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_08013eb4);
            return;
          }
          if (*(char *)(iVar2 + 3) == '\x01') {
            FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_08013eb0);
            return;
          }
        }
      }
      else {
        iVar6 = FUN_080303b8(*(undefined4 *)(iVar2 + 0x7c));
        if (iVar6 != 0) {
          if (*(char *)(iVar2 + 3) == '\0') {
            FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_08013ec0);
            return;
          }
          if (*(char *)(iVar2 + 3) == '\x01') {
            FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_08013ebc);
            return;
          }
        }
      }
    }
  }
  else {
    uVar5 = *(undefined4 *)(DAT_08013e9c + 0x7c);
    if (cVar1 == '\r') {
      if (*(char *)(DAT_08013e9c + 0xb) == '\0') {
        if (*(short *)(*(int *)(param_1 + 4) + 0x2e) < 2) {
          iVar6 = FUN_080303b8(uVar5);
          if (iVar6 != 0) {
            if (*(char *)(iVar2 + 3) == '\0') {
              FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_08013eb4);
              return;
            }
            if (*(char *)(iVar2 + 3) == '\x01') {
              FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_08013eb0);
              return;
            }
          }
        }
        else {
          iVar6 = FUN_080303b8(uVar5);
          if (iVar6 != 0) {
            if (*(char *)(iVar2 + 3) == '\0') {
              FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_08013ec0);
              return;
            }
            if (*(char *)(iVar2 + 3) == '\x01') {
              FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_08013ebc);
              return;
            }
          }
        }
      }
    }
    else if ((cVar1 == '\v') && (iVar6 = FUN_080303b8(uVar5), iVar6 != 0)) {
      FUN_0802da22(*(undefined4 *)(iVar2 + 0x7c),1);
      return;
    }
  }
  return;
}

