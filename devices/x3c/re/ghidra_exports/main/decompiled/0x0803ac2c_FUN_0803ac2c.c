// 0x0803ac2c FUN_0803ac2c size=532

byte FUN_0803ac2c(int param_1)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  code *pcVar4;
  undefined4 extraout_r3;
  undefined4 unaff_r4;
  uint uVar5;
  byte *pbVar6;
  
  FUN_0803b812(param_1 + 0x2cc,param_1 + 0x2d4);
  *(undefined1 *)(param_1 + 0x2e0) = 1;
  *(undefined2 *)(param_1 + 0x2e2) = *(undefined2 *)(param_1 + 0x2d2);
  bVar2 = *(byte *)(param_1 + 0x2cc) & 0x1f;
  if ((*(byte *)(param_1 + 0x2cc) & 0x1f) == 0) {
    if ((*(byte *)(param_1 + 0x2cc) & 0x60) != 0) {
      (**(code **)(*(int *)(param_1 + 4) + 8))();
      return 0;
    }
    switch(*(undefined1 *)(param_1 + 0x2cd)) {
    case 0:
      FUN_0803b2ae();
      break;
    case 1:
      FUN_0803a976();
      break;
    default:
      FUN_0803accc();
      break;
    case 3:
      FUN_0803b7be();
      break;
    case 5:
      FUN_0803b6b4();
      break;
    case 6:
      FUN_0803b1d2();
      break;
    case 8:
      FUN_0803b18c();
      break;
    case 9:
      FUN_0803b70c();
    }
    return 0;
  }
  if (bVar2 == 1) {
    if (*(char *)(param_1 + 0x2e4) == '\x03') {
      (**(code **)(*(int *)(param_1 + 4) + 8))(param_1);
      if (*(short *)(param_1 + 0x2d2) == 0) {
        FUN_0803acbc(param_1);
      }
    }
    else {
      FUN_0803accc(param_1);
    }
    return 0;
  }
  if (bVar2 != 2) {
    return bVar2;
  }
  pbVar6 = (byte *)(param_1 + 0x2cc);
  bVar2 = *(byte *)(param_1 + 0x2d0);
  uVar5 = (uint)bVar2;
  if ((*pbVar6 & 0x7f) >> 5 == 1) {
    pcVar4 = *(code **)(*(int *)(param_1 + 4) + 8);
    (*pcVar4)(param_1,pbVar6,pcVar4,extraout_r3,unaff_r4);
  }
  cVar1 = *(char *)(param_1 + 0x2cd);
  if (cVar1 == '\0') {
    if (*(char *)(param_1 + 0x2e4) == '\x02') {
      if ((bVar2 & 0x7f) != 0) {
        FUN_0803b7e4(param_1,uVar5);
      }
    }
    else if (*(char *)(param_1 + 0x2e4) == '\x03') {
      if ((int)(uVar5 << 0x18) < 0) {
        iVar3 = param_1 + (uVar5 & 0x7f) * 0x2c + 0xc;
      }
      else {
        iVar3 = param_1 + (uVar5 & 0x7f) * 0x2c + 0x16c;
      }
      if (*(char *)(iVar3 + 0xd) == '\x01') {
        *(undefined4 *)(iVar3 + 0x10) = 1;
      }
      else {
        *(undefined4 *)(iVar3 + 0x10) = 0;
      }
      FUN_0803aca4(param_1,iVar3 + 0x10,2);
    }
    else {
      FUN_0803accc(param_1);
    }
  }
  else if (cVar1 == '\x01') {
    if (*(char *)(param_1 + 0x2e4) == '\x02') {
      if ((uVar5 != 0) && (uVar5 != 0x80)) {
        FUN_0803b7e4(param_1,uVar5);
      }
    }
    else if (*(char *)(param_1 + 0x2e4) == '\x03') {
      if (*(short *)(param_1 + 0x2ce) == 0) {
        if ((bVar2 & 0x7f) != 0) {
          FUN_0803a9be(param_1,uVar5);
          (**(code **)(*(int *)(param_1 + 4) + 8))(param_1,pbVar6);
        }
        FUN_0803acbc(param_1);
      }
    }
    else {
      FUN_0803accc(param_1);
    }
  }
  else if (cVar1 == '\x03') {
    if (*(char *)(param_1 + 0x2e4) == '\x02') {
      if ((uVar5 != 0) && (uVar5 != 0x80)) {
        FUN_0803b7e4(param_1,uVar5);
      }
    }
    else if (*(char *)(param_1 + 0x2e4) == '\x03') {
      if (((*(short *)(param_1 + 0x2ce) == 0) && (uVar5 != 0)) && (uVar5 != 0x80)) {
        FUN_0803b7e4(param_1,uVar5);
      }
      (**(code **)(*(int *)(param_1 + 4) + 8))(param_1,pbVar6);
      FUN_0803acbc(param_1);
    }
    else {
      FUN_0803accc(param_1);
    }
  }
  else {
    FUN_0803accc(param_1);
  }
  return 0;
}

