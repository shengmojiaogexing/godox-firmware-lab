// 0x0803066e FUN_0803066e size=240

void FUN_0803066e(int param_1,int param_2)

{
  bool bVar1;
  bool bVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  undefined4 *puVar7;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined1 auStack_40 [16];
  undefined1 auStack_30 [16];
  
  puVar7 = *(undefined4 **)(param_1 + 8);
  FUN_0802ed04(param_2,auStack_40);
  uVar3 = FUN_080180c2(param_2);
  FUN_080274e4(auStack_40,uVar3,uVar3);
  iVar4 = FUN_08017484(auStack_30,puVar7,auStack_40);
  if ((iVar4 == 0) && (iVar4 = FUN_0802fc9e(param_2,0x80000), iVar4 == 0)) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  if (bVar2) {
    *(undefined1 **)(param_1 + 8) = auStack_30;
    FUN_0802ad74(param_2,0x14,param_1);
    FUN_0802ad74(param_2,0x15,param_1);
    FUN_0802ad74(param_2,0x16,param_1);
  }
  bVar1 = true;
  iVar4 = FUN_0802fc9e(param_2,0x80000);
  if (iVar4 == 0) {
    iVar4 = FUN_08017484(&local_50,puVar7,param_2 + 0x14);
    if (iVar4 == 0) {
      bVar1 = false;
    }
  }
  else {
    local_50 = *puVar7;
    uStack_4c = puVar7[1];
    uStack_48 = puVar7[2];
    uStack_44 = puVar7[3];
  }
  if (bVar1) {
    *(undefined4 **)(param_1 + 8) = &local_50;
    uVar5 = FUN_0802ebf4(param_2);
    for (uVar6 = 0; uVar6 < uVar5; uVar6 = uVar6 + 1) {
      FUN_08036440(param_1,*(undefined4 *)(**(int **)(param_2 + 8) + uVar6 * 4));
    }
  }
  if (bVar2) {
    *(undefined1 **)(param_1 + 8) = auStack_30;
    FUN_0802ad74(param_2,0x17,param_1);
    FUN_0802ad74(param_2,0x18,param_1);
    FUN_0802ad74(param_2,0x19,param_1);
  }
  *(undefined4 **)(param_1 + 8) = puVar7;
  return;
}

