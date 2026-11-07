// 0x080085f4 FUN_080085f4 size=884

void FUN_080085f4(undefined4 param_1)

{
  int iVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 *puVar8;
  undefined4 in_stack_ffffffcc;
  undefined2 uVar9;
  
  iVar1 = DAT_08008968;
  uVar9 = (undefined2)((uint)in_stack_ffffffcc >> 0x10);
  *(undefined1 *)(DAT_08008968 + 0xe) = 0;
  uVar7 = DAT_08008970;
  uVar6 = DAT_0800896c;
  puVar8 = (undefined4 *)(iVar1 + 0x80);
  if (*(char *)(iVar1 + 3) == '\0') {
    uVar2 = FUN_08027fb8();
    uVar3 = FUN_0802553e(param_1,2,0,0xa4,uVar6,CONCAT22(uVar9,uVar2),&DAT_08008974);
    *(undefined4 *)(iVar1 + 0x84) = uVar3;
  }
  else if (*(char *)(iVar1 + 3) == '\x01') {
    uVar2 = FUN_08027fb8();
    uVar3 = FUN_0802553e(param_1,2,0,0xa4,uVar7,CONCAT22(uVar9,uVar2),
                         s_Restore_Factory_Setting_08008988);
    *(undefined4 *)(iVar1 + 0x84) = uVar3;
    FUN_080316aa(uVar3,2,0);
  }
  uVar9 = 0;
  uVar3 = FUN_08006220(param_1,1,0xbc,0x158,0xb0,0x4c);
  uVar4 = FUN_08027d38(0x52,0x52);
  FUN_0803157c(uVar3,uVar4,0);
  FUN_08031594(uVar3,0xcc,0);
  FUN_08031696(uVar3,0x26,0);
  FUN_080315c0(uVar3,0);
  FUN_080315b6(uVar3,0);
  FUN_080315b6(uVar3,0,4);
  FUN_080315c0(uVar3,2,4);
  uVar4 = FUN_08027fb8();
  FUN_0803159e(uVar3,uVar4,4);
  *(undefined4 *)(iVar1 + 0x8c) = uVar3;
  uVar4 = FUN_08026bec(param_1);
  *puVar8 = uVar4;
  FUN_0802e2d2(uVar4,2);
  FUN_0802d97a(*puVar8,DAT_080089a0,0x16,0);
  FUN_0802da22(*puVar8,1);
  FUN_08027342(*puVar8,0x10e);
  FUN_080271cc(*puVar8,0,0x168);
  uVar4 = FUN_08027d38(0x70,0x70);
  FUN_0803157c(*puVar8,uVar4,0);
  FUN_08030e2a(*puVar8,0,0x30000);
  FUN_0802db44(*puVar8,2,0,0x53);
  FUN_080313b2(*puVar8,0xbe);
  if (*(byte *)(DAT_08008968 + 0xb3) < 2) {
    FUN_08032c54(DAT_08008968 + 0xac);
  }
  else {
    FUN_08032ea0(DAT_08008968 + 0xac);
  }
  uVar4 = FUN_08027d38(0x70,0x70);
  FUN_08032ed0(DAT_08008968 + 0xac,uVar4);
  FUN_0802da9c(*puVar8,DAT_08008968 + 0xac,0);
  if (*(byte *)(DAT_08008968 + 0xbb) < 2) {
    FUN_08032c54(DAT_08008968 + 0xb4);
  }
  else {
    FUN_08032ea0(DAT_08008968 + 0xb4);
  }
  uVar4 = FUN_08027fb8();
  FUN_08032ed0(DAT_08008968 + 0xb4,uVar4);
  FUN_0802da9c(*puVar8,DAT_08008968 + 0xb4,0x20000);
  if (*(char *)(iVar1 + 3) == '\0') {
    uVar2 = FUN_08027fb8();
    FUN_0802553e(uVar3,9,0,0,uVar6,CONCAT22(uVar9,uVar2),&DAT_080089a4);
  }
  else if (*(char *)(iVar1 + 3) == '\x01') {
    uVar2 = FUN_08027fb8();
    FUN_0802553e(uVar3,9,0,0,uVar7,CONCAT22(uVar9,uVar2),s_Cancel_080089ac);
  }
  uVar9 = 0;
  uVar4 = FUN_08006220(param_1,1,8,0x158,0xb0,0x4c);
  *(undefined4 *)(iVar1 + 0x88) = uVar4;
  uVar5 = FUN_08027d38(0x52,0x52);
  FUN_0803157c(uVar4,uVar5,0);
  FUN_08031594(uVar4,0xcc,0);
  FUN_08031696(uVar4,0x26,0);
  FUN_080315c0(uVar4,0);
  FUN_080315b6(uVar4,0);
  FUN_080315b6(uVar4,0,4);
  FUN_080315c0(uVar4,2,4);
  uVar5 = FUN_08027fb8();
  FUN_0803159e(uVar4,uVar5,4);
  if (*(char *)(iVar1 + 3) == '\0') {
    uVar2 = FUN_08027fb8();
    uVar7 = CONCAT22(uVar9,uVar2);
    FUN_0802553e(uVar4,9,0,0,uVar6,uVar7,&DAT_080089b4);
    uVar9 = (undefined2)((uint)uVar7 >> 0x10);
  }
  else if (*(char *)(iVar1 + 3) == '\x01') {
    uVar2 = FUN_08027fb8();
    uVar6 = CONCAT22(uVar9,uVar2);
    FUN_0802553e(uVar4,9,0,0,uVar7,uVar6,s_Apply_080089bc);
    uVar9 = (undefined2)((uint)uVar6 >> 0x10);
  }
  uVar2 = FUN_08027fb8();
  uVar6 = FUN_0802553e(param_1,2,0,0x7b,DAT_080089c8,CONCAT22(uVar9,uVar2),&DAT_080089c4);
  *(undefined4 *)(iVar1 + 0x90) = uVar6;
  FUN_0802da22(uVar6,1);
  FUN_0802d97a(uVar3,DAT_080089cc,0);
  FUN_0802d97a(uVar4,DAT_080089cc,0);
  uVar6 = FUN_08010acc(param_1,1,0,0xb2,4,0x5c);
  uVar7 = FUN_08027fb8();
  FUN_0803157c(uVar6,uVar7,0);
  FUN_08031594(uVar6,0xff,0);
  iVar1 = DAT_08008968;
  *(undefined4 *)(DAT_08008968 + 0xcc) = uVar3;
  *(undefined4 *)(iVar1 + 0xd0) = uVar4;
  return;
}

