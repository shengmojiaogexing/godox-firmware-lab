// 0x080220e0 FUN_080220e0 size=288

int FUN_080220e0(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  uint local_24;
  
  if (param_2 == 0) {
    iVar4 = 0;
  }
  else {
    iVar4 = *(int *)(param_1 + 0x14);
    piVar5 = *(int **)(iVar4 + 0x14);
    if ((piVar5 == (int *)0x0) || (*piVar5 != param_2)) {
      for (uVar3 = 0; uVar3 < (*(ushort *)(iVar4 + 0x12) & 0x1ff); uVar3 = uVar3 + 1 & 0xffff) {
        local_24 = param_2 - *(int *)(*(int *)(iVar4 + 8) + uVar3 * 0x14);
        if (local_24 <= *(ushort *)(*(int *)(iVar4 + 8) + uVar3 * 0x14 + 4)) {
          iVar6 = 0;
          iVar2 = *(int *)(iVar4 + 8);
          cVar1 = *(char *)(iVar2 + uVar3 * 0x14 + 0x12);
          if (cVar1 == '\x02') {
            iVar6 = *(ushort *)(iVar2 + uVar3 * 0x14 + 6) + local_24;
          }
          else if (cVar1 == '\0') {
            iVar6 = (uint)*(ushort *)(iVar2 + uVar3 * 0x14 + 6) +
                    (uint)*(byte *)(*(int *)(iVar2 + uVar3 * 0x14 + 0xc) + local_24);
          }
          else if (cVar1 == '\x03') {
            iVar7 = uVar3 * 0x14 + 8;
            iVar2 = FUN_08018a5a(&local_24,*(undefined4 *)(*(int *)(iVar4 + 8) + iVar7),
                                 *(undefined2 *)(*(int *)(iVar4 + 8) + uVar3 * 0x14 + 0x10),2,
                                 DAT_08022200);
            if (iVar2 != 0) {
              iVar6 = (uint)*(ushort *)(*(int *)(iVar4 + 8) + uVar3 * 0x14 + 6) +
                      (iVar2 - *(int *)(*(int *)(iVar4 + 8) + iVar7) >> 1);
            }
          }
          else if (cVar1 == '\x01') {
            iVar7 = uVar3 * 0x14 + 8;
            iVar2 = FUN_08018a5a(&local_24,*(undefined4 *)(*(int *)(iVar4 + 8) + iVar7),
                                 *(undefined2 *)(*(int *)(iVar4 + 8) + uVar3 * 0x14 + 0x10),2,
                                 DAT_08022200);
            if (iVar2 != 0) {
              iVar6 = *(int *)(iVar4 + 8);
              iVar6 = (uint)*(ushort *)(iVar6 + uVar3 * 0x14 + 6) +
                      (uint)*(ushort *)
                             (*(int *)(iVar6 + uVar3 * 0x14 + 0xc) +
                             (iVar2 - *(int *)(iVar6 + iVar7) >> 1) * 2);
            }
          }
          if (*(int **)(iVar4 + 0x14) != (int *)0x0) {
            **(int **)(iVar4 + 0x14) = param_2;
            *(int *)(*(int *)(iVar4 + 0x14) + 4) = iVar6;
          }
          return iVar6;
        }
      }
      if (piVar5 != (int *)0x0) {
        *piVar5 = param_2;
        *(undefined4 *)(*(int *)(iVar4 + 0x14) + 4) = 0;
      }
      iVar4 = 0;
    }
    else {
      iVar4 = piVar5[1];
    }
  }
  return iVar4;
}

