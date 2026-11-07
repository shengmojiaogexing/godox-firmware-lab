// 0x0802ae60 FUN_0802ae60 size=10

void FUN_0802ae60(int param_1)

{
  if (param_1 != 0) {
                    /* WARNING: Could not recover jumptable at 0x0802ae66. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(param_1 + 4))();
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

