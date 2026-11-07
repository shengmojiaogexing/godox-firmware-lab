// 0x080f9e54 FUN_080f9e54 size=70

undefined4 FUN_080f9e54(void)

{
  int extraout_r2;
  int local_8;
  
  local_8 = 0;
  if (*(int *)(DAT_080f9e9c + 8) << 0x1a < 0) {
    FUN_080f9c88();
    *(uint *)(extraout_r2 + 0xc) = *(uint *)(extraout_r2 + 0xc) & 0xffffff5f;
    do {
      local_8 = local_8 + 1;
      if (local_8 == 0x8000) break;
    } while ((*(uint *)(extraout_r2 + 0xc) & 0x3f) >> 5 == 0);
    *(undefined4 *)(extraout_r2 + 0x24) = 0xff;
    if (-1 < *(int *)(extraout_r2 + 0xc) * 0x4000000) {
      return 0;
    }
  }
  return 1;
}

