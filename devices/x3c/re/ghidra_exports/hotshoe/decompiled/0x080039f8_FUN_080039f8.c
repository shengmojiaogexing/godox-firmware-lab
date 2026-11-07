// 0x080039f8 FUN_080039f8 size=36

undefined4 FUN_080039f8(void)

{
  uint local_8;
  
  local_8 = 0;
  do {
    if (*DAT_08003a1c << 0xe < 0) {
      return 1;
    }
    local_8 = local_8 + 1;
  } while (local_8 < 0x10000);
  return 0;
}

