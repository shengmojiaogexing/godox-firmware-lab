// 0x08021ff4 FUN_08021ff4 size=22

undefined2 * FUN_08021ff4(void)

{
  undefined2 *puVar1;
  undefined2 uVar2;
  
  uVar2 = FUN_0803b87c(s_Godox_08022010,DAT_0802200c);
  puVar1 = DAT_08022018;
  *DAT_08022018 = uVar2;
  *(undefined4 *)(puVar1 + 2) = DAT_0802200c;
  return puVar1;
}

