// 0x08032ad0 FUN_08032ad0 size=194

undefined4 FUN_08032ad0(int *param_1,uint param_2,int *param_3)

{
  uint uVar1;
  int iVar2;
  ushort *puVar3;
  uint uVar4;
  
  uVar1 = (uint)*(ushort *)(param_1 + 1);
  uVar4 = (uint)*(byte *)((int)param_1 + 7);
  if (uVar1 != 0xffff) {
    if (uVar4 == 0) {
      return 0;
    }
    if (uVar4 < 2) {
      if ((uVar1 & 0xffff3fff) == param_2) {
        if ((int)(uVar1 << 0x10) < 0) {
          return 2;
        }
        if ((int)(uVar1 << 0x11) < 0) {
          iVar2 = FUN_08032c68(uVar1 & 0xffff3fff);
          *param_3 = iVar2;
        }
        else {
          *param_3 = *param_1;
        }
        return 1;
      }
    }
    else {
      iVar2 = *param_1 + uVar4 * 4;
      for (uVar1 = 0; uVar1 < uVar4; uVar1 = uVar1 + 1) {
        if ((*(ushort *)(iVar2 + uVar1 * 2) & 0xffff3fff) == param_2) {
          uVar4 = (uint)*(ushort *)(iVar2 + uVar1 * 2);
          if ((int)(uVar4 << 0x10) < 0) {
            return 2;
          }
          if ((int)(uVar4 << 0x11) < 0) {
            iVar2 = FUN_08032c68();
            *param_3 = iVar2;
          }
          else {
            *param_3 = *(int *)(*param_1 + uVar1 * 4);
          }
          return 1;
        }
      }
    }
    return 0;
  }
  uVar1 = 0;
  while( true ) {
    if (uVar4 <= uVar1) {
      return 0;
    }
    puVar3 = (ushort *)(*param_1 + uVar1 * 8);
    if ((*puVar3 & 0xffff3fff) == param_2) break;
    uVar1 = uVar1 + 1;
  }
  if ((int)((uint)*puVar3 << 0x10) < 0) {
    return 2;
  }
  if ((int)((uint)*puVar3 << 0x11) < 0) {
    iVar2 = FUN_08032c68(*puVar3 & 0xffff3fff);
    *param_3 = iVar2;
  }
  else {
    *param_3 = *(int *)(puVar3 + 2);
  }
  return 1;
}

