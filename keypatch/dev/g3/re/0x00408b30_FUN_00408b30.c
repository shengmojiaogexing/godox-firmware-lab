// 0x00408b30 FUN_00408b30 size=207

void __fastcall FUN_00408b30(void *param_1)

{
  char cVar1;
  undefined4 uVar2;
  uint uVar3;
  QEventLoop local_24 [20];
  
  hid_init();
  QThread::msleep(1000);
  uVar2 = QObject::startTimer(1,1);
  *(undefined4 *)((int)param_1 + 0x10) = uVar2;
  do {
    FUN_00408060((int)param_1);
    if (*(char *)((int)param_1 + 0xc) != '\0') {
      uVar3 = FUN_004082e0(param_1);
      if ((char)uVar3 == '\0') {
        *(undefined1 *)((int)param_1 + 0xc) = 0;
      }
    }
    QEventLoop::QEventLoop(local_24,(QObject *)0x0);
    QTimer::singleShot(1,(QObject *)local_24,"1quit()");
    QEventLoop::exec(0);
    QEventLoop::~QEventLoop(local_24);
    QThread::currentThread();
    cVar1 = QThread::isInterruptionRequested();
  } while (cVar1 == '\0');
  QObject::killTimer(*(int *)((int)param_1 + 0x10));
  return;
}

