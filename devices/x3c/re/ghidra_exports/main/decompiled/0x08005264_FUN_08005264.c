// 0x08005264 FUN_08005264 size=1374

void FUN_08005264(undefined4 param_1)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  undefined4 uVar12;
  undefined1 auStack_30 [12];
  
  uVar6 = FUN_08010acc(param_1,1,0,0,0x170,0x1c0);
  FUN_08031330(uVar6,0xc);
  iVar2 = DAT_08005698;
  uVar9 = 0;
  iVar10 = DAT_08005698 + -0x40;
  iVar11 = DAT_08005698 + -0x80;
  do {
    uVar7 = FUN_08006220(uVar6,0,0,0,0x72,0x80);
    *(undefined4 *)(iVar2 + uVar9 * 4) = uVar7;
    FUN_0802db44(uVar7,1,(uVar9 % 3) * 0x7b + 4,(uVar9 / 3) * 0x89 + 0x30);
    FUN_0803161c(*(undefined4 *)(iVar2 + uVar9 * 4),0,4);
    FUN_08031626(*(undefined4 *)(iVar2 + uVar9 * 4),4);
    FUN_08031630(*(undefined4 *)(iVar2 + uVar9 * 4),0,4);
    uVar7 = FUN_08027f68();
    FUN_08031604(*(undefined4 *)(iVar2 + uVar9 * 4),uVar7,4);
    FUN_080315b6(*(undefined4 *)(iVar2 + uVar9 * 4),0);
    FUN_080315b6(*(undefined4 *)(iVar2 + uVar9 * 4),0xff,4);
    FUN_080315c0(*(undefined4 *)(iVar2 + uVar9 * 4),2,0);
    uVar7 = FUN_08027f68();
    FUN_0803159e(*(undefined4 *)(iVar2 + uVar9 * 4),uVar7,4);
    uVar7 = FUN_08027c88(0x27,0x27);
    FUN_0803157c(*(undefined4 *)(iVar2 + uVar9 * 4),uVar7,0);
    FUN_08031594(*(undefined4 *)(iVar2 + uVar9 * 4),0xff,0);
    if (uVar9 < 6) {
      FUN_08004c48(auStack_30,10,&DAT_0800569c,uVar9 + 0x41);
    }
    else {
      FUN_08004c48(auStack_30,10,&DAT_0800569c,uVar9 + 0x2a);
    }
    uVar5 = 0;
    uVar7 = FUN_08010acc(*(undefined4 *)(iVar2 + uVar9 * 4),2,0,0x60,0x21,8);
    *(undefined4 *)(iVar10 + uVar9 * 4) = uVar7;
    FUN_08031594(uVar7,0xff,0);
    FUN_08031696(*(undefined4 *)(iVar10 + uVar9 * 4),8,0);
    uVar7 = FUN_08027a78();
    FUN_0803157c(*(undefined4 *)(iVar10 + uVar9 * 4),uVar7,0);
    uVar4 = FUN_08027a78();
    uVar7 = FUN_0802553e(*(undefined4 *)(iVar2 + uVar9 * 4),9,0,0xfffffff6,DAT_080056a0,
                         CONCAT22(uVar5,uVar4),auStack_30);
    *(undefined4 *)(iVar11 + uVar9 * 4) = uVar7;
    FUN_0802d97a(*(undefined4 *)(iVar2 + uVar9 * 4),DAT_080056a4,0);
    uVar9 = uVar9 + 1 & 0xff;
  } while (uVar9 < 0x10);
  uVar4 = 0;
  uVar7 = FUN_08006220(uVar6,1,0x7f,0x2dd,0xed,0x80);
  iVar2 = DAT_080056a8;
  *(undefined4 *)(DAT_080056a8 + 0x20) = uVar7;
  FUN_0803161c(uVar7,0,4);
  FUN_08031626(*(undefined4 *)(iVar2 + 0x20),4);
  FUN_08031630(*(undefined4 *)(iVar2 + 0x20),0,4);
  uVar7 = FUN_08027f68();
  FUN_08031604(*(undefined4 *)(iVar2 + 0x20),uVar7,4);
  FUN_080315b6(*(undefined4 *)(iVar2 + 0x20),0);
  FUN_080315b6(*(undefined4 *)(iVar2 + 0x20),0xff,4);
  FUN_080315c0(*(undefined4 *)(iVar2 + 0x20),2,0);
  uVar7 = FUN_08027f68();
  FUN_0803159e(*(undefined4 *)(iVar2 + 0x20),uVar7,4);
  uVar7 = FUN_08027c88(0x27,0x27);
  FUN_0803157c(*(undefined4 *)(iVar2 + 0x20),uVar7,0);
  FUN_08031594(*(undefined4 *)(iVar2 + 0x20),0xff,0);
  uVar7 = FUN_0802b9c4(*(undefined4 *)(iVar2 + 0x20));
  FUN_0802c400(uVar7,DAT_080056ac);
  FUN_0802db44(uVar7,9,0);
  pcVar3 = DAT_080056b0;
  if (*DAT_080056b0 == '\0') {
    uVar8 = FUN_08027c88(200,200);
    FUN_080316b4(uVar7,uVar8,0);
    FUN_0802c400(uVar7,DAT_080056b8);
  }
  else {
    uVar8 = FUN_08027c88(0x27,0x27);
    FUN_080316b4(uVar7,uVar8,0);
    FUN_0802c400(uVar7,DAT_080056b4);
  }
  FUN_0802d97a(*(undefined4 *)(iVar2 + 0x20),DAT_080056bc,0);
  uVar12 = DAT_080056cc;
  uVar8 = DAT_080056c8;
  uVar7 = DAT_080056c4;
  cVar1 = *DAT_080056c0;
  if ((((cVar1 == '\x01') || (cVar1 == '\x05')) || (cVar1 == '\a')) || (cVar1 == '\x04')) {
    if (*DAT_080056d0 == '\0') {
      uVar5 = FUN_08027c88(0xfd,0x9f,10);
      uVar12 = CONCAT22(uVar4,uVar5);
      FUN_0802553e(uVar6,1,9,0x37e,uVar7,uVar12,&DAT_080056d4);
      uVar5 = (undefined2)((uint)uVar12 >> 0x10);
      uVar4 = FUN_08027c88(0x86,0x86);
      uVar7 = CONCAT22(uVar5,uVar4);
      FUN_0802553e(uVar6,1,0x2a,0x37b,uVar8,uVar7,&DAT_080056d8);
      uVar5 = (undefined2)((uint)uVar7 >> 0x10);
      uVar4 = FUN_08027c88(0x86,0x86);
      FUN_0802553e(uVar6,1,0x2a,0x3a0,uVar8,CONCAT22(uVar5,uVar4),&DAT_080056f4);
    }
    else if (*DAT_080056d0 == '\x01') {
      uVar5 = FUN_08027c88(0xfd,0x9f,10);
      uVar8 = CONCAT22(uVar4,uVar5);
      FUN_0802553e(uVar6,1,9,0x37e,uVar7,uVar8,&DAT_080056d4);
      uVar5 = (undefined2)((uint)uVar8 >> 0x10);
      uVar4 = FUN_08027c88(0x86,0x86);
      uVar7 = CONCAT22(uVar5,uVar4);
      FUN_0802553e(uVar6,1,0x2a,0x37b,uVar12,uVar7,s_GR_A_E_for_TTL_M_mode_0800570c);
      uVar5 = (undefined2)((uint)uVar7 >> 0x10);
      uVar4 = FUN_08027c88(0x86,0x86);
      FUN_0802553e(uVar6,1,0x2a,0x3a0,uVar12,CONCAT22(uVar5,uVar4),s_GR_F_9_for_M_mode_only_08005724
                  );
    }
  }
  else if (*DAT_080056d0 == '\0') {
    uVar5 = FUN_08027c88(0xfd,0x9f,10);
    uVar12 = CONCAT22(uVar4,uVar5);
    FUN_0802553e(uVar6,1,9,0x37e,uVar7,uVar12,&DAT_080056d4);
    uVar5 = (undefined2)((uint)uVar12 >> 0x10);
    uVar4 = FUN_08027c88(0x86,0x86);
    uVar7 = CONCAT22(uVar5,uVar4);
    FUN_0802553e(uVar6,1,0x2a,0x37b,uVar8,uVar7,&DAT_0800573c);
    uVar5 = (undefined2)((uint)uVar7 >> 0x10);
    uVar4 = FUN_08027c88(0x86,0x86);
    FUN_0802553e(uVar6,1,0x2a,0x3a0,uVar8,CONCAT22(uVar5,uVar4),&DAT_08005884);
  }
  else if (*DAT_080056d0 == '\x01') {
    uVar5 = FUN_08027c88(0xfd,0x9f,10);
    uVar8 = CONCAT22(uVar4,uVar5);
    FUN_0802553e(uVar6,1,9,0x37e,uVar7,uVar8,&DAT_080056d4);
    uVar5 = (undefined2)((uint)uVar8 >> 0x10);
    uVar4 = FUN_08027c88(0x86,0x86);
    uVar7 = CONCAT22(uVar5,uVar4);
    FUN_0802553e(uVar6,1,0x2a,0x37b,uVar12,uVar7,s_GR_A_C_for_TTL_M_mode_0800589c);
    uVar5 = (undefined2)((uint)uVar7 >> 0x10);
    uVar4 = FUN_08027c88(0x86,0x86);
    FUN_0802553e(uVar6,1,0x2a,0x3a0,uVar12,CONCAT22(uVar5,uVar4),s_GR_D_9_for_M_mode_only_080058b4);
  }
  iVar2 = DAT_080058cc;
  uVar9 = 0;
  do {
    if ((int)((uint)*(byte *)(iVar2 + uVar9 * 6 + 3) << 0x18) < 0) {
      if (*pcVar3 == '\0') {
        uVar6 = FUN_08027f68();
        FUN_0803157c(*(undefined4 *)(iVar10 + uVar9 * 4),uVar6,0);
      }
      else {
        FUN_080230f4(uVar9,*(undefined4 *)(iVar10 + uVar9 * 4));
      }
      uVar6 = FUN_08027f68();
      FUN_080316b4(*(undefined4 *)(iVar11 + uVar9 * 4),uVar6,0);
    }
    else {
      uVar6 = FUN_08027a78();
      FUN_0803157c(*(undefined4 *)(iVar10 + uVar9 * 4),uVar6,0);
      uVar6 = FUN_08027c88(0x37,0x37);
      FUN_080316b4(*(undefined4 *)(iVar11 + uVar9 * 4),uVar6,0);
    }
    uVar9 = uVar9 + 1 & 0xff;
  } while (uVar9 < 0x10);
  return;
}

