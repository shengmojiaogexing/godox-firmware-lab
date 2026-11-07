// 0x0800bca8 FUN_0800bca8 size=300

void FUN_0800bca8(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  
  puVar2 = DAT_0800bdd8;
  iVar1 = DAT_0800bdd4;
  if (*(char *)(param_1 + 8) == '\a') {
    if (*(char *)(DAT_0800bdd4 + 8) == '\0') {
      uVar4 = FUN_08010acc(*(undefined4 *)(DAT_0800bdd4 + 0x50),2,0,0,0x170,0x1c0);
      FUN_08005264(uVar4);
      *(undefined4 *)(iVar1 + 0x7c) = uVar4;
      FUN_0803118e(*(undefined4 *)(iVar1 + 0x50),1);
      uVar4 = FUN_0803205a();
      uVar5 = 4;
      uVar4 = FUN_08010acc(uVar4,1,4,200,4,0x5c);
      *(undefined4 *)(iVar1 + 100) = uVar4;
      FUN_08031696(uVar4,0xf,0);
      uVar4 = FUN_08027f68();
      FUN_0803157c(*(undefined4 *)(iVar1 + 100),uVar4,0);
      FUN_08031594(*(undefined4 *)(iVar1 + 100),0xff,0);
      *(undefined1 *)(iVar1 + 8) = 0x50;
      FUN_0802da22(*puVar2,1,uVar5,uVar4);
      return;
    }
    if (*(char *)(DAT_0800bdd4 + 8) == 'P') {
      iVar3 = FUN_080303b8(*(undefined4 *)(DAT_0800bdd4 + 0x7c));
      if (iVar3 != 0) {
        FUN_0802e428(*(undefined4 *)(iVar1 + 0x7c));
      }
      *(undefined4 *)(iVar1 + 0x7c) = 0;
      iVar3 = FUN_080303b8(*(undefined4 *)(iVar1 + 100));
      if (iVar3 != 0) {
        FUN_0802e428(*(undefined4 *)(iVar1 + 100));
      }
      *(undefined4 *)(iVar1 + 100) = 0;
      uVar4 = FUN_08010acc(*(undefined4 *)(iVar1 + 0x50),2,0,10,0x170,0x1b6);
      FUN_08005264(uVar4);
      *(undefined4 *)(iVar1 + 0x7c) = uVar4;
      FUN_0803118e(*(undefined4 *)(iVar1 + 0x50),1);
      uVar4 = FUN_0803205a();
      uVar5 = 4;
      uVar4 = FUN_08010acc(uVar4,1,4,200,4,0x5c);
      *(undefined4 *)(iVar1 + 100) = uVar4;
      FUN_08031696(uVar4,0xf,0);
      uVar4 = FUN_08027f68();
      FUN_0803157c(*(undefined4 *)(iVar1 + 100),uVar4,0);
      FUN_08031594(*(undefined4 *)(iVar1 + 100),0xff,0);
      *(undefined1 *)(iVar1 + 8) = 0x50;
      FUN_0802da22(*puVar2,1,uVar5,uVar4);
      return;
    }
  }
  return;
}

