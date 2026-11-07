// 0x080333f0 FUN_080333f0 size=152

undefined4 *
FUN_080333f0(int param_1,undefined2 param_2,undefined2 param_3,undefined4 param_4,undefined4 param_5
            )

{
  undefined1 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  
  iVar3 = FUN_08033498();
  puVar1 = DAT_08033488;
  if (iVar3 == 0) {
    *DAT_08033488 = 0;
    uVar4 = FUN_0802d5c4(0x120);
    *DAT_0803348c = uVar4;
    *(undefined4 *)(puVar1 + 0xc) = uVar4;
  }
  FUN_0802821c();
  iVar3 = FUN_08028248();
  if (iVar3 < 0x141) {
    puVar1[1] = 3;
  }
  else {
    FUN_0802821c();
    iVar3 = FUN_08028248();
    if (iVar3 < 0x2d0) {
      puVar1[1] = 2;
    }
    else {
      puVar1[1] = 1;
    }
  }
  puVar2 = DAT_08033490;
  DAT_08033490[3] = param_1;
  *(undefined2 *)(puVar2 + 4) = param_2;
  *(undefined2 *)((int)puVar2 + 0x12) = param_3;
  puVar2[5] = param_5;
  puVar2[6] = param_5;
  puVar2[7] = param_5;
  *puVar2 = DAT_08033494;
  puVar2[8] = param_4;
  FUN_0803871c();
  if ((param_1 == 0) || (puVar5 = (undefined4 *)FUN_080282d8(param_1), puVar5 == puVar2)) {
    FUN_08030f40(0);
  }
  *puVar1 = 1;
  return DAT_08033490;
}

