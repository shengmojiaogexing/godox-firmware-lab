// 0x00403040 FUN_00403040 size=724

void __thiscall FUN_00403040(void *this,undefined4 *param_1)

{
  int iVar1;
  code *pcVar2;
  QArrayData *in_stack_ffffffd0;
  undefined4 local_2c;
  QArrayData *local_28;
  QArrayData *local_24;
  QArrayData *local_20 [4];
  
  QFileInfo::QFileInfo((QFileInfo *)(local_20 + -4));
  local_2c = (QArrayData *)*param_1;
  if (1 < *(int *)local_2c + 1U) {
    LOCK();
    *(int *)local_2c = *(int *)local_2c + 1;
    UNLOCK();
  }
  QFileInfo::QFileInfo((QFileInfo *)local_20,(QString *)&local_2c);
  local_20[0] = in_stack_ffffffd0;
  QFileInfo::~QFileInfo((QFileInfo *)local_20);
  QFileInfo::fileName();
  local_24 = *(QArrayData **)(*(int *)((int)this + 0x4c) + *(int *)((int)this + 0x8c) * 4);
  if (1 < *(int *)local_24 + 1U) {
    LOCK();
    *(int *)local_24 = *(int *)local_24 + 1;
    UNLOCK();
  }
  QString::fromUtf8_helper((char *)local_20,0x435138);
  pcVar2 = append_exref;
  QString::append((QString *)local_20);
  if (*(int *)local_20[0] == 0) {
LAB_00403213:
    QArrayData::deallocate(local_20[0],2,4);
    iVar1 = *(int *)local_24;
  }
  else {
    if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_00403213;
    }
    iVar1 = *(int *)local_24;
  }
  if (1 < iVar1 + 1U) {
    LOCK();
    *(int *)local_24 = *(int *)local_24 + 1;
    UNLOCK();
  }
  local_20[0] = local_24;
  (*pcVar2)(&local_28);
  FUN_004028d0(this,'\0',(QString *)local_20);
  if (*(int *)local_20[0] == 0) {
LAB_004032c0:
    QArrayData::deallocate(local_20[0],2,4);
    iVar1 = *(int *)local_24;
    if (iVar1 != 0) goto LAB_0040317e;
LAB_004032e6:
    QArrayData::deallocate(local_24,2,4);
  }
  else {
    if (*(int *)local_20[0] != -1) {
      LOCK();
      *(int *)local_20[0] = *(int *)local_20[0] + -1;
      UNLOCK();
      if (*(int *)local_20[0] == 0) goto LAB_004032c0;
    }
    iVar1 = *(int *)local_24;
    if (iVar1 == 0) goto LAB_004032e6;
LAB_0040317e:
    if (iVar1 != -1) {
      LOCK();
      *(int *)local_24 = *(int *)local_24 + -1;
      UNLOCK();
      if (*(int *)local_24 == 0) goto LAB_004032e6;
    }
  }
  local_20[0] = (QArrayData *)*param_1;
  if (1 < *(int *)local_20[0] + 1U) {
    LOCK();
    *(int *)local_20[0] = *(int *)local_20[0] + 1;
    UNLOCK();
  }
  FUN_0041f480(this,(QFileInfo *)local_20);
  if (*(int *)local_20[0] == 0) {
LAB_004032a0:
    QArrayData::deallocate(local_20[0],2,4);
  }
  else if (*(int *)local_20[0] != -1) {
    LOCK();
    *(int *)local_20[0] = *(int *)local_20[0] + -1;
    UNLOCK();
    if (*(int *)local_20[0] == 0) goto LAB_004032a0;
  }
  if (*(int *)local_28 == 0) {
LAB_00403250:
    QArrayData::deallocate(local_28,2,4);
    iVar1 = *(int *)local_2c;
  }
  else {
    if (*(int *)local_28 != -1) {
      LOCK();
      *(int *)local_28 = *(int *)local_28 + -1;
      UNLOCK();
      if (*(int *)local_28 == 0) goto LAB_00403250;
    }
    iVar1 = *(int *)local_2c;
  }
  if (iVar1 == 0) {
LAB_00403276:
    QArrayData::deallocate(local_2c,2,4);
    QFileInfo::~QFileInfo((QFileInfo *)(local_20 + -4));
    return;
  }
  if (iVar1 != -1) {
    LOCK();
    *(int *)local_2c = *(int *)local_2c + -1;
    UNLOCK();
    if (*(int *)local_2c == 0) goto LAB_00403276;
  }
  QFileInfo::~QFileInfo((QFileInfo *)(local_20 + -4));
  return;
}

