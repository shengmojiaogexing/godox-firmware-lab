// 0x0801956c FUN_0801956c size=138

void FUN_0801956c(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  
  uVar2 = FUN_08010acc(0,9,0,0,0x170,0x1c0);
  uVar3 = FUN_08027af0();
  FUN_0803157c(uVar2,uVar3,0);
  FUN_08031594(uVar2,0xff,0);
  uVar3 = FUN_0802b9c4(uVar2);
  iVar4 = DAT_080195f8;
  *(undefined4 *)(DAT_080195f8 + 0x1c) = uVar3;
  FUN_0802c400(uVar3,DAT_080195fc);
  FUN_0802e128(*(undefined4 *)(iVar4 + 0x1c));
  iVar4 = FUN_08032068();
  FUN_0802832e(uVar2);
  if (iVar4 != 0) {
    FUN_0802e428(iVar4);
  }
  piVar1 = DAT_08019600;
  if (*DAT_08019600 != 0) {
    FUN_08033540();
    *piVar1 = 0;
  }
  piVar1 = DAT_08019604;
  if (*DAT_08019604 != 0) {
    FUN_08033540();
    *piVar1 = 0;
  }
  piVar1 = DAT_08019608;
  if (*DAT_08019608 != 0) {
    FUN_08033540();
    *piVar1 = 0;
  }
  return;
}

