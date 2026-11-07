// 0x08010b18 FUN_08010b18 size=212

void FUN_08010b18(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  iVar2 = *DAT_08010b20;
  piVar3 = DAT_08010b20 + 1;
  iVar1 = FUN_0803a6d0(iVar2);
  if ((*(uint *)(iVar2 + 0x14) & 1) == 0) {
    if (iVar1 << 0x1e < 0) {
      FUN_0803a6cc(iVar2,2);
    }
    if (iVar1 << 0xd < 0) {
      FUN_0803b300(piVar3);
    }
    if (iVar1 << 0xc < 0) {
      FUN_0803b4ee(piVar3);
    }
    if (iVar1 << 0x13 < 0) {
      FUN_0803b572(piVar3);
      FUN_0803a6cc(iVar2,0x1000);
    }
    if (iVar1 << 0x1c < 0) {
      FUN_0803b83a(piVar3);
      FUN_0803a6cc(iVar2,8);
    }
    if (iVar1 << 0x12 < 0) {
      FUN_0803ae9e(piVar3);
      FUN_0803a6cc(iVar2,0x2000);
    }
    if (iVar1 << 0x1b < 0) {
      FUN_0803b640(piVar3);
    }
    if (iVar1 << 0xb < 0) {
      FUN_0803b2e4(piVar3);
      FUN_0803a6cc(iVar2,0x100000);
    }
    if (iVar1 << 10 < 0) {
      FUN_0803b2f2(piVar3);
      FUN_0803a6cc(iVar2,0x200000);
    }
    if (iVar1 < 0) {
      FUN_0803b8ae(piVar3);
      FUN_0803a6cc(iVar2,0x80000000);
    }
    if (iVar1 << 0x14 < 0) {
      FUN_0803b846(piVar3);
      FUN_0803a6cc(iVar2,0x800);
      return;
    }
  }
  return;
}

