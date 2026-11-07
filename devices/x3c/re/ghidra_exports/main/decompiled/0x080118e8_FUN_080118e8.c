// 0x080118e8 FUN_080118e8 size=254

void FUN_080118e8(uint param_1,uint param_2,uint param_3)

{
  char *pcVar1;
  undefined1 *puVar2;
  char *pcVar3;
  
  pcVar1 = DAT_080119cc;
  if (DAT_080119cc[0xb] == '\0') {
    if (DAT_080119cc[5] == '\0') {
      DAT_080119cc[7] = '\0';
      FUN_080115a0();
      puVar2 = DAT_080119d4;
      if ((byte)pcVar1[1] != param_1) {
        if (param_1 == 2) {
          FUN_08023520(0x36);
          thunk_FUN_0801c118(1000);
          FUN_08014814(param_2);
          pcVar1[2] = (char)param_2;
          FUN_08014864(param_3);
          pcVar1[3] = (char)param_3;
          FUN_08023520(0x33);
          thunk_FUN_0801c118(1000);
          FUN_08023520(0x36);
        }
        else {
          FUN_08023520(0x36);
          thunk_FUN_0801c118(1000);
          FUN_08023520(0x39);
          FUN_08022e38(DAT_080119d0,0x4000);
          pcVar1[2] = '\0';
          pcVar1[3] = '\0';
        }
        pcVar1[1] = (char)param_1;
        return;
      }
      if (param_2 != (byte)pcVar1[2]) {
        pcVar1[0xc] = '\0';
        FUN_08014814(param_2);
        pcVar1[2] = (char)param_2;
        *puVar2 = 0x14;
      }
      if (param_3 != (byte)pcVar1[3]) {
        FUN_08014864(param_3);
        pcVar1[3] = (char)param_3;
        *puVar2 = 0x14;
      }
      pcVar3 = DAT_080119d8;
      if (*DAT_080119d8 != pcVar1[4]) {
        FUN_080128d0();
        pcVar1[4] = *pcVar3;
      }
    }
    else {
      FUN_0801616c();
    }
    if (*pcVar1 != '\0') {
      *pcVar1 = '\0';
      FUN_08023520(0x36);
      FUN_08023520(0x33);
      thunk_FUN_0801c118(1000);
      return;
    }
  }
  return;
}

