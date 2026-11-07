// 0x08037618 FUN_08037618 size=154

int FUN_08037618(int param_1,int param_2,uint param_3)

{
  undefined4 uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  undefined1 auStack_44 [15];
  undefined1 local_35;
  undefined1 auStack_34 [2];
  undefined2 local_32;
  undefined2 local_30;
  undefined2 local_2e;
  undefined2 local_2c;
  undefined2 local_2a;
  undefined2 local_28;
  undefined1 local_26;
  
  iVar3 = -1;
  local_26 = 0x30;
  do {
    if (param_3 == 0) {
      return iVar3;
    }
    FUN_0803ba4c();
    uVar2 = 0x400;
    if (param_3 < 0x401) {
      uVar2 = param_3;
    }
    uVar1 = FUN_08019abc(param_1,uVar2 & 0xffff);
    local_32 = (undefined2)param_2;
    local_2c = 0;
    local_2e = (undefined2)uVar1;
    local_2a = (undefined2)((uint)uVar1 >> 0x10);
    local_28 = (undefined2)((uint)param_2 >> 0x10);
    uVar4 = 0;
    local_30 = (short)uVar2;
    do {
      do {
        uVar4 = uVar4 + 1;
        if (3 < uVar4) goto LAB_0803769e;
        iVar3 = FUN_08037a36(0x20002000,param_1,uVar2 & 0xffff);
      } while (iVar3 != 0);
      local_35 = 0xf;
      iVar3 = FUN_0801aff4(auStack_34,auStack_44,200,0x14);
    } while (iVar3 != 0);
LAB_0803769e:
    param_3 = param_3 - uVar2;
    param_2 = param_2 + uVar2;
    param_1 = param_1 + uVar2;
    if (iVar3 != 0) {
      return iVar3;
    }
  } while( true );
}

