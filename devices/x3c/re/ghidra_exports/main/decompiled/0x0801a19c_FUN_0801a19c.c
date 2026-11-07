// 0x0801a19c FUN_0801a19c size=42

undefined8 FUN_0801a19c(int param_1,int *param_2)

{
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  
  local_18 = *param_2 + *(int *)(param_1 + 0x10);
  local_14 = param_2[1] + *(int *)(param_1 + 0x14);
  local_10 = param_2[2] + *(int *)(param_1 + 0x10);
  local_c = param_2[3] + *(int *)(param_1 + 0x14);
  (**(code **)(param_1 + 0x20))(param_1,&local_18);
  return CONCAT44(local_14,local_18);
}

