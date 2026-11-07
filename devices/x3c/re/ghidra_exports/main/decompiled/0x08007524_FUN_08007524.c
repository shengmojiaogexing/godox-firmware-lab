// 0x08007524 FUN_08007524 size=564

void FUN_08007524(uint param_1,undefined4 param_2,int param_3,undefined4 param_4,undefined4 param_5,
                 undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  bool bVar1;
  undefined1 uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  int extraout_r3;
  int iVar7;
  undefined1 auStack_40 [12];
  undefined4 local_34;
  undefined4 uStack_30;
  int iStack_2c;
  undefined4 uStack_28;
  
  bVar1 = false;
  local_34 = param_1;
  uStack_30 = param_2;
  iStack_2c = param_3;
  uStack_28 = param_4;
  iVar3 = FUN_080303b8(param_3);
  if (((((iVar3 != 0) && (iVar3 = FUN_080303b8(param_4), iVar3 != 0)) &&
       (iVar3 = FUN_080303b8(param_5), iVar3 != 0)) &&
      ((iVar3 = FUN_080303b8(param_6), iVar3 != 0 && (iVar3 = FUN_080303b8(param_7), iVar3 != 0))))
     && (iVar3 = FUN_080303b8(param_8), iVar3 != 0)) {
    uVar4 = 0;
    do {
      if (*(int *)(DAT_08007758 + uVar4 * 0x30 + 0x20) == param_3) {
        bVar1 = true;
        break;
      }
      uVar4 = uVar4 + 1 & 0xff;
    } while (uVar4 < 0x10);
    if (((int)((local_34 >> 0x18) << 0x19) < 0) || (!bVar1)) {
      if ((local_34 & 0xff) == 0) {
        uVar6 = FUN_0800ae28();
        thunk_FUN_08027790(param_3,0,uVar6);
        uVar2 = local_34._1_1_;
        iVar3 = FUN_0800ae50(local_34._1_1_);
        iVar5 = FUN_0802fc5e(param_3);
        if (!bVar1) {
          iVar5 = 0x148;
        }
        iVar7 = FUN_0800ae28();
        iVar7 = (iVar5 * iVar3) / iVar7;
        if (bVar1) {
          iVar7 = iVar7 + 10;
          if (extraout_r3 + 8 < iVar7) {
            iVar7 = extraout_r3 + 8;
          }
        }
        else {
          FUN_0802782e(param_3,iVar3,0);
        }
        if (iVar7 < 2) {
          iVar7 = 1;
        }
        FUN_080316fe(param_4,iVar7);
        FUN_0802da22(param_5,1);
        FUN_0802e2d2(param_6,1);
        FUN_0802e2d2(param_7,1);
        FUN_0802e2d2(param_8,1);
        FUN_0800776c(uVar2,param_6,param_7,param_8);
        FUN_0802db62(param_6,param_7,0x11,0xffffffff,8);
        FUN_0802db62(param_8,param_7,0x14,1,6);
      }
      else if ((local_34 & 0xff) == 1) {
        thunk_FUN_08027790(param_3,0,0x12);
        uVar4 = local_34 >> 0x10 & 0xff;
        iVar3 = FUN_0800acd4(uVar4);
        iVar5 = FUN_0802fc5e(param_3);
        if (!bVar1) {
          iVar5 = 0x148;
        }
        iVar7 = (iVar5 * iVar3) / 0x12;
        thunk_FUN_08027790(param_3,0,0x12);
        if (bVar1) {
          iVar7 = iVar7 + 0xc;
          if (iVar5 + 8 < iVar7) {
            iVar7 = iVar5 + 8;
          }
        }
        else {
          FUN_0802782e(param_3,iVar3,0);
        }
        if (iVar7 < 2) {
          iVar7 = 1;
        }
        FUN_080316fe(param_4,iVar7);
        FUN_0802e2d2(param_5,1);
        FUN_0802da22(param_6,1);
        FUN_0802da22(param_7,1);
        FUN_0802da22(param_8,1);
        if (uVar4 < 0x1e) {
          iVar3 = (int)(0x1e - uVar4) % 10;
          if (iVar3 < 3) {
            uVar6 = 0;
          }
          else if (iVar3 < 6) {
            uVar6 = 3;
          }
          else {
            uVar6 = 7;
          }
          FUN_08004c48(auStack_40,10,s____d__d_08007764,(int)(short)((int)(0x1e - uVar4) / 10),uVar6
                      );
        }
        else {
          FUN_08004c48(auStack_40,10,s___d__d_0800775c,uVar4 / 10 - 3,uVar4 % 10);
        }
        FUN_0802d33c(param_5,auStack_40);
      }
    }
    else {
      FUN_0802da22(param_5,1);
      FUN_0802da22(param_6,1);
      FUN_0802da22(param_7,1);
      FUN_0802da22(param_8,1);
    }
  }
  return;
}

