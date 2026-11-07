// 0x08032078 FUN_08032078 size=660

void FUN_08032078(undefined4 param_1,undefined4 param_2,int param_3,int param_4,byte param_5)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_88;
  undefined4 local_84;
  int local_80;
  int local_7c;
  int local_78;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  int local_44;
  int local_40;
  undefined4 local_3c;
  int local_38;
  int local_34;
  
  iVar2 = FUN_0802ed10(param_1);
  iVar3 = FUN_0803204c();
  if ((*(int *)(iVar2 + 0x18) != 0) && (*(int *)(iVar2 + 0x18) != iVar3)) {
    FUN_08036d66();
    FUN_08026938(*(undefined4 *)(iVar2 + 0x18),0);
    FUN_08031318(*(undefined4 *)(iVar2 + 0x18),0);
    FUN_08030dcc(*(undefined4 *)(iVar2 + 0x18),0x60,0);
    if ((int)((uint)*(byte *)(iVar2 + 0x28) << 0x1e) < 0) {
      FUN_0802e428(iVar3);
    }
    iVar3 = *(int *)(iVar2 + 0x18);
  }
  *(undefined4 *)(iVar2 + 0x18) = param_1;
  if ((*(int *)(iVar2 + 0x14) != 0) && ((int)((uint)*(byte *)(iVar2 + 0x28) << 0x1e) < 0)) {
    FUN_0802e428();
    *(undefined4 *)(iVar2 + 0x14) = 0;
  }
  bVar1 = FUN_080251bc(param_2);
  *(byte *)(iVar2 + 0x28) = *(byte *)(iVar2 + 0x28) & 0xfc | bVar1 & 1 | (param_5 & 1) << 1;
  FUN_08026938(param_1,0);
  uVar4 = FUN_0803204c();
  FUN_08026938(uVar4,0);
  FUN_08031318(param_1,0);
  uVar4 = FUN_0803204c();
  FUN_08031318(uVar4,0);
  FUN_08030dcc(param_1,0x60,0);
  uVar4 = FUN_0803204c();
  FUN_08030dcc(uVar4,0x60,0);
  if (param_3 == 0 && param_4 == 0) {
    FUN_08036d66(param_1);
  }
  else {
    FUN_080269c8(&local_64);
    local_5c = DAT_0803231c;
    local_58 = DAT_08032320;
    param_4 = -param_4;
    local_64 = param_1;
    local_38 = param_3;
    local_34 = param_4;
    FUN_080269c8(&local_a8);
    local_a8 = *(undefined4 *)(iVar2 + 0x10);
    local_7c = param_3;
    local_78 = param_4;
    switch(param_2) {
    case 0:
      local_60 = DAT_08032324;
      local_44 = 0;
      local_40 = 0;
      local_3c = 0;
      break;
    case 1:
      local_60 = DAT_08032324;
      local_44 = FUN_08028248(iVar2);
      local_3c = 0;
      local_40 = local_44;
      break;
    case 2:
      local_60 = DAT_08032324;
      local_44 = FUN_08028248(iVar2);
      local_44 = -local_44;
      local_3c = 0;
      local_40 = local_44;
      break;
    case 3:
      local_60 = DAT_08032328;
      local_44 = FUN_080282e6(iVar2);
      local_3c = 0;
      local_40 = local_44;
      break;
    case 4:
      local_60 = DAT_08032328;
      local_44 = FUN_080282e6(iVar2);
      local_44 = -local_44;
      local_3c = 0;
      local_40 = local_44;
      break;
    case 5:
      local_60 = DAT_08032324;
      local_44 = FUN_08028248(iVar2);
      local_3c = 0;
      local_a4 = DAT_08032324;
      local_40 = local_44;
      local_80 = FUN_08028248(iVar2);
      local_80 = -local_80;
      local_88 = 0;
      local_84 = 0;
      break;
    case 6:
      local_60 = DAT_08032324;
      local_44 = FUN_08028248(iVar2);
      local_44 = -local_44;
      local_3c = 0;
      local_a4 = DAT_08032324;
      local_40 = local_44;
      local_80 = FUN_08028248(iVar2);
      local_88 = 0;
      local_84 = 0;
      break;
    case 7:
      local_60 = DAT_08032328;
      local_44 = FUN_080282e6(iVar2);
      local_3c = 0;
      local_a4 = DAT_08032328;
      local_40 = local_44;
      local_80 = FUN_080282e6(iVar2);
      local_80 = -local_80;
      local_88 = 0;
      local_84 = 0;
      break;
    case 8:
      local_60 = DAT_08032328;
      local_44 = FUN_080282e6(iVar2);
      local_44 = -local_44;
      local_3c = 0;
      local_a4 = DAT_08032328;
      local_40 = local_44;
      local_80 = FUN_080282e6(iVar2);
      local_88 = 0;
      local_84 = 0;
      break;
    case 9:
      local_60 = DAT_0803232c;
      local_44 = 0;
      local_40 = 0;
      local_3c = 0xff;
      break;
    case 10:
      local_a4 = DAT_0803232c;
      local_88 = 0xff;
      local_84 = 0xff;
      local_80 = 0;
      break;
    case 0xb:
      local_a4 = DAT_08032324;
      local_80 = FUN_08028248(iVar2);
      local_80 = -local_80;
      local_88 = 0;
      local_84 = 0;
      break;
    case 0xc:
      local_a4 = DAT_08032324;
      local_80 = FUN_08028248(iVar2);
      local_88 = 0;
      local_84 = 0;
      break;
    case 0xd:
      local_a4 = DAT_08032328;
      local_80 = FUN_080282e6(iVar2);
      local_80 = -local_80;
      local_88 = 0;
      local_84 = 0;
      break;
    case 0xe:
      local_a4 = DAT_08032328;
      local_80 = FUN_080282e6(iVar2);
      local_88 = 0;
      local_84 = 0;
    }
    FUN_0802ad74(iVar3,0x25,0);
    FUN_08026aac(&local_64);
    FUN_08026aac(&local_a8);
  }
  return;
}

