// 0x080359b4 FUN_080359b4 size=124

void FUN_080359b4(void)

{
  undefined4 uVar1;
  
  FUN_0801bd04(DAT_08035a30,1);
  uVar1 = DAT_08035a3c;
  if (*DAT_08035a34 == '\0') {
    if (*DAT_08035a38 == '\x01') {
      FUN_080398ba(DAT_08035a3c,99,0x2f);
    }
    else {
      FUN_080398ba(DAT_08035a3c,99,0x11f);
    }
  }
  else if (*DAT_08035a38 == '\x01') {
    FUN_080398ba(DAT_08035a3c,99,0x2f);
  }
  else {
    FUN_080398ba(DAT_08035a3c,99,0x11f);
  }
  FUN_080398f0(uVar1,0);
  FUN_08039a74(uVar1,1);
  FUN_08035cf0(3);
  FUN_08035c78(0x37,1,0);
  FUN_080398fa(uVar1,1);
  return;
}

