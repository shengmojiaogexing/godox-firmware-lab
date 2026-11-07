// 0x0802d97a FUN_0802d97a size=168

int FUN_0802d97a(int param_1,undefined4 param_2,undefined1 param_3,undefined4 param_4)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  uint uVar5;
  
  FUN_0802dee0(param_1);
  pbVar1 = (byte *)(*(int *)(param_1 + 8) + 0x22);
  bVar2 = *pbVar1;
  bVar3 = bVar2 >> 6;
  uVar5 = ((uint)bVar3 | (uint)bVar2 << 0x1a) + 0x4000000;
  *pbVar1 = (byte)(uVar5 >> 0x1a) | bVar3 << 6;
  iVar4 = FUN_0802d72c(*(undefined4 *)(*(int *)(param_1 + 8) + 0xc),(uVar5 >> 0x1a) * 0xc);
  *(int *)(*(int *)(param_1 + 8) + 0xc) = iVar4;
  if (iVar4 != 0) {
    *(undefined4 *)(iVar4 + ((*(byte *)(*(int *)(param_1 + 8) + 0x22) & 0x3f) - 1) * 0xc) = param_2;
    *(undefined1 *)
     (*(int *)(*(int *)(param_1 + 8) + 0xc) +
     ((*(byte *)(*(int *)(param_1 + 8) + 0x22) & 0x3f) - 1) * 0xc + 8) = param_3;
    *(undefined4 *)
     (*(int *)(*(int *)(param_1 + 8) + 0xc) +
     ((*(byte *)(*(int *)(param_1 + 8) + 0x22) & 0x3f) - 1) * 0xc + 4) = param_4;
    return *(int *)(*(int *)(param_1 + 8) + 0xc) +
           ((*(byte *)(*(int *)(param_1 + 8) + 0x22) & 0x3f) - 1) * 0xc;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

