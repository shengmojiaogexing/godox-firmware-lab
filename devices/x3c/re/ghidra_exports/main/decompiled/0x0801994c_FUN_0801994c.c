// 0x0801994c FUN_0801994c size=356

void FUN_0801994c(int param_1)

{
  undefined1 *puVar1;
  char cVar2;
  ushort uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_14;
  
  cVar2 = *(char *)(param_1 + 8);
  if (cVar2 == '\a') {
    cVar2 = FUN_0802ed92(*(undefined4 *)(param_1 + 4));
    puVar1 = DAT_08019ab0;
    if (DAT_08019ab0[0xe] == '\0') {
      if (cVar2 == '\x03') {
        puVar7 = (undefined4 *)(DAT_08019ab0 + 0x80);
        FUN_0802e2d2(*puVar7,1);
        FUN_080269c8(&local_54);
        local_54 = *puVar7;
        local_50 = DAT_08019ab4;
        local_28 = 1000;
        local_14 = CONCAT22(local_14._2_2_,1);
        local_34 = 0;
        local_30 = 0;
        local_2c = 100;
        FUN_08026aac(&local_54);
        puVar1[0xf] = 0;
        iVar5 = FUN_080303b8(*(undefined4 *)(puVar1 + 0x84));
        if (iVar5 != 0) {
          FUN_0802da22(*(undefined4 *)(puVar1 + 0x84),1);
        }
        iVar5 = FUN_080303b8(*(undefined4 *)(puVar1 + 0x88));
        if (iVar5 != 0) {
          FUN_0802da22(*(undefined4 *)(puVar1 + 0x88),1);
        }
        iVar5 = FUN_080303b8(*(undefined4 *)(puVar1 + 0x8c));
        if (iVar5 != 0) {
          FUN_0802da22(*(undefined4 *)(puVar1 + 0x8c),1);
        }
        puVar1[0xe] = 1;
        FUN_080073f0();
        FUN_08015418();
        uVar4 = FUN_0803c384(*DAT_08019ab8);
        FUN_0803c4a4(uVar4,1);
        uVar3 = FUN_0803c344();
        FUN_0801a234((uint)uVar3 % 0xff + 0x37 & 0xff);
      }
      else if (cVar2 == '\x01') {
        DAT_08019ab0[4] = *DAT_08019ab0;
        *puVar1 = 0;
      }
    }
  }
  else if (cVar2 == '\x0e') {
    uVar6 = *(undefined4 *)(param_1 + 4);
    uVar4 = FUN_0802ebc8(uVar6,0);
    iVar5 = FUN_080303b8(uVar6);
    if (iVar5 != 0) {
      local_14 = FUN_08027fb8();
      FUN_0803157c(uVar6,local_14,0);
    }
    iVar5 = FUN_080303b8(uVar4);
    if (iVar5 != 0) {
      local_14 = FUN_08027ac8();
      FUN_080316b4(uVar4,local_14,0);
      return;
    }
  }
  else if (cVar2 == '\x0f') {
    uVar6 = *(undefined4 *)(param_1 + 4);
    uVar4 = FUN_0802ebc8(uVar6,0);
    iVar5 = FUN_080303b8(uVar6);
    if (iVar5 != 0) {
      local_14 = FUN_08027d38(0x52,0x52);
      FUN_0803157c(uVar6,local_14,0);
    }
    iVar5 = FUN_080303b8(uVar4);
    if (iVar5 != 0) {
      local_14 = FUN_08027fb8();
      FUN_080316b4(uVar4,local_14,0);
      return;
    }
  }
  return;
}

