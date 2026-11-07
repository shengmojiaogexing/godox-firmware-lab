// 0x0800ff7c FUN_0800ff7c size=168

void FUN_0800ff7c(uint param_1,undefined4 param_2)

{
  if (*DAT_08010038 == '\0') {
    switch(param_1 / 10) {
    case 0:
    case 1:
    case 2:
      FUN_08004c48(param_2,10,&DAT_0801003c);
      return;
    case 3:
      FUN_08004c48(param_2,10,&DAT_08010040);
      return;
    case 4:
      FUN_08004c48(param_2,10,&DAT_08010044);
      return;
    case 5:
      FUN_08004c48(param_2,10,&DAT_08010048);
      return;
    case 6:
      FUN_08004c48(param_2,10,&DAT_0801004c);
      return;
    case 7:
      FUN_08004c48(param_2,10,&DAT_08010050);
      return;
    case 8:
      FUN_08004c48(param_2,10,&DAT_08010054);
      return;
    case 9:
      FUN_08004c48(param_2,10,&DAT_08010058);
      return;
    }
  }
  else if (*DAT_08010038 == '\x01') {
    switch(param_1 / 10) {
    case 0:
    case 1:
    case 2:
      FUN_08004c48(param_2,10,&DAT_0801005c);
      return;
    case 3:
      FUN_08004c48(param_2,10,&DAT_08010060);
      return;
    case 4:
      FUN_08004c48(param_2,10,&DAT_08010064);
      return;
    case 5:
      FUN_08004c48(param_2,10,&DAT_08010068);
      return;
    case 6:
      FUN_08004c48(param_2,10,&DAT_0801006c);
      return;
    case 7:
      FUN_08004c48(param_2,10,&DAT_08010070);
      return;
    case 8:
      FUN_08004c48(param_2,10,&DAT_08010074);
      return;
    case 9:
      FUN_08004c48(param_2,10,&LAB_08010078);
      return;
    }
  }
  return;
}

