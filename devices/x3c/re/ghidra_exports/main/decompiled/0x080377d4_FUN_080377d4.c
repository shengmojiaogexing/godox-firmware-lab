// 0x080377d4 FUN_080377d4 size=176

void FUN_080377d4(void)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  short local_38 [2];
  int local_34;
  int local_30;
  ushort local_2c;
  short local_2a;
  short local_28 [2];
  int local_24;
  int local_20;
  ushort local_1c;
  short local_1a;
  undefined4 local_18;
  
  iVar3 = 0;
  local_18 = 0;
  FUN_08037a36(DAT_08037884,&local_18,4);
  FUN_08037700();
  puVar1 = DAT_08037888;
  *(undefined2 *)(DAT_08037888 + 3) = 0;
  puVar1[1] = 0;
  *(undefined2 *)((int)puVar1 + 0xe) = 0;
  local_24 = 0;
  iVar4 = DAT_08037884 + -4;
  do {
    FUN_08037568(10);
    iVar2 = FUN_080376b4(iVar4,local_28,0x10);
    if ((((iVar2 == 0) && (iVar2 = FUN_080376b4(iVar4,local_38,0x10), iVar2 == 0)) &&
        (local_24 == local_34)) &&
       (((local_20 == local_30 && (local_1c == local_2c)) &&
        ((local_1a == local_2a && (local_28[0] == local_38[0])))))) {
      if (local_24 == DAT_0803788c) {
        *(short *)(puVar1 + 3) = local_28[0];
        puVar1[1] = local_20;
        puVar1[2] = local_2c + 0x20000000;
        *(undefined2 *)((int)puVar1 + 0xe) = 1;
        *puVar1 = 0;
        return;
      }
      if (local_24 == DAT_08037890) {
        return;
      }
    }
    iVar3 = iVar3 + 1;
    if (9 < iVar3) {
      return;
    }
  } while( true );
}

