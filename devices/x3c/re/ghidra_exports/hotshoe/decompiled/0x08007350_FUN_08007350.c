// 0x08007350 FUN_08007350 size=382

/* WARNING: Function: __ARM_common_switch8 replaced with injection: switch8_r3 */
/* WARNING (jumptable): Removing unreachable block (ram,0x0800735a) */
/* WARNING (jumptable): Removing unreachable block (ram,0x08007376) */
/* WARNING: Instruction at (ram,0x08007378) overlaps instruction at (ram,0x08007376)
    */
/* WARNING: Removing unreachable block (ram,0x0800735a) */
/* WARNING: Removing unreachable block (ram,0x08007376) */

byte * FUN_08007350(byte *param_1,byte *param_2)

{
  byte bVar1;
  bool bVar2;
  byte *pbVar3;
  code *UNRECOVERED_JUMPTABLE;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  undefined4 in_cr1;
  undefined4 in_cr15;
  
  bVar6 = (byte *)0x11 < param_1;
  bVar7 = SBORROW4((int)param_1,0x12);
  if (param_1 == (byte *)0x12) goto switchD_0800735a_caseD_18;
  if (0x12 < (int)param_1) goto switchD_0800735a_caseD_a;
  bVar5 = param_1 == (byte *)0x0;
  UNRECOVERED_JUMPTABLE = (code *)0x800735f;
  if (param_1 < (byte *)0x11) {
    bVar1 = param_1[0x800735f];
  }
  else {
    bVar1 = 0x3c;
  }
  pbVar3 = (byte *)((uint)bVar1 * 2);
  switch(bVar1) {
  case 0:
  case 1:
switchD_0800735a_caseD_2:
switchD_0800735a_caseD_3:
switchD_0800735a_caseD_4:
switchD_0800735a_caseD_5:
    param_1 = &SysTick;
switchD_0800735a_caseD_6:
switchD_0800735a_caseD_7:
switchD_0800735a_caseD_8:
switchD_0800735a_caseD_9:
switchD_0800735a_caseD_a:
    bVar6 = (byte *)0xed < param_1;
    bVar7 = SBORROW4((int)param_1,0xee);
    param_1 = param_1 + -0xee;
switchD_0800735a_caseD_b:
    bVar5 = param_1 == (byte *)0x0;
    pbVar3 = param_1;
switchD_0800735a_caseD_c:
    bVar4 = (int)param_1 < 0;
    UNRECOVERED_JUMPTABLE = (code *)0x800737b;
    if (pbVar3 < (byte *)0x11) {
      bVar1 = pbVar3[0x800737b];
    }
    else {
      bVar1 = 0x2e;
    }
    bVar2 = bVar5;
    switch(bVar1) {
    case 0:
switchD_0800735a_caseD_e:
      break;
    case 2:
      goto switchD_0800735a_caseD_10;
    case 3:
      goto switchD_0800735a_caseD_11;
    case 4:
      goto switchD_0800735a_caseD_12;
    case 5:
      goto switchD_0800735a_caseD_13;
    case 6:
      goto switchD_0800735a_caseD_14;
    case 7:
      goto switchD_0800735a_caseD_15;
    case 8:
      goto switchD_0800735a_caseD_16;
    case 9:
      goto switchD_0800735a_caseD_17;
    case 10:
      goto switchD_0800735a_caseD_18;
    case 0xb:
      goto switchD_0800735a_caseD_19;
    case 0xc:
switchD_0800735a_caseD_1a:
      param_1 = (byte *)0x15;
    case 0xd:
switchD_0800735a_caseD_1b:
      return param_1;
    case 0xe:
switchD_0800735a_caseD_1c:
      param_1 = (byte *)0x13;
    case 0xf:
switchD_0800735a_caseD_1d:
      return param_1;
    case 0x10:
switchD_0800735a_caseD_1e:
      param_1 = &MemManage;
    case 0x11:
switchD_0800735a_caseD_1f:
      return param_1;
    case 0x12:
switchD_0800735a_caseD_20:
      param_1 = (byte *)0xd;
    case 0x13:
switchD_0800735a_caseD_21:
      return param_1;
    case 0x14:
switchD_0800735a_caseD_22:
      param_1 = (byte *)0xb;
    case 0x15:
switchD_0800735a_caseD_23:
      return param_1;
    case 0x16:
switchD_0800735a_caseD_24:
      param_1 = &NMI;
    case 0x17:
switchD_0800735a_caseD_25:
      return param_1;
    case 0x18:
switchD_0800735a_caseD_26:
      param_1 = (byte *)0x5;
    case 0x19:
switchD_0800735a_caseD_27:
      return param_1;
    case 0x1a:
switchD_0800735a_caseD_28:
      param_1 = (byte *)0x3;
    case 0x1b:
switchD_0800735a_caseD_29:
      return param_1;
    case 0x1c:
switchD_0800735a_caseD_2a:
      param_1 = (byte *)0xfd;
    case 0x1d:
switchD_0800735a_caseD_2b:
      return param_1;
    case 0x1e:
switchD_0800735a_caseD_2c:
      param_1 = (byte *)0xfb;
    case 0x1f:
switchD_0800735a_caseD_2d:
      return param_1;
    case 0x20:
switchD_0800735a_caseD_2e:
      param_1 = (byte *)0xf8;
    case 0x21:
switchD_0800735a_caseD_2f:
      return param_1;
    case 0x22:
switchD_0800735a_caseD_30:
      param_1 = (byte *)0xf5;
    case 0x23:
switchD_0800735a_caseD_31:
      return param_1;
    case 0x24:
switchD_0800735a_caseD_32:
      param_1 = (byte *)0xf3;
    case 0x25:
switchD_0800735a_caseD_33:
      return param_1;
    case 0x26:
switchD_0800735a_caseD_34:
      param_1 = (byte *)0xf0;
    case 0x27:
switchD_0800735a_caseD_35:
      return param_1;
    case 0x28:
switchD_0800735a_caseD_36:
      param_1 = (byte *)0xed;
    case 0x29:
switchD_0800735a_caseD_37:
      return param_1;
    case 0x2a:
switchD_0800735a_caseD_38:
      param_1 = (byte *)0xeb;
    case 0x2b:
switchD_0800735a_caseD_39:
      return param_1;
    case 0x2c:
switchD_0800735a_caseD_3a:
      param_1 = (byte *)0xe8;
    case 0x2d:
switchD_0800735a_caseD_3b:
      return param_1;
    case 0x2e:
switchD_0800735a_caseD_3c:
      param_1 = (byte *)0x0;
    case 0x2f:
switchD_0800735a_caseD_3d:
      return param_1;
    case 0x30:
switchD_0800735a_caseD_3e:
      bVar5 = param_1 == (byte *)0x0;
    case 0x31:
switchD_0800735a_caseD_3f:
      if (!bVar5) {
switchD_0800735a_caseD_40:
        bVar6 = (byte *)0x2 < param_1;
        bVar5 = param_1 == (byte *)0x3;
switchD_0800735a_caseD_41:
        if (bVar6 && !bVar5) {
switchD_0800735a_caseD_44:
          bVar6 = (byte *)0x6 < param_1;
          bVar5 = param_1 == (byte *)0x7;
switchD_0800735a_caseD_45:
          if (bVar6 && !bVar5) {
switchD_0800735a_caseD_48:
            bVar6 = (byte *)0x9 < param_1;
            bVar5 = param_1 == (byte *)0xa;
switchD_0800735a_caseD_49:
            if (bVar6 && !bVar5) {
switchD_0800735a_caseD_4c:
              bVar6 = &HardFault < param_1;
              bVar5 = param_1 == (byte *)0xd;
switchD_0800735a_caseD_4d:
              if (bVar6 && !bVar5) {
switchD_0800735a_caseD_50:
                bVar6 = &MemManage < param_1;
                bVar5 = param_1 == (byte *)0x11;
switchD_0800735a_caseD_51:
                if (bVar6 && !bVar5) {
switchD_0800735a_caseD_54:
                  bVar6 = (byte *)0x13 < param_1;
                  bVar5 = param_1 == &BusFault;
switchD_0800735a_caseD_55:
                  if (bVar6 && !bVar5) {
switchD_0800735a_caseD_58:
                    bVar6 = (byte *)0x16 < param_1;
                    bVar5 = param_1 == (byte *)0x17;
switchD_0800735a_caseD_59:
                    if (bVar6 && !bVar5) {
switchD_0800735a_caseD_5c:
                      bVar6 = (byte *)0x1a < param_1;
                      bVar5 = param_1 == (byte *)0x1b;
switchD_0800735a_caseD_5d:
                      if (bVar6 && !bVar5) {
switchD_0800735a_caseD_60:
                        bVar6 = (byte *)0x1d < param_1;
                        bVar5 = param_1 == (byte *)0x1e;
switchD_0800735a_caseD_61:
                        if (bVar6 && !bVar5) {
switchD_0800735a_caseD_64:
                          bVar6 = &Reserved2 < param_1;
                          bVar5 = param_1 == (byte *)0x21;
switchD_0800735a_caseD_65:
                          if (bVar6 && !bVar5) {
switchD_0800735a_caseD_68:
                            bVar6 = &Reserved3 < param_1;
                            bVar5 = param_1 == (byte *)0x25;
switchD_0800735a_caseD_69:
                            if (bVar6 && !bVar5) {
switchD_0800735a_caseD_6c:
                              bVar6 = (byte *)0x27 < param_1;
                              bVar5 = param_1 == &Reserved4;
switchD_0800735a_caseD_6d:
                              if (bVar6 && !bVar5) {
switchD_0800735a_caseD_70:
                                bVar6 = (byte *)0x2a < param_1;
                                bVar5 = param_1 == (byte *)0x2b;
switchD_0800735a_caseD_71:
                                if (bVar6 && !bVar5) {
switchD_0800735a_caseD_74:
                                  bVar6 = (byte *)0x2e < param_1;
                                  bVar5 = param_1 == (byte *)0x2f;
switchD_0800735a_caseD_75:
                                  if (bVar6 && !bVar5) {
switchD_0800735a_caseD_78:
                                    bVar6 = (byte *)0x31 < param_1;
                                    bVar5 = param_1 == (byte *)0x32;
switchD_0800735a_caseD_79:
                                    if (bVar6 && !bVar5) {
switchD_0800735a_caseD_7c:
                                      bVar6 = &Reserved6 < param_1;
                                      bVar5 = param_1 == (byte *)0x35;
switchD_0800735a_caseD_7d:
                                      if (bVar6 && !bVar5) {
switchD_0800735a_caseD_80:
                                        bVar6 = &PendSV < param_1;
                                        bVar5 = param_1 == (byte *)0x39;
switchD_0800735a_caseD_81:
                                        if (bVar6 && !bVar5) {
switchD_0800735a_caseD_84:
                                          bVar6 = (byte *)0x3b < param_1;
                                          bVar5 = param_1 == &SysTick;
switchD_0800735a_caseD_85:
                                          if (bVar6 && !bVar5) {
switchD_0800735a_caseD_88:
                                            bVar6 = (byte *)0x3e < param_1;
                                            bVar5 = param_1 == (byte *)0x3f;
switchD_0800735a_caseD_89:
                                            if (bVar6 && !bVar5) {
switchD_0800735a_caseD_8c:
                                              bVar6 = (byte *)0x42 < param_1;
                                              bVar5 = param_1 == (byte *)0x43;
switchD_0800735a_caseD_8d:
                                              if (bVar6 && !bVar5) {
switchD_0800735a_caseD_90:
                                                bVar6 = (byte *)0x45 < param_1;
                                                bVar5 = param_1 == (byte *)0x46;
switchD_0800735a_caseD_91:
                                                if (bVar6 && !bVar5) {
switchD_0800735a_caseD_94:
                                                  bVar6 = (byte *)0x48 < param_1;
                                                  bVar5 = param_1 == (byte *)0x49;
switchD_0800735a_caseD_95:
                                                  if (!bVar6 || bVar5) {
switchD_0800735a_caseD_96:
                                                    param_1 = (byte *)0x3a;
switchD_0800735a_caseD_97:
                    /* WARNING: Could not recover jumptable at 0x0800748c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                                    pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)
                                                                               (param_1);
                                                    return pbVar3;
                                                  }
switchD_0800735a_caseD_98:
                                                  bVar6 = (byte *)0x4c < param_1;
                                                  bVar5 = param_1 == (byte *)0x4d;
switchD_0800735a_caseD_99:
                                                  if (!bVar6 || bVar5) {
switchD_0800735a_caseD_9a:
                                                    param_1 = (byte *)0x3d;
switchD_0800735a_caseD_9b:
                    /* WARNING: Could not recover jumptable at 0x08007494. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                                    pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)
                                                                               (param_1);
                                                    return pbVar3;
                                                  }
switchD_0800735a_caseD_9c:
                                                  bVar6 = (byte *)0x4f < param_1;
                                                  bVar5 = param_1 == (byte *)0x50;
switchD_0800735a_caseD_9d:
                                                  if (!bVar6 || bVar5) {
switchD_0800735a_caseD_9e:
                                                    param_1 = &IRQ;
switchD_0800735a_caseD_9f:
                    /* WARNING: Could not recover jumptable at 0x0800749c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                                    pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)
                                                                               (param_1);
                                                    return pbVar3;
                                                  }
switchD_0800735a_caseD_a0:
                                                  bVar6 = (byte *)0x52 < param_1;
                                                  bVar5 = param_1 == (byte *)0x53;
switchD_0800735a_caseD_a1:
                                                  if (!bVar6 || bVar5) {
switchD_0800735a_caseD_a2:
                                                    param_1 = (byte *)0x42;
switchD_0800735a_caseD_a3:
                    /* WARNING: Could not recover jumptable at 0x080074a4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                                    pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)
                                                                               (param_1);
                                                    return pbVar3;
                                                  }
switchD_0800735a_caseD_a4:
                                                  bVar6 = (byte *)0x56 < param_1;
                                                  bVar5 = param_1 == (byte *)0x57;
switchD_0800735a_caseD_a5:
                                                  if (!bVar6 || bVar5) {
switchD_0800735a_caseD_a6:
                                                    param_1 = (byte *)0x45;
switchD_0800735a_caseD_a7:
                    /* WARNING: Could not recover jumptable at 0x080074ac. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                                    pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)
                                                                               (param_1);
                                                    return pbVar3;
                                                  }
switchD_0800735a_caseD_a8:
                                                  bVar6 = (byte *)0x59 < param_1;
                                                  bVar5 = param_1 == (byte *)0x5a;
switchD_0800735a_caseD_a9:
                                                  if (!bVar6 || bVar5) {
switchD_0800735a_caseD_aa:
                                                    param_1 = (byte *)0x48;
switchD_0800735a_caseD_ab:
                    /* WARNING: Could not recover jumptable at 0x080074b4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                                    pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)
                                                                               (param_1);
                                                    return pbVar3;
                                                  }
                                                }
switchD_0800735a_caseD_92:
                                                param_1 = &PendSV;
switchD_0800735a_caseD_93:
                    /* WARNING: Could not recover jumptable at 0x08007484. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                                pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                                                return pbVar3;
                                              }
switchD_0800735a_caseD_8e:
                                              param_1 = (byte *)0x35;
switchD_0800735a_caseD_8f:
                    /* WARNING: Could not recover jumptable at 0x0800747c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                              pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                                              return pbVar3;
                                            }
switchD_0800735a_caseD_8a:
                                            param_1 = (byte *)0x32;
switchD_0800735a_caseD_8b:
                    /* WARNING: Could not recover jumptable at 0x08007474. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                            pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                                            return pbVar3;
                                          }
switchD_0800735a_caseD_86:
                                          param_1 = &Reserved5;
switchD_0800735a_caseD_87:
                    /* WARNING: Could not recover jumptable at 0x0800746c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                          pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                                          return pbVar3;
                                        }
switchD_0800735a_caseD_82:
                                        param_1 = (byte *)0x2d;
switchD_0800735a_caseD_83:
                    /* WARNING: Could not recover jumptable at 0x08007464. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                        pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                                        return pbVar3;
                                      }
switchD_0800735a_caseD_7e:
                                      param_1 = (byte *)0x2a;
switchD_0800735a_caseD_7f:
                    /* WARNING: Could not recover jumptable at 0x0800745c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                      pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                                      return pbVar3;
                                    }
switchD_0800735a_caseD_7a:
                                    param_1 = &Reserved4;
switchD_0800735a_caseD_7b:
                    /* WARNING: Could not recover jumptable at 0x08007454. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                    pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                                    return pbVar3;
                                  }
switchD_0800735a_caseD_76:
                                  param_1 = (byte *)0x25;
switchD_0800735a_caseD_77:
                    /* WARNING: Could not recover jumptable at 0x0800744c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                  pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                                  return pbVar3;
                                }
switchD_0800735a_caseD_72:
                                param_1 = (byte *)0x22;
switchD_0800735a_caseD_73:
                    /* WARNING: Could not recover jumptable at 0x08007444. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                                pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                                return pbVar3;
                              }
switchD_0800735a_caseD_6e:
                              param_1 = &Reserved2;
switchD_0800735a_caseD_6f:
                    /* WARNING: Could not recover jumptable at 0x0800743c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                              pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                              return pbVar3;
                            }
switchD_0800735a_caseD_6a:
                            param_1 = (byte *)0x1d;
switchD_0800735a_caseD_6b:
                    /* WARNING: Could not recover jumptable at 0x08007434. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                            pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                            return pbVar3;
                          }
switchD_0800735a_caseD_66:
                          param_1 = (byte *)0x1a;
switchD_0800735a_caseD_67:
                    /* WARNING: Could not recover jumptable at 0x0800742c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                          pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                          return pbVar3;
                        }
switchD_0800735a_caseD_62:
                        param_1 = &UsageFault;
switchD_0800735a_caseD_63:
                    /* WARNING: Could not recover jumptable at 0x08007424. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                        pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                        return pbVar3;
                      }
switchD_0800735a_caseD_5e:
                      param_1 = (byte *)0x15;
switchD_0800735a_caseD_5f:
                    /* WARNING: Could not recover jumptable at 0x0800741c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                      pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                      return pbVar3;
                    }
switchD_0800735a_caseD_5a:
                    param_1 = (byte *)0x12;
switchD_0800735a_caseD_5b:
                    /* WARNING: Could not recover jumptable at 0x08007414. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                    pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                    return pbVar3;
                  }
switchD_0800735a_caseD_56:
                  param_1 = &MemManage;
switchD_0800735a_caseD_57:
                    /* WARNING: Could not recover jumptable at 0x0800740c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                  pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                  return pbVar3;
                }
switchD_0800735a_caseD_52:
                param_1 = (byte *)0xd;
switchD_0800735a_caseD_53:
                    /* WARNING: Could not recover jumptable at 0x08007404. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
                return pbVar3;
              }
switchD_0800735a_caseD_4e:
              param_1 = (byte *)0xa;
switchD_0800735a_caseD_4f:
                    /* WARNING: Could not recover jumptable at 0x080073fc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
              pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
              return pbVar3;
            }
switchD_0800735a_caseD_4a:
            param_1 = &NMI;
switchD_0800735a_caseD_4b:
                    /* WARNING: Could not recover jumptable at 0x080073f4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
            return pbVar3;
          }
switchD_0800735a_caseD_46:
          param_1 = (byte *)0x5;
switchD_0800735a_caseD_47:
                    /* WARNING: Could not recover jumptable at 0x080073ec. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
          return pbVar3;
        }
switchD_0800735a_caseD_42:
        param_1 = (byte *)0x2;
      }
switchD_0800735a_caseD_43:
                    /* WARNING: Could not recover jumptable at 0x080073e4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      pbVar3 = (byte *)(*UNRECOVERED_JUMPTABLE)(param_1);
      return pbVar3;
    case 0x32:
      goto switchD_0800735a_caseD_40;
    case 0x33:
      goto switchD_0800735a_caseD_41;
    case 0x34:
      goto switchD_0800735a_caseD_42;
    case 0x35:
      goto switchD_0800735a_caseD_43;
    case 0x36:
      goto switchD_0800735a_caseD_44;
    case 0x37:
      goto switchD_0800735a_caseD_45;
    case 0x38:
      goto switchD_0800735a_caseD_46;
    case 0x39:
      goto switchD_0800735a_caseD_47;
    case 0x3a:
      goto switchD_0800735a_caseD_48;
    case 0x3b:
      goto switchD_0800735a_caseD_49;
    case 0x3c:
      goto switchD_0800735a_caseD_4a;
    case 0x3d:
      goto switchD_0800735a_caseD_4b;
    case 0x3e:
      goto switchD_0800735a_caseD_4c;
    case 0x3f:
      goto switchD_0800735a_caseD_4d;
    case 0x40:
      goto switchD_0800735a_caseD_4e;
    case 0x41:
      goto switchD_0800735a_caseD_4f;
    case 0x42:
      goto switchD_0800735a_caseD_50;
    case 0x43:
      goto switchD_0800735a_caseD_51;
    case 0x44:
      goto switchD_0800735a_caseD_52;
    case 0x45:
      goto switchD_0800735a_caseD_53;
    case 0x46:
      goto switchD_0800735a_caseD_54;
    case 0x47:
      goto switchD_0800735a_caseD_55;
    case 0x48:
      goto switchD_0800735a_caseD_56;
    case 0x49:
      goto switchD_0800735a_caseD_57;
    case 0x4a:
      goto switchD_0800735a_caseD_58;
    case 0x4b:
      goto switchD_0800735a_caseD_59;
    case 0x4c:
      goto switchD_0800735a_caseD_5a;
    case 0x4d:
      goto switchD_0800735a_caseD_5b;
    case 0x4e:
      goto switchD_0800735a_caseD_5c;
    case 0x4f:
      goto switchD_0800735a_caseD_5d;
    case 0x50:
      goto switchD_0800735a_caseD_5e;
    case 0x51:
      goto switchD_0800735a_caseD_5f;
    case 0x52:
      goto switchD_0800735a_caseD_60;
    case 0x53:
      goto switchD_0800735a_caseD_61;
    case 0x54:
      goto switchD_0800735a_caseD_62;
    case 0x55:
      goto switchD_0800735a_caseD_63;
    case 0x56:
      goto switchD_0800735a_caseD_64;
    case 0x57:
      goto switchD_0800735a_caseD_65;
    case 0x58:
      goto switchD_0800735a_caseD_66;
    case 0x59:
      goto switchD_0800735a_caseD_67;
    case 0x5a:
      goto switchD_0800735a_caseD_68;
    case 0x5b:
      goto switchD_0800735a_caseD_69;
    case 0x5c:
      goto switchD_0800735a_caseD_6a;
    case 0x5d:
      goto switchD_0800735a_caseD_6b;
    case 0x5e:
      goto switchD_0800735a_caseD_6c;
    case 0x5f:
      goto switchD_0800735a_caseD_6d;
    case 0x60:
      goto switchD_0800735a_caseD_6e;
    case 0x61:
      goto switchD_0800735a_caseD_6f;
    case 0x62:
      goto switchD_0800735a_caseD_70;
    case 99:
      goto switchD_0800735a_caseD_71;
    case 100:
      goto switchD_0800735a_caseD_72;
    case 0x65:
      goto switchD_0800735a_caseD_73;
    case 0x66:
      goto switchD_0800735a_caseD_74;
    case 0x67:
      goto switchD_0800735a_caseD_75;
    case 0x68:
      goto switchD_0800735a_caseD_76;
    case 0x69:
      goto switchD_0800735a_caseD_77;
    case 0x6a:
      goto switchD_0800735a_caseD_78;
    case 0x6b:
      goto switchD_0800735a_caseD_79;
    case 0x6c:
      goto switchD_0800735a_caseD_7a;
    case 0x6d:
      goto switchD_0800735a_caseD_7b;
    case 0x6e:
      goto switchD_0800735a_caseD_7c;
    case 0x6f:
      goto switchD_0800735a_caseD_7d;
    case 0x70:
      goto switchD_0800735a_caseD_7e;
    case 0x71:
      goto switchD_0800735a_caseD_7f;
    case 0x72:
      goto switchD_0800735a_caseD_80;
    case 0x73:
      goto switchD_0800735a_caseD_81;
    case 0x74:
      goto switchD_0800735a_caseD_82;
    case 0x75:
      goto switchD_0800735a_caseD_83;
    case 0x76:
      goto switchD_0800735a_caseD_84;
    case 0x77:
      goto switchD_0800735a_caseD_85;
    case 0x78:
      goto switchD_0800735a_caseD_86;
    case 0x79:
      goto switchD_0800735a_caseD_87;
    case 0x7a:
      goto switchD_0800735a_caseD_88;
    case 0x7b:
      goto switchD_0800735a_caseD_89;
    case 0x7c:
      goto switchD_0800735a_caseD_8a;
    case 0x7d:
      goto switchD_0800735a_caseD_8b;
    case 0x7e:
      goto switchD_0800735a_caseD_8c;
    case 0x7f:
      goto switchD_0800735a_caseD_8d;
    case 0x80:
      goto switchD_0800735a_caseD_8e;
    case 0x81:
      goto switchD_0800735a_caseD_8f;
    case 0x82:
      goto switchD_0800735a_caseD_90;
    case 0x83:
      goto switchD_0800735a_caseD_91;
    case 0x84:
      goto switchD_0800735a_caseD_92;
    case 0x85:
      goto switchD_0800735a_caseD_93;
    case 0x86:
      goto switchD_0800735a_caseD_94;
    case 0x87:
      goto switchD_0800735a_caseD_95;
    case 0x88:
      goto switchD_0800735a_caseD_96;
    case 0x89:
      goto switchD_0800735a_caseD_97;
    case 0x8a:
      goto switchD_0800735a_caseD_98;
    case 0x8b:
      goto switchD_0800735a_caseD_99;
    case 0x8c:
      goto switchD_0800735a_caseD_9a;
    case 0x8d:
      goto switchD_0800735a_caseD_9b;
    case 0x8e:
      goto switchD_0800735a_caseD_9c;
    case 0x8f:
      goto switchD_0800735a_caseD_9d;
    case 0x90:
      goto switchD_0800735a_caseD_9e;
    case 0x91:
      goto switchD_0800735a_caseD_9f;
    case 0x92:
      goto switchD_0800735a_caseD_a0;
    case 0x93:
      goto switchD_0800735a_caseD_a1;
    case 0x94:
      goto switchD_0800735a_caseD_a2;
    case 0x95:
      goto switchD_0800735a_caseD_a3;
    case 0x96:
      goto switchD_0800735a_caseD_a4;
    case 0x97:
      goto switchD_0800735a_caseD_a5;
    case 0x98:
      goto switchD_0800735a_caseD_a6;
    case 0x99:
      goto switchD_0800735a_caseD_a7;
    case 0x9a:
      goto switchD_0800735a_caseD_a8;
    case 0x9b:
      goto switchD_0800735a_caseD_a9;
    case 0x9c:
      goto switchD_0800735a_caseD_aa;
    case 0x9d:
      goto switchD_0800735a_caseD_ab;
    case 0x9e:
switchD_0800735a_caseD_ac:
    case 0x9f:
switchD_0800735a_caseD_ad:
      param_2 = &DAT_20000131;
switchD_0800735a_caseD_ae:
      param_1 = (byte *)0x0;
switchD_0800735a_caseD_af:
      param_2 = (byte *)(uint)*param_2;
switchD_0800735a_caseD_b0:
      bVar5 = param_2 == (byte *)0x1;
switchD_0800735a_caseD_b1:
      if (!bVar5) {
        return param_1;
      }
switchD_0800735a_caseD_b2:
      param_2 = &DAT_2000016f;
switchD_0800735a_caseD_b3:
      param_2 = (byte *)(uint)*param_2;
switchD_0800735a_caseD_b4:
      bVar5 = param_2 == &PendSV;
switchD_0800735a_caseD_b5:
      if (!bVar5) {
switchD_0800735a_caseD_b6:
        bVar5 = param_2 == (byte *)0x42;
switchD_0800735a_caseD_b7:
        if (!bVar5) {
switchD_0800735a_caseD_b8:
          bVar5 = param_2 == (byte *)0x48;
switchD_0800735a_caseD_b9:
          if (!bVar5) {
switchD_0800735a_caseD_ba:
            bVar5 = param_2 == (byte *)0x3d;
switchD_0800735a_caseD_bb:
            if (!bVar5) {
switchD_0800735a_caseD_bc:
              bVar5 = param_2 == &IRQ;
switchD_0800735a_caseD_bd:
              if (!bVar5) {
switchD_0800735a_caseD_be:
                bVar5 = param_2 == (byte *)0x43;
switchD_0800735a_caseD_bf:
                if (!bVar5) {
switchD_0800735a_caseD_c0:
                  bVar5 = param_2 == (byte *)0x3f;
switchD_0800735a_caseD_c1:
                  if (!bVar5) {
switchD_0800735a_caseD_c2:
                    bVar5 = param_2 == (byte *)0x46;
switchD_0800735a_caseD_c3:
                    if (!bVar5) {
switchD_0800735a_caseD_c4:
                      bVar5 = param_2 == (byte *)0x44;
switchD_0800735a_caseD_c5:
                      if (!bVar5) {
switchD_0800735a_caseD_c6:
                        bVar5 = param_2 == (byte *)0x39;
switchD_0800735a_caseD_c7:
                        if (!bVar5) {
switchD_0800735a_caseD_c8:
                          bVar5 = param_2 == (byte *)0x3a;
switchD_0800735a_caseD_c9:
                          if (!bVar5) {
switchD_0800735a_caseD_ca:
                            bVar5 = param_2 == (byte *)0x32;
switchD_0800735a_caseD_cb:
                            if (!bVar5) {
switchD_0800735a_caseD_cc:
                              bVar5 = param_2 == (byte *)0x3b;
switchD_0800735a_caseD_cd:
                              if (!bVar5) {
switchD_0800735a_caseD_ce:
                                bVar5 = param_2 == (byte *)0xef;
switchD_0800735a_caseD_cf:
                                if (bVar5) {
switchD_0800735a_caseD_e7:
                                  param_1 = (byte *)0x9;
switchD_0800735a_caseD_e8:
                                  return param_1;
                                }
switchD_0800735a_caseD_d0:
                                bVar5 = param_2 == &SysTick;
switchD_0800735a_caseD_d1:
                                if (!bVar5) {
switchD_0800735a_caseD_d2:
                                  bVar5 = param_2 == (byte *)0x3e;
switchD_0800735a_caseD_d3:
                                  if (!bVar5) {
switchD_0800735a_caseD_d4:
                                    bVar5 = param_2 == (byte *)0x41;
switchD_0800735a_caseD_d5:
                                    if (!bVar5) {
switchD_0800735a_caseD_d6:
                                      bVar5 = param_2 == (byte *)0x45;
switchD_0800735a_caseD_d7:
                                      if (!bVar5) {
switchD_0800735a_caseD_d8:
                                        bVar5 = param_2 == (byte *)0x47;
switchD_0800735a_caseD_d9:
                                        if (!bVar5) {
switchD_0800735a_caseD_da:
                                          param_2 = param_2 + -0x49;
switchD_0800735a_caseD_db:
                                          bVar6 = (byte *)0x16 < param_2;
                                          bVar5 = param_2 == (byte *)0x17;
switchD_0800735a_caseD_dc:
                                          if (bVar6 && !bVar5) {
                                            return param_1;
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                                goto switchD_0800735a_caseD_dd;
                              }
                            }
switchD_0800735a_caseD_e5:
                            param_1 = (byte *)0x7;
switchD_0800735a_caseD_e6:
                            return param_1;
                          }
                        }
switchD_0800735a_caseD_e3:
                        param_1 = (byte *)0x6;
switchD_0800735a_caseD_e4:
                        return param_1;
                      }
                    }
                  }
switchD_0800735a_caseD_e1:
                  param_1 = (byte *)0x5;
switchD_0800735a_caseD_e2:
                  return param_1;
                }
              }
            }
switchD_0800735a_caseD_dd:
            param_1 = (byte *)0x3;
switchD_0800735a_caseD_de:
            return param_1;
          }
        }
      }
switchD_0800735a_caseD_df:
      param_1 = (byte *)0x2;
switchD_0800735a_caseD_e0:
      return param_1;
    case 0xa0:
      goto switchD_0800735a_caseD_ae;
    case 0xa1:
      goto switchD_0800735a_caseD_af;
    case 0xa2:
      goto switchD_0800735a_caseD_b0;
    case 0xa3:
      goto switchD_0800735a_caseD_b1;
    case 0xa4:
      goto switchD_0800735a_caseD_b2;
    case 0xa5:
      goto switchD_0800735a_caseD_b3;
    case 0xa6:
      goto switchD_0800735a_caseD_b4;
    case 0xa7:
      goto switchD_0800735a_caseD_b5;
    case 0xa8:
      goto switchD_0800735a_caseD_b6;
    case 0xa9:
      goto switchD_0800735a_caseD_b7;
    case 0xaa:
      goto switchD_0800735a_caseD_b8;
    case 0xab:
      goto switchD_0800735a_caseD_b9;
    case 0xac:
      goto switchD_0800735a_caseD_ba;
    case 0xad:
      goto switchD_0800735a_caseD_bb;
    case 0xae:
      goto switchD_0800735a_caseD_bc;
    case 0xaf:
      goto switchD_0800735a_caseD_bd;
    case 0xb0:
      goto switchD_0800735a_caseD_be;
    case 0xb1:
      goto switchD_0800735a_caseD_bf;
    case 0xb2:
      goto switchD_0800735a_caseD_c0;
    case 0xb3:
      goto switchD_0800735a_caseD_c1;
    case 0xb4:
      goto switchD_0800735a_caseD_c2;
    case 0xb5:
      goto switchD_0800735a_caseD_c3;
    case 0xb6:
      goto switchD_0800735a_caseD_c4;
    case 0xb7:
      goto switchD_0800735a_caseD_c5;
    case 0xb8:
      goto switchD_0800735a_caseD_c6;
    case 0xb9:
      goto switchD_0800735a_caseD_c7;
    case 0xba:
      goto switchD_0800735a_caseD_c8;
    case 0xbb:
      goto switchD_0800735a_caseD_c9;
    case 0xbc:
      goto switchD_0800735a_caseD_ca;
    case 0xbd:
      goto switchD_0800735a_caseD_cb;
    case 0xbe:
      goto switchD_0800735a_caseD_cc;
    case 0xbf:
      goto switchD_0800735a_caseD_cd;
    case 0xc0:
      goto switchD_0800735a_caseD_ce;
    case 0xc1:
      goto switchD_0800735a_caseD_cf;
    case 0xc2:
      goto switchD_0800735a_caseD_d0;
    case 0xc3:
      goto switchD_0800735a_caseD_d1;
    case 0xc4:
      goto switchD_0800735a_caseD_d2;
    case 0xc5:
      goto switchD_0800735a_caseD_d3;
    case 0xc6:
      goto switchD_0800735a_caseD_d4;
    case 199:
      goto switchD_0800735a_caseD_d5;
    case 200:
      goto switchD_0800735a_caseD_d6;
    case 0xc9:
      goto switchD_0800735a_caseD_d7;
    case 0xca:
      goto switchD_0800735a_caseD_d8;
    case 0xcb:
      goto switchD_0800735a_caseD_d9;
    case 0xcc:
      goto switchD_0800735a_caseD_da;
    case 0xcd:
      goto switchD_0800735a_caseD_db;
    case 0xce:
      goto switchD_0800735a_caseD_dc;
    case 0xcf:
      goto switchD_0800735a_caseD_dd;
    case 0xd0:
      goto switchD_0800735a_caseD_de;
    case 0xd1:
      goto switchD_0800735a_caseD_df;
    case 0xd2:
      goto switchD_0800735a_caseD_e0;
    case 0xd3:
      goto switchD_0800735a_caseD_e1;
    case 0xd4:
      goto switchD_0800735a_caseD_e2;
    case 0xd5:
      goto switchD_0800735a_caseD_e3;
    case 0xd6:
      goto switchD_0800735a_caseD_e4;
    case 0xd7:
      goto switchD_0800735a_caseD_e5;
    case 0xd8:
      goto switchD_0800735a_caseD_e6;
    case 0xd9:
      goto switchD_0800735a_caseD_e7;
    case 0xda:
      goto switchD_0800735a_caseD_e8;
    case 0xdb:
switchD_0800735a_caseD_e9:
    case 0xdc:
switchD_0800735a_caseD_ea:
switchD_0800735a_caseD_eb:
switchD_0800735a_caseD_ec:
switchD_0800735a_caseD_ed:
      param_1 = &DAT_20000168;
switchD_0800735a_caseD_ee:
      param_1 = (byte *)(uint)*param_1;
switchD_0800735a_caseD_ef:
      bVar6 = (byte *)0x7f < param_1;
      bVar5 = param_1 == (byte *)0x80;
switchD_0800735a_caseD_f0:
      if (bVar6 && !bVar5) {
switchD_0800735a_caseD_fa:
        param_1 = (byte *)0x1388;
switchD_0800735a_caseD_fb:
        return param_1;
      }
switchD_0800735a_caseD_f1:
      bVar6 = (byte *)0xf < param_1;
switchD_0800735a_caseD_f2:
      if (!bVar6) {
switchD_0800735a_caseD_f3:
        param_1 = &MemManage;
      }
switchD_0800735a_caseD_f4:
      param_2 = (byte *)&DAT_08007c48;
switchD_0800735a_caseD_f5:
      param_1 = (byte *)((int)param_1 << 1);
switchD_0800735a_caseD_f6:
      param_1 = param_1 + (int)param_2;
switchD_0800735a_caseD_f7:
      param_1 = param_1 + -0x40;
switchD_0800735a_caseD_f8:
      param_1 = (byte *)(uint)*(ushort *)(param_1 + 0x20);
switchD_0800735a_caseD_f9:
      return param_1;
    case 0xdd:
      goto switchD_0800735a_caseD_eb;
    case 0xde:
      goto switchD_0800735a_caseD_ec;
    case 0xdf:
      goto switchD_0800735a_caseD_ed;
    case 0xe0:
      goto switchD_0800735a_caseD_ee;
    case 0xe1:
      goto switchD_0800735a_caseD_ef;
    case 0xe2:
      goto switchD_0800735a_caseD_f0;
    case 0xe3:
      goto switchD_0800735a_caseD_f1;
    case 0xe4:
      goto switchD_0800735a_caseD_f2;
    case 0xe5:
      goto switchD_0800735a_caseD_f3;
    case 0xe6:
      goto switchD_0800735a_caseD_f4;
    case 0xe7:
      goto switchD_0800735a_caseD_f5;
    case 0xe8:
      goto switchD_0800735a_caseD_f6;
    case 0xe9:
      goto switchD_0800735a_caseD_f7;
    case 0xea:
      goto switchD_0800735a_caseD_f8;
    case 0xeb:
      goto switchD_0800735a_caseD_f9;
    case 0xec:
      goto switchD_0800735a_caseD_fa;
    case 0xed:
      goto switchD_0800735a_caseD_fb;
    case 0xee:
switchD_0800735a_caseD_fc:
    case 0xef:
switchD_0800735a_caseD_fd:
switchD_0800735a_caseD_fe:
switchD_0800735a_caseD_ff:
switchD_08007376_caseD_f2:
switchD_08007376_caseD_f3:
      param_1 = (byte *)((int)param_2 >> 0xe);
switchD_08007376_caseD_f4:
switchD_08007376_caseD_f5:
      bVar7 = SBORROW4((int)param_1,0x18);
      bVar4 = (int)(param_1 + -0x18) < 0;
      bVar5 = param_1 == &UsageFault;
switchD_08007376_caseD_f6:
      bVar2 = false;
      if (bVar5) {
        return (byte *)0x12;
      }
switchD_08007376_caseD_f7:
      if (bVar2 || bVar4 != bVar7) {
switchD_08007376_caseD_f8:
        bVar7 = SBORROW4((int)param_1,0xb);
        bVar4 = (int)(param_1 + -0xb) < 0;
        bVar5 = param_1 == (byte *)0xb;
switchD_08007376_caseD_f9:
        bVar2 = false;
        if (bVar5) {
          return &NMI;
        }
switchD_08007376_caseD_fa:
        if (bVar2 || bVar4 != bVar7) {
switchD_08007376_caseD_fb:
          bVar5 = param_1 == (byte *)0x0;
switchD_08007376_caseD_fc:
          if (bVar5) {
            return param_1;
          }
switchD_08007376_caseD_fd:
          bVar5 = param_1 == (byte *)0x3;
switchD_08007376_caseD_fe:
          if (bVar5) {
            return (byte *)0x2;
          }
switchD_08007376_caseD_ff:
          if (param_1 == (byte *)0x5) {
            return &Reset;
          }
          if (param_1 == &NMI) {
            return (byte *)0x6;
          }
        }
        else {
          if (param_1 == (byte *)0xd) {
            return (byte *)0xa;
          }
          if (param_1 == &MemManage) {
            return &HardFault;
          }
          if (param_1 == (byte *)0x13) {
            return (byte *)0xe;
          }
          if (param_1 == (byte *)0x15) {
            return &MemManage;
          }
        }
      }
      else {
        if (param_1 == (byte *)0xf3) {
          return (byte *)0xf6;
        }
        if ((int)param_1 < 0xf4) {
          if (param_1 == (byte *)0xe8) {
            return (byte *)0xee;
          }
          if (param_1 == (byte *)0xeb) {
            return (byte *)0xf0;
          }
          if (param_1 == (byte *)0xed) {
            return (byte *)0xf2;
          }
          if (param_1 == (byte *)0xf0) {
            return (byte *)0xf4;
          }
        }
        else {
          if (param_1 == (byte *)0xf5) {
            return (byte *)0xf8;
          }
          if (param_1 == (byte *)0xf8) {
            return (byte *)0xfa;
          }
          if (param_1 == (byte *)0xfb) {
            return (byte *)0xfc;
          }
          if (param_1 == (byte *)0xfd) {
            return (byte *)0xfe;
          }
        }
      }
      return (byte *)0x0;
    case 0xf0:
      goto switchD_0800735a_caseD_fe;
    case 0xf1:
      goto switchD_0800735a_caseD_ff;
    case 0xf2:
      goto switchD_08007376_caseD_f2;
    case 0xf3:
      goto switchD_08007376_caseD_f3;
    case 0xf4:
      goto switchD_08007376_caseD_f4;
    case 0xf5:
      goto switchD_08007376_caseD_f5;
    case 0xf6:
      goto switchD_08007376_caseD_f6;
    case 0xf7:
      goto switchD_08007376_caseD_f7;
    case 0xf8:
      goto switchD_08007376_caseD_f8;
    case 0xf9:
      goto switchD_08007376_caseD_f9;
    case 0xfa:
      goto switchD_08007376_caseD_fa;
    case 0xfb:
      goto switchD_08007376_caseD_fb;
    case 0xfc:
      goto switchD_08007376_caseD_fc;
    case 0xfd:
      goto switchD_08007376_caseD_fd;
    case 0xfe:
      goto switchD_08007376_caseD_fe;
    case 0xff:
      goto switchD_08007376_caseD_ff;
    }
    break;
  case 2:
    goto switchD_0800735a_caseD_2;
  case 3:
    goto switchD_0800735a_caseD_3;
  case 4:
    goto switchD_0800735a_caseD_4;
  case 5:
    goto switchD_0800735a_caseD_5;
  case 6:
    goto switchD_0800735a_caseD_6;
  case 7:
    goto switchD_0800735a_caseD_7;
  case 8:
    goto switchD_0800735a_caseD_8;
  case 9:
    goto switchD_0800735a_caseD_9;
  case 10:
    goto switchD_0800735a_caseD_a;
  case 0xb:
    goto switchD_0800735a_caseD_b;
  case 0xc:
    goto switchD_0800735a_caseD_c;
  case 0xd:
    coprocessor_movefromRt(0xc,3,0,in_cr15,in_cr1);
    break;
  case 0xe:
    goto switchD_0800735a_caseD_e;
  case 0x10:
    goto switchD_0800735a_caseD_10;
  case 0x11:
    goto switchD_0800735a_caseD_11;
  case 0x12:
    goto switchD_0800735a_caseD_12;
  case 0x13:
    goto switchD_0800735a_caseD_13;
  case 0x14:
    goto switchD_0800735a_caseD_14;
  case 0x15:
    goto switchD_0800735a_caseD_15;
  case 0x16:
    goto switchD_0800735a_caseD_16;
  case 0x17:
    goto switchD_0800735a_caseD_17;
  case 0x18:
    goto switchD_0800735a_caseD_18;
  case 0x19:
    goto switchD_0800735a_caseD_19;
  case 0x1a:
    goto switchD_0800735a_caseD_1a;
  case 0x1b:
    goto switchD_0800735a_caseD_1b;
  case 0x1c:
    goto switchD_0800735a_caseD_1c;
  case 0x1d:
    goto switchD_0800735a_caseD_1d;
  case 0x1e:
    goto switchD_0800735a_caseD_1e;
  case 0x1f:
    goto switchD_0800735a_caseD_1f;
  case 0x20:
    goto switchD_0800735a_caseD_20;
  case 0x21:
    goto switchD_0800735a_caseD_21;
  case 0x22:
    goto switchD_0800735a_caseD_22;
  case 0x23:
    goto switchD_0800735a_caseD_23;
  case 0x24:
    goto switchD_0800735a_caseD_24;
  case 0x25:
    goto switchD_0800735a_caseD_25;
  case 0x26:
    goto switchD_0800735a_caseD_26;
  case 0x27:
    goto switchD_0800735a_caseD_27;
  case 0x28:
    goto switchD_0800735a_caseD_28;
  case 0x29:
    goto switchD_0800735a_caseD_29;
  case 0x2a:
    goto switchD_0800735a_caseD_2a;
  case 0x2b:
    goto switchD_0800735a_caseD_2b;
  case 0x2c:
    goto switchD_0800735a_caseD_2c;
  case 0x2d:
    goto switchD_0800735a_caseD_2d;
  case 0x2e:
    goto switchD_0800735a_caseD_2e;
  case 0x2f:
    goto switchD_0800735a_caseD_2f;
  case 0x30:
    goto switchD_0800735a_caseD_30;
  case 0x31:
    goto switchD_0800735a_caseD_31;
  case 0x32:
    goto switchD_0800735a_caseD_32;
  case 0x33:
    goto switchD_0800735a_caseD_33;
  case 0x34:
    goto switchD_0800735a_caseD_34;
  case 0x35:
    goto switchD_0800735a_caseD_35;
  case 0x36:
    goto switchD_0800735a_caseD_36;
  case 0x37:
    goto switchD_0800735a_caseD_37;
  case 0x38:
    goto switchD_0800735a_caseD_38;
  case 0x39:
    goto switchD_0800735a_caseD_39;
  case 0x3a:
    goto switchD_0800735a_caseD_3a;
  case 0x3b:
    goto switchD_0800735a_caseD_3b;
  case 0x3c:
    goto switchD_0800735a_caseD_3c;
  case 0x3d:
    goto switchD_0800735a_caseD_3d;
  case 0x3e:
    goto switchD_0800735a_caseD_3e;
  case 0x3f:
    goto switchD_0800735a_caseD_3f;
  case 0x40:
    goto switchD_0800735a_caseD_40;
  case 0x41:
    goto switchD_0800735a_caseD_41;
  case 0x42:
    goto switchD_0800735a_caseD_42;
  case 0x43:
    goto switchD_0800735a_caseD_43;
  case 0x44:
    goto switchD_0800735a_caseD_44;
  case 0x45:
    goto switchD_0800735a_caseD_45;
  case 0x46:
    goto switchD_0800735a_caseD_46;
  case 0x47:
    goto switchD_0800735a_caseD_47;
  case 0x48:
    goto switchD_0800735a_caseD_48;
  case 0x49:
    goto switchD_0800735a_caseD_49;
  case 0x4a:
    goto switchD_0800735a_caseD_4a;
  case 0x4b:
    goto switchD_0800735a_caseD_4b;
  case 0x4c:
    goto switchD_0800735a_caseD_4c;
  case 0x4d:
    goto switchD_0800735a_caseD_4d;
  case 0x4e:
    goto switchD_0800735a_caseD_4e;
  case 0x4f:
    goto switchD_0800735a_caseD_4f;
  case 0x50:
    goto switchD_0800735a_caseD_50;
  case 0x51:
    goto switchD_0800735a_caseD_51;
  case 0x52:
    goto switchD_0800735a_caseD_52;
  case 0x53:
    goto switchD_0800735a_caseD_53;
  case 0x54:
    goto switchD_0800735a_caseD_54;
  case 0x55:
    goto switchD_0800735a_caseD_55;
  case 0x56:
    goto switchD_0800735a_caseD_56;
  case 0x57:
    goto switchD_0800735a_caseD_57;
  case 0x58:
    goto switchD_0800735a_caseD_58;
  case 0x59:
    goto switchD_0800735a_caseD_59;
  case 0x5a:
    goto switchD_0800735a_caseD_5a;
  case 0x5b:
    goto switchD_0800735a_caseD_5b;
  case 0x5c:
    goto switchD_0800735a_caseD_5c;
  case 0x5d:
    goto switchD_0800735a_caseD_5d;
  case 0x5e:
    goto switchD_0800735a_caseD_5e;
  case 0x5f:
    goto switchD_0800735a_caseD_5f;
  case 0x60:
    goto switchD_0800735a_caseD_60;
  case 0x61:
    goto switchD_0800735a_caseD_61;
  case 0x62:
    goto switchD_0800735a_caseD_62;
  case 99:
    goto switchD_0800735a_caseD_63;
  case 100:
    goto switchD_0800735a_caseD_64;
  case 0x65:
    goto switchD_0800735a_caseD_65;
  case 0x66:
    goto switchD_0800735a_caseD_66;
  case 0x67:
    goto switchD_0800735a_caseD_67;
  case 0x68:
    goto switchD_0800735a_caseD_68;
  case 0x69:
    goto switchD_0800735a_caseD_69;
  case 0x6a:
    goto switchD_0800735a_caseD_6a;
  case 0x6b:
    goto switchD_0800735a_caseD_6b;
  case 0x6c:
    goto switchD_0800735a_caseD_6c;
  case 0x6d:
    goto switchD_0800735a_caseD_6d;
  case 0x6e:
    goto switchD_0800735a_caseD_6e;
  case 0x6f:
    goto switchD_0800735a_caseD_6f;
  case 0x70:
    goto switchD_0800735a_caseD_70;
  case 0x71:
    goto switchD_0800735a_caseD_71;
  case 0x72:
    goto switchD_0800735a_caseD_72;
  case 0x73:
    goto switchD_0800735a_caseD_73;
  case 0x74:
    goto switchD_0800735a_caseD_74;
  case 0x75:
    goto switchD_0800735a_caseD_75;
  case 0x76:
    goto switchD_0800735a_caseD_76;
  case 0x77:
    goto switchD_0800735a_caseD_77;
  case 0x78:
    goto switchD_0800735a_caseD_78;
  case 0x79:
    goto switchD_0800735a_caseD_79;
  case 0x7a:
    goto switchD_0800735a_caseD_7a;
  case 0x7b:
    goto switchD_0800735a_caseD_7b;
  case 0x7c:
    goto switchD_0800735a_caseD_7c;
  case 0x7d:
    goto switchD_0800735a_caseD_7d;
  case 0x7e:
    goto switchD_0800735a_caseD_7e;
  case 0x7f:
    goto switchD_0800735a_caseD_7f;
  case 0x80:
    goto switchD_0800735a_caseD_80;
  case 0x81:
    goto switchD_0800735a_caseD_81;
  case 0x82:
    goto switchD_0800735a_caseD_82;
  case 0x83:
    goto switchD_0800735a_caseD_83;
  case 0x84:
    goto switchD_0800735a_caseD_84;
  case 0x85:
    goto switchD_0800735a_caseD_85;
  case 0x86:
    goto switchD_0800735a_caseD_86;
  case 0x87:
    goto switchD_0800735a_caseD_87;
  case 0x88:
    goto switchD_0800735a_caseD_88;
  case 0x89:
    goto switchD_0800735a_caseD_89;
  case 0x8a:
    goto switchD_0800735a_caseD_8a;
  case 0x8b:
    goto switchD_0800735a_caseD_8b;
  case 0x8c:
    goto switchD_0800735a_caseD_8c;
  case 0x8d:
    goto switchD_0800735a_caseD_8d;
  case 0x8e:
    goto switchD_0800735a_caseD_8e;
  case 0x8f:
    goto switchD_0800735a_caseD_8f;
  case 0x90:
    goto switchD_0800735a_caseD_90;
  case 0x91:
    goto switchD_0800735a_caseD_91;
  case 0x92:
    goto switchD_0800735a_caseD_92;
  case 0x93:
    goto switchD_0800735a_caseD_93;
  case 0x94:
    goto switchD_0800735a_caseD_94;
  case 0x95:
    goto switchD_0800735a_caseD_95;
  case 0x96:
    goto switchD_0800735a_caseD_96;
  case 0x97:
    goto switchD_0800735a_caseD_97;
  case 0x98:
    goto switchD_0800735a_caseD_98;
  case 0x99:
    goto switchD_0800735a_caseD_99;
  case 0x9a:
    goto switchD_0800735a_caseD_9a;
  case 0x9b:
    goto switchD_0800735a_caseD_9b;
  case 0x9c:
    goto switchD_0800735a_caseD_9c;
  case 0x9d:
    goto switchD_0800735a_caseD_9d;
  case 0x9e:
    goto switchD_0800735a_caseD_9e;
  case 0x9f:
    goto switchD_0800735a_caseD_9f;
  case 0xa0:
    goto switchD_0800735a_caseD_a0;
  case 0xa1:
    goto switchD_0800735a_caseD_a1;
  case 0xa2:
    goto switchD_0800735a_caseD_a2;
  case 0xa3:
    goto switchD_0800735a_caseD_a3;
  case 0xa4:
    goto switchD_0800735a_caseD_a4;
  case 0xa5:
    goto switchD_0800735a_caseD_a5;
  case 0xa6:
    goto switchD_0800735a_caseD_a6;
  case 0xa7:
    goto switchD_0800735a_caseD_a7;
  case 0xa8:
    goto switchD_0800735a_caseD_a8;
  case 0xa9:
    goto switchD_0800735a_caseD_a9;
  case 0xaa:
    goto switchD_0800735a_caseD_aa;
  case 0xab:
    goto switchD_0800735a_caseD_ab;
  case 0xac:
    goto switchD_0800735a_caseD_ac;
  case 0xad:
    goto switchD_0800735a_caseD_ad;
  case 0xae:
    goto switchD_0800735a_caseD_ae;
  case 0xaf:
    goto switchD_0800735a_caseD_af;
  case 0xb0:
    goto switchD_0800735a_caseD_b0;
  case 0xb1:
    goto switchD_0800735a_caseD_b1;
  case 0xb2:
    goto switchD_0800735a_caseD_b2;
  case 0xb3:
    goto switchD_0800735a_caseD_b3;
  case 0xb4:
    goto switchD_0800735a_caseD_b4;
  case 0xb5:
    goto switchD_0800735a_caseD_b5;
  case 0xb6:
    goto switchD_0800735a_caseD_b6;
  case 0xb7:
    goto switchD_0800735a_caseD_b7;
  case 0xb8:
    goto switchD_0800735a_caseD_b8;
  case 0xb9:
    goto switchD_0800735a_caseD_b9;
  case 0xba:
    goto switchD_0800735a_caseD_ba;
  case 0xbb:
    goto switchD_0800735a_caseD_bb;
  case 0xbc:
    goto switchD_0800735a_caseD_bc;
  case 0xbd:
    goto switchD_0800735a_caseD_bd;
  case 0xbe:
    goto switchD_0800735a_caseD_be;
  case 0xbf:
    goto switchD_0800735a_caseD_bf;
  case 0xc0:
    goto switchD_0800735a_caseD_c0;
  case 0xc1:
    goto switchD_0800735a_caseD_c1;
  case 0xc2:
    goto switchD_0800735a_caseD_c2;
  case 0xc3:
    goto switchD_0800735a_caseD_c3;
  case 0xc4:
    goto switchD_0800735a_caseD_c4;
  case 0xc5:
    goto switchD_0800735a_caseD_c5;
  case 0xc6:
    goto switchD_0800735a_caseD_c6;
  case 199:
    goto switchD_0800735a_caseD_c7;
  case 200:
    goto switchD_0800735a_caseD_c8;
  case 0xc9:
    goto switchD_0800735a_caseD_c9;
  case 0xca:
    goto switchD_0800735a_caseD_ca;
  case 0xcb:
    goto switchD_0800735a_caseD_cb;
  case 0xcc:
    goto switchD_0800735a_caseD_cc;
  case 0xcd:
    goto switchD_0800735a_caseD_cd;
  case 0xce:
    goto switchD_0800735a_caseD_ce;
  case 0xcf:
    goto switchD_0800735a_caseD_cf;
  case 0xd0:
    goto switchD_0800735a_caseD_d0;
  case 0xd1:
    goto switchD_0800735a_caseD_d1;
  case 0xd2:
    goto switchD_0800735a_caseD_d2;
  case 0xd3:
    goto switchD_0800735a_caseD_d3;
  case 0xd4:
    goto switchD_0800735a_caseD_d4;
  case 0xd5:
    goto switchD_0800735a_caseD_d5;
  case 0xd6:
    goto switchD_0800735a_caseD_d6;
  case 0xd7:
    goto switchD_0800735a_caseD_d7;
  case 0xd8:
    goto switchD_0800735a_caseD_d8;
  case 0xd9:
    goto switchD_0800735a_caseD_d9;
  case 0xda:
    goto switchD_0800735a_caseD_da;
  case 0xdb:
    goto switchD_0800735a_caseD_db;
  case 0xdc:
    goto switchD_0800735a_caseD_dc;
  case 0xdd:
    goto switchD_0800735a_caseD_dd;
  case 0xde:
    goto switchD_0800735a_caseD_de;
  case 0xdf:
    goto switchD_0800735a_caseD_df;
  case 0xe0:
    goto switchD_0800735a_caseD_e0;
  case 0xe1:
    goto switchD_0800735a_caseD_e1;
  case 0xe2:
    goto switchD_0800735a_caseD_e2;
  case 0xe3:
    goto switchD_0800735a_caseD_e3;
  case 0xe4:
    goto switchD_0800735a_caseD_e4;
  case 0xe5:
    goto switchD_0800735a_caseD_e5;
  case 0xe6:
    goto switchD_0800735a_caseD_e6;
  case 0xe7:
    goto switchD_0800735a_caseD_e7;
  case 0xe8:
    goto switchD_0800735a_caseD_e8;
  case 0xe9:
    goto switchD_0800735a_caseD_e9;
  case 0xea:
    goto switchD_0800735a_caseD_ea;
  case 0xeb:
    goto switchD_0800735a_caseD_eb;
  case 0xec:
    goto switchD_0800735a_caseD_ec;
  case 0xed:
    goto switchD_0800735a_caseD_ed;
  case 0xee:
    goto switchD_0800735a_caseD_ee;
  case 0xef:
    goto switchD_0800735a_caseD_ef;
  case 0xf0:
    goto switchD_0800735a_caseD_f0;
  case 0xf1:
    goto switchD_0800735a_caseD_f1;
  case 0xf2:
    goto switchD_0800735a_caseD_f2;
  case 0xf3:
    goto switchD_0800735a_caseD_f3;
  case 0xf4:
    goto switchD_0800735a_caseD_f4;
  case 0xf5:
    goto switchD_0800735a_caseD_f5;
  case 0xf6:
    goto switchD_0800735a_caseD_f6;
  case 0xf7:
    goto switchD_0800735a_caseD_f7;
  case 0xf8:
    goto switchD_0800735a_caseD_f8;
  case 0xf9:
    goto switchD_0800735a_caseD_f9;
  case 0xfa:
    goto switchD_0800735a_caseD_fa;
  case 0xfb:
    goto switchD_0800735a_caseD_fb;
  case 0xfc:
    goto switchD_0800735a_caseD_fc;
  case 0xfd:
    goto switchD_0800735a_caseD_fd;
  case 0xfe:
    goto switchD_0800735a_caseD_fe;
  case 0xff:
    goto switchD_0800735a_caseD_ff;
  }
switchD_0800735a_caseD_10:
switchD_0800735a_caseD_11:
switchD_0800735a_caseD_12:
switchD_0800735a_caseD_13:
switchD_0800735a_caseD_14:
switchD_0800735a_caseD_15:
switchD_0800735a_caseD_16:
switchD_0800735a_caseD_17:
switchD_0800735a_caseD_18:
  param_1 = &UsageFault;
switchD_0800735a_caseD_19:
  return param_1;
}

