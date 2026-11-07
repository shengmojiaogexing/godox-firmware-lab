// 0x0803995c FUN_0803995c size=280

void FUN_0803995c(int param_1,char *param_2,uint param_3)

{
  char cVar1;
  
  cVar1 = *param_2;
  if (cVar1 == '\0') {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffffe;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffffd | ((byte)param_2[1] & 1) << 1
    ;
    *(uint *)(param_1 + 0x20) =
         *(uint *)(param_1 + 0x20) & 0xfffffff7 | ((byte)param_2[1] >> 1 & 1) << 3;
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffffffc | (byte)param_2[2] & 3;
    *(uint *)(param_1 + 0x18) =
         *(uint *)(param_1 + 0x18) & 0xffffff0f | ((byte)param_2[3] & 0xf) << 4;
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffffff3 | (param_3 & 3) << 2;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 1;
    return;
  }
  if (cVar1 == '\x02') {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xffffffef;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xffffffdf | ((byte)param_2[1] & 1) << 5
    ;
    *(uint *)(param_1 + 0x20) =
         *(uint *)(param_1 + 0x20) & 0xffffff7f | ((byte)param_2[1] >> 1 & 1) << 7;
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffffcff | ((byte)param_2[2] & 3) << 8
    ;
    *(uint *)(param_1 + 0x18) =
         *(uint *)(param_1 + 0x18) & 0xffff0fff | ((byte)param_2[3] & 0xf) << 0xc;
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffff3ff | (param_3 & 3) << 10;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 0x10;
    return;
  }
  if (cVar1 == '\x04') {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffeff;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xfffffdff | ((byte)param_2[1] & 1) << 9
    ;
    *(uint *)(param_1 + 0x20) =
         *(uint *)(param_1 + 0x20) & 0xfffff7ff | ((byte)param_2[1] >> 1 & 1) << 0xb;
    *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffffffc | (byte)param_2[2] & 3;
    *(uint *)(param_1 + 0x1c) =
         *(uint *)(param_1 + 0x1c) & 0xffffff0f | ((byte)param_2[3] & 0xf) << 4;
    *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffffff3 | (param_3 & 3) << 2;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 0x100;
    return;
  }
  if (cVar1 == '\x06') {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & 0xffffefff;
    *(uint *)(param_1 + 0x20) =
         *(uint *)(param_1 + 0x20) & 0xffffdfff | ((byte)param_2[1] & 1) << 0xd;
    *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffffcff | ((byte)param_2[2] & 3) << 8
    ;
    *(uint *)(param_1 + 0x1c) =
         *(uint *)(param_1 + 0x1c) & 0xffff0fff | ((byte)param_2[3] & 0xf) << 0xc;
    *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffff3ff | (param_3 & 3) << 10;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | 0x1000;
  }
  return;
}

