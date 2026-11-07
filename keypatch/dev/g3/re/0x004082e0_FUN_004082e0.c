// 0x004082e0 FUN_004082e0 size=2103

uint __fastcall FUN_004082e0(void *param_1)

{
  uint uVar1;
  QArrayData *pQVar2;
  char cVar3;
  byte bVar4;
  QArrayData QVar5;
  int iVar6;
  undefined4 uVar7;
  uint uVar8;
  DWORD DVar9;
  void *extraout_ECX;
  void *this;
  void *this_00;
  void *extraout_ECX_00;
  void *extraout_ECX_01;
  void *pvVar10;
  void *extraout_ECX_02;
  void *extraout_ECX_03;
  int iVar11;
  undefined4 *puVar12;
  int local_80;
  uint local_68;
  QArrayData *local_64;
  QArrayData *local_60;
  uint local_5c;
  uint local_58;
  
  if ((DAT_00459060 == '\0') && (iVar6 = __cxa_guard_acquire(&DAT_00459060), iVar6 != 0)) {
    QFile::QFile((QFile *)&DAT_00459068);
    __cxa_guard_release(&DAT_00459060);
    FUN_004014c0((_onexit_t)&LAB_00408c10);
  }
  if ((DAT_00459050 == '\0') && (iVar6 = __cxa_guard_acquire(&DAT_00459050), iVar6 != 0)) {
    DAT_00459058 = shared_null_exref;
    __cxa_guard_release(&DAT_00459050);
    FUN_004014c0((_onexit_t)&LAB_004070a0);
  }
  local_68 = 0;
  switch(*(undefined4 *)((int)param_1 + 0xc4)) {
  case 0:
    FUN_00420940(param_1,0);
    if (*(int *)((int)param_1 + 0x14) == 0) {
      FUN_00420980(param_1,6);
      return 0;
    }
    iVar6 = *(int *)((int)param_1 + 8);
    iVar6 = QString::compare_helper
                      (iVar6 + *(int *)(iVar6 + 0xc),*(undefined4 *)(iVar6 + 4),0,0xffffffff,1);
    if (iVar6 == 0) {
      FUN_00420980(param_1,7);
      return 0;
    }
    goto LAB_004084bf;
  case 1:
    QFile::setFileName((QString *)((int)param_1 + 8));
    uVar8 = QFile::open(1);
    if ((char)uVar8 == '\0') {
      FUN_00420980(param_1,0);
      return uVar8 & 0xff;
    }
LAB_004084fc:
    *(int *)((int)param_1 + 0xc4) = *(int *)((int)param_1 + 0xc4) + 1;
    return uVar8;
  case 2:
    uVar7 = FUN_00407990((int *)&DAT_00459068,(QString *)&DAT_00459058,&local_68);
    if ((*(char *)((int)param_1 + 0x9a) != '\0') &&
       (((char)uVar7 == '\0' || (iVar6 = QString::indexOf((int)param_1 + 0xbc,0,1), iVar6 == -1))))
    {
      FUN_00420980(param_1,10);
      goto LAB_004087a7;
    }
    iVar6 = QFile::size();
    DAT_00434008 = iVar6 - local_68;
    puVar12 = (undefined4 *)((int)param_1 + 0x1cU & 0xfffffffc);
    DAT_0045905c = DAT_00434008 + 0x3ff >> 10;
    *(undefined4 *)((int)param_1 + 0x18) = 0;
    *(undefined4 *)((int)param_1 + 0x55) = 0;
    uVar8 = (uint)((int)param_1 + 0x18 + (0x41 - (int)puVar12)) >> 2;
    for (; uVar8 != 0; uVar8 = uVar8 - 1) {
      *puVar12 = 0;
      puVar12 = puVar12 + 1;
    }
    DVar9 = hid_write((void *)0x0,*(undefined4 **)((int)param_1 + 0x14),
                      (void *)((int)param_1 + 0x18),0x41);
    if (DVar9 == 0xffffffff) goto LAB_00408796;
    DAT_00459080 = 0;
LAB_004084bf:
    *(int *)((int)param_1 + 0xc4) = *(int *)((int)param_1 + 0xc4) + 1;
    break;
  case 3:
    DAT_00459080 = DAT_00459080 + 1;
    if (9 < DAT_00459080) {
      *(undefined4 *)((int)param_1 + 0xc4) = 4;
      return 1;
    }
    break;
  case 4:
    FUN_00407610(0,(undefined1 *)0x0,0,(undefined2 *)((int)param_1 + 0x18));
    DVar9 = hid_write(this,*(undefined4 **)((int)param_1 + 0x14),(undefined2 *)((int)param_1 + 0x18)
                      ,0x41);
    if (DVar9 != 0xffffffff) {
      *(undefined1 *)((int)param_1 + 0xc0) = 0;
      DAT_0045907c = 0;
      *(int *)((int)param_1 + 0xc4) = *(int *)((int)param_1 + 0xc4) + 1;
      return 1;
    }
    goto LAB_00408796;
  case 5:
    if (*(char *)((int)param_1 + 0xc0) != '\0') {
      *(undefined4 *)((int)param_1 + 0xc4) = 6;
    }
    goto LAB_0040877e;
  case 6:
    FUN_00407610(2,(undefined1 *)0x0,0,(undefined2 *)((int)param_1 + 0x18));
    DVar9 = hid_write(this_00,*(undefined4 **)((int)param_1 + 0x14),
                      (undefined2 *)((int)param_1 + 0x18),0x41);
    if (DVar9 != 0xffffffff) {
      *(undefined1 *)((int)param_1 + 0xc1) = 0;
      DAT_0045907c = 0;
      *(int *)((int)param_1 + 0xc4) = *(int *)((int)param_1 + 0xc4) + 1;
      return 1;
    }
    goto LAB_00408796;
  case 7:
    if (*(char *)((int)param_1 + 0xc1) != '\0') {
      *(undefined4 *)((int)param_1 + 0xc4) = 8;
      DAT_00459078 = 0;
      DAT_00459070 = 0;
      DAT_00459074 = 0;
    }
    goto LAB_0040877e;
  case 8:
    if (*(char *)((int)param_1 + 0x9a) == '\0') {
      local_5c = CONCAT31(local_5c._1_3_,(char)DAT_00459078);
    }
    else {
      local_5c = DAT_00459078 >> 0x18 | (DAT_00459078 & 0xff0000) >> 8 |
                 (DAT_00459078 & 0xff00) << 8 | DAT_00459078 << 0x18;
    }
    local_64 = (QArrayData *)shared_null_exref;
    QIODevice::read(0x40000459068);
    pQVar2 = local_64;
    local_64 = local_60;
    local_60 = pQVar2;
    if (*(int *)pQVar2 == 0) {
LAB_00408a0b:
      QArrayData::deallocate(pQVar2,1,4);
    }
    else if (*(int *)pQVar2 != -1) {
      LOCK();
      *(int *)pQVar2 = *(int *)pQVar2 + -1;
      UNLOCK();
      if (*(int *)pQVar2 == 0) goto LAB_00408a0b;
    }
    iVar6 = 0;
    uVar8 = 0;
    do {
      if (DAT_00459070 < DAT_00434008) {
        if ((1 < *(uint *)local_64) || (*(uint *)(local_64 + 0xc) != 0x10)) {
          QByteArray::reallocData
                    (*(uint *)(local_64 + 4) + 1,(uint)(int)(char)local_64[0xb] >> 0x1f);
        }
        if (iVar6 < (int)*(uint *)(local_64 + 4)) {
          uVar8 = uVar8 + (byte)local_64[*(uint *)(local_64 + 0xc) + iVar6];
        }
        DAT_00459070 = DAT_00459070 + 1;
      }
      else {
        uVar8 = uVar8 + 0xff;
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 != 0x400);
    uVar1 = uVar8 >> 0x18 | (uVar8 & 0xff0000) >> 8 | (uVar8 & 0xff00) << 8;
    if (*(char *)((int)param_1 + 0x9a) == '\0') {
      local_5c._1_3_ = (undefined3)uVar1;
      local_58._0_1_ = (undefined1)uVar8;
      bVar4 = 4;
      if (0xff < DAT_0045905c) {
        bVar4 = (byte)(DAT_00459078 >> 4) & 0xf0 | 4;
      }
      FUN_00407610(bVar4,(undefined1 *)&local_5c,5,(undefined2 *)((int)param_1 + 0x18));
      pvVar10 = extraout_ECX_00;
    }
    else {
      local_58 = uVar1 | uVar8 << 0x18;
      FUN_00407610(8,(undefined1 *)&local_5c,8,(undefined2 *)((int)param_1 + 0x18));
      pvVar10 = extraout_ECX_03;
    }
    DVar9 = hid_write(pvVar10,*(undefined4 **)((int)param_1 + 0x14),(void *)((int)param_1 + 0x18),
                      0x41);
    local_80 = 0;
    pvVar10 = extraout_ECX_01;
    if (DVar9 != 0xffffffff) {
      do {
        iVar11 = 1;
        iVar6 = local_80 + -1;
        do {
          if (DAT_00459074 < DAT_00434008) {
            if ((*(uint *)local_64 < 2) && (*(uint *)(local_64 + 0xc) == 0x10)) {
              if (iVar6 + iVar11 < (int)*(uint *)(local_64 + 4)) goto LAB_0040898e;
LAB_004089b3:
              QVar5 = (QArrayData)0x0;
            }
            else {
              QByteArray::reallocData
                        (*(uint *)(local_64 + 4) + 1,(uint)(int)(char)local_64[0xb] >> 0x1f);
              pvVar10 = extraout_ECX_02;
              if ((int)*(uint *)(local_64 + 4) <= iVar6 + iVar11) goto LAB_004089b3;
LAB_0040898e:
              QVar5 = local_64[*(uint *)(local_64 + 0xc) + iVar6 + iVar11];
            }
            *(QArrayData *)((int)param_1 + iVar11 + 0x18) = QVar5;
          }
          else {
            *(undefined1 *)((int)param_1 + iVar11 + 0x18) = 0xff;
          }
          DAT_00459074 = DAT_00459074 + 1;
          iVar11 = iVar11 + 1;
        } while (iVar11 != 0x41);
        DVar9 = hid_write(pvVar10,*(undefined4 **)((int)param_1 + 0x14),
                          (void *)((int)param_1 + 0x18),0x41);
        if (DVar9 == 0xffffffff) break;
        local_80 = local_80 + 0x40;
        pvVar10 = extraout_ECX;
        if (local_80 == 0x400) {
          DAT_00459078 = DAT_00459078 + 1;
          DAT_0045907c = 0;
          *(undefined1 *)((int)param_1 + 0xc2) = 0;
          *(int *)((int)param_1 + 0xc4) = *(int *)((int)param_1 + 0xc4) + 1;
          if (*(int *)local_64 != 0) {
            if (*(int *)local_64 == -1) {
              return 1;
            }
            LOCK();
            *(int *)local_64 = *(int *)local_64 + -1;
            UNLOCK();
            if (*(int *)local_64 != 0) {
              return 1;
            }
          }
          QArrayData::deallocate(local_64,1,4);
          return 1;
        }
      } while( true );
    }
    FUN_00420980(param_1,8);
    if (*(int *)local_64 != 0) {
      if (*(int *)local_64 == -1) goto LAB_004087a7;
      LOCK();
      *(int *)local_64 = *(int *)local_64 + -1;
      UNLOCK();
      if (*(int *)local_64 != 0) goto LAB_004087a7;
    }
    QArrayData::deallocate(local_64,1,4);
    goto LAB_004087a7;
  case 9:
    bVar4 = *(byte *)((int)param_1 + 0xc2);
    if (bVar4 != 0) {
      FUN_00420940(param_1,(char)(int)ROUND(((float10)DAT_00459078 / (float10)DAT_0045905c) *
                                            (float10)100.0));
      uVar8 = (uint)bVar4;
      if (DAT_00459078 < DAT_0045905c) {
        *(int *)((int)param_1 + 0xc4) = *(int *)((int)param_1 + 0xc4) + -1;
        return uVar8;
      }
      goto LAB_004084fc;
    }
LAB_0040877e:
    DAT_0045907c = DAT_0045907c + 1;
    if (5000 < DAT_0045907c) {
LAB_00408796:
      FUN_00420980(param_1,8);
LAB_004087a7:
      uVar8 = QIODevice::isOpen();
      if ((char)uVar8 == '\0') {
        return uVar8;
      }
      QFileDevice::close();
      return 0;
    }
    break;
  case 10:
    cVar3 = QIODevice::isOpen();
    if (cVar3 != '\0') {
      QFileDevice::close();
    }
    *(undefined1 *)((int)param_1 + 0xc) = 0;
    FUN_00420980(param_1,9);
  }
  return 1;
}

