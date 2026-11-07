// 0x004070e0 FUN_004070e0 size=258

void __fastcall FUN_004070e0(QObject *param_1)

{
  code *pcVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined4 unaff_ESI;
  code *local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  QObject::QObject(param_1,(QObject *)0x0);
  *(undefined ***)param_1 = &PTR_LAB_0044ee30;
  uVar2 = QString::fromAscii_helper((char *)0x0,-1);
  *(undefined4 *)(param_1 + 8) = uVar2;
  pcVar1 = shared_null_exref;
  param_1[0xc] = (QObject)0x0;
  *(code **)(param_1 + 0xbc) = pcVar1;
  *(undefined4 *)(param_1 + 0x14) = 0;
  param_1[0x9a] = (QObject)0x0;
  *(undefined2 *)(param_1 + 0xc0) = 0;
  param_1[0xc2] = (QObject)0x0;
  *(undefined4 *)(param_1 + 0xc4) = 0;
  local_14 = FUN_004208c0;
  local_10 = 0;
  local_c = FUN_00408b30;
  puVar3 = operator_new(0x10);
  *puVar3 = 1;
  puVar3[1] = &LAB_00430dc0;
  puVar3[2] = 0;
  puVar3[3] = unaff_ESI;
  QObject::connectImpl
            ((Connection *)&local_14,param_1,&local_10,param_1,(int)register0x00000010 + -8,puVar3,0
             ,0,&DAT_004340b0);
  QMetaObject::Connection::~Connection((Connection *)&local_14);
  return;
}

