// 0x0802acb4 FUN_0802acb4 size=54

void FUN_0802acb4(int param_1)

{
  if (param_1 == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  if (*(code **)(param_1 + 0x2c) != (code *)0x0) {
    (**(code **)(param_1 + 0x2c))();
  }
  return;
}

