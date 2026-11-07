// 0x0801c6a2 FUN_0801c6a2 size=444

void FUN_0801c6a2(int param_1,int param_2,int *param_3)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 local_6c [3];
  undefined2 local_60;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  undefined1 local_48 [8];
  int local_40;
  undefined4 local_3c;
  int local_38;
  int local_34;
  int local_30;
  uint local_2c;
  undefined1 auStack_28 [16];
  
  if (((*(int *)(param_2 + 0x14) != 0) && (2 < *(byte *)(param_2 + 0x1e))) &&
     (iVar1 = FUN_08017484(auStack_28,param_3,*(undefined4 *)(param_1 + 8)), iVar1 != 0)) {
    uVar3 = *(undefined4 *)(param_1 + 8);
    *(undefined1 **)(param_1 + 8) = auStack_28;
    iVar1 = FUN_0802c55a(*(undefined4 *)(param_2 + 0x14));
    if (iVar1 == 2) {
      FUN_08033a84(&local_30,*(undefined4 *)(param_2 + 0x14),*(undefined4 *)(param_2 + 0x18),0,0,
                   0x1fffffff,0);
      local_40 = (*param_3 + ((param_3[2] - *param_3) + 1) / 2) - local_30 / 2;
      local_38 = local_30 + local_40 + -1;
      local_3c = (param_3[1] + ((param_3[3] - param_3[1]) + 1) / 2) - (int)local_2c / 2;
      local_34 = local_3c + local_2c + -1;
      FUN_08028d04(local_6c);
      local_6c[0] = *(undefined4 *)(param_2 + 0x18);
      local_60 = *(undefined2 *)(param_2 + 0x1c);
      local_48[0] = *(undefined1 *)(param_2 + 0x1e);
      FUN_08028800(param_1,local_6c,&local_40,*(undefined4 *)(param_2 + 0x14),0);
    }
    else {
      iVar1 = FUN_0802c05c(*(undefined4 *)(param_2 + 0x14),&local_2c);
      if (iVar1 == 1) {
        FUN_080287cc(local_48);
        local_38 = CONCAT31(local_38._1_3_,*(undefined1 *)(param_2 + 4));
        local_3c = CONCAT13(*(undefined1 *)(param_2 + 0x1e),
                            CONCAT12(*(undefined1 *)(param_2 + 0x1f),*(undefined2 *)(param_2 + 0x1c)
                                    ));
        if (*(char *)(param_2 + 0x20) == '\0') {
          local_58 = (*param_3 + ((param_3[2] - *param_3) + 1) / 2) - ((local_2c & 0x1fffff) >> 0xb)
          ;
          local_54 = (param_3[1] + ((param_3[3] - param_3[1]) + 1) / 2) - (local_2c >> 0x16);
          local_50 = (((local_2c & 0x1fffff) >> 10) - 1) + local_58;
          local_4c = ((local_2c >> 0x15) - 1) + local_54;
          FUN_08028758(param_1,local_48,&local_58,*(undefined4 *)(param_2 + 0x14));
        }
        else {
          local_54 = param_3[1];
          local_4c = local_54 + ((local_2c >> 0x15) - 1);
          for (; local_54 <= param_3[3]; local_54 = local_54 + (local_2c >> 0x15)) {
            local_58 = *param_3;
            local_50 = (((local_2c & 0x1fffff) >> 10) - 1) + local_58;
            for (; local_58 <= param_3[2]; local_58 = local_58 + uVar2) {
              FUN_08028758(param_1,local_48,&local_58,*(undefined4 *)(param_2 + 0x14));
              uVar2 = (local_2c & 0x1fffff) >> 10;
              local_50 = local_50 + uVar2;
            }
            local_4c = local_4c + (local_2c >> 0x15);
          }
        }
      }
    }
    *(undefined4 *)(param_1 + 8) = uVar3;
  }
  return;
}

