// 0x080119dc FUN_080119dc size=270

void FUN_080119dc(void)

{
  char cVar1;
  bool bVar2;
  char *pcVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  ushort uVar7;
  
  uVar7 = 0;
  bVar2 = false;
  FUN_0801c0fe(10);
  iVar4 = DAT_08011af0;
  pcVar3 = DAT_08011aec;
  cVar1 = *DAT_08011aec;
  *(char *)(DAT_08011af0 + 10) = cVar1;
  FUN_08014814(cVar1 + -1);
  FUN_08023520(0x36);
  FUN_08023520(0x33);
  thunk_FUN_0801c118(1000);
  FUN_08023520(0x36);
  FUN_08023540(0x17,0x32);
  FUN_08023540(6,0x3d);
  FUN_08023540(0x12,0x33);
  FUN_08023540(8,0x12);
  FUN_08023540(2,0xc);
  FUN_08023540(0,0xb);
  FUN_08023520(0x36);
  uVar5 = DAT_08011af4;
  do {
    FUN_080335bc();
    FUN_0803ba4c();
    iVar6 = FUN_08022eb4(uVar5,0x100);
    if (iVar6 == 0) {
      if ((bVar2) && (uVar7 = uVar7 + 1, 200 < uVar7)) {
        FUN_08023520(0x36);
        FUN_08023540(6,4);
        FUN_08023540(0x12,0x73);
        FUN_08023540(0x17,0x30);
        FUN_08023540(8,0);
        FUN_08023540(2,6);
        FUN_08023540(0,0x29);
        FUN_080118e8(1,*pcVar3 + -1,0);
        FUN_08010b24();
        return;
      }
    }
    else {
      uVar7 = 0;
      bVar2 = true;
    }
    if (*(char *)(iVar4 + 0xe) != *(char *)(iVar4 + 10)) {
      FUN_08014814(*(char *)(iVar4 + 10) + -1);
      *(undefined1 *)(iVar4 + 0xe) = *(undefined1 *)(iVar4 + 10);
    }
    iVar6 = DAT_08011af0;
    *(undefined1 *)(DAT_08011af0 + 0x14) = 0;
    *(undefined1 *)(iVar6 + 0x15) = 0;
    FUN_08010b6c();
    FUN_08023520(0x35);
  } while( true );
}

