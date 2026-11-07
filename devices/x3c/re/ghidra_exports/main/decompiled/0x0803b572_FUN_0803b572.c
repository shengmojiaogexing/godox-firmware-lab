// 0x0803b572 FUN_0803b572 size=206

void FUN_0803b572(int *param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = *param_1;
  uVar2 = 0;
  *(uint *)(iVar3 + 0x804) = *(uint *)(iVar3 + 0x804) & 0xfffffffe;
  FUN_0803b0c0(param_1);
  FUN_0803a684(iVar3,0x10);
  do {
    iVar1 = iVar3 + uVar2 * 0x20;
    *(undefined4 *)(iVar1 + 0x908) = 0xff;
    *(undefined4 *)(iVar1 + 0xb08) = 0xff;
    uVar2 = uVar2 + 1;
  } while (uVar2 < 8);
  *(undefined4 *)(iVar3 + 0x818) = 0xffffffff;
  *(undefined4 *)(iVar3 + 0x81c) = 0x10001;
  *(uint *)(iVar3 + 0x814) = *(uint *)(iVar3 + 0x814) | 1;
  *(uint *)(iVar3 + 0x814) = *(uint *)(iVar3 + 0x814) | 2;
  *(uint *)(iVar3 + 0x814) = *(uint *)(iVar3 + 0x814) | 8;
  *(uint *)(iVar3 + 0x810) = *(uint *)(iVar3 + 0x810) | 1;
  *(uint *)(iVar3 + 0x810) = *(uint *)(iVar3 + 0x810) | 2;
  *(uint *)(iVar3 + 0x810) = *(uint *)(iVar3 + 0x810) | 8;
  FUN_0803aef0(param_1,0,0,0x40);
  FUN_0803aef0(param_1,0x80,0,0x40);
  FUN_0803aef0(param_1,1,0,0x40);
  FUN_0803aef0(param_1,0x81,0,0x40);
  FUN_0803a86c(iVar3,0);
  FUN_0803a382(iVar3);
  *(undefined1 *)(param_1 + 0xb9) = 1;
  if (*(code **)(param_1[1] + 0x20) == (code *)0x0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0803b63a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1[1] + 0x20))(param_1,1);
  return;
}

