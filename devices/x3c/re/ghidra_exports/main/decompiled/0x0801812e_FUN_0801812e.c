// 0x0801812e FUN_0801812e size=80

uint FUN_0801812e(int param_1,undefined4 param_2,uint param_3)

{
  char cVar1;
  ushort uVar2;
  int *piVar3;
  uint local_18;
  
  local_18 = param_3;
  if ((((param_1 != 0) && (piVar3 = (int *)FUN_0802f9bc(param_1,param_2,0x61), piVar3 != (int *)0x0)
       ) && (*piVar3 != 0)) && (cVar1 = FUN_0802f9bc(param_1,param_2,0x62), cVar1 != '\0')) {
    uVar2 = (*(code *)*piVar3)(piVar3,param_3);
    local_18 = (uint)uVar2;
  }
  return local_18;
}

