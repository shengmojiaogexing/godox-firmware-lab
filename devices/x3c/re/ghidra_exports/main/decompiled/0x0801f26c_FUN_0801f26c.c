// 0x0801f26c FUN_0801f26c size=166

void FUN_0801f26c(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined1 auStack_c4 [4];
  undefined4 local_c0;
  undefined4 local_bc;
  undefined1 *local_b8;
  undefined1 *local_b4;
  undefined4 local_90;
  undefined1 auStack_7c [76];
  undefined1 auStack_30 [16];
  undefined1 auStack_20 [16];
  
  FUN_0802f0ec(param_1,auStack_20,auStack_30);
  iVar1 = FUN_080274b0(auStack_20);
  if (((iVar1 != 0) || (iVar1 = FUN_080274b0(auStack_30), iVar1 != 0)) &&
     (iVar1 = FUN_080371dc(param_1,auStack_7c), iVar1 == 1)) {
    FUN_0802e788(auStack_c4,param_2);
    local_c0 = DAT_0801f314;
    local_bc = 2;
    local_b4 = auStack_7c;
    local_90 = 0x10000;
    iVar1 = FUN_080274b0(auStack_20);
    if (iVar1 != 0) {
      local_b8 = auStack_20;
      FUN_0802ad74(param_1,0x1a,auStack_c4);
      FUN_08029a14(param_2,auStack_7c,auStack_20);
      FUN_0802ad74(param_1,0x1b,auStack_c4);
    }
    iVar1 = FUN_080274b0(auStack_30);
    if (iVar1 != 0) {
      local_b8 = auStack_30;
      FUN_0802ad74(param_1,0x1a,auStack_c4);
      local_b8 = auStack_30;
      FUN_08029a14(param_2,auStack_7c,auStack_30);
      FUN_0802ad74(param_1,0x1b,auStack_c4);
    }
  }
  return;
}

