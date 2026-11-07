// 0x08032ff8 FUN_08032ff8 size=302

void FUN_08032ff8(int *param_1,uint param_2,undefined4 param_3)

{
  uint uVar1;
  code *pcVar2;
  byte bVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  
  pcVar2 = DAT_08033008;
  if (*(ushort *)(param_1 + 1) != 0xffff) {
    uVar8 = param_2 & 0xffff3fff;
    uVar5 = (uint)*(byte *)((int)param_1 + 7);
    if (uVar5 < 2) {
      if (uVar5 == 1) {
        if ((*(ushort *)(param_1 + 1) & 0xffff3fff) == uVar8) {
          (*DAT_08033008)(param_2,param_3,param_1 + 1,param_1);
          return;
        }
        piVar4 = (int *)FUN_0802d5c4(0xc);
        if (piVar4 == (int *)0x0) {
          return;
        }
        iVar6 = *param_1;
        *param_1 = (int)piVar4;
        bVar3 = *(char *)((int)param_1 + 7) + 1;
        *(byte *)((int)param_1 + 7) = bVar3;
        *(short *)(piVar4 + bVar3) = (short)param_1[1];
        *piVar4 = iVar6;
        (*pcVar2)(param_2,param_3,(undefined2 *)((int)(piVar4 + bVar3) + 2),piVar4 + 1,iVar6);
      }
      else {
        *(undefined1 *)((int)param_1 + 7) = 1;
        (*pcVar2)(param_2,param_3,param_1 + 1,param_1);
      }
    }
    else {
      iVar7 = *param_1;
      iVar6 = iVar7 + uVar5 * 4;
      uVar1 = uVar5;
      while (uVar1 = uVar1 - 1, -1 < (int)uVar1) {
        if ((*(ushort *)(iVar6 + uVar1 * 2) & 0xffff3fff) == uVar8) {
          (*DAT_08033008)(param_2,param_3,iVar6 + uVar1 * 2,iVar7 + uVar1 * 4);
          return;
        }
      }
      iVar6 = FUN_0802d72c(iVar7,(uVar5 + 1) * 6);
      if (iVar6 == 0) {
        return;
      }
      *param_1 = iVar6;
      iVar7 = iVar6 + (uint)*(byte *)((int)param_1 + 7) * 4;
      uVar5 = (uint)*(byte *)((int)param_1 + 7);
      while (uVar1 = uVar5 - 1, -1 < (int)uVar1) {
        *(undefined2 *)(iVar7 + (uVar5 + 1) * 2) = *(undefined2 *)(iVar7 + uVar1 * 2);
        uVar5 = uVar1;
      }
      bVar3 = *(char *)((int)param_1 + 7) + 1;
      *(byte *)((int)param_1 + 7) = bVar3;
      iVar7 = bVar3 - 1;
      (*pcVar2)(param_2,param_3,iVar6 + (uint)bVar3 * 4 + iVar7 * 2,iVar6 + iVar7 * 4);
    }
    uVar5 = FUN_08018536(uVar8);
    *(byte *)((int)param_1 + 6) = *(byte *)((int)param_1 + 6) | (byte)(1 << (uVar5 & 0xff));
  }
  return;
}

