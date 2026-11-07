// 0x08029220 FUN_08029220 size=68

undefined4 FUN_08029220(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  bool bVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  int *piVar5;
  
  bVar1 = false;
  piVar5 = DAT_08029264;
  while( true ) {
    puVar3 = (undefined4 *)*piVar5;
    if (puVar3 == (undefined4 *)0x0) {
      if (bVar1) {
        uVar4 = 2;
      }
      else {
        uVar4 = 1;
      }
      return uVar4;
    }
    iVar2 = (*(code *)*puVar3)(param_1,param_2,param_3,param_4,puVar3);
    if (iVar2 == 0) break;
    if (iVar2 == 2) {
      bVar1 = true;
    }
    piVar5 = piVar5 + 2;
  }
  return 0;
}

