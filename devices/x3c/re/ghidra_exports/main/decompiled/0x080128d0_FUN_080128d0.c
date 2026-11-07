// 0x080128d0 FUN_080128d0 size=62

void FUN_080128d0(int param_1)

{
  FUN_08023520(0x36);
  if (param_1 == 0) {
    FUN_08023540(0x3e,0xff);
  }
  else if (param_1 == 1) {
    FUN_08023540(0x3e,0x46);
  }
  else {
    FUN_08023540(0x3e,0x81);
  }
  FUN_08023520(0x33);
  thunk_FUN_0801c118(1000);
  return;
}

