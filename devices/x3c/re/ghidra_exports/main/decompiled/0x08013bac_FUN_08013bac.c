// 0x08013bac FUN_08013bac size=32

void FUN_08013bac(void)

{
  FUN_080068f4();
  FUN_08013b60();
  if (*DAT_08013bcc == '\x01') {
    *DAT_08013bcc = '\0';
    FUN_08013b80();
    return;
  }
  return;
}

