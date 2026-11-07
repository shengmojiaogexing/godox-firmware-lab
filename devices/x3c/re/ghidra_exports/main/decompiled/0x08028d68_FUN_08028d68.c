// 0x08028d68 FUN_08028d68 size=102

int FUN_08028d68(undefined4 *param_1,undefined4 *param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  
  if (param_1[0xf] == 0) {
    iVar3 = 0;
  }
  else {
    puVar1 = (undefined4 *)FUN_0802d5c4(param_1[0x13]);
    if (puVar1 == (undefined4 *)0x0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    FUN_0802d8d6(puVar1,param_1[0x13]);
    piVar2 = (int *)FUN_08018528();
    puVar1[0xd] = *param_1;
    puVar1[0xc] = param_1[1];
    puVar1[0xb] = param_1[2];
    *(char *)(puVar1 + 0xe) = (char)((*(byte *)(*piVar2 + 0x1c) & 0x7f) >> 6);
    uVar4 = param_2[1];
    uVar5 = param_2[2];
    uVar6 = param_2[3];
    *puVar1 = *param_2;
    puVar1[1] = uVar4;
    puVar1[2] = uVar5;
    puVar1[3] = uVar6;
    iVar3 = (*(code *)param_1[0xf])(param_1,puVar1,param_3);
    if (iVar3 == 0) {
      FUN_0802d6e4(puVar1);
    }
  }
  return iVar3;
}

