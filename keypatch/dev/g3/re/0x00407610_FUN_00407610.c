// 0x00407610 FUN_00407610 size=100

void __cdecl FUN_00407610(undefined1 param_1,undefined1 *param_2,byte param_3,undefined2 *param_4)

{
  undefined1 *puVar1;
  undefined1 uVar2;
  byte bVar3;
  byte *pbVar4;
  undefined2 *puVar5;
  uint uVar6;
  
  *param_4 = 0xaa00;
  *(undefined1 *)(param_4 + 1) = 0x55;
  *(undefined1 *)((int)param_4 + 3) = param_1;
  if (param_3 != 0) {
    puVar1 = param_2 + param_3;
    puVar5 = param_4 + 2;
    do {
      uVar2 = *param_2;
      param_2 = param_2 + 1;
      *(undefined1 *)puVar5 = uVar2;
      puVar5 = (undefined2 *)((int)puVar5 + 1);
    } while (puVar1 != param_2);
  }
  pbVar4 = (byte *)((int)param_4 + 1);
  uVar6 = 0;
  do {
    bVar3 = *pbVar4;
    pbVar4 = pbVar4 + 1;
    uVar6 = uVar6 + bVar3;
  } while ((byte *)((int)param_4 + 0x3d) != pbVar4);
  *(uint *)((int)param_4 + 0x3d) =
       uVar6 >> 0x18 | (uVar6 & 0xff0000) >> 8 | (uVar6 & 0xff00) << 8 | uVar6 * 0x1000000;
  return;
}

