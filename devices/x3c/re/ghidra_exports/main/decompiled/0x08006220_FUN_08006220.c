// 0x08006220 FUN_08006220 size=188

undefined4
FUN_08006220(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5
            ,undefined4 param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = FUN_08027954();
  FUN_08030e2a(uVar1,0,DAT_080062dc);
  FUN_08031696(uVar1,0xf,0);
  FUN_080313b2(uVar1,param_5,param_6);
  FUN_08031594(uVar1,0);
  FUN_080316a0(uVar1,0);
  FUN_080315b6(uVar1,0);
  FUN_0803161c(uVar1,0,4);
  FUN_08031630(uVar1,0,4);
  FUN_080315b6(uVar1,0);
  FUN_080315c0(uVar1,2,0);
  FUN_080315b6(uVar1,0xff,4);
  FUN_080315c0(uVar1,2,4);
  uVar2 = FUN_08027fcc();
  FUN_0803159e(uVar1,uVar2,4);
  if (param_2 == 0) {
    FUN_08031318(uVar1,param_3,param_4);
  }
  else {
    FUN_0802db44(uVar1,param_2,param_3,param_4,uVar2);
  }
  return uVar1;
}

