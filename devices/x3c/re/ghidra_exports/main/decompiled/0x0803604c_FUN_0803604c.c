// 0x0803604c FUN_0803604c size=32

void FUN_0803604c(void)

{
  *DAT_0803606c = *DAT_0803606c | 4;
  *DAT_0803606c = *DAT_0803606c | 2;
  *DAT_08036070 = *DAT_08036070 | 4;
  do {
    WaitForInterrupt();
  } while( true );
}

