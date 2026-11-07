// 0x0802b08c FUN_0802b08c size=126

undefined4 FUN_0802b08c(int *param_1,undefined1 *param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (param_2 == (undefined1 *)0x0) {
    uVar3 = 0xb;
  }
  else {
    iVar1 = FUN_0802b01c(*param_2);
    if (iVar1 == 0) {
      uVar3 = 3;
    }
    else if ((*(code **)(iVar1 + 4) == (code *)0x0) ||
            (iVar2 = (**(code **)(iVar1 + 4))(), iVar2 != 0)) {
      if (*(int *)(iVar1 + 8) == 0) {
        uVar3 = 9;
      }
      else {
        uVar3 = FUN_0802b080(param_2);
        iVar2 = (**(code **)(iVar1 + 8))(iVar1,uVar3,param_3);
        if ((iVar2 == 0) || (iVar2 == -1)) {
          uVar3 = 0xc;
        }
        else {
          param_1[1] = iVar1;
          *param_1 = iVar2;
          if (*(short *)(iVar1 + 2) != 0) {
            iVar1 = FUN_0802d5c4(0x10);
            param_1[2] = iVar1;
            if (iVar1 == 0) {
              do {
                    /* WARNING: Do nothing block with infinite loop */
              } while( true );
            }
            FUN_0802d8d6(iVar1,0x10);
            *(undefined4 *)param_1[2] = 0xffffffff;
            *(undefined4 *)(param_1[2] + 4) = 0xfffffffe;
          }
          uVar3 = 0;
        }
      }
    }
    else {
      uVar3 = 1;
    }
  }
  return uVar3;
}

