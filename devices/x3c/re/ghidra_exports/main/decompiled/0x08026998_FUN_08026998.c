// 0x08026998 FUN_08026998 size=44

void FUN_08026998(int param_1,int param_2)

{
  int *piVar1;
  
  piVar1 = (int *)FUN_08017f30(DAT_080269c4);
  while ((piVar1 != (int *)0x0 &&
         ((*piVar1 != param_1 || ((piVar1[1] != param_2 && (param_2 != 0))))))) {
    piVar1 = (int *)FUN_08017f5e(DAT_080269c4,piVar1);
  }
  return;
}

