// 0x08021d48 FUN_08021d48 size=266

undefined8 FUN_08021d48(int param_1,code *param_2,code *param_3)

{
  bool bVar1;
  bool bVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int *piVar7;
  
  if ((*(byte *)(param_1 + 0x1c) & 1) == 0) {
    piVar6 = *(int **)(param_1 + 0xc);
    piVar7 = (int *)0x0;
    bVar2 = true;
    do {
      do {
        bVar1 = true;
        if (piVar6 == (int *)0x0) {
          if ((0xffffffffU - ((int)((uint)*(byte *)(param_1 + 0x1c) << 0x1c) >> 0x1f) & (uint)piVar7
              ) != 0) {
            uVar3 = 0;
            goto LAB_08021d5c;
          }
          if (!bVar2) {
            uVar3 = 0;
            goto LAB_08021d5c;
          }
          piVar6 = (int *)(*param_2)(param_1);
          bVar1 = false;
          bVar2 = false;
        }
        if ((piVar7 == (int *)0x0) && (piVar7 = piVar6, piVar6 == (int *)0x0)) {
          uVar3 = 0;
          goto LAB_08021d5c;
        }
        if ((bVar1) && (piVar6 = (int *)(*param_3)(param_1,piVar6), piVar6 == piVar7)) {
          uVar3 = 0;
          goto LAB_08021d5c;
        }
      } while ((piVar6 == (int *)0x0) || (iVar4 = FUN_0802f7b8(*piVar6), iVar4 << 0x18 < 0));
      iVar4 = *piVar6;
      while ((iVar4 != 0 && (iVar5 = FUN_0802fc9e(iVar4,1), iVar5 == 0))) {
        iVar4 = FUN_0802ee0a(iVar4);
      }
    } while ((iVar4 != 0) && (iVar4 = FUN_0802fc9e(iVar4,1), iVar4 != 0));
    if (*(int **)(param_1 + 0xc) == piVar6) {
      uVar3 = 0;
    }
    else {
      if (*(int **)(param_1 + 0xc) != (int *)0x0) {
        uVar3 = FUN_08022204(param_1);
        iVar4 = FUN_0802ad74(**(undefined4 **)(param_1 + 0xc),0xf,uVar3);
        if (iVar4 != 1) {
          uVar3 = 0;
          goto LAB_08021d5c;
        }
        FUN_080302d0(**(undefined4 **)(param_1 + 0xc));
      }
      *(int **)(param_1 + 0xc) = piVar6;
      uVar3 = FUN_08022204(param_1);
      iVar4 = FUN_0802ad74(**(undefined4 **)(param_1 + 0xc),0xe,uVar3);
      if (iVar4 == 1) {
        FUN_080302d0(**(undefined4 **)(param_1 + 0xc));
        if (*(code **)(param_1 + 0x10) != (code *)0x0) {
          (**(code **)(param_1 + 0x10))(param_1);
        }
        uVar3 = 1;
      }
      else {
        uVar3 = 0;
      }
    }
  }
  else {
    uVar3 = 0;
  }
LAB_08021d5c:
  return CONCAT44(param_1,uVar3);
}

