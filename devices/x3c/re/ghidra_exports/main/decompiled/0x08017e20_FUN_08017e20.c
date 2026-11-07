// 0x08017e20 FUN_08017e20 size=226

void FUN_08017e20(int *param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  undefined1 auStack_30 [16];
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  
  if (param_1 == (int *)0x0) {
    param_1 = (int *)FUN_0802821c();
  }
  if (((param_1 != (int *)0x0) && (iVar1 = FUN_0802830e(param_1), iVar1 != 0)) &&
     (-1 < (int)((uint)*(byte *)(param_1 + 10) << 0x1d))) {
    if (param_2 == 0) {
      *(undefined2 *)(param_1 + 0x94) = 0;
    }
    else {
      local_20 = 0;
      local_1c = 0;
      local_18 = FUN_08028248(param_1);
      local_18 = local_18 + -1;
      local_14 = FUN_080282e6(param_1);
      local_14 = local_14 + -1;
      iVar1 = FUN_08017484(auStack_30,param_2,&local_20);
      if (iVar1 != 0) {
        iVar1 = *param_1;
        if ((int)((uint)*(byte *)(iVar1 + 0x1c) << 0x1e) < 0) {
          param_1[0xc] = local_20;
          param_1[0xd] = local_1c;
          param_1[0xe] = local_18;
          param_1[0xf] = local_14;
          *(undefined2 *)(param_1 + 0x94) = 1;
          if (param_1[1] != 0) {
            FUN_080336c4();
          }
        }
        else {
          if (*(code **)(iVar1 + 0x24) != (code *)0x0) {
            (**(code **)(iVar1 + 0x24))(iVar1,auStack_30);
          }
          for (uVar2 = 0; uVar2 < *(ushort *)(param_1 + 0x94); uVar2 = uVar2 + 1 & 0xffff) {
            iVar1 = FUN_080174c4(auStack_30,param_1 + uVar2 * 4 + 0xc,0);
            if (iVar1 != 0) {
              return;
            }
          }
          if (*(ushort *)(param_1 + 0x94) < 0x20) {
            FUN_0802740c(param_1 + (uint)*(ushort *)(param_1 + 0x94) * 4 + 0xc,auStack_30);
          }
          else {
            *(undefined2 *)(param_1 + 0x94) = 0;
            FUN_0802740c(param_1 + 0xc,&local_20);
          }
          *(short *)(param_1 + 0x94) = (short)param_1[0x94] + 1;
          if (param_1[1] != 0) {
            FUN_080336c4();
          }
        }
      }
    }
  }
  return;
}

