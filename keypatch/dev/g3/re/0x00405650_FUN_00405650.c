// 0x00405650 FUN_00405650 size=1542

/* WARNING: Exceeded maximum restarts with more pending */

void __fastcall FUN_00405650(void *param_1)

{
  char cVar1;
  int iVar2;
  code *pcVar3;
  QString *pQVar4;
  QArrayData *this;
  QUrl local_34 [4];
  QArrayData *local_30;
  QArrayData *local_2c;
  QArrayData *local_28;
  QArrayData *local_24;
  QArrayData *local_20 [4];
  
  QUrl::QUrl(local_34,*(undefined4 *)((int)param_1 + 0x18),0);
  QUrl::fileName(local_34,0x7f00000);
  pcVar3 = applicationDirPath_exref;
  QCoreApplication::applicationDirPath();
  local_28 = local_2c;
  if (1 < *(int *)local_2c + 1U) {
    LOCK();
    *(int *)local_2c = *(int *)local_2c + 1;
    UNLOCK();
  }
  QString::fromUtf8_helper((char *)local_20,0x4350e4);
  QString::append((QString *)local_20);
  if (*(int *)local_20[0] == 0) {
LAB_00405940:
    QArrayData::deallocate(local_20[0],2,4);
    iVar2 = *(int *)local_28;
    local_30 = local_28;
  }
  else {
    if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_00405940;
    }
    iVar2 = *(int *)local_28;
    local_30 = local_28;
  }
  if (1 < iVar2 + 1U) {
    LOCK();
    *(int *)local_30 = *(int *)local_30 + 1;
    UNLOCK();
  }
  local_28 = local_30;
  QString::append((QString *)&local_24);
  if (*(int *)local_28 == 0) {
LAB_004059c0:
    QArrayData::deallocate(local_28,2,4);
  }
  else if (*(int *)local_28 != -1) {
    LOCK();
    *(int *)local_28 = *(int *)local_28 + -1;
    UNLOCK();
    if (*(int *)local_28 == 0) goto LAB_004059c0;
  }
  if (*(int *)local_2c == 0) {
LAB_004059a0:
    QArrayData::deallocate(local_2c,2,4);
  }
  else if (*(int *)local_2c != -1) {
    LOCK();
    *(int *)local_2c = *(int *)local_2c + -1;
    UNLOCK();
    if (*(int *)local_2c == 0) goto LAB_004059a0;
  }
  if (*(int *)local_24 == 0) {
LAB_00405980:
    QArrayData::deallocate(local_24,2,4);
  }
  else if (*(int *)local_24 != -1) {
    LOCK();
    *(int *)local_24 = *(int *)local_24 + -1;
    UNLOCK();
    if (*(int *)local_24 == 0) goto LAB_00405980;
  }
  cVar1 = QFile::exists((QString *)&local_30);
  if (cVar1 == '\0') {
    FUN_0041a750(*(int *)((int)param_1 + 0xb0));
    (*pcVar3)(&local_24);
    local_28 = local_24;
    if (1 < *(int *)local_24 + 1U) {
      LOCK();
      *(int *)local_24 = *(int *)local_24 + 1;
      UNLOCK();
    }
    QString::fromUtf8_helper((char *)local_20,0x43513f);
    QString::append((QString *)local_20);
    if (*(int *)local_20[0] == 0) {
LAB_004058f0:
      QArrayData::deallocate(local_20[0],2,4);
      iVar2 = *(int *)local_24;
      if (iVar2 != 0) goto LAB_0040581c;
LAB_00405916:
      QArrayData::deallocate(local_24,2,4);
    }
    else {
      if (*(int *)local_20[0] != -1) {
        LOCK();
        *(int *)local_20[0] = *(int *)local_20[0] + -1;
        UNLOCK();
        if (*(int *)local_20[0] == 0) goto LAB_004058f0;
      }
      iVar2 = *(int *)local_24;
      if (iVar2 == 0) goto LAB_00405916;
LAB_0040581c:
      if (iVar2 != -1) {
        LOCK();
        *(int *)local_24 = *(int *)local_24 + -1;
        UNLOCK();
        if (*(int *)local_24 == 0) goto LAB_00405916;
      }
    }
    FUN_00404f90((QString *)&local_28);
    local_20[0] = (QArrayData *)**(undefined4 **)((int)param_1 + 0x18);
    if (1 < *(int *)local_20[0] + 1U) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + 1;
      UNLOCK();
    }
    FUN_00402520(param_1,(QString *)local_20);
    if (*(int *)local_20[0] == 0) {
LAB_00405c10:
      QArrayData::deallocate(local_20[0],2,4);
    }
    else if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_00405c10;
    }
    if (*(char *)((int)param_1 + 0xa8) == '\0') {
      FUN_00402c90(param_1,0xb);
      iVar2 = *(int *)local_28;
      pcVar3 = ~QUrl_exref;
      if (iVar2 == 0) goto LAB_00405be3;
LAB_004058ac:
      if (iVar2 != -1) {
        LOCK();
        *(int *)local_28 = *(int *)local_28 + -1;
        UNLOCK();
        if (*(int *)local_28 == 0) goto LAB_00405be3;
      }
    }
    else {
      QUrl::QUrl(*(undefined4 *)((int)param_1 + 0x18),0);
      QUrl::fileName(&local_24,0x7f00000);
      pQVar4 = (QString *)local_20;
      FUN_00417f40((int *)((int)param_1 + 0x90),(QString *)local_20);
      if (*(int *)local_20[0] == 0) {
LAB_00405c70:
        QArrayData::deallocate((QArrayData *)pQVar4,2,4);
      }
      else if (*(int *)local_20[0] != -1) {
        LOCK();
        *(int *)local_20[0] = *(int *)local_20[0] + -1;
        UNLOCK();
        if (*(int *)local_20[0] == 0) goto LAB_00405c70;
      }
      local_20[0] = (QArrayData *)QString::fromAscii_helper("*",1);
      FUN_0041a7f0((int *)((int)param_1 + 0x90),*(int *)((int)param_1 + 0x8c),(int *)local_20);
      if (*(int *)local_20[0] == 0) {
LAB_00405c40:
        QArrayData::deallocate(local_20[0],2,4);
        pcVar3 = ~QUrl_exref;
        QUrl::~QUrl((QUrl *)&local_24);
      }
      else {
        if (*(int *)local_20[0] != -1) {
          LOCK();
          *(int *)local_20[0] = *(int *)local_20[0] + -1;
          UNLOCK();
          if (*(int *)local_20[0] == 0) goto LAB_00405c40;
        }
        pcVar3 = ~QUrl_exref;
        QUrl::~QUrl((QUrl *)&local_24);
      }
      iVar2 = *(int *)local_28;
      if (iVar2 != 0) goto LAB_004058ac;
LAB_00405be3:
      QArrayData::deallocate(local_28,2,4);
    }
    iVar2 = *(int *)local_30;
  }
  else {
    FUN_0041a750(*(int *)((int)param_1 + 0xb0));
    QUrl::QUrl(*(undefined4 *)((int)param_1 + 0x18),0);
    this = (QArrayData *)((int)param_1 + 0x90);
    QUrl::fileName(&local_24,0x7f00000);
    pQVar4 = (QString *)local_20;
    FUN_00417f40(this,(QString *)local_20);
    if (*(int *)local_20[0] == 0) {
LAB_00405a40:
      QArrayData::deallocate((QArrayData *)pQVar4,2,4);
    }
    else if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_00405a40;
    }
    local_20[0] = (QArrayData *)QString::fromAscii_helper("*",1);
    FUN_0041a7f0((int *)this,*(int *)((int)param_1 + 0x8c),(int *)local_20);
    if (*(int *)local_20[0] == 0) {
LAB_00405ab0:
      QArrayData::deallocate(this,2,4);
    }
    else if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_00405ab0;
    }
    pcVar3 = ~QUrl_exref;
    QUrl::~QUrl((QUrl *)&local_24);
    iVar2 = *(int *)local_30;
  }
  if (iVar2 == 0) {
LAB_00405ae1:
    QArrayData::deallocate(local_30,2,4);
    (*pcVar3)();
    return;
  }
  if (iVar2 != -1) {
    LOCK();
    *(int *)local_30 = *(int *)local_30 + -1;
    UNLOCK();
    if (*(int *)local_30 == 0) goto LAB_00405ae1;
  }
  (*pcVar3)();
  return;
}

