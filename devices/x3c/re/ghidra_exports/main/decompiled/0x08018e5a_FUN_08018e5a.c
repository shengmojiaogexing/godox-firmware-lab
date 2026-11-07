// 0x08018e5a FUN_08018e5a size=30

uint FUN_08018e5a(int param_1)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = 0;
  if ((param_1 != 0) && (uVar1 = FUN_08018e98(), uVar1 < 0x20000)) {
    if (uVar1 < 0xd) {
      uVar1 = 0xc;
    }
    uVar2 = uVar1;
  }
  return uVar2;
}

