// 0x08015b38 FUN_08015b38 size=264

void FUN_08015b38(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 extraout_r1;
  undefined4 uVar3;
  undefined4 extraout_r1_00;
  uint uVar4;
  int extraout_r2;
  int extraout_r3;
  uint *puVar5;
  undefined4 local_20;
  
  if (*(int *)(DAT_08015c40 + 0xc) == DAT_08015c44) {
    local_20 = param_4;
    FUN_080060ac(0,0x16f,0,0x1bf);
    puVar5 = DAT_08015c48;
    *DAT_08015c48 = *DAT_08015c48 & 0xffffffef;
    *puVar5 = *puVar5 | 8;
    *puVar5 = *puVar5 | 0x20;
    uVar2 = 0;
    do {
      uVar1 = 0;
      do {
        DAT_6c000001 = (undefined1)param_1;
        uVar1 = uVar1 + 1;
      } while (uVar1 < 0x170);
      uVar2 = uVar2 + 1;
    } while (uVar2 < 0x1c0);
    FUN_0801c118(5);
    *puVar5 = *puVar5 | 0x10;
  }
  else {
    local_20 = 0x6f010000;
    FUN_08014418(0x2a,&local_20,4);
    local_20 = 0xbf010000;
    FUN_08014418(0x2b,&local_20,4);
    puVar5 = DAT_08015c48 + -5;
    FUN_08022e34(puVar5,0x10);
    FUN_08025e94(0x2c);
    uVar3 = extraout_r1;
    do {
      uVar4 = 0;
      do {
        FUN_08025e9c(param_1 >> 8,uVar3,uVar4);
        FUN_08025e9c(param_1 & 0xff);
        uVar4 = extraout_r2 + 1U & 0xffff;
        uVar3 = extraout_r1_00;
      } while (uVar4 < 0x1c0);
    } while ((extraout_r3 + 1U & 0xffff) < 0x170);
    FUN_0801c118(5);
    FUN_08022e38(puVar5,0x10);
  }
  return;
}

