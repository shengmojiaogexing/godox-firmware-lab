// 0x0802fff4 FUN_0802fff4 size=732

void FUN_0802fff4(undefined4 param_1,int param_2,undefined4 *param_3,undefined4 param_4)

{
  undefined1 uVar1;
  byte bVar2;
  undefined2 uVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  
  uVar7 = 0xff;
  if ((param_2 == 0) ||
     (uVar7 = FUN_0802f8cc(param_1,param_2,param_3,param_4,param_2,param_3,param_4), 2 < uVar7)) {
    if (param_2 != 0) {
      uVar1 = FUN_0802f846(param_1,param_2);
      *(undefined1 *)(param_3 + 1) = uVar1;
    }
    uVar4 = FUN_0802f9bc(param_1,param_2,0xb);
    *param_3 = uVar4;
    if (*(char *)((int)param_3 + 5) != '\0') {
      bVar2 = FUN_0802f9bc(param_1,param_2,0x21);
      *(byte *)((int)param_3 + 5) = bVar2;
      if (2 < bVar2) {
        uVar3 = FUN_0802f81c(param_1,param_2);
        *(undefined2 *)((int)param_3 + 6) = uVar3;
        iVar5 = FUN_0802f9bc(param_1,param_2,0x26);
        if ((iVar5 == 0) || ((*(byte *)(iVar5 + 9) & 7) == 0)) {
          bVar2 = FUN_0802f9bc(param_1,param_2,0x23);
          *(byte *)((int)param_3 + 0x11) = *(byte *)((int)param_3 + 0x11) & 0xf8 | bVar2 & 7;
          if ((bVar2 & 7) != 0) {
            uVar3 = FUN_0802f81c(param_1,param_2);
            *(undefined2 *)(param_3 + 2) = uVar3;
            uVar4 = FUN_0802f9bc(param_1,param_2,0x22);
            uVar3 = FUN_0801812e(param_1,param_2,uVar4);
            *(undefined2 *)(param_3 + 3) = uVar3;
            uVar1 = FUN_0802f9bc(param_1,param_2,0x24);
            *(undefined1 *)((int)param_3 + 10) = uVar1;
            uVar1 = FUN_0802f9bc(param_1,param_2,0x25);
            *(undefined1 *)((int)param_3 + 0xe) = uVar1;
          }
          bVar2 = FUN_0802f9bc(param_1,param_2,0x27);
          *(byte *)((int)param_3 + 0x11) = *(byte *)((int)param_3 + 0x11) & 199 | (bVar2 & 7) << 3;
        }
        else {
          FUN_0802d764(param_3 + 2,iVar5,10);
        }
      }
    }
    iVar5 = FUN_0802f9bc(param_1,param_2,0x32);
    param_3[9] = iVar5;
    if ((iVar5 != 0) && (*(char *)(param_3 + 10) != '\0')) {
      bVar2 = FUN_0802f9bc(param_1,param_2,0x31);
      *(byte *)(param_3 + 10) = bVar2;
      if (2 < bVar2) {
        bVar2 = FUN_0802f9bc(param_1,param_2,0x33);
        *(byte *)((int)param_3 + 0x29) = *(byte *)((int)param_3 + 0x29) & 0xc1 | (bVar2 & 0x1f) << 1
        ;
        uVar4 = FUN_0802f9bc(param_1,param_2,0x30);
        uVar3 = FUN_0801812e(param_1,param_2,uVar4);
        *(undefined2 *)((int)param_3 + 0x22) = uVar3;
      }
    }
    iVar5 = FUN_0802f8f6(param_1,param_2);
    param_3[0xb] = iVar5;
    if ((iVar5 != 0) && (*(char *)(param_3 + 0xd) != '\0')) {
      uVar6 = FUN_0802f8e4(param_1,param_2);
      *(char *)(param_3 + 0xd) = (char)uVar6;
      if (2 < uVar6) {
        uVar4 = FUN_0802f8f0(param_1,param_2);
        param_3[0xc] = uVar4;
        uVar4 = FUN_0802f9bc(param_1,param_2,0x36);
        uVar3 = FUN_0801812e(param_1,param_2,uVar4);
        *(undefined2 *)((int)param_3 + 0x2a) = uVar3;
      }
    }
    if (*(char *)((int)param_3 + 0x1e) != '\0') {
      iVar5 = FUN_0802f9bc(param_1,param_2,0x28);
      param_3[5] = iVar5;
      if (iVar5 != 0) {
        bVar2 = FUN_0802f9bc(param_1,param_2,0x29);
        *(byte *)((int)param_3 + 0x1e) = bVar2;
        if (2 < bVar2) {
          iVar5 = FUN_0802c55a(param_3[5]);
          if (iVar5 == 2) {
            uVar4 = FUN_0802fa94(param_1,param_2);
            param_3[6] = uVar4;
            uVar3 = FUN_0802fa76(param_1,param_2);
            *(undefined2 *)(param_3 + 7) = uVar3;
          }
          else {
            uVar4 = FUN_0802f9bc(param_1,param_2,0x2a);
            uVar3 = FUN_0801812e(param_1,param_2,uVar4);
            *(undefined2 *)(param_3 + 7) = uVar3;
            uVar1 = FUN_0802f9bc(param_1,param_2,0x2b);
            *(undefined1 *)((int)param_3 + 0x1f) = uVar1;
            iVar5 = FUN_0802f9bc(param_1,param_2,0x2c);
            *(bool *)(param_3 + 8) = iVar5 != 0;
          }
        }
      }
    }
    if (*(char *)(param_3 + 0x12) != '\0') {
      iVar5 = FUN_0802fa70(param_1,param_2);
      param_3[0xe] = iVar5;
      if ((iVar5 != 0) && (2 < *(byte *)(param_3 + 0x12))) {
        uVar6 = FUN_0802fa5e(param_1,param_2);
        *(char *)(param_3 + 0x12) = (char)uVar6;
        if (2 < uVar6) {
          uVar4 = FUN_0802fa52(param_1,param_2);
          param_3[0xf] = uVar4;
          uVar4 = FUN_0802fa58(param_1,param_2);
          param_3[0x10] = uVar4;
          uVar4 = FUN_0802fa6a(param_1,param_2);
          param_3[0x11] = uVar4;
          uVar4 = FUN_0802f9bc(param_1,param_2,0x44);
          uVar3 = FUN_0801812e(param_1,param_2,uVar4);
          *(undefined2 *)((int)param_3 + 0x36) = uVar3;
        }
      }
    }
    if ((param_2 != 0) && (uVar7 < 0xfd)) {
      uVar1 = (undefined1)(uVar7 * *(byte *)(param_3 + 0x12) >> 8);
      *(undefined1 *)((int)param_3 + 5) = uVar1;
      *(undefined1 *)((int)param_3 + 0x1e) = uVar1;
      *(undefined1 *)(param_3 + 10) = uVar1;
      *(undefined1 *)(param_3 + 0xd) = uVar1;
      *(undefined1 *)(param_3 + 0x12) = uVar1;
      return;
    }
  }
  else {
    *(undefined1 *)((int)param_3 + 5) = 0;
    *(undefined1 *)((int)param_3 + 0x1e) = 0;
    *(undefined1 *)(param_3 + 10) = 0;
    *(undefined1 *)(param_3 + 0xd) = 0;
    *(undefined1 *)(param_3 + 0x12) = 0;
  }
  return;
}

