// 0x0801fedc FUN_0801fedc size=228

void FUN_0801fedc(undefined4 param_1,int param_2)

{
  char *pcVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined2 *puVar4;
  undefined4 *puVar5;
  byte bVar6;
  int iVar7;
  
  if (*DAT_0801ffc0 == '\0') {
    iVar7 = FUN_0802b744(*DAT_0801ffc4);
    iVar2 = DAT_0801ffcc;
    pcVar1 = DAT_0801ffc8;
    if (iVar7 == 0) {
      *(int *)(DAT_0801ffcc + 0x20) = -(int)*DAT_0801ffc8;
    }
    else {
      *(int *)(DAT_0801ffcc + 0x20) = (int)*DAT_0801ffc8;
    }
    bVar6 = *(char *)(iVar2 + 8) + 1;
    *(byte *)(iVar2 + 8) = bVar6;
    if (bVar6 < 3) {
      *(char *)(iVar2 + 9) = *(char *)(iVar2 + 9) + *pcVar1;
    }
    else {
      *(undefined1 *)(iVar2 + 8) = 0;
      if ((int)*(char *)(iVar2 + 9) + 2U < 5) {
        *(undefined1 *)(iVar2 + 1) = 0;
      }
      else {
        *(undefined1 *)(iVar2 + 1) = 1;
      }
      *(undefined1 *)(iVar2 + 9) = 0;
    }
    puVar4 = DAT_0801ffd4;
    puVar3 = DAT_0801ffd0;
    if (*(int *)(iVar2 + 0x20) != 0) {
      *DAT_0801ffd0 = 0;
      puVar5 = DAT_0801ffd8;
      *DAT_0801ffdc = *DAT_0801ffd8;
      *DAT_0801ffe0 = *puVar5;
      *DAT_0801ffe8 = (uint)*DAT_0801ffe4;
      *puVar4 = 0;
      *DAT_0801ffec = 100;
      *DAT_0801fff0 = 100;
    }
    if (*DAT_0801fff4 != '\0') {
      *DAT_0801fff4 = *DAT_0801fff4 + -1;
    }
    iVar7 = FUN_08022eb4(DAT_0801fff8,0x8000);
    if ((iVar7 == 0) && (*DAT_0801ffc0 == '\0')) {
      *puVar3 = 0;
      *puVar4 = 0;
      *DAT_0801ffec = 100;
      *DAT_0801fff0 = 100;
      *(undefined1 *)(iVar2 + 10) = 1;
    }
    else {
      *(undefined1 *)(iVar2 + 7) = 0;
      *(undefined1 *)(iVar2 + 10) = 0;
    }
    bVar6 = *(byte *)(iVar2 + 10);
    *(uint *)(iVar2 + 0x14) = (uint)bVar6;
    *pcVar1 = '\0';
    *(undefined2 *)(param_2 + 0x10) = *(undefined2 *)(iVar2 + 0x20);
    *(byte *)(param_2 + 0x12) = bVar6;
    *(undefined4 *)(iVar2 + 0x20) = 0;
  }
  return;
}

