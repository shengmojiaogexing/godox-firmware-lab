// 0x08029268 FUN_08029268 size=186

undefined4 FUN_08029268(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  byte bVar1;
  byte bVar2;
  undefined1 uVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  if (param_3 < *(int *)(param_5 + 0xc)) {
    uVar4 = 1;
  }
  else if (*(int *)(param_5 + 0x14) < param_3) {
    uVar4 = 1;
  }
  else {
    iVar6 = *(int *)(param_5 + 8);
    iVar5 = param_2 + param_4;
    if (iVar5 < iVar6) {
      uVar4 = 1;
    }
    else {
      iVar7 = *(int *)(param_5 + 0x10);
      if (iVar7 < param_2) {
        uVar4 = 1;
      }
      else {
        if (iVar7 < iVar5) {
          param_4 = (param_4 - (iVar5 - iVar7)) + 1;
        }
        if (param_2 < iVar6) {
          param_4 = param_4 - (iVar6 - param_2);
          param_1 = param_1 + (iVar6 - param_2);
        }
        iVar5 = *(int *)(param_5 + 0x18);
        if (iVar5 < param_3) {
          iVar6 = *(int *)(param_5 + 0x1c);
          if (param_3 < iVar6) {
            bVar1 = *(byte *)(param_5 + 0x21);
            bVar2 = *(byte *)(param_5 + 0x20);
            for (iVar7 = 0; iVar7 < param_4; iVar7 = iVar7 + 1) {
              uVar3 = FUN_08035164(*(undefined1 *)(param_1 + iVar7),
                                   (int)(((uint)bVar1 - (uint)bVar2) * (param_3 - iVar5)) /
                                   ((iVar6 - iVar5) + 1) + (uint)bVar2 & 0xff);
              *(undefined1 *)(param_1 + iVar7) = uVar3;
            }
            uVar4 = 2;
          }
          else {
            for (iVar5 = 0; iVar5 < param_4; iVar5 = iVar5 + 1) {
              uVar3 = FUN_08035164(*(undefined1 *)(param_1 + iVar5),*(undefined1 *)(param_5 + 0x21))
              ;
              *(undefined1 *)(param_1 + iVar5) = uVar3;
            }
            uVar4 = 2;
          }
        }
        else {
          for (iVar5 = 0; iVar5 < param_4; iVar5 = iVar5 + 1) {
            uVar3 = FUN_08035164(*(undefined1 *)(param_1 + iVar5),*(undefined1 *)(param_5 + 0x20));
            *(undefined1 *)(param_1 + iVar5) = uVar3;
          }
          uVar4 = 2;
        }
      }
    }
  }
  return uVar4;
}

