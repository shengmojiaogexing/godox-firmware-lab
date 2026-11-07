// 0x08012890 FUN_08012890 size=60

void FUN_08012890(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char *pcVar1;
  byte bVar2;
  undefined2 local_18;
  undefined2 uStack_16;
  undefined4 uStack_14;
  
  pcVar1 = DAT_080128cc;
  bVar2 = 0;
  if (*DAT_080128cc != '\0') {
    *DAT_080128cc = '\0';
    _local_18 = CONCAT22((short)((uint)param_3 >> 0x10),0x1c5);
    uStack_14 = param_4;
    FUN_08013688(&local_18,2);
    do {
      if (pcVar1[1] != '\0') {
        return;
      }
      FUN_0801c0fe(1);
      bVar2 = bVar2 + 1;
    } while (bVar2 < 0xc9);
  }
  return;
}

