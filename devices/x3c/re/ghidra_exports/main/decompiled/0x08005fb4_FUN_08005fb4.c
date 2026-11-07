// 0x08005fb4 FUN_08005fb4 size=158

uint FUN_08005fb4(void)

{
  ushort uVar1;
  byte bVar2;
  undefined4 uVar3;
  ushort *puVar4;
  ushort *puVar5;
  int iVar6;
  byte bVar7;
  ushort *puVar8;
  
  iVar6 = DAT_08006060;
  puVar5 = DAT_0800605c;
  puVar4 = DAT_08006058;
  uVar3 = DAT_08006054;
  bVar7 = 0;
  puVar8 = DAT_0800605c + -8;
  do {
    FUN_08018c88(uVar3,1);
    FUN_08018e46(uVar3,1);
    FUN_0801c118(200);
    FUN_0801c31c(2);
    uVar1 = *puVar4;
    bVar2 = *(byte *)(iVar6 + 4);
    puVar5[bVar2] = puVar4[1];
    *(byte *)(iVar6 + 4) = bVar2 + 1;
    puVar8[bVar2] = uVar1;
    bVar7 = bVar7 + 1;
  } while (bVar7 < 8);
  return (((uint)*puVar8 + (uint)puVar5[-7] + (uint)puVar5[-6] + (uint)puVar5[-5] + (uint)puVar5[-4]
           + (uint)puVar5[-3] + (uint)puVar5[-2] + (uint)puVar5[-1] >> 3) * 1000) /
         ((uint)*puVar5 + (uint)puVar5[1] + (uint)puVar5[2] + (uint)puVar5[3] + (uint)puVar5[4] +
          (uint)puVar5[5] + (uint)puVar5[6] + (uint)puVar5[7] >> 3);
}

