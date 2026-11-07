// 0x08017768 FUN_08017768 size=204

void FUN_08017768(int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  code *pcVar5;
  uint uVar6;
  
  uVar3 = FUN_080334d8();
  puVar1 = DAT_0801780c;
  if (param_1 == 0) {
    uVar4 = FUN_0802821c();
    puVar1[1] = uVar4;
  }
  else {
    DAT_0801780c[1] = *(undefined4 *)(param_1 + 0xc);
    FUN_080336ac(param_1);
  }
  FUN_080317f8(*(undefined4 *)(puVar1[1] + 0x10));
  if (*(int *)(puVar1[1] + 0x14) != 0) {
    FUN_080317f8();
  }
  FUN_080317f8(*(undefined4 *)(puVar1[1] + 0x1c));
  FUN_080317f8(*(undefined4 *)(puVar1[1] + 0x20));
  if (*(int *)(puVar1[1] + 0x10) == 0) {
    *(undefined2 *)(puVar1[1] + 0x250) = 0;
    return;
  }
  FUN_08031b5c();
  FUN_0803637c();
  if (*(short *)(puVar1[1] + 0x250) != 0) {
    FUN_0802d8d6(puVar1[1] + 0x30,0x200);
    FUN_0802d8d6(puVar1[1] + 0x230,0x20);
    *(undefined2 *)(puVar1[1] + 0x250) = 0;
    uVar3 = FUN_080334c0(uVar3);
    pcVar5 = *(code **)(*(int *)puVar1[1] + 0x30);
    if (pcVar5 != (code *)0x0) {
      (*pcVar5)(*(int *)puVar1[1],uVar3,*puVar1);
    }
  }
  FUN_0802d5f8();
  FUN_08017864();
  iVar2 = DAT_08017838;
  uVar6 = 0;
  do {
    if (*(int *)(iVar2 + uVar6 * 0x1c) != 0) {
      FUN_0802d6e4();
    }
    FUN_0802d8d6(iVar2 + uVar6 * 0x1c,0x1c);
    uVar6 = uVar6 + 1 & 0xff;
  } while (uVar6 < 4);
  return;
}

