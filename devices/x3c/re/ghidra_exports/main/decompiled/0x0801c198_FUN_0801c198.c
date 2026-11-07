// 0x0801c198 FUN_0801c198 size=378

void FUN_0801c198(int param_1,uint *param_2,ushort *param_3)

{
  ushort uVar1;
  ushort uVar2;
  char *pcVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint *puVar8;
  uint uVar9;
  
  pcVar3 = DAT_0801c308;
  if (*DAT_0801c308 != '\0') {
    uVar6 = *param_2;
    uVar5 = param_2[2];
    uVar9 = (uVar5 - uVar6) + 1;
    uVar7 = param_2[3];
    uVar4 = param_2[1];
    if (*DAT_0801c30c == DAT_0801c310) {
      if ((uVar6 & 1) == 0) {
        *(short *)(DAT_0801c308 + 2) = (short)uVar6;
      }
      else {
        *(short *)(DAT_0801c308 + 2) = (short)uVar6 + 1;
      }
      if ((uVar4 & 1) == 0) {
        *(short *)(pcVar3 + 4) = (short)uVar4;
      }
      else {
        *(short *)(pcVar3 + 4) = (short)uVar4 + 1;
      }
      uVar1 = *(ushort *)(pcVar3 + 2);
      uVar5 = (uVar5 - uVar1) + 1;
      uVar6 = uVar5 & 0xffff;
      if ((uVar5 & 1) != 0) {
        uVar6 = uVar6 - 1 & 0xffff;
      }
      *(ushort *)(pcVar3 + 6) = uVar1 + (short)uVar6 + -1;
      uVar2 = *(ushort *)(pcVar3 + 4);
      uVar5 = (uVar7 - uVar2) + 1;
      uVar7 = uVar5 & 0xffff;
      if ((uVar5 & 1) != 0) {
        uVar7 = uVar7 - 1 & 0xffff;
      }
      *(ushort *)(pcVar3 + 8) = uVar2 + (short)uVar7 + -1;
      if (uVar4 != uVar2) {
        param_3 = param_3 + uVar9;
      }
      FUN_0800ce2c((uint)uVar1);
      FUN_0801c118(0x32);
      puVar8 = DAT_0801c314;
      *DAT_0801c314 = *DAT_0801c314 & 0xffffffef;
      for (uVar4 = 0; uVar4 < uVar7; uVar4 = uVar4 + 1) {
        for (uVar5 = 0; uVar5 < (uVar9 & 0xffff); uVar5 = uVar5 + 1) {
          if (((uVar5 != 0) || (*param_2 == (uint)*(ushort *)(pcVar3 + 2))) &&
             ((uVar5 != uVar6 - 1 || (param_2[2] == (uint)*(ushort *)(pcVar3 + 6))))) {
            DAT_6c000001 = (undefined1)*param_3;
          }
          param_3 = param_3 + 1;
        }
      }
      FUN_0801c118(5);
      *puVar8 = *puVar8 | 0x10;
    }
    else {
      FUN_0800ce2c(uVar6 & 0xffff,uVar4 & 0xffff,uVar5 & 0xffff,uVar7 & 0xffff);
      puVar8 = DAT_0801c314 + -5;
      FUN_08022e34(puVar8,0x10);
      FUN_08025e94(0x2c);
      for (uVar5 = 0; uVar5 < ((uVar7 - uVar4) + 1 & 0xffff) * (uVar9 & 0xffff); uVar5 = uVar5 + 1)
      {
        FUN_08025e9c(*param_3 >> 8);
        FUN_08025e9c((char)*param_3);
        param_3 = param_3 + 1;
      }
      FUN_0801c118(5);
      FUN_08022e38(puVar8,0x10);
    }
  }
  *(undefined4 *)(*(int *)(param_1 + 0x18) + 0x10) = 0;
  *(undefined4 *)(*(int *)(param_1 + 0x18) + 0x14) = 0;
  return;
}

