// 0x08035180 FUN_08035180 size=218

uint FUN_08035180(undefined4 param_1,uint *param_2,uint param_3,undefined4 param_4)

{
  undefined1 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  uint uVar4;
  undefined4 local_18;
  undefined4 uStack_14;
  
  puVar1 = DAT_0803525c;
  DAT_0803525c[6] = DAT_0803525c[6] + '\x01';
  local_18 = param_3;
  uStack_14 = param_4;
  iVar3 = FUN_080376b4(DAT_08035260,&local_18,8);
  if (iVar3 != 0) {
    uVar4 = (uint)(byte)(puVar1[3] + 1);
    puVar1[3] = puVar1[3] + 1;
    if (5 < uVar4) {
      puVar1[3] = 0;
      uVar4 = FUN_080374b0();
    }
    return uVar4;
  }
  uVar4 = local_18 & 0xff;
  if ((uVar4 == 0xff) && (local_18._1_1_ < 3)) {
    if (local_18._1_1_ == 1) {
      puVar1[5] = 0;
      *(undefined1 *)((int)param_2 + 0x12) = 1;
      puVar1[2] = 1;
      puVar2 = DAT_08035264;
      *DAT_08035268 = *DAT_08035264;
      *DAT_0803526c = *puVar2;
      *DAT_08035274 = (uint)*DAT_08035270;
      *DAT_08035278 = 0;
      *puVar1 = 1;
      *DAT_0803527c = 100;
      *param_2 = (uint)local_18._2_1_ | (uStack_14._1_1_ & 0xf) << 8;
      param_2[1] = (uint)CONCAT11(uStack_14._1_1_ >> 4,local_18._3_1_);
    }
    else {
      puVar1[4] = 0;
      if ((byte)puVar1[5] < 3) {
        puVar1[5] = puVar1[5] + 1;
      }
      else {
        *(undefined1 *)((int)param_2 + 0x12) = 0;
        puVar1[2] = 0;
        *puVar1 = 0;
      }
      *param_2 = (uint)local_18._2_1_ | (uStack_14._1_1_ & 0xf) << 8;
      param_2[1] = (uint)CONCAT11(uStack_14._1_1_ >> 4,local_18._3_1_);
    }
    *(uint *)(puVar1 + 0xc) = *param_2;
    uVar4 = param_2[1];
    *(uint *)(puVar1 + 0x10) = uVar4;
  }
  else if (uVar4 == 0xfe) {
    return (uint)local_18._1_1_;
  }
  return uVar4;
}

