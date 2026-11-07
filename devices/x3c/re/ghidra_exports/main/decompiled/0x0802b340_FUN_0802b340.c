// 0x0802b340 FUN_0802b340 size=252

ushort FUN_0802b340(ushort *param_1,int param_2,int param_3,undefined4 param_4)

{
  short sVar1;
  ushort uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  ushort local_24;
  undefined4 local_20;
  undefined4 local_1c;
  
  iVar3 = (int)(param_2 * (uint)(byte)param_1[1]) >> 8;
  if (iVar3 < param_3) {
    if (param_3 < (int)(param_2 * (uint)(byte)(param_1 + ((byte)param_1[4] - 1) * 2)[1]) >> 8) {
      iVar4 = 0;
      for (uVar5 = 1; local_20 = param_3, local_1c = param_4, uVar5 < (byte)param_1[4];
          uVar5 = uVar5 + 1 & 0xff) {
        if (param_3 <= (int)(param_2 * (uint)(byte)param_1[uVar5 * 2 + 1]) >> 8) {
          local_1c = FUN_08027ed8();
          local_20 = FUN_08027ed8();
          iVar3 = (int)(param_2 * (uint)(byte)param_1[(uVar5 - 1) * 2 + 1]) >> 8;
          iVar4 = ((int)(param_2 * (uint)(byte)param_1[uVar5 * 2 + 1]) >> 8) - iVar3;
          break;
        }
      }
      if (iVar4 == 0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      uVar2 = (ushort)(((param_3 - iVar3) * 0xff) / iVar4) & 0xff;
      sVar1 = 0xff - uVar2;
      local_24 = (ushort)((uint)(((int)(short)(ushort)(byte)local_1c * (int)sVar1 +
                                 (int)(short)(ushort)(byte)local_20 * (int)(short)uVar2) * 0x8081)
                         >> 0x1a) & 0x1f |
                 (ushort)(((uint)(((int)(short)(ushort)local_1c._1_1_ * (int)sVar1 +
                                  (int)(short)(ushort)local_20._1_1_ * (int)(short)uVar2) * 0x8081)
                           >> 0x19 & 0x3f) << 5) |
                 (ushort)(((uint)(((int)(short)(ushort)local_1c._2_1_ * (int)sVar1 +
                                  (int)(short)(ushort)local_20._2_1_ * (int)(short)uVar2) * 0x8081)
                           >> 0x1a & 0x1f) << 0xb);
    }
    else {
      local_24 = param_1[((byte)param_1[4] - 1) * 2];
    }
  }
  else {
    local_24 = *param_1;
  }
  return local_24;
}

