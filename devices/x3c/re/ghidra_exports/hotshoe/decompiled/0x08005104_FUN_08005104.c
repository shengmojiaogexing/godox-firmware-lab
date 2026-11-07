// 0x08005104 FUN_08005104 size=214

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_08005104(void)

{
  uint uVar1;
  int iVar2;
  char cVar3;
  undefined4 in_r3;
  undefined4 local_20;
  undefined2 local_1c;
  undefined2 uStack_1a;
  undefined4 uStack_18;
  
  local_20 = CONCAT13(0x2e,CONCAT12(0x31,CONCAT11(0x30,(char)DAT_080051dc)));
  cVar3 = -0x35;
  _local_1c = CONCAT22((short)((uint)DAT_080051e0 >> 0x10),0x3230);
  uVar1 = 1;
  do {
    cVar3 = *(char *)((int)&local_20 + uVar1) + cVar3;
    uVar1 = uVar1 + 1 & 0xff;
  } while (uVar1 < 6);
  uStack_18 = in_r3;
  FUN_080026ee();
  FUN_08002fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff | 0x40000000;
  FUN_08004960(10);
  FUN_08002f0c(&DAT_48000000,0x8000);
  FUN_08004982(0x32);
  uVar1 = 0;
  do {
    FUN_080048ec(*(undefined1 *)((int)&local_20 + uVar1));
    FUN_08004982(0x14);
    uVar1 = uVar1 + 1 & 0xff;
  } while (uVar1 < 6);
  FUN_080048ec(cVar3);
  FUN_08004982(0x14);
  iVar2 = FUN_080048ec(0xff);
  FUN_08002fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
  _DAT_4800000c = _DAT_4800000c | 0x40000000;
  FUN_080026ac();
  FUN_08002fca(DAT_080051e4,0x20);
  return CONCAT44(local_20,(uint)(iVar2 != 0xca));
}

