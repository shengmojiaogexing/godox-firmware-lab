// 0x080250f4 FUN_080250f4 size=160

void FUN_080250f4(int param_1,uint param_2,uint param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  undefined1 auStack_34 [16];
  undefined4 local_24;
  undefined4 uStack_20;
  uint local_1c;
  
  iVar1 = FUN_080303fc(param_1);
  if ((iVar1 != 0) && (param_2 != param_3)) {
    if (0x168 < param_2) {
      param_2 = param_2 - 0x168 & 0xffff;
    }
    if (0x168 < param_3) {
      param_3 = param_3 - 0x168 & 0xffff;
    }
    uVar4 = *(ushort *)(param_1 + 0x2c) + param_2 & 0xffff;
    uVar3 = *(ushort *)(param_1 + 0x2c) + param_3 & 0xffff;
    if (0x168 < uVar4) {
      uVar4 = uVar4 - 0x168 & 0xffff;
    }
    if (0x168 < uVar3) {
      uVar3 = uVar3 - 0x168 & 0xffff;
    }
    FUN_08021f30(param_1,&local_24,&local_1c);
    uVar2 = FUN_0802f7da(param_1,param_4);
    iVar1 = FUN_0802f9bc(param_1,param_4,0x51);
    FUN_080283a6(local_24,uStack_20,local_1c & 0xffff,uVar4,uVar3,uVar2,iVar1 != 0,auStack_34,iVar1)
    ;
    FUN_08030308(param_1,auStack_34);
  }
  return;
}

