// 0x0803b1d2 FUN_0803b1d2 size=184

undefined4 FUN_0803b1d2(int param_1)

{
  ushort uVar1;
  ushort uVar2;
  ushort *puVar3;
  
  puVar3 = (ushort *)0x0;
  switch(*(ushort *)(param_1 + 0x2ce) >> 8) {
  default:
    FUN_0803accc(param_1);
    return 0;
  case 1:
    puVar3 = (ushort *)(*(code *)**(undefined4 **)(param_1 + 8))();
    break;
  case 2:
    puVar3 = (ushort *)(**(code **)(*(int *)(param_1 + 8) + 8))();
    break;
  case 3:
    switch((char)*(ushort *)(param_1 + 0x2ce)) {
    case '\0':
      puVar3 = (ushort *)(**(code **)(*(int *)(param_1 + 8) + 0x10))();
      break;
    case '\x01':
      puVar3 = (ushort *)(**(code **)(*(int *)(param_1 + 8) + 0x14))();
      break;
    case '\x02':
      puVar3 = (ushort *)(**(code **)(*(int *)(param_1 + 8) + 0x18))();
      break;
    case '\x03':
      puVar3 = (ushort *)(**(code **)(*(int *)(param_1 + 8) + 0x1c))();
      break;
    case '\x04':
      puVar3 = (ushort *)(**(code **)(*(int *)(param_1 + 8) + 0x24))();
      break;
    case '\x05':
      puVar3 = (ushort *)(**(code **)(*(int *)(param_1 + 8) + 0x20))();
      break;
    default:
      (**(code **)(*(int *)(param_1 + 4) + 8))(param_1,param_1 + 0x2cc);
      return 0;
    }
    break;
  case 6:
    FUN_0803accc(param_1);
    break;
  case 7:
    FUN_0803accc(param_1);
    return 0;
  }
  if (((puVar3 != (ushort *)0x0) && (uVar2 = *puVar3, uVar2 != 0)) &&
     (uVar1 = *(ushort *)(param_1 + 0x2d2), uVar1 != 0)) {
    if (uVar1 <= uVar2) {
      uVar2 = uVar1;
    }
    FUN_0803aca4(param_1,*(undefined4 *)(puVar3 + 2),uVar2);
  }
  return 0;
}

