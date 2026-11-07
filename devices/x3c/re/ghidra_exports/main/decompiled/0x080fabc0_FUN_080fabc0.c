// 0x080fabc0 FUN_080fabc0 size=436

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_080fabc0(void)

{
  char cVar1;
  char cVar2;
  uint uVar3;
  int iVar4;
  char cVar5;
  byte bVar6;
  uint local_20;
  undefined4 local_1c;
  undefined4 local_18;
  
  local_1c = DAT_080fad74;
  local_18 = DAT_080fad78;
  cVar5 = -0x49;
  if (*DAT_080fad7c == '\x1a') {
    local_1c._2_2_ = (undefined2)((uint)DAT_080fad74 >> 0x10);
    local_1c._0_2_ = CONCAT11(1,(char)DAT_080fad74);
  }
  FUN_080f86ee();
  FUN_080f8fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff | 0x40000000;
  FUN_080fa960(10);
  FUN_080f8f0c(&DAT_48000000,0x8000);
  FUN_080fa960(0x32);
  iVar4 = DAT_080fad90;
  cVar1 = *DAT_080fad80;
  if (cVar1 == '\t') {
    if ((*DAT_080fad88 == '\x01') && ((*DAT_080fad84 & 3) != 2)) {
      uVar3 = 0;
      do {
        if (*(char *)(DAT_080fad8c + uVar3) == '\0') {
          bVar6 = (*(char *)(DAT_080fad90 + uVar3) - *(char *)(DAT_080fad94 + uVar3)) - 0xb;
          if (bVar6 < 0xd0) {
            if (bVar6 < 0x60) {
              bVar6 = 0x60;
            }
          }
          else {
            bVar6 = 0xcf;
          }
          *(byte *)((int)&local_1c + uVar3 + 3) = bVar6;
        }
        uVar3 = uVar3 + 1 & 0xff;
      } while (uVar3 < 5);
    }
  }
  else if ((*DAT_080fad88 == '\0') && ((*DAT_080fad84 & 3) != 2)) {
    uVar3 = 0;
    do {
      if (*(char *)(DAT_080fad8c + uVar3) == '\0') {
        if (cVar1 == '\'') goto LAB_080facb4;
        if (cVar1 == '\x1b') {
          cVar2 = *(char *)(iVar4 + uVar3) + -8;
LAB_080faca2:
          *(char *)(iVar4 + uVar3) = cVar2;
        }
        else {
          if (cVar1 == '\x11') {
LAB_080facca:
            cVar2 = *(char *)(iVar4 + uVar3) + -5;
            goto LAB_080faca2;
          }
          if (cVar1 == '9') {
            cVar2 = *(char *)(iVar4 + uVar3) + '\x02';
            goto LAB_080faca2;
          }
          if (cVar1 == '6') {
            if (*DAT_080fad7c == '\x1a') {
              cVar2 = *(char *)(iVar4 + uVar3) + -0xd;
              goto LAB_080faca2;
            }
          }
          else if (cVar1 == '!') goto LAB_080facca;
        }
LAB_080facb4:
        cVar2 = *(char *)(iVar4 + uVar3);
        bVar6 = *(byte *)(DAT_080fad98 + uVar3);
        if (bVar6 < 0xa1) {
          if (bVar6 < 0xa0) {
            cVar2 = (-0x60 - bVar6) + cVar2;
          }
        }
        else {
          cVar2 = (cVar2 - bVar6) + -0x60;
        }
        if (*DAT_080fad9c == '\0') {
          cVar2 = cVar2 - *(char *)(DAT_080fad94 + uVar3);
        }
        *(char *)((int)&local_1c + uVar3 + 3) = cVar2;
      }
      uVar3 = uVar3 + 1 & 0xff;
    } while (uVar3 < 5);
  }
  uVar3 = 1;
  do {
    cVar5 = *(char *)((int)&local_1c + uVar3) + cVar5;
    uVar3 = uVar3 + 1 & 0xff;
  } while (uVar3 < 8);
  uVar3 = 0;
  do {
    FUN_080fa878(*(undefined1 *)((int)&local_1c + uVar3));
    FUN_080fa960(0x14);
    uVar3 = uVar3 + 1 & 0xff;
  } while (uVar3 < 8);
  FUN_080fa878(cVar5);
  FUN_080fa960(0x14);
  iVar4 = FUN_080fa878(0xff);
  local_20 = (uint)(iVar4 != 0xb6);
  FUN_080f8fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
  _DAT_4800000c = _DAT_4800000c | 0x40000000;
  FUN_080f86ac();
  FUN_080f8fca(DAT_080fada0,0x20);
  return (ulonglong)CONCAT14(iVar4 != 0xb6,local_20);
}

