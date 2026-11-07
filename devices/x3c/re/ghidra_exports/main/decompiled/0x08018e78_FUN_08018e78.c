// 0x08018e78 FUN_08018e78 size=16

int FUN_08018e78(uint param_1,uint param_2)

{
  if ((param_2 & param_2 - 1) == 0) {
    return param_1 - (param_2 - 1 & param_1);
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

