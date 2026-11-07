// 0x0800385c FUN_0800385c size=68

void FUN_0800385c(uint param_1,int param_2)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  
  puVar1 = DAT_080038a0;
  *DAT_080038a0 = *DAT_080038a0 & 0xfffffffd;
  *puVar1 = *puVar1 & 0xfffffffe | param_1 & 1;
  iVar2 = DAT_080038a4;
  *(uint *)(DAT_080038a4 + 0x10) = *(uint *)(DAT_080038a4 + 0x10) | 4;
  if (param_2 == 0) {
    WaitForInterrupt();
  }
  else {
    if (param_2 != 1) {
      if (param_2 != 2) {
        return;
      }
      uVar3 = *(uint *)(iVar2 + 0x10) | 2;
      goto LAB_08003890;
    }
    WaitForEvent();
  }
  uVar3 = *(uint *)(iVar2 + 0x10) & 0xfffffffb;
LAB_08003890:
  *(uint *)(iVar2 + 0x10) = uVar3;
  return;
}

