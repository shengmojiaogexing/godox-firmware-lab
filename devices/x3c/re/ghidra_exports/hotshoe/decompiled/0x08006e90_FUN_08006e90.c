// 0x08006e90 FUN_08006e90 size=90

void FUN_08006e90(void)

{
  uint *puVar1;
  
  puVar1 = DAT_08006eec;
  *DAT_08006eec = *DAT_08006eec | 1;
  puVar1[1] = puVar1[1] & DAT_08006ef0;
  *puVar1 = *puVar1 & DAT_08006ef4;
  *puVar1 = *puVar1 & 0xfffbffff;
  puVar1[1] = puVar1[1] & 0xffc0ffff;
  puVar1[1] = puVar1[1] & 0xfffffff0;
  puVar1[0xc] = puVar1[0xc] & 0xfffffeac;
  puVar1[0xd] = puVar1[0xd] & 0xfffffffe;
  puVar1[2] = 0;
  FUN_08006e14();
  *(undefined4 *)(DAT_08006efc + 8) = DAT_08006ef8;
  return;
}

