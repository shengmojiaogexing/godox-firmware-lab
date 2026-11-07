// 0x00407990 FUN_00407990 size=1549

undefined4 __cdecl FUN_00407990(int *param_1,QString *param_2,uint *param_3)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  QArrayData QVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  QArrayData *pQVar8;
  undefined4 uVar9;
  uint uVar10;
  QArrayData *_Str;
  bool bVar11;
  longlong lVar12;
  QByteArray *pQVar13;
  QString *local_5c;
  uint local_54;
  int local_50;
  QArrayData *local_44;
  QArrayData *local_40;
  QArrayData *local_3c;
  QArrayData *local_38;
  QCryptographicHash local_34 [4];
  QArrayData *local_30;
  QArrayData *local_2c;
  code *local_28;
  undefined4 local_24;
  undefined4 local_20;
  
  pcVar1 = shared_null_exref;
  local_24 = 0x996655aa;
  local_20 = 0x33228877;
  local_44 = (QArrayData *)shared_null_exref;
  local_40 = (QArrayData *)shared_null_exref;
  lVar12 = (**(code **)(*param_1 + 0x40))();
  iVar5 = (int)((ulonglong)lVar12 >> 0x20);
  uVar7 = (uint)lVar12;
  if ((SBORROW4(0,iVar5) != SBORROW4(-iVar5,(uint)(9 < uVar7))) !=
      (int)(-iVar5 - (uint)(9 < uVar7)) < 0) {
    lVar12 = lVar12 + -10;
    (**(code **)(*param_1 + 0x44))(lVar12);
    QIODevice::read(CONCAT44(10,param_1));
    local_40 = local_3c;
    if (*(int *)pcVar1 == 0) {
LAB_00407d74:
      QArrayData::deallocate((QArrayData *)pcVar1,1,4);
    }
    else if (*(int *)pcVar1 != -1) {
      LOCK();
      *(int *)pcVar1 = *(int *)pcVar1 + -1;
      UNLOCK();
      if (*(int *)pcVar1 == 0) goto LAB_00407d74;
    }
    QVar4 = (QArrayData)0xaa;
    iVar3 = 0;
    while (QVar4 == local_3c[iVar3 + *(uint *)(local_3c + 0xc) + 2]) {
      if (iVar3 + 1 == 8) {
        if ((*(uint *)local_3c < 2) && (*(uint *)(local_3c + 0xc) == 0x10)) {
          uVar6 = 0x10;
        }
        else {
          QByteArray::reallocData
                    (*(uint *)(local_3c + 4) + 1,(uint)(int)(char)local_3c[0xb] >> 0x1f);
          uVar6 = *(uint *)(local_3c + 0xc);
        }
        QVar4 = local_3c[uVar6];
        if ((*(uint *)local_3c < 2) && (*(uint *)(local_3c + 0xc) == 0x10)) {
          uVar6 = 0x10;
        }
        else {
          QByteArray::reallocData
                    (*(uint *)(local_3c + 4) + 1,(uint)(int)(char)local_3c[0xb] >> 0x1f);
          uVar6 = *(uint *)(local_3c + 0xc);
        }
        uVar6 = (uint)(ushort)(short)(char)QVar4 | (uint)(byte)local_3c[uVar6 + 1] << 8;
        if (iVar5 < (int)(uint)(uVar7 < uVar6 + 0x1a)) {
          (**(code **)(*param_1 + 0x44))(0,0);
          goto LAB_00407d2f;
        }
        uVar10 = (uint)lVar12 - uVar6;
        local_54 = uVar10 - 0x10;
        local_50 = ((int)((ulonglong)lVar12 >> 0x20) - (uint)((uint)lVar12 < uVar6)) + -1 +
                   (uint)(0xf < uVar10);
        (**(code **)(*param_1 + 0x44))(local_54,local_50);
        QIODevice::read(CONCAT44(0x10,param_1));
        local_44 = local_38;
        if (*(int *)pcVar1 == 0) {
LAB_00407d95:
          QArrayData::deallocate((QArrayData *)pcVar1,1,4);
        }
        else if (*(int *)pcVar1 != -1) {
          LOCK();
          *(int *)pcVar1 = *(int *)pcVar1 + -1;
          UNLOCK();
          if (*(int *)pcVar1 == 0) goto LAB_00407d95;
        }
        QCryptographicHash::QCryptographicHash(local_34,1);
        (**(code **)(*param_1 + 0x44))(0,0);
        if ((SBORROW4(0,local_50) != SBORROW4(-local_50,(uint)(local_54 != 0))) !=
            (int)(-local_50 - (uint)(local_54 != 0)) < 0) goto LAB_00407bec;
        goto LAB_00407c67;
      }
      QVar4 = *(QArrayData *)((int)&local_24 + iVar3 + 1);
      iVar3 = iVar3 + 1;
    }
    (**(code **)(*param_1 + 0x44))(0,0);
  }
LAB_00407d2f:
  uVar9 = 0;
  uVar7 = *(uint *)local_40;
  local_3c = local_40;
  if (uVar7 == 0) goto LAB_00407cd9;
