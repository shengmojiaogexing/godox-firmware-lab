// 0x0800798c FUN_0800798c size=146

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0800798c(undefined1 param_1)

{
  bool bVar1;
  uint *puVar2;
  undefined1 *puVar3;
  int iVar4;
  int iVar5;
  byte bVar6;
  
  FUN_0800337c();
  FUN_08002f0c(&DAT_48000000,8);
  FUN_080072b4(0xf);
  puVar2 = DAT_08007a20;
  _DAT_48000000 = _DAT_48000000 & 0xffffffcf;
  _DAT_4800000c = _DAT_4800000c & 0xffffffcf | 0x20;
  *DAT_08007a20 = *DAT_08007a20 & 0xfffffffc;
  *puVar2 = *puVar2 | 1;
  puVar2[3] = puVar2[3] & 0xfffffffe;
  puVar2[3] = puVar2[3] | 2;
  iVar4 = FUN_08002fb6(&DAT_48000000,4);
  if (iVar4 == 0) {
    puVar2[5] = puVar2[5] ^ 1;
  }
  puVar3 = DAT_08007a28;
  iVar4 = DAT_08007a24;
  *(undefined1 *)(DAT_08007a24 + 2) = param_1;
  *puVar3 = 1;
  iVar5 = FUN_08002fc0(puVar2,1);
  bVar6 = *(byte *)(iVar4 + 2);
  if (iVar5 != 0) {
    bVar6 = ~bVar6;
  }
  FUN_080042cc(DAT_08007a2c,bVar6);
  *DAT_08007a30 = 0;
  *DAT_08007a34 = 0;
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    enableIRQinterrupts(0);
  }
  return;
}

