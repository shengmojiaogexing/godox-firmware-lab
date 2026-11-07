// 0x0803840c FUN_0803840c size=670

undefined8 FUN_0803840c(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  char *pcVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte bVar7;
  int iVar8;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  
  bVar7 = 0;
  uStack_24 = param_2;
  uStack_20 = param_3;
  uStack_1c = param_4;
  FUN_08004c48(&uStack_24,10,&DAT_080386bc,*DAT_080386b8);
  iVar3 = DAT_080386c0;
  FUN_0802d33c(*(undefined4 *)(DAT_080386c0 + 4),&uStack_24);
  iVar8 = FUN_080303b8(*(undefined4 *)(iVar3 + 8));
  pcVar4 = DAT_080386cc;
  if (iVar8 != 0) {
    if (*DAT_080386c4 == '\0') {
      cVar1 = *DAT_080386cc;
      if (cVar1 == '\0') {
        FUN_0802d33c(*(undefined4 *)(iVar3 + 8),&DAT_080386d0);
      }
      else if (cVar1 == '\x01') {
        FUN_0802d33c(*(undefined4 *)(iVar3 + 8),&DAT_080386d4);
      }
      else if (cVar1 == '\x02') {
        FUN_0802d33c(*(undefined4 *)(iVar3 + 8),&DAT_080386d8);
      }
      if ((((*DAT_080386dc == '\x03') || (*DAT_080386dc == '\a')) && (*pcVar4 == '\0')) ||
         (*DAT_080386e0 != '\0')) {
        FUN_0802da22(*(undefined4 *)(iVar3 + 8),1);
      }
      else {
        FUN_0802e2d2(*(undefined4 *)(iVar3 + 8),1);
      }
    }
    else {
      FUN_0802da22(*(undefined4 *)(iVar3 + 8),1);
    }
  }
  iVar8 = FUN_080303b8(*(undefined4 *)(iVar3 + 0xc));
  if (iVar8 != 0) {
    if (((*DAT_080386c8 == '\0') && (*DAT_080386e4 != '\0')) && (*DAT_080386e8 == '\0')) {
      FUN_0802e2d2(*(undefined4 *)(iVar3 + 0xc),1);
    }
    else {
      FUN_0802da22(*(undefined4 *)(iVar3 + 0xc),1);
    }
  }
  iVar8 = FUN_080303b8(*(undefined4 *)(iVar3 + 0x14));
  if (iVar8 != 0) {
    if (*DAT_080386ec == '\0') {
      FUN_0802da22(*(undefined4 *)(iVar3 + 0x14),1);
    }
    else {
      FUN_0802e2d2(*(undefined4 *)(iVar3 + 0x14),1);
    }
  }
  iVar8 = FUN_080303b8(*(undefined4 *)(iVar3 + 0x18));
  if (iVar8 != 0) {
    if (*DAT_080386f0 == '\0') {
      FUN_0802da22(*(undefined4 *)(iVar3 + 0x18),1);
    }
    else {
      FUN_0802e2d2(*(undefined4 *)(iVar3 + 0x18),1);
    }
  }
  iVar8 = FUN_080303b8(*(undefined4 *)(iVar3 + 0x1c));
  pbVar6 = DAT_080386fc;
  pbVar5 = DAT_080386f8;
  local_28 = param_1;
  if (iVar8 != 0) {
    if (*DAT_080386f4 == '\0') {
      DAT_080386fc[2] = 0;
      switch(*pbVar5) {
      case 0:
        local_28 = FUN_08027d64(0xfc,0x67,0x2e);
        FUN_080316b4(*(undefined4 *)(iVar3 + 0x1c),local_28,0);
        FUN_080316cc(*(undefined4 *)(iVar3 + 0x1c),DAT_08038708,0);
        FUN_0802d33c(*(undefined4 *)(iVar3 + 0x1c),&DAT_0803870c);
        break;
      case 1:
        local_28 = FUN_08027fcc();
        FUN_080316b4(*(undefined4 *)(iVar3 + 0x1c),local_28,0);
        FUN_080316cc(*(undefined4 *)(iVar3 + 0x1c),DAT_08038708,0);
        FUN_0802d33c(*(undefined4 *)(iVar3 + 0x1c),&DAT_0803870c);
        break;
      case 2:
        local_28 = FUN_08027fcc();
        FUN_080316b4(*(undefined4 *)(iVar3 + 0x1c),local_28,0);
        FUN_080316cc(*(undefined4 *)(iVar3 + 0x1c),DAT_08038708,0);
        FUN_0802d33c(*(undefined4 *)(iVar3 + 0x1c),&DAT_08038710);
        break;
      case 3:
        local_28 = FUN_08027fcc();
        FUN_080316b4(*(undefined4 *)(iVar3 + 0x1c),local_28,0);
        FUN_080316cc(*(undefined4 *)(iVar3 + 0x1c),DAT_08038708,0);
        FUN_0802d33c(*(undefined4 *)(iVar3 + 0x1c),&DAT_08038714);
        break;
      case 4:
        local_28 = FUN_08027fcc();
        FUN_080316b4(*(undefined4 *)(iVar3 + 0x1c),local_28,0);
        FUN_080316cc(*(undefined4 *)(iVar3 + 0x1c),DAT_08038718,0);
        FUN_0802d33c(*(undefined4 *)(iVar3 + 0x1c),&DAT_08038714);
      }
    }
    else {
      if (DAT_080386fc[2] == 0) {
        *DAT_080386fc = *DAT_080386f8;
        pbVar6[2] = 1;
      }
      local_28 = FUN_08027fcc();
      FUN_080316b4(*(undefined4 *)(iVar3 + 0x1c),local_28,0);
      if (*DAT_08038700 == '\0') {
        bVar7 = 7;
      }
      else if (*DAT_08038700 == '\x01') {
        if (*DAT_08038704 == '\0') {
          bVar7 = 1;
        }
        else {
          bVar7 = 3;
        }
      }
      bVar2 = pbVar6[1];
      pbVar6[1] = bVar2 + 1;
      if (bVar7 <= (byte)(bVar2 + 1)) {
        pbVar6[1] = 0;
        bVar7 = *pbVar6;
        *pbVar6 = bVar7 + 1;
        if (4 < (byte)(bVar7 + 1)) {
          bVar7 = *pbVar5;
          if (3 < bVar7) {
            bVar7 = 3;
          }
          *pbVar6 = bVar7;
        }
        bVar7 = *pbVar6;
        if (bVar7 == 0) {
          bVar7 = 1;
        }
        *pbVar6 = bVar7;
        switch(bVar7) {
        case 0:
        case 1:
          FUN_080316cc(*(undefined4 *)(iVar3 + 0x1c),DAT_08038708,0);
          FUN_0802d33c(*(undefined4 *)(iVar3 + 0x1c),&DAT_0803870c);
          break;
        case 2:
          FUN_080316cc(*(undefined4 *)(iVar3 + 0x1c),DAT_08038708,0);
          FUN_0802d33c(*(undefined4 *)(iVar3 + 0x1c),&DAT_08038710);
          break;
        case 3:
          FUN_080316cc(*(undefined4 *)(iVar3 + 0x1c),DAT_08038708,0);
          FUN_0802d33c(*(undefined4 *)(iVar3 + 0x1c),&DAT_08038714);
          break;
        case 4:
          FUN_080316cc(*(undefined4 *)(iVar3 + 0x1c),DAT_08038718,0);
          FUN_0802d33c(*(undefined4 *)(iVar3 + 0x1c),&DAT_08038714);
        }
      }
    }
  }
  return CONCAT44(uStack_24,local_28);
}