LAB_00407d3a:
  if (uVar7 == 0xffffffff) goto LAB_00407cf2;
  LOCK();
  *(uint *)local_40 = *(uint *)local_40 - 1;
  UNLOCK();
  local_3c = local_40;
  if (*(uint *)local_40 == 0) goto LAB_00407cd9;
  iVar5 = *(int *)local_44;
  goto joined_r0x00407d4f;
  while( true ) {
    if (*(int *)local_28 != -1) {
      LOCK();
      *(int *)local_28 = *(int *)local_28 + -1;
      UNLOCK();
      if (*(int *)local_28 == 0) goto LAB_00407c40;
    }
    if ((SBORROW4(0,local_50) != SBORROW4(-local_50,(uint)(local_54 != 0))) ==
        (int)(-local_50 - (uint)(local_54 != 0)) < 0) break;
LAB_00407bec:
    uVar10 = local_54;
    if ((SBORROW4(0,local_50) != SBORROW4(-local_50,(uint)(0x2000 < local_54))) !=
        (int)(-local_50 - (uint)(0x2000 < local_54)) < 0) {
      uVar10 = 0x2000;
    }
    QIODevice::read(CONCAT44(uVar10,param_1));
    uVar10 = *(uint *)(local_28 + 4);
    bVar11 = local_54 < uVar10;
    local_54 = local_54 - uVar10;
    local_50 = (local_50 - ((int)uVar10 >> 0x1f)) - (uint)bVar11;
    pQVar13 = (QByteArray *)&local_28;
    QCryptographicHash::addData((QByteArray *)&local_28);
    if (*(int *)local_28 == 0) {
LAB_00407c40:
      QArrayData::deallocate((QArrayData *)pQVar13,1,4);
      if ((SBORROW4(0,local_50) != SBORROW4(-local_50,(uint)(local_54 != 0))) ==
          (int)(-local_50 - (uint)(local_54 != 0)) < 0) break;
      goto LAB_00407bec;
    }
  }
LAB_00407c67:
  QCryptographicHash::result();
  pcVar1 = *(code **)(*param_1 + 0x44);
  if ((*(size_t *)(local_38 + 4) == *(size_t *)(local_30 + 4)) &&
     (iVar3 = memcmp(local_38 + *(int *)(local_38 + 0xc),local_30 + *(int *)(local_30 + 0xc),
                     *(size_t *)(local_38 + 4)), iVar3 == 0)) {
    pQVar8 = (QArrayData *)((iVar5 - (uint)(uVar7 < uVar6)) + -1 + (uint)(9 < uVar7 - uVar6));
    (*pcVar1)((uVar7 - uVar6) - 10);
    QIODevice::read(CONCAT44(uVar6,param_1));
    cVar2 = QByteArray::isNull();
    if (cVar2 == '\0') {
      _Str = local_2c + *(int *)(local_2c + 0xc);
      if ((_Str != (QArrayData *)0x0) && (iVar5 = *(int *)(local_2c + 4), iVar5 != 0)) {
        iVar3 = 0;
        QVar4 = *_Str;
        while (QVar4 != (QArrayData)0x0) {
          iVar3 = iVar3 + 1;
          if (iVar5 == iVar3) {
            if (iVar5 == -1) {
              strlen((char *)_Str);
            }
            break;
          }
          QVar4 = _Str[iVar3];
        }
      }
      QString::fromUtf8_helper((char *)&local_28,(int)_Str);
    }
    else {
      local_28 = shared_null_exref;
    }
    local_5c = (QString *)&local_28;
    QString::operator=(param_2,local_5c);
    *param_3 = uVar6 + 0x1a;
    (**(code **)(*param_1 + 0x44))(0,0);
    if (*(int *)local_28 == 0) {
LAB_00407fa0:
      QArrayData::deallocate(pQVar8,2,4);
    }
    else if (*(int *)local_28 != -1) {
      LOCK();
      *(int *)local_28 = *(int *)local_28 + -1;
      UNLOCK();
      if (*(int *)local_28 == 0) goto LAB_00407fa0;
    }
    if (*(int *)local_2c == 0) {
LAB_00407f80:
      QArrayData::deallocate(local_2c,1,4);
    }
    else if (*(int *)local_2c != -1) {
      LOCK();
      *(int *)local_2c = *(int *)local_2c + -1;
      UNLOCK();
      if (*(int *)local_2c == 0) goto LAB_00407f80;
    }
    uVar9 = 1;
  }
  else {
    (*pcVar1)(0,0);
    uVar9 = 0;
  }
  if (*(int *)local_30 == 0) {
LAB_00407e10:
    QArrayData::deallocate(local_30,1,4);
  }
  else if (*(int *)local_30 != -1) {
    LOCK();
    *(int *)local_30 = *(int *)local_30 + -1;
    UNLOCK();
    if (*(int *)local_30 == 0) goto LAB_00407e10;
  }
  QCryptographicHash::~QCryptographicHash(local_34);
  uVar7 = *(uint *)local_3c;
  if (uVar7 != 0) goto LAB_00407d3a;
LAB_00407cd9:
  QArrayData::deallocate(local_3c,1,4);
LAB_00407cf2:
  iVar5 = *(int *)local_44;
joined_r0x00407d4f:
  if (iVar5 == 0) {
LAB_00407d51:
    QArrayData::deallocate(local_44,1,4);
    return uVar9;
  }
  if (iVar5 != -1) {
    LOCK();
    *(int *)local_44 = *(int *)local_44 + -1;
    UNLOCK();
    if (*(int *)local_44 == 0) goto LAB_00407d51;
  }
  return uVar9;
}

