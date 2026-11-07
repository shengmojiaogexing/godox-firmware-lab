// 0x00405d20 FUN_00405d20 size=2534

void __thiscall FUN_00405d20(void *this,int param_1)

{
  char cVar1;
  int iVar2;
  QArrayData **ppQVar3;
  code *pcVar4;
  QString *pQVar5;
  QArrayData *pQVar6;
  QUrl *pQVar7;
  code *pcVar8;
  code *pcVar9;
  QArrayData *local_44;
  QArrayData *local_40;
  QArrayData *local_3c;
  QArrayData *local_38;
  QArrayData *local_34;
  QArrayData *local_30;
  QArrayData *local_2c;
  QArrayData *local_28;
  QArrayData *local_24;
  QArrayData *local_20 [4];
  
  iVar2 = *(int *)((int)this + 0x90);
  FUN_00402230(&local_44,*(int *)((int)this + 0x18) + *(int *)((int)this + 0x8c) * 4);
  pQVar5 = (QString *)(param_1 * 0x84 + iVar2 + 0xc);
  local_24 = local_44;
  if (1 < *(int *)local_44 + 1U) {
    LOCK();
    *(int *)local_44 = *(int *)local_44 + 1;
    UNLOCK();
  }
  pcVar9 = fromUtf8_helper_exref;
  QString::fromUtf8_helper((char *)local_20,0x43513b);
  QString::append((QString *)local_20);
  if (*(int *)local_20[0] == 0) {
LAB_004063b0:
    QArrayData::deallocate(local_20[0],2,4);
    iVar2 = *(int *)local_24;
    local_40 = local_24;
  }
  else {
    if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_004063b0;
    }
    iVar2 = *(int *)local_24;
    local_40 = local_24;
  }
  if (1 < iVar2 + 1U) {
    LOCK();
    *(int *)local_40 = *(int *)local_40 + 1;
    UNLOCK();
  }
  local_24 = local_40;
  QString::append(pQVar5);
  if (*(int *)local_24 == 0) {
LAB_004063f0:
    QArrayData::deallocate(local_24,2,4);
  }
  else if (*(int *)local_24 != -1) {
    LOCK();
    *(int *)local_24 = *(int *)local_24 + -1;
    UNLOCK();
    if (*(int *)local_24 == 0) goto LAB_004063f0;
  }
  pcVar8 = QUrl_exref;
  QUrl::QUrl((QUrl *)local_20,&local_40,0);
  FUN_00406960((void *)((int)this + 0x9c),(QUrl *)local_20);
  pcVar4 = ~QUrl_exref;
  QUrl::~QUrl((QUrl *)local_20);
  if (*(char *)((int)this + 0xa8) == '\0') {
    FUN_00402c90(this,0xb);
    iVar2 = *(int *)local_40;
    if (iVar2 != 0) goto LAB_0040631f;
LAB_00406451:
    QArrayData::deallocate(local_40,2,4);
  }
  else {
    ppQVar3 = &local_40;
    (*pcVar8)(ppQVar3,0);
    QUrl::fileName(&local_28,0x7f00000,ppQVar3,fileName_exref);
    pcVar8 = applicationDirPath_exref;
    QCoreApplication::applicationDirPath();
    local_2c = local_30;
    if (1 < *(int *)local_30 + 1U) {
      LOCK();
      *(int *)local_30 = *(int *)local_30 + 1;
      UNLOCK();
    }
    (*pcVar9)((QUrl *)local_20,"/Config/Temp/",0xd);
    QString::append((QString *)local_20);
    if (*(int *)local_20[0] == 0) {
LAB_00406360:
      QArrayData::deallocate(local_20[0],2,4);
      iVar2 = *(int *)local_2c;
      local_3c = local_2c;
    }
    else {
      if (*(int *)local_20[0] != -1) {
        LOCK();
        *(int *)local_20[0] = *(int *)local_20[0] + -1;
        UNLOCK();
        if (*(int *)local_20[0] == 0) goto LAB_00406360;
      }
      iVar2 = *(int *)local_2c;
      local_3c = local_2c;
    }
    if (1 < iVar2 + 1U) {
      LOCK();
      *(int *)local_3c = *(int *)local_3c + 1;
      UNLOCK();
    }
    local_2c = local_3c;
    QString::append((QString *)&local_24);
    if (*(int *)local_2c == 0) {
LAB_00406480:
      QArrayData::deallocate(local_2c,2,4);
      iVar2 = *(int *)local_30;
      if (iVar2 != 0) goto LAB_00405f43;
LAB_004064a6:
      QArrayData::deallocate(local_30,2,4);
      iVar2 = *(int *)local_24;
      if (iVar2 == 0) goto LAB_004064cc;
LAB_00405f62:
      if (iVar2 != -1) {
        LOCK();
        *(int *)local_24 = *(int *)local_24 + -1;
        UNLOCK();
        if (*(int *)local_24 == 0) goto LAB_004064cc;
      }
    }
    else {
      if (*(int *)local_2c != -1) {
        LOCK();
        *(int *)local_2c = *(int *)local_2c + -1;
        UNLOCK();
        if (*(int *)local_2c == 0) goto LAB_00406480;
      }
      iVar2 = *(int *)local_30;
      if (iVar2 == 0) goto LAB_004064a6;
LAB_00405f43:
      if (iVar2 != -1) {
        LOCK();
        *(int *)local_30 = *(int *)local_30 + -1;
        UNLOCK();
        if (*(int *)local_30 == 0) goto LAB_004064a6;
      }
      iVar2 = *(int *)local_24;
      if (iVar2 != 0) goto LAB_00405f62;
LAB_004064cc:
      QArrayData::deallocate(local_24,2,4);
    }
    (*pcVar4)();
    (*pcVar8)((QString *)&local_24);
    local_38 = local_24;
    if (1 < *(int *)local_24 + 1U) {
      LOCK();
      *(int *)local_24 = *(int *)local_24 + 1;
      UNLOCK();
    }
    (*pcVar9)((QUrl *)local_20,"/Config/Temp/bin/",0x11);
    pQVar5 = (QString *)&local_38;
    pcVar4 = (code *)0x405fc0;
    QString::append(pQVar5);
    if (*(int *)local_20[0] == 0) {
LAB_004064f0:
      pcVar4 = (code *)0x406509;
      QArrayData::deallocate(local_20[0],2,4);
      iVar2 = *(int *)local_24;
      if (iVar2 != 0) goto LAB_00405fef;
LAB_00406516:
      pcVar4 = (code *)0x40652f;
      pQVar6 = local_24;
      QArrayData::deallocate(local_24,2,4);
    }
    else {
      if (*(int *)local_20[0] != -1) {
        LOCK();
        *(int *)local_20[0] = *(int *)local_20[0] + -1;
        UNLOCK();
        if (*(int *)local_20[0] == 0) goto LAB_004064f0;
      }
      iVar2 = *(int *)local_24;
      local_20[0] = (QArrayData *)pQVar5;
      if (iVar2 == 0) goto LAB_00406516;
LAB_00405fef:
      pQVar6 = local_20[0];
      if (iVar2 != -1) {
        LOCK();
        *(int *)local_24 = *(int *)local_24 + -1;
        UNLOCK();
        if (*(int *)local_24 == 0) goto LAB_00406516;
      }
    }
    FUN_00404f90((QString *)pQVar6);
    local_20[0] = local_38;
    if (1 < *(int *)local_38 + 1U) {
      LOCK();
      *(int *)local_38 = *(int *)local_38 + 1;
      UNLOCK();
    }
    local_24 = local_3c;
    if (1 < *(int *)local_3c + 1U) {
      LOCK();
      *(int *)local_3c = *(int *)local_3c + 1;
      UNLOCK();
    }
    FUN_00402440((QString *)&local_24,(QString *)local_20);
    if (*(int *)local_24 == 0) {
LAB_004065b0:
      pcVar9 = (code *)0x4;
      QArrayData::deallocate(local_24,2,4);
    }
    else if (*(int *)local_24 != -1) {
      LOCK();
      *(int *)local_24 = *(int *)local_24 + -1;
      UNLOCK();
      if (*(int *)local_24 == 0) goto LAB_004065b0;
    }
    if (*(int *)local_20[0] == 0) {
LAB_00406590:
      pcVar9 = (code *)0x4;
      QArrayData::deallocate(local_20[0],2,4);
    }
    else if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_00406590;
    }
    pcVar8 = (code *)0x0;
    (*(code *)&local_40)();
    (*pcVar4)((QUrl *)local_20,0x7f00000);
    (*pcVar8)();
    local_20[0] = (QArrayData *)QString::fromAscii_helper(".zip",4);
    pQVar7 = (QUrl *)local_20;
    cVar1 = QString::endsWith((QUrl *)local_20,1);
    if (*(int *)local_20[0] == 0) {
LAB_00406570:
      pQVar7 = (QUrl *)0x4;
      QArrayData::deallocate(local_20[0],2,4);
    }
    else if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_00406570;
    }
    if (cVar1 != '\0') {
      pQVar7 = (QUrl *)0x40611d;
      QString::chop(4);
    }
    (*(code *)pQVar7)();
    local_28 = local_2c;
    if (1 < *(int *)local_2c + 1U) {
      LOCK();
      *(int *)local_2c = *(int *)local_2c + 1;
      UNLOCK();
    }
    (*pcVar9)((QUrl *)local_20,"/Config/Temp/bin/",0x11);
    QString::append((QString *)local_20);
    if (*(int *)local_20[0] == 0) {
LAB_00406534:
      QArrayData::deallocate(local_20[0],2,4);
      iVar2 = *(int *)local_28;
    }
    else {
      if (*(int *)local_20[0] != -1) {
        LOCK();
        *(int *)local_20[0] = *(int *)local_20[0] + -1;
        UNLOCK();
        if (*(int *)local_20[0] == 0) goto LAB_00406534;
      }
      iVar2 = *(int *)local_28;
    }
    if (1 < iVar2 + 1U) {
      LOCK();
      *(int *)local_28 = *(int *)local_28 + 1;
      UNLOCK();
    }
    local_24 = local_28;
    QString::append((QString *)&local_34);
    local_30 = local_24;
    if (1 < *(int *)local_24 + 1U) {
      LOCK();
      *(int *)local_24 = *(int *)local_24 + 1;
      UNLOCK();
    }
    (*pcVar9)((QUrl *)local_20,&DAT_00435163,4);
    QString::append((QString *)local_20);
    if (*(int *)local_20[0] == 0) {
LAB_00406710:
      QArrayData::deallocate(local_20[0],2,4);
    }
    else if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_00406710;
    }
    if (*(int *)local_24 == 0) {
LAB_004066f0:
      QArrayData::deallocate(local_24,2,4);
    }
    else if (*(int *)local_24 != -1) {
      LOCK();
      *(int *)local_24 = *(int *)local_24 + -1;
      UNLOCK();
      if (*(int *)local_24 == 0) goto LAB_004066f0;
    }
    if (*(int *)local_28 == 0) {
LAB_004066d0:
      QArrayData::deallocate(local_28,2,4);
    }
    else if (*(int *)local_28 != -1) {
      LOCK();
      *(int *)local_28 = *(int *)local_28 + -1;
      UNLOCK();
      if (*(int *)local_28 == 0) goto LAB_004066d0;
    }
    if (*(int *)local_2c == 0) {
LAB_00406690:
      QArrayData::deallocate(local_2c,2,4);
      iVar2 = *(int *)local_30;
    }
    else {
      if (*(int *)local_2c != -1) {
        LOCK();
        *(int *)local_2c = *(int *)local_2c + -1;
        UNLOCK();
        if (*(int *)local_2c == 0) goto LAB_00406690;
      }
      iVar2 = *(int *)local_30;
    }
    if (1 < iVar2 + 1U) {
      LOCK();
      *(int *)local_30 = *(int *)local_30 + 1;
      UNLOCK();
    }
    local_20[0] = local_30;
    FUN_00403040(this,(undefined4 *)local_20);
    if (*(int *)local_20[0] == 0) {
LAB_004065d0:
      QArrayData::deallocate(local_20[0],2,4);
      iVar2 = *(int *)local_30;
      if (iVar2 != 0) goto LAB_004062a3;
LAB_004065f6:
      QArrayData::deallocate(local_30,2,4);
      iVar2 = *(int *)local_34;
      if (iVar2 == 0) goto LAB_0040661c;
LAB_004062c2:
      if (iVar2 != -1) {
        LOCK();
        *(int *)local_34 = *(int *)local_34 + -1;
        UNLOCK();
        if (*(int *)local_34 == 0) goto LAB_0040661c;
      }
      iVar2 = *(int *)local_38;
      if (iVar2 != 0) goto LAB_004062e1;
LAB_00406642:
      QArrayData::deallocate(local_38,2,4);
      iVar2 = *(int *)local_3c;
      if (iVar2 == 0) goto LAB_00406668;
LAB_00406300:
      if (iVar2 != -1) {
        LOCK();
        *(int *)local_3c = *(int *)local_3c + -1;
        UNLOCK();
        if (*(int *)local_3c == 0) goto LAB_00406668;
      }
    }
    else {
      if (*(int *)local_20[0] != -1) {
        LOCK();
        *(int *)local_20[0] = *(int *)local_20[0] + -1;
        UNLOCK();
        if (*(int *)local_20[0] == 0) goto LAB_004065d0;
      }
      iVar2 = *(int *)local_30;
      if (iVar2 == 0) goto LAB_004065f6;
LAB_004062a3:
      if (iVar2 != -1) {
        LOCK();
        *(int *)local_30 = *(int *)local_30 + -1;
        UNLOCK();
        if (*(int *)local_30 == 0) goto LAB_004065f6;
      }
      iVar2 = *(int *)local_34;
      if (iVar2 != 0) goto LAB_004062c2;
LAB_0040661c:
      QArrayData::deallocate(local_34,2,4);
      iVar2 = *(int *)local_38;
      if (iVar2 == 0) goto LAB_00406642;
LAB_004062e1:
      if (iVar2 != -1) {
        LOCK();
        *(int *)local_38 = *(int *)local_38 + -1;
        UNLOCK();
        if (*(int *)local_38 == 0) goto LAB_00406642;
      }
      iVar2 = *(int *)local_3c;
      if (iVar2 != 0) goto LAB_00406300;
LAB_00406668:
      QArrayData::deallocate(local_3c,2,4);
    }
    iVar2 = *(int *)local_40;
    if (iVar2 == 0) goto LAB_00406451;
LAB_0040631f:
    if (iVar2 != -1) {
      LOCK();
      *(int *)local_40 = *(int *)local_40 + -1;
      UNLOCK();
      if (*(int *)local_40 == 0) goto LAB_00406451;
    }
  }
  if (*(int *)local_44 == 0) {
LAB_00406410:
    QArrayData::deallocate(local_44,2,4);
    return;
  }
  if (*(int *)local_44 != -1) {
    LOCK();
    *(int *)local_44 = *(int *)local_44 + -1;
    UNLOCK();
    if (*(int *)local_44 == 0) goto LAB_00406410;
  }
  return;
}

