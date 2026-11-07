// 0x0801aea8 FUN_0801aea8 size=84

undefined4 FUN_0801aea8(int param_1,int param_2)

{
  int iVar1;
  byte bVar2;
  undefined2 uVar3;
  uint uVar4;
  int iVar5;
  
  iVar5 = *(int *)(*(int *)(param_1 + 4) + 0x24);
  uVar3 = FUN_0803b298(param_1,param_2);
  if (param_2 == 1) {
    *DAT_0801aefc = 1;
    bVar2 = FUN_0803b298(param_1);
    *DAT_0801af00 = bVar2;
    iVar1 = DAT_0801af04;
    for (uVar4 = 0; uVar4 < bVar2; uVar4 = uVar4 + 1 & 0xff) {
      *(undefined1 *)(iVar1 + uVar4) = *(undefined1 *)(iVar5 + uVar4);
    }
  }
  FUN_0803af24(param_1,1,iVar5,uVar3);
  return 0;
}

