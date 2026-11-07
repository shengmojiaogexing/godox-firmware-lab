// 0x08004878 FUN_08004878 size=112

uint FUN_08004878(uint param_1)

{
  bool bVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  char cVar5;
  
  uVar2 = DAT_080048e8;
  uVar4 = 0;
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    enableIRQinterrupts(1);
  }
  cVar5 = '\b';
  do {
    FUN_08002f0c(uVar2,8);
    if ((int)(param_1 << 0x18) < 0) {
      FUN_08002fca(uVar2,0x20);
    }
    else {
      FUN_08002f0c(uVar2,0x20);
    }
    param_1 = (param_1 & 0x7f) << 1;
    FUN_08002fca(uVar2,8);
    uVar4 = (uVar4 & 0x7f) << 1;
    iVar3 = FUN_08002fb6(uVar2,0x10);
    if (iVar3 != 0) {
      uVar4 = uVar4 | 1;
    }
    cVar5 = cVar5 + -1;
  } while (cVar5 != '\0');
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    enableIRQinterrupts(0);
  }
  return uVar4;
}

