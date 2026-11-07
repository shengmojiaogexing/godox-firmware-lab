// 0x08025f30 FUN_08025f30 size=172

int FUN_08025f30(void)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_20 [12];
  uint local_14;
  undefined1 local_10;
  undefined1 local_f;
  undefined1 local_e;
  undefined1 local_d;
  
  FUN_0801bd04(DAT_08025fdc,1);
  iVar1 = DAT_08025fe0;
  FUN_08039a8c(DAT_08025fe0,1);
  FUN_08039904(iVar1,0);
  FUN_08039908(iVar1,1);
  local_10 = 6;
  local_e = 1;
  local_f = 0;
  local_d = 0;
  FUN_0803995c(iVar1,&local_10,0);
  FUN_08039924(iVar1,6,3);
  FUN_08035c78(0x32,0);
  FUN_080398fa(iVar1,1);
  *(undefined4 *)(DAT_08025fe0 + 0x14) = 0;
  FUN_08039a74(iVar1,0x10,1);
  iVar2 = DAT_08025fe4;
  do {
  } while (*(int *)(DAT_08025fe4 + 0x14) != 2);
  FUN_08039ab0(iVar1);
  FUN_0801bb20(auStack_20);
  if ((*DAT_08025fe8 & 0x1c00) == 0) {
    local_14 = local_14 / *(uint *)(iVar2 + 0x18);
  }
  else {
    local_14 = (local_14 << 1) / *(uint *)(iVar2 + 0x18);
  }
  return local_14 << 3;
}

