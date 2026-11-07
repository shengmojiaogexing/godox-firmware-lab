// 0x08037ff4 FUN_08037ff4 size=78

void FUN_08037ff4(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  uint uVar1;
  uint uVar2;
  undefined1 auStack_8c [44];
  undefined1 auStack_60 [6];
  undefined2 local_5a;
  
  FUN_08029a38(auStack_60);
  uVar1 = FUN_08027b80(0xff,0xff);
  uVar2 = uVar1 & 0xffff;
  local_5a = (undefined2)uVar1;
  FUN_08029a14(param_1,auStack_60,param_2);
  FUN_08028d04(auStack_8c);
  FUN_08028800(param_1,auStack_8c,param_2,param_3,0,uVar1,uVar2);
  return;
}

