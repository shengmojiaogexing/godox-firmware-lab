// 0x08037a36 FUN_08037a36 size=134

int FUN_08037a36(int param_1,int param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  undefined1 local_40;
  undefined1 local_3f;
  undefined1 local_3e;
  undefined1 local_3d;
  undefined1 local_3c [28];
  
  do {
    if (param_3 == 0) {
      return 0;
    }
    if (param_3 < 0x1c) {
      uVar3 = param_3;
    }
    else {
      uVar3 = 0x1c;
    }
    local_40 = (undefined1)((uint)param_1 >> 0x18);
    local_3f = (undefined1)((uint)param_1 >> 0x10);
    local_3e = (undefined1)((uint)param_1 >> 8);
    local_3d = (undefined1)param_1;
    for (uVar2 = 0; uVar2 < uVar3; uVar2 = uVar2 + 1) {
      local_3c[uVar2] = *(undefined1 *)(param_2 + uVar2);
    }
    iVar4 = -1;
    uVar2 = 0;
    do {
      iVar1 = FUN_0803749c(param_1,&local_40,uVar3 + 4 & 0xffff);
      if (iVar1 == 0) {
        iVar4 = 0;
        break;
      }
      uVar2 = uVar2 + 1;
    } while (uVar2 < 3);
    if (iVar4 != 0) {
      return iVar4;
    }
    param_1 = param_1 + uVar3;
    param_2 = param_2 + uVar3;
    param_3 = param_3 - uVar3 & 0xffff;
  } while( true );
}

