// 0x08031a38 FUN_08031a38 size=384

undefined8 FUN_08031a38(void)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int iVar7;
  undefined4 local_20;
  
  FUN_0801c318();
  FUN_0802803c(DAT_08031a78 + -0x1c,DAT_08031a78,0,DAT_08031a74);
  FUN_0802805c(DAT_08031a7c);
  piVar1 = DAT_08031a7c;
  *DAT_08031a7c = 0x170;
  piVar1[1] = 0x1c0;
  piVar1[8] = DAT_08031a80;
  piVar1[6] = DAT_08031a78 + -0x1c;
  puVar2 = (undefined4 *)FUN_08017f8c(DAT_080281fc);
  if (puVar2 == (undefined4 *)0x0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  if (piVar1[0x12] == 0) {
    iVar3 = FUN_0802d5c4(piVar1[0x15]);
    if (iVar3 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    (*(code *)piVar1[0x13])(piVar1,iVar3);
    piVar1[0x12] = iVar3;
  }
  FUN_0802d8d6(puVar2,0x25c);
  *puVar2 = piVar1;
  puVar2[0x95] = 1;
  iVar3 = DAT_08028200;
  iVar7 = *(int *)(DAT_08028200 + 0xc);
  *(undefined4 **)(DAT_08028200 + 0xc) = puVar2;
  iVar4 = FUN_08033500(DAT_08028204,0x1c,puVar2);
  puVar2[1] = iVar4;
  if (iVar4 == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  if ((piVar1[7] << 0x1e < 0) && (*(uint *)(piVar1[6] + 0xc) < (uint)(piVar1[1] * *piVar1))) {
    piVar1[7] = piVar1[7] & 0xfffffffd;
  }
  local_20 = 0xffff;
  *(undefined2 *)((int)puVar2 + 0x2a) = 0xffff;
  *(undefined1 *)((int)puVar2 + 0x29) = 0xff;
  iVar4 = FUN_08033498();
  if (iVar4 == 0) {
    uVar5 = FUN_080318a8(0);
    uVar6 = FUN_080318a8(5);
    local_20 = DAT_0802820c;
    uVar5 = FUN_080333f0(puVar2,uVar6,uVar5,0);
    puVar2[2] = uVar5;
  }
  else {
    uVar5 = FUN_080333dc();
    puVar2[2] = uVar5;
  }
  uVar5 = FUN_0802e40c(0);
  puVar2[4] = uVar5;
  uVar5 = FUN_0802e40c(0);
  puVar2[7] = uVar5;
  uVar5 = FUN_0802e40c(0);
  puVar2[8] = uVar5;
  uVar5 = DAT_08028208;
  FUN_08030e2a(puVar2[7],0,DAT_08028208);
  FUN_08030e2a(puVar2[8],0,uVar5);
  FUN_0802e2d2(puVar2[7],2);
  FUN_0802e2d2(puVar2[8],2);
  FUN_08031384(puVar2[7],0);
  FUN_08031384(puVar2[8],0);
  FUN_080302d0(puVar2[4]);
  *(int *)(iVar3 + 0xc) = iVar7;
  if (iVar7 == 0) {
    *(undefined4 **)(iVar3 + 0xc) = puVar2;
  }
  FUN_080336b2(puVar2[1]);
  return CONCAT44(local_20,puVar2);
}

