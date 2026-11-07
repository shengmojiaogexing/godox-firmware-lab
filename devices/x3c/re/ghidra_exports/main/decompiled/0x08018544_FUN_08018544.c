// 0x08018544 FUN_08018544 size=54

undefined1 FUN_08018544(uint param_1)

{
  uint uVar1;
  
  if (param_1 == 0xffff) {
    return 0x1f;
  }
  if (param_1 == 0) {
    return 0;
  }
  if (param_1 < 0x70) {
    return *(undefined1 *)(DAT_0801857c + param_1);
  }
  uVar1 = param_1 - 0x70 & 0xffff;
  if ((*DAT_08018580 == 0) || (*(uint *)(DAT_08018584 + 4) <= uVar1)) {
    return 0;
  }
  return *(undefined1 *)(*DAT_08018580 + uVar1);
}

