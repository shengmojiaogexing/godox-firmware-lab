// 0x0802a43c FUN_0802a43c size=134

void FUN_0802a43c(undefined4 *param_1,uint param_2,undefined4 param_3)

{
  int *piVar1;
  uint local_20;
  undefined4 uStack_1c;
  undefined4 local_18;
  
  local_18 = *param_1;
  if (*(int *)(param_2 + 0x3c) < 0) {
    local_20 = 5;
  }
  else {
    local_20 = 4;
  }
  local_20 = param_2 & 0x300 |
             (((uint)((ushort *)param_1[1])[4] - (uint)*(ushort *)param_1[1]) + 1 & 0x7ff) << 10 |
             (((uint)*(ushort *)(param_1[1] + 0xc) - (uint)*(ushort *)(param_1[1] + 4)) + 1) *
             0x200000 | local_20;
  *param_1 = *(undefined4 *)(param_2 + 0x34);
  param_1[1] = *(undefined4 *)(param_2 + 0x30);
  param_1[2] = *(undefined4 *)(param_2 + 0x2c);
  uStack_1c = param_3;
  piVar1 = (int *)FUN_08018528();
  *(uint *)(*piVar1 + 0x1c) =
       *(uint *)(*piVar1 + 0x1c) & 0xffffffbf | (*(byte *)(param_2 + 0x38) & 1) << 6;
  FUN_08028758(param_1,param_3,param_2 + 0x10,&local_20);
  FUN_0802acea(param_1);
  FUN_0802b902(&local_20);
  return;
}

