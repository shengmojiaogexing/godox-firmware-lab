// 0x0800fe78 FUN_0800fe78 size=226

void FUN_0800fe78(void)

{
  undefined1 *puVar1;
  char *pcVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  
  *DAT_0800ff5c = *DAT_0800ff5c | 0x80;
  *DAT_0800ff60 = 1;
  puVar1 = DAT_0800ff64;
  *DAT_0800ff64 = 0;
  *DAT_0800ff68 = 0;
  *DAT_0800ff6c = 0;
  FUN_0802b798(*DAT_0800ff70);
  uVar3 = FUN_08010acc(0,9,0,0,0x170,0x1c0);
  uVar4 = FUN_08027aa0();
  FUN_0803157c(uVar3,uVar4,0);
  FUN_08031594(uVar3,0xff,0);
  FUN_08010acc(uVar3,0,0,0,0x170,0x28);
  FUN_0800e168();
  uVar4 = FUN_08010acc(uVar3,2,0,0x28,0x170,0x198);
  *(undefined4 *)(puVar1 + 0xc) = uVar4;
  FUN_0800f4bc();
  FUN_080079e8(uVar3);
  pcVar2 = DAT_0800ff74;
  if (*DAT_0800ff74 == '\0') {
    FUN_08032078(uVar3,6,200,0,1);
  }
  else {
    FUN_0802821c();
    iVar5 = FUN_080282b0();
    FUN_0802832e(uVar3);
    if (iVar5 != 0) {
      FUN_0802e428(iVar5);
    }
    *pcVar2 = '\0';
  }
  FUN_0802e2d2(uVar3,0x10);
  *DAT_0800ff78 = 6;
  return;
}

