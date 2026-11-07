// 0x004028d0 FUN_004028d0 size=874

void __thiscall FUN_004028d0(void *this,char param_1,QString *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  QArrayData *local_2c;
  QArrayData *local_28;
  QArrayData *local_24;
  QArrayData *local_20 [4];
  
  QTime::currentTime();
  iVar1 = QTime::hour();
  iVar2 = QTime::minute();
  iVar3 = QTime::second();
  local_28 = (QArrayData *)QString::fromAscii_helper("[%1:%2:%3] ",0xb);
  uVar5 = 0x40;
  QString::arg(&local_28,iVar1,iVar1 >> 0x1f,2,10,0x30);
  uVar4 = 0x40;
  QString::arg(&local_24,iVar2,iVar2 >> 0x1f,2,10,CONCAT22(uVar5,0x30));
  QString::arg(local_20,iVar3,iVar3 >> 0x1f,2,10,CONCAT22(uVar4,0x30));
  if (*(int *)local_20[0] == 0) {
LAB_00402bb2:
    QArrayData::deallocate(local_20[0],2,4);
    iVar1 = *(int *)local_24;
    if (iVar1 != 0) goto LAB_004029fa;
LAB_00402bd8:
    QArrayData::deallocate(local_24,2,4);
    iVar1 = *(int *)local_28;
    if (iVar1 == 0) goto LAB_00402bfe;
LAB_00402a19:
    if (iVar1 != -1) {
      LOCK();
      *(int *)local_28 = *(int *)local_28 + -1;
      UNLOCK();
      if (*(int *)local_28 == 0) goto LAB_00402bfe;
    }
    iVar1 = *(int *)local_2c;
    if (param_1 != '\0') goto LAB_00402af0;
LAB_00402a3d:
    if (1 < iVar1 + 1U) {
      LOCK();
      *(int *)local_2c = *(int *)local_2c + 1;
      UNLOCK();
    }
    local_24 = local_2c;
    QString::append(param_2);
    local_20[0] = (QArrayData *)QString::fromAscii_helper("black",5);
    FUN_0041f3c0(this,local_20,&local_24);
    iVar1 = *(int *)local_20[0];
    if (iVar1 == 0) goto LAB_00402b43;
LAB_00402a90:
    if (iVar1 != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_00402b43;
    }
    iVar1 = *(int *)local_24;
    if (iVar1 != 0) goto LAB_00402aaf;
LAB_00402b69:
    QArrayData::deallocate(local_24,2,4);
    iVar1 = *(int *)local_2c;
  }
  else {
    if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_00402bb2;
    }
    iVar1 = *(int *)local_24;
    if (iVar1 == 0) goto LAB_00402bd8;
LAB_004029fa:
    if (iVar1 != -1) {
      LOCK();
      *(int *)local_24 = *(int *)local_24 + -1;
      UNLOCK();
      if (*(int *)local_24 == 0) goto LAB_00402bd8;
    }
    iVar1 = *(int *)local_28;
    if (iVar1 != 0) goto LAB_00402a19;
LAB_00402bfe:
    QArrayData::deallocate(local_28,2,4);
    iVar1 = *(int *)local_2c;
    if (param_1 == '\0') goto LAB_00402a3d;
LAB_00402af0:
    if (1 < iVar1 + 1U) {
      LOCK();
      *(int *)local_2c = *(int *)local_2c + 1;
      UNLOCK();
    }
    local_24 = local_2c;
    QString::append(param_2);
    local_20[0] = (QArrayData *)QString::fromAscii_helper("red",3);
    FUN_0041f3c0(this,local_20,&local_24);
    iVar1 = *(int *)local_20[0];
    if (iVar1 != 0) goto LAB_00402a90;
LAB_00402b43:
    QArrayData::deallocate(local_20[0],2,4);
    iVar1 = *(int *)local_24;
    if (iVar1 == 0) goto LAB_00402b69;
LAB_00402aaf:
    if (iVar1 != -1) {
      LOCK();
      *(int *)local_24 = *(int *)local_24 + -1;
      UNLOCK();
      if (*(int *)local_24 == 0) goto LAB_00402b69;
    }
    iVar1 = *(int *)local_2c;
  }
  if (iVar1 == 0) {
LAB_00402b8f:
    QArrayData::deallocate(local_2c,2,4);
    return;
  }
  if (iVar1 != -1) {
    LOCK();
    *(int *)local_2c = *(int *)local_2c + -1;
    UNLOCK();
    if (*(int *)local_2c == 0) goto LAB_00402b8f;
  }
  return;
}

