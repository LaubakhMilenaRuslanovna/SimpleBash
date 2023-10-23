#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int flgb;
  int flge;
  int flgs;
  int flgt;
  int flgE;
  int flgT;
  int flgn;
  int flgv;
} flags;

int flag_ch(int argc, char *argv[], flags *flag);
void s21_cat(int argc, char *argv[], flags *flag);
void fv(int *chr);
void printFile(int argc, char *argv[], flags *flag);

#endif
