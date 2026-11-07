// 0x0802d764 FUN_0802d764 size=280

undefined4 * FUN_0802d764(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  
  uVar4 = (uint)param_1 & 3;
  puVar3 = param_1;
  if (((uint)param_2 & 3) != uVar4) {
    for (; 0x20 < param_3; param_3 = param_3 - 0x20) {
      *(undefined1 *)puVar3 = *(undefined1 *)param_2;
      *(undefined1 *)((int)puVar3 + 1) = *(undefined1 *)((int)param_2 + 1);
      *(undefined1 *)((int)puVar3 + 2) = *(undefined1 *)((int)param_2 + 2);
      *(undefined1 *)((int)puVar3 + 3) = *(undefined1 *)((int)param_2 + 3);
      *(undefined1 *)(puVar3 + 1) = *(undefined1 *)(param_2 + 1);
      *(undefined1 *)((int)puVar3 + 5) = *(undefined1 *)((int)param_2 + 5);
      *(undefined1 *)((int)puVar3 + 6) = *(undefined1 *)((int)param_2 + 6);
      *(undefined1 *)((int)puVar3 + 7) = *(undefined1 *)((int)param_2 + 7);
      *(undefined1 *)(puVar3 + 2) = *(undefined1 *)(param_2 + 2);
      *(undefined1 *)((int)puVar3 + 9) = *(undefined1 *)((int)param_2 + 9);
      *(undefined1 *)((int)puVar3 + 10) = *(undefined1 *)((int)param_2 + 10);
      *(undefined1 *)((int)puVar3 + 0xb) = *(undefined1 *)((int)param_2 + 0xb);
      *(undefined1 *)(puVar3 + 3) = *(undefined1 *)(param_2 + 3);
      *(undefined1 *)((int)puVar3 + 0xd) = *(undefined1 *)((int)param_2 + 0xd);
      *(undefined1 *)((int)puVar3 + 0xe) = *(undefined1 *)((int)param_2 + 0xe);
      *(undefined1 *)((int)puVar3 + 0xf) = *(undefined1 *)((int)param_2 + 0xf);
      *(undefined1 *)(puVar3 + 4) = *(undefined1 *)(param_2 + 4);
      *(undefined1 *)((int)puVar3 + 0x11) = *(undefined1 *)((int)param_2 + 0x11);
      *(undefined1 *)((int)puVar3 + 0x12) = *(undefined1 *)((int)param_2 + 0x12);
      *(undefined1 *)((int)puVar3 + 0x13) = *(undefined1 *)((int)param_2 + 0x13);
      *(undefined1 *)(puVar3 + 5) = *(undefined1 *)(param_2 + 5);
      *(undefined1 *)((int)puVar3 + 0x15) = *(undefined1 *)((int)param_2 + 0x15);
      *(undefined1 *)((int)puVar3 + 0x16) = *(undefined1 *)((int)param_2 + 0x16);
      *(undefined1 *)((int)puVar3 + 0x17) = *(undefined1 *)((int)param_2 + 0x17);
      *(undefined1 *)(puVar3 + 6) = *(undefined1 *)(param_2 + 6);
      *(undefined1 *)((int)puVar3 + 0x19) = *(undefined1 *)((int)param_2 + 0x19);
      *(undefined1 *)((int)puVar3 + 0x1a) = *(undefined1 *)((int)param_2 + 0x1a);
      *(undefined1 *)((int)puVar3 + 0x1b) = *(undefined1 *)((int)param_2 + 0x1b);
      *(undefined1 *)(puVar3 + 7) = *(undefined1 *)(param_2 + 7);
      *(undefined1 *)((int)puVar3 + 0x1d) = *(undefined1 *)((int)param_2 + 0x1d);
      *(undefined1 *)((int)puVar3 + 0x1e) = *(undefined1 *)((int)param_2 + 0x1e);
      puVar2 = (undefined1 *)((int)param_2 + 0x1f);
      param_2 = param_2 + 8;
      *(undefined1 *)((int)puVar3 + 0x1f) = *puVar2;
      puVar3 = puVar3 + 8;
    }
    for (; param_3 != 0; param_3 = param_3 - 1) {
      *(undefined1 *)puVar3 = *(undefined1 *)param_2;
      puVar3 = (undefined4 *)((int)puVar3 + 1);
      param_2 = (undefined4 *)((int)param_2 + 1);
    }
    return param_1;
  }
  if (uVar4 != 0) {
    iVar5 = 4 - uVar4;
    for (; (iVar5 != 0 && (param_3 != 0)); param_3 = param_3 - 1) {
      *(undefined1 *)puVar3 = *(undefined1 *)param_2;
      iVar5 = iVar5 + -1;
      puVar3 = (undefined4 *)((int)puVar3 + 1);
      param_2 = (undefined4 *)((int)param_2 + 1);
    }
  }
  for (; 0x20 < param_3; param_3 = param_3 - 0x20) {
    *puVar3 = *param_2;
    puVar3[1] = param_2[1];
    puVar3[2] = param_2[2];
    puVar3[3] = param_2[3];
    puVar3[4] = param_2[4];
    puVar3[5] = param_2[5];
    puVar3[6] = param_2[6];
    puVar1 = param_2 + 7;
    param_2 = param_2 + 8;
    puVar3[7] = *puVar1;
    puVar3 = puVar3 + 8;
  }
  for (; 4 < param_3; param_3 = param_3 - 4) {
    uVar6 = *param_2;
    param_2 = param_2 + 1;
    *puVar3 = uVar6;
    puVar3 = puVar3 + 1;
  }
  for (; param_3 != 0; param_3 = param_3 - 1) {
    *(undefined1 *)puVar3 = *(undefined1 *)param_2;
    puVar3 = (undefined4 *)((int)puVar3 + 1);
    param_2 = (undefined4 *)((int)param_2 + 1);
  }
  return param_1;
}

