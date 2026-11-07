// 0x08018e98 FUN_08018e98 size=16

uint FUN_08018e98(int param_1,uint param_2)

{
  if ((param_2 & param_2 - 1) == 0) {
    return (param_1 + param_2) - 1 & ~(param_2 - 1);
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

