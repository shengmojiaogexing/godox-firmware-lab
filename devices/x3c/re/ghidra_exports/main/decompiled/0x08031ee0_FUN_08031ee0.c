// 0x08031ee0 FUN_08031ee0 size=218

void FUN_08031ee0(int param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  if (param_2 == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  uVar1 = FUN_080223b4(param_1);
  *(undefined2 *)(param_1 + 0x2e) = 0;
  *(undefined2 *)(param_1 + 0x30) = 0;
  *(undefined2 *)(param_1 + 0x2c) = 0;
  for (iVar2 = 0; *(char *)(param_2 + iVar2) != '\0'; iVar2 = iVar2 + 1) {
    if (*(char *)(param_2 + iVar2) == '\n') {
      *(short *)(param_1 + 0x2c) = *(short *)(param_1 + 0x2c) + 1;
    }
  }
  *(short *)(param_1 + 0x2c) = *(short *)(param_1 + 0x2c) + 1;
  if (param_3 == 0) {
    *(byte *)(param_1 + 0x32) = *(byte *)(param_1 + 0x32) & 0xfe;
    FUN_0802d33c(uVar1,param_2);
  }
  else {
    *(byte *)(param_1 + 0x32) = *(byte *)(param_1 + 0x32) | 1;
    iVar2 = FUN_08004df8(param_2);
    iVar2 = iVar2 + 1;
    iVar3 = FUN_0802d624(iVar2 * 7);
    uVar4 = 0;
    do {
      FUN_08004db0(iVar2 * uVar4 + iVar3,param_2);
      *(undefined1 *)(iVar2 * (uVar4 + 1) + iVar3 + -1) = 10;
      uVar4 = uVar4 + 1 & 0xff;
    } while (uVar4 < 7);
    *(undefined1 *)(iVar2 * 7 + iVar3 + -1) = 0;
    FUN_0802d33c(uVar1,iVar3);
    FUN_0802d6c0(iVar3);
    *(short *)(param_1 + 0x2e) = *(short *)(param_1 + 0x2c) * 3;
    *(short *)(param_1 + 0x2c) = *(short *)(param_1 + 0x2c) * 7;
    FUN_08024eb8(param_1);
  }
  *(undefined2 *)(param_1 + 0x30) = *(undefined2 *)(param_1 + 0x2e);
  FUN_08030c20(uVar1);
  return;
}

