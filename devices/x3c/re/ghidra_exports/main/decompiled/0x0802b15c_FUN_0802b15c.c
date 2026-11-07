// 0x0802b15c FUN_0802b15c size=308

int FUN_0802b15c(undefined4 *param_1,int param_2,uint *param_3,uint *param_4)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  uint *puVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  uint *puVar8;
  uint *local_2c;
  
  iVar6 = 0;
  puVar1 = (uint *)param_1[2];
  uVar5 = puVar1[2];
  uVar7 = puVar1[3];
  iVar2 = param_1[1];
  puVar8 = (uint *)(uint)*(ushort *)(iVar2 + 2);
  local_2c = param_4;
  if ((uVar5 < *puVar1) || (puVar1[1] <= uVar5)) {
    if (puVar8 < param_3) {
      iVar6 = (**(code **)(iVar2 + 0x10))(iVar2,*param_1,param_2,param_3,param_4);
    }
    else {
      if (uVar7 == 0) {
        uVar7 = FUN_0802d5c4(puVar8);
        *(uint *)(param_1[2] + 0xc) = uVar7;
        if (uVar7 == 0) {
          do {
                    /* WARNING: Do nothing block with infinite loop */
          } while( true );
        }
      }
      local_2c = (uint *)0x0;
      iVar6 = (**(code **)(param_1[1] + 0x10))(param_1[1],*param_1,uVar7,puVar8,&local_2c);
      *(uint *)param_1[2] = uVar5;
      *(uint *)(param_1[2] + 4) = uVar5 + (int)local_2c;
      if (local_2c <= param_3) {
        param_3 = local_2c;
      }
      *param_4 = (uint)param_3;
      FUN_0802d764(param_2,uVar7,param_3);
    }
  }
  else {
    uVar3 = uVar5 - *puVar1 & 0xffff;
    puVar4 = (uint *)(puVar1[1] - uVar5);
    puVar1 = (uint *)((int)puVar8 - uVar3);
    if (puVar4 <= (uint *)((int)puVar8 - uVar3)) {
      puVar1 = puVar4;
    }
    if (puVar1 < param_3) {
      FUN_0802d764(param_2,uVar7 + uVar3,puVar1,param_4,param_3);
      local_2c = (uint *)0x0;
      if (puVar8 < param_3) {
        iVar6 = (**(code **)(param_1[1] + 0x10))
                          (param_1[1],*param_1,param_2 + (int)puVar1,(int)param_3 - (int)puVar1,
                           &local_2c);
      }
      else {
        iVar6 = (**(code **)(param_1[1] + 0x10))(param_1[1],*param_1,uVar7,puVar8,&local_2c);
        iVar2 = ((int *)param_1[2])[1];
        *(int *)param_1[2] = iVar2;
        *(int *)(param_1[2] + 4) = iVar2 + (int)local_2c;
        puVar8 = local_2c;
        if ((uint *)((int)param_3 - (int)puVar1) < local_2c) {
          puVar8 = (uint *)((int)param_3 - (int)puVar1);
        }
        FUN_0802d764(param_2 + (int)puVar1,uVar7,(uint)puVar8 & 0xffff);
      }
      if ((uint *)((int)local_2c + (int)puVar1) < param_3) {
        param_3 = (uint *)((int)local_2c + (int)puVar1);
      }
      *param_4 = (uint)param_3;
    }
    else {
      FUN_0802d764(param_2,uVar7 + uVar3,param_3,param_4,param_3);
      *param_4 = (uint)param_3;
    }
  }
  if (iVar6 == 0) {
    *(uint *)(param_1[2] + 8) = *(int *)(param_1[2] + 8) + *param_4;
  }
  return iVar6;
}

