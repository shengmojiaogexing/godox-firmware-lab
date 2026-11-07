// 0x0800fd90 FUN_0800fd90 size=210

void FUN_0800fd90(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 *puVar1;
  int iVar2;
  char cVar3;
  undefined4 uVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  
  iVar2 = DAT_0800fe6c;
  iVar5 = DAT_0800fe68;
  puVar1 = DAT_0800fe64;
  uVar4 = *(undefined4 *)(param_1 + 4);
  cVar3 = *(char *)(param_1 + 8);
  if (cVar3 == '\a') {
    FUN_0802e34c(uVar4,8,param_3,param_4,param_4);
    FUN_0802b8b2(*puVar1,0);
    uVar4 = FUN_08027f90();
    FUN_0803157c(*(undefined4 *)(iVar5 + 4),uVar4,0);
    *(undefined1 *)(iVar2 + 1) = 0;
  }
  else if (cVar3 == '\r') {
    iVar5 = FUN_0802fcb6(uVar4,8);
    if (iVar5 != 0) {
      pcVar6 = (char *)FUN_0802ad6c(param_1);
      if (*pcVar6 == '\x14') {
        cVar3 = *DAT_0800fe70;
        if (cVar3 != *DAT_0800fe74) {
          cVar3 = cVar3 + '\n';
        }
        *DAT_0800fe70 = cVar3;
      }
      else if (*pcVar6 == '\x13') {
        if (*DAT_0800fe70 == '\x14') {
          cVar3 = '\x14';
        }
        else {
          cVar3 = *DAT_0800fe70 + -10;
        }
        *DAT_0800fe70 = cVar3;
      }
    }
  }
  else if (cVar3 == '\x0e') {
    if (*(char *)(DAT_0800fe6c + 1) == '\0') {
      iVar7 = FUN_0802fcb6(uVar4,8);
      if (iVar7 != 0) {
        uVar4 = FUN_08027ce0(0xfd,0x9f,10);
        FUN_0803157c(*(undefined4 *)(iVar5 + 4),uVar4,0);
        *(undefined1 *)(iVar2 + 1) = 1;
      }
    }
    else {
      FUN_0802e34c(uVar4,8,param_3,param_4,param_4);
      FUN_0802b8b2(*puVar1,0);
      uVar4 = FUN_08027f90();
      FUN_0803157c(*(undefined4 *)(iVar5 + 4),uVar4,0);
      *(undefined1 *)(iVar2 + 1) = 0;
    }
  }
  return;
}

