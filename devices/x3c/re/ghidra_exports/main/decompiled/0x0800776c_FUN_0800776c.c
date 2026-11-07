// 0x0800776c FUN_0800776c size=396

void FUN_0800776c(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  undefined1 auStack_2c [12];
  undefined1 auStack_20 [12];
  
  if (*DAT_08007904 == '\0') {
    uVar1 = param_1 % 10;
    switch(param_1 / 10) {
    case 0:
      if (uVar1 == 0) {
        FUN_08004c48(auStack_20,10,&DAT_08007914);
      }
      else {
        FUN_08004c48(auStack_20,10,&DAT_08007910);
      }
      break;
    case 1:
      if (uVar1 == 0) {
        FUN_08004c48(auStack_20,10,&DAT_08007910);
      }
      else {
        FUN_08004c48(auStack_20,10,&DAT_08007918);
      }
      break;
    case 2:
      if (uVar1 == 0) {
        FUN_08004c48(auStack_20,10,&DAT_08007918);
      }
      else {
        FUN_08004c48(auStack_20,10,&DAT_0800791c);
      }
      break;
    case 3:
      if (uVar1 == 0) {
        FUN_08004c48(auStack_20,10,&DAT_0800791c);
      }
      else {
        FUN_08004c48(auStack_20,10,&DAT_08007920);
      }
      break;
    case 4:
      if (uVar1 == 0) {
        FUN_08004c48(auStack_20,10,&DAT_08007920);
      }
      else {
        FUN_08004c48(auStack_20,10,&DAT_08007924);
      }
      break;
    case 5:
      if (uVar1 == 0) {
        FUN_08004c48(auStack_20,10,&DAT_08007924);
      }
      else {
        FUN_08004c48(auStack_20,10,&DAT_08007928);
      }
      break;
    case 6:
      if (uVar1 == 0) {
        FUN_08004c48(auStack_20,10,&DAT_08007928);
      }
      else {
        FUN_08004c48(auStack_20,10,&DAT_0800792c);
      }
      break;
    case 7:
      if (uVar1 == 0) {
        FUN_08004c48(auStack_20,10,&DAT_0800792c);
      }
      else {
        FUN_08004c48(auStack_20,10,&DAT_08007930);
      }
      break;
    case 8:
      if (uVar1 == 0) {
        FUN_08004c48(auStack_20,10,&DAT_08007930);
      }
      else {
        FUN_08004c48(auStack_20,10,&DAT_08007934);
      }
      break;
    case 9:
      if (uVar1 == 0) {
        FUN_08004c48(auStack_20,10,&DAT_08007934);
      }
    }
    FUN_08004c48(auStack_2c,10,s__0__d_08007938,10 - uVar1);
    if (uVar1 == 0) {
      FUN_0802da22(param_4,1);
    }
    else {
      FUN_0802e2d2(param_4,1);
    }
    FUN_0802d33c(param_3,auStack_20);
    FUN_0802d33c(param_4,auStack_2c);
  }
  else {
    uVar1 = 100 - param_1 & 0xff;
    FUN_08004c48(auStack_20,10,s__d__d_08007908,uVar1 / 10,uVar1 % 10);
    FUN_0802d33c(param_3,auStack_20);
    FUN_0802da22(param_2,1);
    FUN_0802da22(param_4,1);
  }
  return;
}

