// 0x0803871c FUN_0803871c size=2670

void FUN_0803871c(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined2 uVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  
  iVar2 = DAT_08038b0c;
  iVar6 = DAT_08038b08;
  if ((*(byte *)(DAT_08038b08 + 0x20) & 1) == 0) {
    uVar4 = FUN_08031868(0x12,4,param_3,param_4,param_2,param_3,param_4);
    *(undefined2 *)(iVar2 + 2) = uVar4;
  }
  else {
    uVar4 = FUN_08027b62(DAT_08038b10);
    *(undefined2 *)(iVar2 + 2) = uVar4;
  }
  if ((*(byte *)(iVar6 + 0x20) & 1) == 0) {
    uVar4 = FUN_0803182c(0x12,4);
    *(undefined2 *)(iVar2 + 4) = uVar4;
  }
  else {
    uVar4 = FUN_08031868(0x12,5);
    *(undefined2 *)(iVar2 + 4) = uVar4;
  }
  if ((*(byte *)(iVar6 + 0x20) & 1) == 0) {
    uVar4 = FUN_08027f38();
    *(undefined2 *)(iVar2 + 6) = uVar4;
  }
  else {
    uVar4 = FUN_08027b62(DAT_08038b14);
    *(undefined2 *)(iVar2 + 6) = uVar4;
  }
  if ((*(byte *)(iVar6 + 0x20) & 1) == 0) {
    uVar4 = FUN_08031868(0x12,2);
    *(undefined2 *)(iVar2 + 8) = uVar4;
  }
  else {
    uVar4 = FUN_08027b62(DAT_08038b18);
    *(undefined2 *)(iVar2 + 8) = uVar4;
  }
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0xd0);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0xd8);
  FUN_080331b4(DAT_08038b08 + -0x28,DAT_08038b20,DAT_08038b1c,0x50,0x46,0);
  FUN_080331b4(DAT_08038b08 + -0x14,DAT_08038b20,DAT_08038b1c,0x50,0,0);
  FUN_080331a2(*(int *)(iVar2 + 0xc) + 0xd0,DAT_08038b08 + -0x28);
  FUN_080331a2(*(int *)(iVar2 + 0xc) + 0xd8,DAT_08038b08 + -0x14);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 8);
  if ((*(byte *)(iVar6 + 0x20) & 1) == 0) {
    uVar5 = FUN_080318a8(0x12);
  }
  else {
    uVar5 = FUN_0803182c(0x12,2);
  }
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 8,uVar5);
  FUN_08033136(*(int *)(iVar2 + 0xc) + 8,0x7fff);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),7);
  FUN_08032f9e(*(int *)(iVar2 + 0xc) + 8,uVar5);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),5);
  FUN_080331aa(*(int *)(iVar2 + 0xc) + 8,uVar5);
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 8,0x66);
  FUN_080331a2(*(int *)(iVar2 + 0xc) + 8,DAT_08038b08 + -0x14);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x10);
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 0x10,0xff);
  FUN_080391b4(*(undefined4 *)(iVar2 + 0xc));
  FUN_08032f0c(*(undefined4 *)(iVar2 + 0xc),0xff);
  FUN_08032ef6(*(undefined4 *)(iVar2 + 0xc),*(undefined2 *)(iVar2 + 2));
  FUN_08033174(*(undefined4 *)(iVar2 + 0xc),*(undefined2 *)(iVar2 + 4));
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xe);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xc);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),10);
  }
  FUN_08032fe8(*(undefined4 *)(iVar2 + 0xc),uVar5);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xe);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xc);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),10);
  }
  FUN_08032fd0(*(undefined4 *)(iVar2 + 0xc),uVar5);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x18);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xc);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),8);
  }
  FUN_08033136(*(int *)(iVar2 + 0xc) + 0x18,uVar5);
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 0x18,0xff);
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 0x18,*(undefined2 *)(iVar2 + 6));
  FUN_08032f14(*(int *)(iVar2 + 0xc) + 0x18,*(undefined2 *)(iVar2 + 8));
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),2);
  FUN_08032f32(*(int *)(iVar2 + 0xc) + 0x18,uVar5);
  FUN_08032f2a(*(int *)(iVar2 + 0xc) + 0x18,1);
  FUN_08033174(*(int *)(iVar2 + 0xc) + 0x18,*(undefined2 *)(iVar2 + 4));
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x18);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x14);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x10);
  }
  FUN_08032f9e(*(int *)(iVar2 + 0xc) + 0x18,uVar5);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xe);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xc);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),10);
  }
  FUN_08032fe8(*(int *)(iVar2 + 0xc) + 0x18,uVar5);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xe);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xc);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),10);
  }
  FUN_08032fd0(*(int *)(iVar2 + 0xc) + 0x18,uVar5);
  uVar5 = FUN_080318a8(0x12);
  FUN_08032f52(*(int *)(iVar2 + 0xc) + 0x18,uVar5);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),1);
  FUN_08032f68(*(int *)(iVar2 + 0xc) + 0x18,uVar5);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0xa0);
  iVar1 = DAT_08038b08;
  FUN_08032f70(*(int *)(iVar2 + 0xc) + 0xa0,*(undefined4 *)(DAT_08038b08 + 0x10));
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),3);
  FUN_08032f96(*(int *)(iVar2 + 0xc) + 0xa0,uVar5);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),3);
  FUN_08032f8e(*(int *)(iVar2 + 0xc) + 0xa0,uVar5);
  FUN_08032f86(*(int *)(iVar2 + 0xc) + 0xa0,0x7f);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0xa8);
  FUN_08032f70(*(int *)(iVar2 + 0xc) + 0xa8,*(undefined2 *)(iVar1 + 0x12));
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),3);
  FUN_08032f96(*(int *)(iVar2 + 0xc) + 0xa8,uVar5);
  FUN_08032f86(*(int *)(iVar2 + 0xc) + 0xa8,0x7f);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x20);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x10);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xc);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),8);
  }
  FUN_08033136(*(int *)(iVar2 + 0xc) + 0x20,uVar5);
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 0x20,0xff);
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 0x20,*(undefined2 *)(iVar2 + 8));
  if ((*(byte *)(iVar6 + 0x20) & 1) == 0) {
    uVar5 = FUN_080318a8(0x12);
    FUN_0803313e(*(int *)(iVar2 + 0xc) + 0x20,uVar5);
    iVar7 = FUN_08028228(0);
    if (iVar7 * 3 + 0x50 < 0x140) {
      iVar7 = 1;
    }
    else {
      iVar7 = FUN_08028228(0);
      iVar7 = (iVar7 * 3 + 0x50) / 0xa0;
    }
    FUN_08033164(*(int *)(iVar2 + 0xc) + 0x20,iVar7);
    FUN_0803315c(*(int *)(iVar2 + 0xc) + 0x20,0x7f);
    iVar7 = FUN_08028228(0);
    if (iVar7 * 4 + 0x50 < 0x140) {
      iVar7 = 1;
    }
    else {
      iVar7 = FUN_08028228(0);
      iVar7 = (iVar7 * 4 + 0x50) / 0xa0;
    }
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),iVar7);
    FUN_08033154(*(int *)(iVar2 + 0xc) + 0x20,uVar5);
  }
  FUN_08033174(*(int *)(iVar2 + 0xc) + 0x20,*(undefined2 *)(iVar2 + 4));
  iVar7 = *(int *)(iVar2 + 0xc) + 0x20;
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x18);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x14);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x10);
  }
  FUN_08032fd8(iVar7,uVar5);
  FUN_08032fe0(iVar7,uVar5);
  iVar7 = *(int *)(iVar2 + 0xc) + 0x20;
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xe);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xc);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),10);
  }
  FUN_08032ff0(iVar7,uVar5);
  FUN_08032fc8(iVar7,uVar5);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),5);
  FUN_08032fd0(*(int *)(iVar2 + 0xc) + 0x20,uVar5);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),5);
  FUN_08032fe8(*(int *)(iVar2 + 0xc) + 0x20,uVar5);
  puVar3 = DAT_08039060;
  *DAT_08039060 = DAT_0803905c;
  puVar3[2] = DAT_08039064;
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x58);
  FUN_08032f42(*(int *)(iVar2 + 0xc) + 0x58,DAT_08039060);
  FUN_08032f4a(*(int *)(iVar2 + 0xc) + 0x58,0x23);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x60);
  FUN_08032f42(*(int *)(iVar2 + 0xc) + 0x60,DAT_08039060 + 2);
  FUN_08032f4a(*(int *)(iVar2 + 0xc) + 0x60,0x7f);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0xc0);
  FUN_08032f3a(*(int *)(iVar2 + 0xc) + 0xc0,1);
  FUN_08032f2a(*(int *)(iVar2 + 0xc) + 0xc0,1);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x80);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x18);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x14);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x10);
  }
  FUN_08032f9e(*(int *)(iVar2 + 0xc) + 0x80,uVar5);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x18);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x14);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x10);
  }
  FUN_08032fe8(*(int *)(iVar2 + 0xc) + 0x80,uVar5);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x18);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x14);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x10);
  }
  FUN_08032fd0(*(int *)(iVar2 + 0xc) + 0x80,uVar5);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x78);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xe);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xc);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),10);
  }
  FUN_08032f9e(*(int *)(iVar2 + 0xc) + 0x78,uVar5);
  iVar7 = *(int *)(iVar2 + 0xc) + 0x78;
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xe);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xc);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),10);
  }
  FUN_08032fe8(iVar7,uVar5);
  FUN_08032fd0(iVar7,uVar5);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x88);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),10);
  FUN_08032fe8(*(int *)(iVar2 + 0xc) + 0x88,uVar5);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),10);
  FUN_08032fd0(*(int *)(iVar2 + 0xc) + 0x88,uVar5);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x90);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0x14);
  FUN_0803318a(*(int *)(iVar2 + 0xc) + 0x90,uVar5);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x98);
  FUN_0803316c(*(int *)(iVar2 + 0xc) + 0x98,2);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x68);
  FUN_08032f9e(*(int *)(iVar2 + 0xc) + 0x68,0);
  FUN_08032fe8(*(int *)(iVar2 + 0xc) + 0x68,0);
  FUN_08032fd0(*(int *)(iVar2 + 0xc) + 0x68,0);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x70);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),8);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),6);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),2);
  }
  FUN_08032f9e(*(int *)(iVar2 + 0xc) + 0x70,uVar5);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),8);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),6);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),2);
  }
  FUN_08032fe8(*(int *)(iVar2 + 0xc) + 0x70,uVar5);
  if (*(char *)(iVar2 + 1) == '\x01') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),8);
  }
  else if (*(char *)(iVar2 + 1) == '\x02') {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),6);
  }
  else {
    uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),2);
  }
  FUN_08032fd0(*(int *)(iVar2 + 0xc) + 0x70,uVar5);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x28);
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 0x28,*(undefined4 *)(iVar1 + 0x10));
  uVar5 = FUN_08027f38();
  FUN_08033174(*(int *)(iVar2 + 0xc) + 0x28,uVar5);
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 0x28,0xff);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x30);
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 0x30,*(undefined4 *)(iVar1 + 0x10));
  FUN_08033174(*(int *)(iVar2 + 0xc) + 0x30,*(undefined4 *)(iVar1 + 0x10));
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 0x30,0x33);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x38);
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 0x38,*(undefined2 *)(iVar1 + 0x12));
  uVar5 = FUN_08027f38();
  FUN_08033174(*(int *)(iVar2 + 0xc) + 0x38,uVar5);
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 0x38,0xff);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x40);
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 0x40,*(undefined2 *)(iVar1 + 0x12));
  FUN_08033174(*(int *)(iVar2 + 0xc) + 0x40,*(undefined2 *)(iVar1 + 0x12));
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 0x40,0x33);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x48);
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 0x48,*(undefined2 *)(iVar2 + 8));
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 0x48,0xff);
  FUN_08033174(*(int *)(iVar2 + 0xc) + 0x48,*(undefined2 *)(iVar2 + 4));
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x50);
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 0x50,*(undefined2 *)(iVar2 + 6));
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 0x50,0xff);
  FUN_08033174(*(int *)(iVar2 + 0xc) + 0x50,*(undefined2 *)(iVar2 + 4));
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0xb0);
  FUN_08033136(*(int *)(iVar2 + 0xc) + 0xb0,0x7fff);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0xb8);
  FUN_08033136(*(int *)(iVar2 + 0xc) + 0xb8,0);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 200);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),3);
  FUN_0803319a(*(int *)(iVar2 + 0xc) + 200,uVar5);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),3);
  FUN_08033192(*(int *)(iVar2 + 0xc) + 200,uVar5);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0xf0);
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 0xf0,*(undefined4 *)(iVar1 + 0x10));
  FUN_08032f0c(*(int *)(iVar2 + 0xc) + 0xf0,0xff);
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),6);
  FUN_08032f9e(*(int *)(iVar2 + 0xc) + 0xf0,uVar5);
  FUN_08033136(*(int *)(iVar2 + 0xc) + 0xf0,0x7fff);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0xe0);
  FUN_08032ec8(*(int *)(iVar2 + 0xc) + 0xe0,200);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0xe8);
  FUN_08032ec8(*(int *)(iVar2 + 0xc) + 0xe8,0x78);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x100);
  FUN_08032ed0(*(int *)(iVar2 + 0xc) + 0x100,*(undefined2 *)(iVar2 + 8));
  uVar5 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),0xf);
  FUN_08032eee(*(int *)(iVar2 + 0xc) + 0x100,uVar5);
  FUN_08032ee6(*(int *)(iVar2 + 0xc) + 0x100,1);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x108);
  FUN_08032ed0(*(int *)(iVar2 + 0xc) + 0x108,*(undefined4 *)(iVar1 + 0x10));
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x110);
  iVar6 = FUN_08028008(*(undefined4 *)(iVar6 + 0xc),4);
  FUN_08032f9e(*(int *)(iVar2 + 0xc) + 0x110,-iVar6);
  uVar5 = FUN_08027f38();
  FUN_08032ef6(*(int *)(iVar2 + 0xc) + 0x110,uVar5);
  FUN_080391b4(*(int *)(iVar2 + 0xc) + 0x118);
  FUN_08032f68(*(int *)(iVar2 + 0xc) + 0x118,1);
  FUN_08032f52(*(int *)(iVar2 + 0xc) + 0x118,*(undefined2 *)(iVar2 + 4));
  return;
}

