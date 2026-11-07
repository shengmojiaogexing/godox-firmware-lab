// 0x08014418 FUN_08014418 size=62

void FUN_08014418(undefined4 param_1,int param_2,uint param_3)

{
  undefined4 uVar1;
  uint uVar2;
  
  uVar1 = DAT_08014458;
  FUN_08022e34(DAT_08014458,0x10);
  FUN_08025e94(param_1);
  for (uVar2 = 0; uVar2 < param_3; uVar2 = uVar2 + 1 & 0xff) {
    FUN_08025e9c(*(undefined1 *)(param_2 + uVar2));
  }
  FUN_0801c118(5);
  FUN_08022e38(uVar1,0x10);
  return;
}

