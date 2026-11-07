// 0x08006ff4 FUN_08006ff4 size=50

void FUN_08006ff4(void)

{
  byte bVar1;
  byte *pbVar2;
  
  pbVar2 = DAT_08007028;
  bVar1 = *DAT_08007028;
  *DAT_08007028 = bVar1 + 1;
  if (0x31 < (byte)(bVar1 + 1)) {
    *pbVar2 = 0;
    *DAT_0800702c = 1;
    *DAT_08007030 = 1;
  }
  pbVar2 = DAT_08007034;
  bVar1 = *DAT_08007034;
  *DAT_08007034 = bVar1 + 1;
  if (0xe < (byte)(bVar1 + 1)) {
    *pbVar2 = 0;
    *DAT_08007038 = 1;
  }
  return;
}

