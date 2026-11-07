// 0x0802c924 FUN_0802c924 size=96

void FUN_0802c924(void)

{
  char *pcVar1;
  char local_10;
  
  pcVar1 = DAT_0802c984;
  if (*DAT_0802c984 == '\0') {
    FUN_0802d710();
    FUN_08018588();
    FUN_08017868();
    FUN_08017454();
    FUN_08017874();
    FUN_080287fe();
    FUN_080182c0();
    FUN_08017f7c(DAT_0802c988,0x25c);
    FUN_08017f7c(DAT_0802c98c,0x88);
    FUN_08018534();
    FUN_08017a4c();
    local_10 = (char)DAT_0802c990;
    if (local_10 == '\x11') {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    FUN_0802adea();
    *pcVar1 = '\x01';
  }
  return;
}

