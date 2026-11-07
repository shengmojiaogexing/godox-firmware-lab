// 0x0802a5c4 FUN_0802a5c4 size=252

void FUN_0802a5c4(int param_1,int *param_2,int *param_3,uint param_4)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  undefined1 auStack_88 [5];
  undefined1 local_83;
  undefined1 local_6a;
  undefined2 local_66;
  undefined4 local_64;
  undefined1 local_54;
  undefined1 local_40;
  undefined1 auStack_3c [16];
  int local_2c;
  int local_28;
  int local_24;
  ushort local_1e;
  ushort local_1c;
  short local_1a;
  short local_18;
  
  iVar1 = FUN_0802ae6a(*param_2,&local_24,param_4,0);
  if (iVar1 == 0) {
    if (((0x1f < param_4) && (param_4 != 0xf8ff)) && (param_4 != 0x200c)) {
      FUN_08027518(auStack_3c,(int)local_1a + *param_3,(int)local_18 + param_3[1],
                   (uint)local_1e + (int)local_1a + *param_3,
                   (uint)local_1c + (int)local_18 + param_3[1]);
      FUN_08029a38(auStack_88);
      local_83 = 2;
      local_54 = 2;
      local_40 = 2;
      local_6a = 2;
      local_66 = (undefined2)param_2[3];
      local_64 = 1;
      (**(code **)(param_1 + 0x10))(param_1,auStack_88,auStack_3c);
    }
  }
  else {
    uVar3 = (uint)local_1c;
    if ((uVar3 != 0) && (local_1e != 0)) {
      local_2c = *param_3 + (int)local_1a;
      local_28 = (((*(int *)(*param_2 + 8) - *(int *)(*param_2 + 0xc)) + param_3[1]) - uVar3) -
                 (int)local_18;
      piVar2 = *(int **)(param_1 + 8);
      if ((((*piVar2 <= (int)(local_2c + (uint)local_1e)) &&
           ((local_2c <= piVar2[2] && (piVar2[1] <= (int)(uVar3 + local_28))))) &&
          (local_28 <= piVar2[3])) &&
         ((iVar1 = FUN_0802ae60(local_24,param_4), iVar1 != 0 &&
          ((*(byte *)(local_24 + 0x10) & 3) == 0)))) {
        FUN_0801dd34(param_1,param_2,&local_2c,&local_24,iVar1);
      }
    }
  }
  return;
}

