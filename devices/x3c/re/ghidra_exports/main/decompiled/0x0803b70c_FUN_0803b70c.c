// 0x0803b70c FUN_0803b70c size=166

undefined4 FUN_0803b70c(int param_1)

{
  char cVar1;
  char *pcVar2;
  
  pcVar2 = DAT_0803b7b4;
  cVar1 = *(char *)(param_1 + 0x2ce);
  *DAT_0803b7b4 = cVar1;
  if ((*(short *)(param_1 + 0x2d0) == 0) && (*(short *)(param_1 + 0x2d2) == 0)) {
    if (*(char *)(param_1 + 0x2e4) == '\x02') {
      if (cVar1 == '\0') {
        FUN_0803acbc(param_1);
      }
      else {
        *(char *)(param_1 + 0x2e9) = cVar1;
        *(undefined1 *)(param_1 + 0x2e4) = 3;
        (*(code *)**(undefined4 **)(param_1 + 4))(param_1);
        FUN_0803acbc(param_1);
      }
    }
    else if (*(char *)(param_1 + 0x2e4) == '\x03') {
      if (cVar1 == '\0') {
        *(undefined1 *)(param_1 + 0x2e4) = 2;
        *(undefined1 *)(param_1 + 0x2e9) = 0;
        (**(code **)(*(int *)(param_1 + 4) + 4))(param_1);
        FUN_0803acbc(param_1);
      }
      else if (*(char *)(param_1 + 0x2e9) == cVar1) {
        (**(code **)(*(int *)(param_1 + 4) + 4))(param_1);
        *(char *)(param_1 + 0x2e9) = *pcVar2;
        (*(code *)**(undefined4 **)(param_1 + 4))(param_1);
        FUN_0803acbc(param_1);
      }
      else {
        FUN_0803acbc(param_1);
      }
    }
    else {
      FUN_0803accc(param_1);
    }
  }
  else {
    FUN_0803accc(param_1);
  }
  return 0;
}

