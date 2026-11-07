// 0x0802c0bc FUN_0802c0bc size=196

int FUN_0802c0bc(undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 *puVar2;
  undefined2 uVar3;
  
  FUN_0802d8d6(param_1,0x28,param_3,param_4,param_1,param_2,param_3,param_4);
  uVar3 = (undefined2)param_3;
  if (param_2 == 0) {
    iVar1 = 0;
  }
  else {
    iVar1 = FUN_0802c55a(param_2);
    if ((iVar1 == 0) && (*(int *)(param_2 + 8) == 0)) {
      iVar1 = 0;
    }
    else {
      *(undefined2 *)(param_1 + 2) = uVar3;
      *(char *)(param_1 + 4) = (char)iVar1;
      param_1[3] = param_4;
      if (iVar1 == 1) {
        iVar1 = FUN_08004df8(param_2);
        iVar1 = FUN_0802d5c4(iVar1 + 1);
        param_1[1] = iVar1;
        if (iVar1 == 0) {
          do {
                    /* WARNING: Do nothing block with infinite loop */
          } while( true );
        }
        FUN_08004db0(iVar1,param_2);
      }
      else {
        param_1[1] = param_2;
      }
      iVar1 = 0;
      puVar2 = (undefined4 *)FUN_08017f30(DAT_0802c180);
      for (; puVar2 != (undefined4 *)0x0; puVar2 = (undefined4 *)FUN_08017f5e(DAT_0802c180,puVar2))
      {
        if ((((code *)*puVar2 != (code *)0x0) && (puVar2[1] != 0)) &&
           (iVar1 = (*(code *)*puVar2)(puVar2,param_2,param_1 + 5), iVar1 == 1)) {
          *param_1 = puVar2;
          iVar1 = (*(code *)puVar2[1])(puVar2,param_1);
          if (iVar1 == 1) {
            return 1;
          }
          FUN_0802d8d6(param_1 + 5,4);
          param_1[8] = 0;
          param_1[6] = 0;
          param_1[9] = 0;
          param_1[7] = 0;
        }
      }
      if (*(char *)(param_1 + 4) == '\x01') {
        FUN_0802d6e4(param_1[1]);
      }
    }
  }
  return iVar1;
}

