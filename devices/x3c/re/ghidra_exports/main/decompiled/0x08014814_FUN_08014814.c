// 0x08014814 FUN_08014814 size=76

void FUN_08014814(int param_1)

{
  FUN_08023520(0x36);
  thunk_FUN_0801c118(1000);
  FUN_08023540(0xd,*(undefined1 *)(DAT_08014860 + param_1));
  FUN_08023540(0xe,*(undefined1 *)(DAT_08014860 + 0x21 + param_1));
  FUN_08023540(0xf,*(undefined1 *)(DAT_08014860 + 0x42 + param_1));
  thunk_FUN_0801c118(0x14);
  FUN_08023520(0x33);
  thunk_FUN_0801c118(700);
  return;
}

