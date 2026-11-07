// 0x080382c8 FUN_080382c8 size=312

void FUN_080382c8(uint *param_1,char *param_2)

{
  char cVar1;
  
  param_1[7] = param_1[7] & 0xfffff7ff;
  cVar1 = *param_2;
  if (cVar1 == '\0') {
    *param_1 = *param_1 & 0xffff7fff;
    *param_1 = *param_1 & 0xffffbfff;
    *param_1 = *param_1 & 0xfffffbff;
  }
  else if (cVar1 == '\x01') {
    *param_1 = *param_1 & 0xffff7fff;
    *param_1 = *param_1 & 0xffffbfff;
    *param_1 = *param_1 | 0x400;
  }
  else if (cVar1 == '\x02') {
    *param_1 = *param_1 | 0x8000;
    *param_1 = *param_1 & 0xffffbfff;
    *param_1 = *param_1 & 0xfffffbff;
  }
  else if (cVar1 == '\x03') {
    *param_1 = *param_1 | 0x8000;
    *param_1 = *param_1 | 0x4000;
    *param_1 = *param_1 & 0xfffffbff;
  }
  *param_1 = *param_1 & 0xfffffdff | ((byte)param_2[7] & 1) << 9;
  if ((param_2[1] == '\x01') && (param_2[7] == '\x01')) {
    *param_1 = *param_1 | 0x100;
  }
  else {
    *param_1 = *param_1 & 0xfffffeff;
  }
  *param_1 = *param_1 & 0xfffffffb | ((byte)param_2[1] & 1) << 2;
  if ((byte)param_2[2] < 8) {
    param_1[1] = param_1[1] & 0xfffffdff;
    param_1[1] = param_1[1] & 0xfffffeff;
    *param_1 = *param_1 & 0xffffffc7 | ((byte)param_2[2] & 7) << 3;
  }
  else if (param_2[2] == 10) {
    param_1[1] = param_1[1] | 0x200;
    param_1[1] = param_1[1] & 0xfffffeff;
    *param_1 = *param_1 & 0xffffffc7;
  }
  else {
    param_1[1] = param_1[1] & 0xfffffdff;
    param_1[1] = param_1[1] | 0x100;
    *param_1 = *param_1 & 0xffffffc7 | ((byte)param_2[2] & 7) << 3;
  }
  *param_1 = *param_1 & 0xffffff7f | ((byte)param_2[3] & 1) << 7;
  *param_1 = *param_1 & 0xfffff7ff | ((byte)param_2[4] & 1) << 0xb;
  *param_1 = *param_1 & 0xfffffffd | ((byte)param_2[5] & 1) << 1;
  *param_1 = *param_1 & 0xfffffffe | (byte)param_2[6] & 1;
  return;
}

