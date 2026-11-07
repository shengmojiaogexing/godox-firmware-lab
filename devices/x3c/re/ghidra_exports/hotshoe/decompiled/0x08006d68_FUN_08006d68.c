// 0x08006d68 FUN_08006d68 size=134

void FUN_08006d68(void)

{
  char cVar1;
  undefined4 uVar2;
  char *pcVar3;
  char *pcVar4;
  undefined1 uVar5;
  int iVar6;
  
  if (*DAT_08006df0 == '\0') {
    FUN_08002f0c(&DAT_48000000,2);
    uVar2 = DAT_08006df4;
    uVar5 = FUN_08004040(DAT_08006df4);
    *(undefined1 *)(DAT_08006df8 + 1) = uVar5;
    iVar6 = FUN_08004000(uVar2,0x40);
    if (iVar6 == 1) {
      FUN_08003f0c(uVar2,0x40);
    }
    FUN_08002fca(&DAT_48000000,8);
    pcVar3 = DAT_08006e00;
    *DAT_08006dfc = 1;
    pcVar4 = DAT_08006e04;
    cVar1 = *pcVar3;
    if ((cVar1 == -0x5b) || (cVar1 == -0x58)) {
      FUN_08004508();
    }
    else if ((((*DAT_08006e04 == -0x5b) || (cVar1 == -0x3c)) || (cVar1 == -0x4c)) &&
            (*DAT_08006e08 != '\'')) {
      FUN_080044bc();
    }
    FUN_08006618();
    if (*DAT_08006e0c == '\0') {
      *pcVar4 = *pcVar3;
    }
  }
  return;
}

