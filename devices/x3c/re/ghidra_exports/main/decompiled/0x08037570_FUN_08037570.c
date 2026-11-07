// 0x08037570 FUN_08037570 size=162

int FUN_08037570(int param_1,int param_2,uint param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  undefined1 auStack_40 [2];
  ushort local_3e;
  ushort local_3c;
  undefined1 local_31;
  undefined1 auStack_30 [2];
  undefined2 local_2e;
  undefined2 local_2c;
  undefined2 local_2a;
  undefined2 local_28;
  undefined2 local_24;
  undefined1 local_22;
  
  iVar2 = -1;
  local_22 = 0x31;
  do {
    if (param_3 == 0) {
      return iVar2;
    }
    uVar1 = 0x400;
    if (param_3 < 0x401) {
      uVar1 = param_3;
    }
    local_2e = (undefined2)param_2;
    local_2c = (undefined2)uVar1;
    local_2a = 0;
    local_28 = 3;
    local_24 = (undefined2)((uint)param_2 >> 0x10);
    uVar3 = 0;
    while (uVar4 = uVar3 + 1, uVar3 < 3) {
      local_31 = 0xf;
      iVar2 = FUN_0801aff4(auStack_30,auStack_40,0x14,10);
      uVar3 = uVar4;
      if (iVar2 == 0) {
        FUN_080376b4(DAT_08037614,param_1,uVar1 & 0xffff);
        uVar4 = FUN_08019abc(param_1,uVar1 & 0xffff);
        if (((uint)local_3e == (uVar4 & 0xffff)) && ((uint)local_3c == uVar4 >> 0x10)) break;
        iVar2 = -4;
      }
    }
    param_2 = param_2 + uVar1;
    param_3 = param_3 - uVar1;
    param_1 = param_1 + uVar1;
    if (iVar2 != 0) {
      return iVar2;
    }
  } while( true );
}

