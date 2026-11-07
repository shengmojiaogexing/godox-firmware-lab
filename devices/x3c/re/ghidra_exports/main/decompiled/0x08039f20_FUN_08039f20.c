// 0x08039f20 FUN_08039f20 size=164

undefined4 FUN_08039f20(int param_1,uint param_2,uint param_3,int *param_4)

{
  int *piVar1;
  int *piVar2;
  uint uVar3;
  undefined4 uVar4;
  uint uVar5;
  
  uVar4 = 0;
  piVar1 = (int *)FUN_08017f70(DAT_08039fc4);
  while ((piVar1 != (int *)0x0 && (piVar1 != param_4))) {
    piVar2 = (int *)FUN_08017f68(DAT_08039fc4,piVar1);
    if (((*piVar1 == param_1) && ((piVar1[2] == param_2 || (param_2 == 0xf0000)))) &&
       ((*(ushort *)(piVar1 + 1) == param_3 || (param_3 == 0xffff)))) {
      for (uVar5 = 0; uVar5 < (*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3; uVar5 = uVar5 + 1) {
        uVar3 = *(uint *)(*(int *)(param_1 + 0xc) + uVar5 * 8 + 4);
        if (((int)(uVar3 << 6) < 0) && ((param_2 == 0xf0000 || ((uVar3 & 0xffffff) == param_2)))) {
          FUN_08032dcc(*(undefined4 *)(*(int *)(param_1 + 0xc) + uVar5 * 8),(short)piVar1[1]);
        }
      }
      FUN_08026938(piVar1,0);
      FUN_0801801e(DAT_08039fc4,piVar1);
      FUN_0802d6e4(piVar1);
      uVar4 = 1;
    }
    piVar1 = piVar2;
  }
  return uVar4;
}

