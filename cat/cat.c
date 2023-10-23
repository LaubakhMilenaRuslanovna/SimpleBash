#include "s21_cat.h"

int main(int argc, char *argv[]) {
  flags flag = {0};  //Зануляем нашу структуру
  if (argc > 1) {
    if (!flag_ch(argc, argv, &flag)) { /*Перенапрвляем на на наш flag*/
      if (flag.flgb == 1 &&
          flag.flgn == 1) {  //Если b и n равно 1, то делаем n = 0, чтобы флаги
                             //работали верно,преимущество над b
        flag.flgn = 0;
      }
      if (flag.flgt == 1 || flag.flge == 1) flag.flgv = 1;
      s21_cat(argc, argv, &flag);
    }
  } else {
    printf("not arguments");
  }
  return 1;
}

int flag_ch(int argc, char *argv[], flags *flag) {
  int error = 0;
  for (int i = 1; i < argc; i++) {
    int j = 1;
    if (argv[i][0] == '-' &&
        argv[i][1] != '-') { /* Если аргумент,который мы получили и его первый
           символ равен тире,то это означает что это опция.
           воторой символ должен быть равен флагу*/
      while (argv[i][j] != '\0') {
        switch (argv[i][j]) {
          case 'b':
            flag->flgb = 1;  //Нумерует все непустые строки
            break;
          case 'e':
            flag->flge = 1;  // Ставит знак $ вместо символа конца строки
            break;
          case 's':
            flag->flgs = 1;  //Сжимает пустые строки
            break;
          case 't':
            flag->flgt = 1;  //Отображает табы и печатает ^I
            break;
          case 'E':
            flag->flgE = 1;  // также отображает символы конца строки как $
            break;
          case 'T':
            flag->flgT = 1;  //Отображает табы и печатает ^I
            break;
          case 'n':
            flag->flgn = 1;  //Печатает все входящие строки
            break;
          case 'v':
            flag->flgv = 1;  //Показывает скрытые символы
            break;
          default:
            fprintf(stderr,
                    "illegal option cat--\n");  // вывод стандартной ошибки
            error = 1;
            break;
        }
        j++;
      }
    } else if (argv[i][0] == '-' && argv[i][1] == '-' && argv[i][2] != '-') {
      if (strcmp(&argv[i][0], " --number") == 0)
        flag->flgn = 1; /* namber тоже самое что и flgn*/
      if (strcmp(&argv[i][0], " --number-nonblank") == 0) flag->flgb = 1;
      /* namber-nonblank тоже самое что flgb*/ /* strcmp() сравнивает две
                                                 строки*/
      if (strcmp(&argv[i][0], " --squeeze-blank") == 0)
        flag->flgs = 1; /* squeeze-blank тоже самое чо flgs*/
    } else if (argv[j][0] == '-' && argv[j][1] == '-' && argv[j][2] == '-') {
      fprintf(stderr, "illegal option gnu--\n");
      error = 1;
    }
  }
  return error;
}

void printFile(int argc, char *argv[], flags *flag) {
  FILE *fp;
  char lin = '\n'; /*  новая линяя*/
  int simvol;
  unsigned int buff_str =
      0; /* Объединеет 2 пустые строчки-счетчик пустых строчек*/
  for (int i = 1; i < argc; i++)
    if (argv[i][0] != '-') {
      fp = fopen(argv[i], "r"); /* чтение файла*/
      if (fp == NULL) {
        fprintf(stderr, "%s: No such file  or directory\n",
                argv[i]); /*печатеает из файла(ошибка)*/
      } else {
        int i = 1;
        while ((simvol = fgetc(fp)) != EOF) {
          if ((flag->flgs == 1 && simvol == '\n') &&
              (lin == '\n' ||
               lin == '\0')) { /* Сжимает несколько смежных пустых строк*/
            buff_str++;
            if (buff_str > 1) continue;
          } else {
            buff_str = 0;
          }
          if ((flag->flgb == 1 && simvol != '\n' && lin == '\n') ||
              (flag->flgn == 1 && lin == '\n' &&
               flag->flgb != 1)) { /* -b нумерует только непустые строки/ -n
                                нумерует все выходные строки*/
            printf("%6d\t", i++);
          }
          if (((flag->flge == 1) || (flag->flgE == 1)) && simvol == '\n') {
            putchar('$');  // водит символ $ в конце строки
          }
          if (flag->flgv == 1 && simvol != '\n') {  //Выводит тайные символы
            fv(&simvol);
          }
          if (((flag->flgt == 1) || (flag->flgT == 1)) &&
              simvol == '\t') {  //отображает табы как ^I
            putchar('^');
            simvol = 'I';
          }
          lin = simvol;
          putchar(simvol);
        }
        fclose(fp);
      }
    }
}

void s21_cat(int argc, char *argv[], flags *flag) {
  int legal = 1;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      legal = 0;
    }
    if (legal == 0 && argv[i][0] == '-') {
      legal = 2;
    }
  }
  if (legal == 2) {
    flag->flgb = 0;
    flag->flge = 0;
    flag->flgs = 0;
    flag->flgt = 0;
    flag->flgE = 0;
    flag->flgT = 0;
    flag->flgn = 0;
    flag->flgv = 0;
  }
  printFile(argc, argv, flag);
}

void fv(int *chr) {
  if ((*chr <= 8) || (*chr >= 11 && *chr <= 31)) {
    *chr += 64;
    putchar('^');
  } else if ((*chr == 127)) {
    *chr -= 64;
    putchar('^');
  }
}
