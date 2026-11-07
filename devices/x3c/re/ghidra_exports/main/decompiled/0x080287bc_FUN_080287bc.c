// 0x080287bc FUN_080287bc size=16

void FUN_080287bc(int param_1)

{
  if (*(code **)(param_1 + 0x18) != (code *)0x0) {
    (**(code **)(param_1 + 0x18))();
  }
  return;
}

