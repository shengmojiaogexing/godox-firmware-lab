// 0x08020340 FUN_08020340 size=120

void FUN_08020340(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,uint param_6)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  uint local_24;
  
  puVar1 = DAT_080203b8;
  puVar6 = DAT_080203b8 + 1;
  if (param_1 == 0) {
    local_24 = *DAT_080203b8;
  }
  else {
    local_24 = *puVar6;
  }
  uVar2 = FUN_08020648(param_2);
  uVar3 = FUN_08020648(param_3);
  uVar4 = FUN_08020648(param_4);
  uVar5 = FUN_08020648(param_5);
  uVar2 = local_24 & 0xc0808080 | (uVar2 & 0x3f) << 0x18 | (uVar3 & 0x3f) << 0x10 |
          (uVar4 & 0x7f) << 8 | uVar5 & 0x7f | (param_6 & 1) << 0x16;
  FUN_080207a4();
  if (param_1 == 0) {
    *puVar1 = uVar2;
  }
  else {
    *puVar6 = uVar2;
  }
  FUN_080207b4();
  return;
}

