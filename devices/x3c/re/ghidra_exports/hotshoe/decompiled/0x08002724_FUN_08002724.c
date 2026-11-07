// 0x08002724 FUN_08002724 size=224

void FUN_08002724(void)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  int iVar4;
  undefined1 *puVar5;
  undefined2 *puVar6;
  uint uVar7;
  
  *DAT_08002804 = 0x11;
  *DAT_08002808 = 0x38;
  *DAT_0800280c = 5;
  *DAT_08002810 = 5;
  *DAT_08002814 = 0x14;
  *DAT_08002818 = 0;
  puVar1 = DAT_08002824;
  *DAT_0800281c = 0;
  *DAT_08002820 = 0;
  puVar3 = DAT_0800282c;
  puVar2 = DAT_08002828;
  uVar7 = 0;
  do {
    *puVar1 = 1;
    while( true ) {
      puVar2[uVar7] = 0x38;
      puVar3[uVar7] = 0;
      iVar4 = DAT_08002830;
      uVar7 = uVar7 + 1 & 0xff;
      if (4 < uVar7) {
        uVar7 = 0;
        do {
          *(undefined1 *)(iVar4 + uVar7) = 0;
          puVar5 = DAT_08002840;
          uVar7 = uVar7 + 1 & 0xff;
        } while (uVar7 < 0x10);
        *DAT_08002834 = 0;
        *DAT_08002838 = 0;
        *DAT_08002804 = 0;
        *DAT_0800283c = 1;
        *puVar5 = 0x48;
        *DAT_08002844 = 0x28;
        *DAT_08002848 = 0x6d;
        *DAT_0800284c = 1;
        *DAT_08002808 = 0x50;
        *DAT_0800280c = 5;
        *DAT_08002810 = 5;
        *DAT_08002850 = 5;
        *DAT_08002854 = 0;
        *puVar1 = 1;
        *puVar2 = 0x46;
        *puVar3 = 0;
        puVar1[1] = 6;
        puVar2[1] = 0x46;
        puVar3[1] = 0;
        puVar1[2] = 6;
        puVar2[2] = 0x46;
        puVar3[2] = 0;
        puVar1[3] = 6;
        puVar2[3] = 0x46;
        puVar3[3] = 0;
        puVar1[4] = 6;
        puVar2[4] = 0x46;
        puVar6 = DAT_08002858;
        puVar3[4] = 0;
        *puVar6 = 5;
        puVar1 = DAT_08002868;
        *DAT_0800285c = 0;
        *DAT_08002860 = 0;
        *DAT_08002864 = 0;
        *puVar1 = 0xff;
        *(undefined1 *)(DAT_0800286c + 7) = 0;
        *DAT_08002870 = 0;
        *DAT_08002874 = 0;
        return;
      }
      if (uVar7 == 0) break;
      puVar1[uVar7] = 6;
    }
  } while( true );
}

