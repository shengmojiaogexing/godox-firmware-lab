// 0x00407ff0 FUN_00407ff0 size=101

void __fastcall FUN_00407ff0(int param_1)

{
  undefined2 *puVar1;
  void *this;
  void *this_00;
  
  puVar1 = (undefined2 *)(param_1 + 0x18);
  *(undefined1 *)(param_1 + 0x9a) = 0;
  FUN_00407610(0xa0,(undefined1 *)0x0,0,puVar1);
  hid_write(this,*(undefined4 **)(param_1 + 0x14),puVar1,0x41);
  hid_write(this_00,*(undefined4 **)(param_1 + 0x14),puVar1,0x41);
  return;
}

