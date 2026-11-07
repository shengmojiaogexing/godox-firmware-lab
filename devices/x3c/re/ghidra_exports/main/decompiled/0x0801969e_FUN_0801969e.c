// 0x0801969e FUN_0801969e size=76

int FUN_0801969e(undefined4 param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  undefined4 local_18;
  int local_14;
  
  local_14 = 0;
  local_18 = 0;
  iVar2 = 0;
  if ((param_2 != 0) && (FUN_0803502e(param_2,&local_14,&local_18), local_14 < 0xb)) {
    iVar2 = FUN_080372ee(param_1,&local_14,&local_18);
  }
  if (iVar2 != 0) {
    uVar1 = FUN_08019848(iVar2);
    if (uVar1 < param_2) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    FUN_08036974(param_1,iVar2,local_14,local_18);
  }
  return iVar2;
}

