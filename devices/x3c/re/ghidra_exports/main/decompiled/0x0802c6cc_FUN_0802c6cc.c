// 0x0802c6cc FUN_0802c6cc size=240

void FUN_0802c6cc(int param_1)

{
  char cVar1;
  int iVar2;
  undefined4 *puVar3;
  char cVar4;
  int *piVar5;
  undefined4 *puVar6;
  undefined4 uVar7;
  undefined4 local_28;
  undefined4 local_24;
  short local_18;
  char local_16;
  char local_15;
  
  iVar2 = DAT_0802c7bc;
  piVar5 = *(int **)(param_1 + 0xc);
  *(int **)(DAT_0802c7bc + 8) = piVar5;
  if (*(int *)(*piVar5 + 0x10) != 0) {
    FUN_08024e60();
    puVar3 = DAT_0802c7c0;
    if ((-1 < (int)((uint)*(byte *)((int)*(int **)(iVar2 + 8) + 5) << 0x1d)) &&
       (*(int *)(*(int *)(**(int **)(iVar2 + 8) + 0x10) + 0x14) == 0)) {
      do {
        FUN_08017a98(*(undefined4 *)(iVar2 + 8),&local_28);
        cVar4 = local_15;
        *puVar3 = local_28;
        puVar3[1] = local_24;
        *(char *)((int)puVar3 + 0x12) = local_16;
        FUN_08024e60(*(undefined4 *)(iVar2 + 8));
        *(undefined4 *)(iVar2 + 4) = 0;
        puVar6 = *(undefined4 **)(iVar2 + 8);
        if ((*(char *)(puVar6 + 1) == '\x01') &&
           (((local_16 == '\0' && (*DAT_0802c7c4 != '\0')) && (*DAT_0802c7c8 != '\0')))) {
          *(undefined1 *)(puVar6 + 1) = 1;
        }
        else {
          *(char *)(puVar6 + 1) = local_16;
        }
        if (*(char *)(puVar6 + 1) == '\x01') {
          uVar7 = FUN_080334d8();
          *(undefined4 *)(*(int *)(**(int **)(iVar2 + 8) + 0x10) + 600) = uVar7;
        }
        else if ((*(char *)*puVar6 == '\x04') && (local_18 != 0)) {
          uVar7 = FUN_080334d8();
          *(undefined4 *)(*(int *)(**(int **)(iVar2 + 8) + 0x10) + 600) = uVar7;
        }
        puVar6 = *(undefined4 **)(iVar2 + 8);
        cVar1 = *(char *)*puVar6;
        if (cVar1 == '\x01') {
          FUN_08024a74(puVar6,&local_28);
        }
        else if (cVar1 == '\x02') {
          FUN_08024804(puVar6,&local_28);
        }
        else if (cVar1 == '\x04') {
          FUN_08024344(puVar6,&local_28);
        }
        else if (cVar1 == '\x03') {
          FUN_080241e4(puVar6,&local_28);
        }
        FUN_08024e60(*(undefined4 *)(iVar2 + 8));
      } while (cVar4 != '\0');
      *(undefined4 *)(iVar2 + 8) = 0;
      *(undefined4 *)(iVar2 + 4) = 0;
    }
  }
  return;
}

