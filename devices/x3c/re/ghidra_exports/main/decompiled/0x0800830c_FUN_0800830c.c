// 0x0800830c FUN_0800830c size=156

void FUN_0800830c(void)

{
  undefined1 local_18 [4];
  undefined4 local_14;
  undefined1 local_10;
  undefined1 local_f;
  uint local_c;
  
  disableIRQinterrupts();
  FUN_080203ec();
  enableIRQinterrupts();
  local_c = FUN_08025f30();
  disableIRQinterrupts();
  FUN_080204d8(0x7f,local_c / 0xe00 - 1 & 0xffff);
  FUN_0802030c(0,DAT_080083a8);
  FUN_080203bc(0);
  FUN_08020340(0,1,0xc,0,0,0);
  FUN_08020600(0x1000);
  FUN_080202bc(0,1);
  FUN_08020510(0x100);
  FUN_08020968(local_18);
  local_f = 1;
  local_18[0] = 0;
  local_14 = 0x20000;
  local_10 = 0;
  FUN_0802099c(local_18);
  FUN_08035c78(0x29,1,0);
  FUN_080206d8(0xc,0,0);
  enableIRQinterrupts();
  return;
}

