#ifndef SRC_CAT_S21_GREP_H_
#define SRC_CAT_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int flge;
  int flgi;
  int flgv;
  int flgc;
  int flgl;
  int flgn;
  int flgh;
  int flgs;
  int flgf;
  int flgo;
} flags;

int flagcheck(int argc, char *argv[], char *pattern, flags *flag);
void s21_grep(int argc, char *argv[], char *pattern, flags *flag);
int patter(char *pattern, char *tmp);
void poscheck(int argc, char *argv[], char *pattern, flags *flag);
void greps(char *file, int file_count, char *pattern, flags *flag);
void output(FILE *fp, regex_t reg, char *file, int file_count, flags *flag);

#endif  // SRC_GREP_OUTPUT_H_
