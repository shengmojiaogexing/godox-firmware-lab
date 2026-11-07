// 0x0803c4a4 FUN_0803c4a4 size=230

/* WARNING: Removing unreachable block (ram,0x0803c4f6) */
/* WARNING: Removing unreachable block (ram,0x0803c504) */
/* WARNING: Removing unreachable block (ram,0x0803c47c) */
/* WARNING: Removing unreachable block (ram,0x0803c470) */
/* WARNING: Removing unreachable block (ram,0x0803c4fc) */
/* WARNING: Removing unreachable block (ram,0x0803c46a) */
/* WARNING: Removing unreachable block (ram,0x0803c460) */
/* WARNING: Removing unreachable block (ram,0x0803c458) */
/* WARNING: Removing unreachable block (ram,0x0803c45a) */
/* WARNING: Removing unreachable block (ram,0x0803c45e) */
/* WARNING: Removing unreachable block (ram,0x0803c476) */
/* WARNING: Removing unreachable block (ram,0x0803c448) */

ulonglong FUN_0803c4a4(uint param_1,uint param_2,undefined4 param_3,uint param_4)

{
  uint uVar1;
  char in_OV;
  bool bVar2;
  bool bVar3;
  
  uVar1 = param_1 >> 0x17 & 0xff;
  bVar2 = uVar1 == 0;
  if (!bVar2) {
    param_4 = uVar1 ^ 0xff;
    bVar2 = param_4 == 0;
  }
  bVar3 = bVar2;
  if (!bVar2) {
    in_OV = SBORROW4(param_4,param_2);
    bVar3 = param_4 == param_2;
  }
  bVar2 = !bVar2 && (int)(param_4 - param_2) < 0;
  if (!bVar3 && bVar2 == (bool)in_OV) {
    in_OV = SCARRY4(param_2,uVar1);
    bVar2 = (int)(param_2 + uVar1) < 0;
    bVar3 = param_2 + uVar1 == 0;
  }
  if (!bVar3 && bVar2 == (bool)in_OV) {
    return CONCAT44(param_2,param_1 + param_2 * 0x800000);
  }
  if (uVar1 == 0) {
    return CONCAT44(param_2,param_1) & 0xffffffff80000000;
  }
  if (param_4 == 0) {
    if ((param_1 & 0x7fffff) == 0) {
      return CONCAT44(param_2,param_1);
    }
    return CONCAT44(param_2,DAT_0803c1bc);
  }
  param_1 = param_1 & 0x80000000;
  if (-1 < (int)param_2) {
    param_1 = param_1 | 0x7f800000;
  }
  return CONCAT44(param_2,param_1);
}

