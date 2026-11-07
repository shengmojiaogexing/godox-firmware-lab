// 0x08032c68 FUN_08032c68 size=228

undefined8 FUN_08032c68(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 local_10;
  undefined2 local_c;
  undefined4 local_8;
  
  local_10 = param_2;
  if (param_1 != 0x45) {
    local_8._2_2_ = (undefined2)((uint)param_4 >> 0x10);
    if (param_1 < 0x46) {
      if (param_1 != 0x30) {
        if (param_1 < 0x31) {
          if (param_1 != 0x22) {
            if (param_1 < 0x23) {
              if ((param_1 == 3) || (param_1 == 6)) {
                local_8 = 0x1fffffff;
                goto LAB_08032cec;
              }
              if (param_1 == 0x20) {
                local_10 = FUN_08027c5c(0xff,0xff);
                local_c = (undefined2)local_10;
                local_8 = CONCAT22(local_8._2_2_,local_c);
                goto LAB_08032cec;
              }
            }
            else {
              if (param_1 == 0x25) {
                local_8 = 0xff;
                goto LAB_08032cec;
              }
              if (param_1 == 0x29) goto LAB_08032d30;
            }
            goto LAB_08032cea;
          }
        }
        else if (param_1 != 0x36) {
          if (param_1 < 0x37) {
            if (param_1 != 0x31) {
              if (param_1 == 0x33) {
                local_8 = 0xf;
                goto LAB_08032cec;
              }
              goto LAB_08032cea;
            }
          }
          else if (param_1 != 0x37) {
            if (param_1 != 0x44) goto LAB_08032cea;
            goto LAB_08032d14;
          }
          goto LAB_08032d30;
        }
      }
      goto LAB_08032d14;
    }
    if (param_1 != 0x53) {
      if (param_1 < 0x54) {
        if (param_1 == 0x4d) {
LAB_08032d14:
          local_10 = FUN_08027c5c(0,0);
          local_c = (undefined2)local_10;
          local_8 = CONCAT22(local_8._2_2_,local_c);
          goto LAB_08032cec;
        }
        if (param_1 < 0x4e) {
          if (param_1 != 0x46) {
            if (param_1 != 0x47) goto LAB_08032cea;
            goto LAB_08032d14;
          }
        }
        else if (param_1 != 0x4e) {
          if (param_1 != 0x52) goto LAB_08032cea;
          goto LAB_08032d14;
        }
      }
      else {
        if (param_1 == 0x57) {
          local_8 = DAT_08032d4c;
          goto LAB_08032cec;
        }
        if (param_1 < 0x58) {
          if (param_1 == 0x55) goto LAB_08032d14;
          if (param_1 != 0x56) goto LAB_08032cea;
        }
        else if (param_1 != 0x60) {
          if (param_1 == 0x6c) {
            local_8 = 0x100;
            goto LAB_08032cec;
          }
LAB_08032cea:
          local_8 = 0;
          goto LAB_08032cec;
        }
      }
    }
  }
LAB_08032d30:
  local_8 = 0xff;
LAB_08032cec:
  return CONCAT44(local_10,local_8);
}

