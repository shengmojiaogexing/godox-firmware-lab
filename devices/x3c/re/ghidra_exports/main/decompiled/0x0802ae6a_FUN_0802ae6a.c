// 0x0802ae6a FUN_0802ae6a size=174

undefined4 FUN_0802ae6a(undefined4 *param_1,int *param_2,uint param_3,undefined4 param_4)

{
  short sVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  
  if (param_1 == (undefined4 *)0x0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  if (param_2 == (int *)0x0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  puVar5 = (undefined4 *)0x0;
  *param_2 = 0;
  puVar4 = param_1;
  do {
    if (puVar4 == (undefined4 *)0x0) {
      if (puVar5 == (undefined4 *)0x0) {
        if (((param_3 < 0x20) || (param_3 == 0xf8ff)) || (param_3 == 0x200c)) {
          *(undefined2 *)((int)param_2 + 6) = 0;
          *(undefined2 *)(param_2 + 1) = 0;
        }
        else {
          sVar1 = (short)((uint)(param_1[2] - ((int)param_1[2] >> 0x1f)) >> 1);
          *(short *)((int)param_2 + 6) = sVar1;
          *(short *)(param_2 + 1) = sVar1 + 2;
        }
        *param_2 = 0;
        *(undefined2 *)(param_2 + 2) = *(undefined2 *)(param_1 + 2);
        *(undefined2 *)((int)param_2 + 10) = 0;
        *(undefined2 *)(param_2 + 3) = 0;
        *(byte *)((int)param_2 + 0xe) = (*(byte *)((int)param_2 + 0xe) & 0xf0) + 1 | 0x10;
        uVar3 = 0;
      }
      else {
        (*(code *)*puVar5)(puVar5,param_2,param_3,param_4);
        *param_2 = (int)puVar5;
        uVar3 = 1;
      }
      return uVar3;
    }
    iVar2 = (*(code *)*puVar4)(puVar4,param_2,param_3,param_4);
    if (iVar2 != 0) {
      if (-1 < (int)((uint)*(byte *)((int)param_2 + 0xe) << 0x1b)) {
        *param_2 = (int)puVar4;
        return 1;
      }
      if (puVar5 == (undefined4 *)0x0) {
        puVar5 = puVar4;
      }
    }
    puVar4 = (undefined4 *)puVar4[6];
  } while( true );
}

