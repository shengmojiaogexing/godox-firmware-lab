// 0x08031ad0 FUN_08031ad0 size=138

int FUN_08031ad0(undefined4 param_1,int param_2,uint param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  uint local_20;
  undefined4 local_1c;
  
  iVar2 = 0;
  local_20 = param_3;
  local_1c = param_4;
  iVar1 = FUN_080174c4(param_1,param_2 + 0x14,0);
  if (iVar1 == 0) {
    iVar2 = 0;
  }
  else {
    iVar1 = FUN_0802fc9e(param_2,1);
    if (iVar1 == 0) {
      iVar1 = FUN_080180d0(param_2);
      if (iVar1 == 0) {
        local_20 = local_20 & 0xffffff00;
        local_1c = param_1;
        FUN_0802ad74(param_2,0x12,&local_20);
        if ((char)local_20 == '\x02') {
          iVar2 = 0;
        }
        else {
          iVar1 = FUN_0802ebf4(param_2);
          do {
            iVar1 = iVar1 + -1;
            if (iVar1 < 0) break;
            iVar2 = FUN_08031ad0(param_1,*(undefined4 *)(**(int **)(param_2 + 8) + iVar1 * 4));
          } while (iVar2 == 0);
          if (iVar2 == 0) {
            if ((char)local_20 == '\0') {
              iVar2 = param_2;
            }
          }
        }
      }
      else {
        iVar2 = 0;
      }
    }
    else {
      iVar2 = 0;
    }
  }
  return iVar2;
}

