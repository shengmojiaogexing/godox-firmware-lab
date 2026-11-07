// 0x0802b448 FUN_0802b448 size=156

longlong FUN_0802b448(int param_1,uint param_2,uint param_3,undefined4 param_4)

{
  char *pcVar1;
  int iVar2;
  uint uVar3;
  uint local_20;
  uint local_1c;
  undefined4 local_18;
  
  pcVar1 = DAT_0802b4e4;
  if ((*(byte *)(param_1 + 9) & 7) != 0) {
    local_1c = param_3;
    local_18 = param_4;
    if (*DAT_0802b4e4 == '\0') {
      FUN_0802b4ec(0);
      *pcVar1 = '\x01';
    }
    if ((*(byte *)(param_1 + 9) & 7) == 2) {
      uVar3 = param_2;
    }
    else {
      uVar3 = param_3;
    }
    local_18 = FUN_0801b5bc(param_1,uVar3,param_2);
    local_1c = 0;
    iVar2 = FUN_080254c0(DAT_0802b4e8,&local_18,&local_1c);
    if (iVar2 == 1) {
      uVar3 = *(uint *)(local_1c + 4) >> 0x1e;
      *(uint *)(local_1c + 4) = (uVar3 | *(uint *)(local_1c + 4) << 2) + 4 >> 2 | uVar3 << 0x1e;
      return CONCAT44(param_2,local_1c);
    }
    local_1c = FUN_08018ea8(param_1,param_2,param_3);
    if (local_1c != 0) {
      local_20 = param_2;
      for (iVar2 = 0; iVar2 < *(int *)(local_1c + 0x10); iVar2 = iVar2 + 1) {
        local_20 = FUN_0802b340(param_1,*(int *)(local_1c + 0x10),iVar2);
        *(undefined2 *)(*(int *)(local_1c + 8) + iVar2 * 2) = (undefined2)local_20;
      }
      return CONCAT44(local_20,local_1c);
    }
  }
  return (ulonglong)param_2 << 0x20;
}

