// 0x00408060 FUN_00408060 size=19

void __fastcall FUN_00408060(int param_1)

{
  char *_Str;
  undefined4 *puVar1;
  int iVar2;
  size_t sVar3;
  undefined4 *puVar4;
  QArrayData *in_stack_ffffffac;
  uint uVar5;
  uint uVar6;
  QArrayData *apQStack_20 [4];
  
  if (DAT_00459044 + 1 < 0x14) {
    DAT_00459044 = DAT_00459044 + 1;
    return;
  }
  DAT_00459044 = 0;
  if (*(int *)(param_1 + 0x14) != 0) {
    puVar1 = hid_enumerate(0x483);
    if (puVar1 != (undefined4 *)0x0) {
      iVar2 = strcmp((char *)*puVar1,DAT_00459040);
      puVar4 = puVar1;
      do {
        if (iVar2 == 0) {
          hid_free_enumeration(puVar1);
          DAT_00459049 = 1;
          if (DAT_00459048 == '\0') {
            FUN_004208f0((void *)param_1,1);
            FUN_00420980((void *)param_1,4);
            DAT_00459048 = DAT_00459049;
            return;
          }
          DAT_00459049 = 1;
          return;
        }
        puVar4 = (undefined4 *)puVar4[8];
      } while (puVar4 != (undefined4 *)0x0);
    }
    hid_free_enumeration(puVar1);
    hid_close(*(undefined4 **)(param_1 + 0x14));
    apQStack_20[0] = (QArrayData *)QString::fromAscii_helper("*",1);
    FUN_004209c0((void *)param_1,apQStack_20);
    if (*(int *)apQStack_20[0] != 0) {
      if (*(int *)apQStack_20[0] == -1) goto LAB_00408293;
      LOCK();
      *(int *)apQStack_20[0] = *(int *)apQStack_20[0] + -1;
      UNLOCK();
      if (*(int *)apQStack_20[0] != 0) goto LAB_00408293;
    }
    QArrayData::deallocate(apQStack_20[0],2,4);
LAB_00408293:
    *(undefined1 *)(param_1 + 0x9a) = 0;
    *(undefined4 *)(param_1 + 0x14) = 0;
    return;
  }
  DAT_00459049 = '\0';
  if (DAT_00459048 != '\0') {
    FUN_004208f0((void *)param_1,0);
    FUN_00420980((void *)param_1,5);
    DAT_00459048 = DAT_00459049;
  }
  puVar1 = hid_enumerate(0x483);
  if (puVar1 == (undefined4 *)0x0) goto LAB_00408200;
  _Str = (char *)*puVar1;
  sVar3 = strlen(_Str);
  uVar5 = 0x40817a;
  DAT_00459040 = operator_new__(sVar3 + 1);
  strcpy(DAT_00459040,_Str);
  puVar4 = hid_open_path();
  *(undefined4 **)(param_1 + 0x14) = puVar4;
  if (puVar4 == (undefined4 *)0x0) goto LAB_00408200;
  apQStack_20[0] = (QArrayData *)QString::fromAscii_helper("*",1);
  uVar6 = 0x4081c4;
  FUN_004209c0((void *)param_1,apQStack_20);
  if (*(int *)apQStack_20[0] == 0) {
LAB_004081e0:
    QArrayData::deallocate(in_stack_ffffffac,uVar5,uVar6);
  }
  else if (*(int *)apQStack_20[0] != -1) {
    LOCK();
    *(int *)apQStack_20[0] = *(int *)apQStack_20[0] + -1;
    UNLOCK();
    if (*(int *)apQStack_20[0] == 0) goto LAB_004081e0;
  }
  FUN_00407ff0(param_1);
LAB_00408200:
  hid_free_enumeration(puVar1);
  return;
}

