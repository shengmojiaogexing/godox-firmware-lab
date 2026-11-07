// 0x0803637c FUN_0803637c size=190

void FUN_0803637c(void)

{
  uint uVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  
  piVar2 = DAT_0803643c;
  *DAT_0803643c = 0;
  piVar3 = (int *)piVar2[1];
  if (*(ushort *)(piVar3 + 0x94) != 0) {
    uVar5 = (uint)*(ushort *)(piVar3 + 0x94);
    do {
      uVar1 = uVar5 - 1;
      uVar6 = 0;
      if ((int)uVar1 < 0) goto LAB_080363a6;
      iVar4 = uVar5 + 0x22f;
      uVar5 = uVar1;
    } while (*(char *)((int)piVar3 + iVar4) != '\0');
    uVar6 = uVar1;
LAB_080363a6:
    if (*(code **)(*piVar3 + 0x40) != (code *)0x0) {
      (**(code **)(*piVar3 + 0x40))();
    }
    piVar3 = (int *)piVar2[1];
    *(uint *)(*(int *)(*piVar3 + 0x18) + 0x18) =
         *(uint *)(*(int *)(*piVar3 + 0x18) + 0x18) & 0xfffffffe;
    *(uint *)(*(int *)(*piVar3 + 0x18) + 0x18) =
         *(uint *)(*(int *)(*piVar3 + 0x18) + 0x18) & 0xfffffffd;
    *(byte *)(piVar3 + 10) = *(byte *)(piVar3 + 10) | 4;
    for (uVar5 = 0; piVar3 = (int *)piVar2[1], (int)uVar5 < (int)(uint)*(ushort *)(piVar3 + 0x94);
        uVar5 = uVar5 + 1) {
      if (*(char *)((int)piVar3 + uVar5 + 0x230) == '\0') {
        if (uVar5 == uVar6) {
          *(uint *)(*(int *)(*piVar3 + 0x18) + 0x18) =
               *(uint *)(*(int *)(*piVar3 + 0x18) + 0x18) | 1;
        }
        *(uint *)(*(int *)(*piVar3 + 0x18) + 0x18) =
             *(uint *)(*(int *)(*piVar3 + 0x18) + 0x18) & 0xfffffffd;
        FUN_080360a0(piVar3 + uVar5 * 4 + 0xc);
        iVar4 = FUN_080274b0(piVar2[1] + uVar5 * 0x10 + 0x30);
        *piVar2 = iVar4 + *piVar2;
      }
    }
    *(byte *)(piVar3 + 10) = *(byte *)(piVar2[1] + 0x28) & 0xfb;
  }
  return;
}

