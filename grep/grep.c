#include "s21_grep.h"

//  -e | Шаблон
//  -i | Игнорирует различия регистра.
//  -v | Инвертирует смысл поиска соответствий.
//  -c | Выводит только количество совпадающих строк.
//  -l | Выводит только совпадающие файлы.
//  -n | Предваряет каждую строку вывода номером строки из файла ввода.
//  -h | Выводит совпадающие строки, не предваряя их именами файлов.
//  -s | Подавляет сообщения об ошибках о несуществующих или нечитаемых файлах.
//  -f file | Получает регулярные выражения из файла.
//  -o | Печатает только совпадающие (непустые) части совпавшей строки.

int main(int argc, char *argv[]) {
  int res;
  char pattern[1024];
  flags flag = {0};
  res = flagcheck(argc, argv, pattern, &flag);
  if (argc >= 3 && res != 1) {
    s21_grep(argc, argv, pattern, &flag);
  }
  return res;
}

int flagcheck(int argc, char *argv[], char *pattern, flags *flag) {
  int i = 0, result = 0;
  while ((i = getopt_long(argc, argv, "e:ivclnhsof:", NULL, NULL)) != -1) {
    switch (i) {
      case 'e':
        flag->flge = 1;
        sprintf(pattern, "%s", optarg);
        break;
      case 'i':
        flag->flgi = 1;
        break;  //-> указывает на элемент структуры
      case 'v':
        flag->flgv = 1;
        break;
      case 'c':
        flag->flgc = 1;
        break;
      case 'l':
        flag->flgl = 1;
        break;
      case 'n':
        flag->flgn = 1;
        break;
      case 'h':
        flag->flgh = 1;
        break;
      case 's':
        flag->flgs = 1;
        break;
      case 'f':
        flag->flgf = 1;
        sprintf(pattern, "%s", optarg);
        break;
      case 'o':
        flag->flgo = 1;
        break;
      default:
        result = 1;
        if (!flag->flgs) {
          printf("illegal option --");
        }
        break;
    }
  }
  return result;
}

int patter(char *pattern, char *tmp) {
  FILE *fp = fopen(tmp, "r"); /* чтение файла*/
  int error = 0;
  if (fp != NULL) {
    error = 0;
    int simvol = 0, i = 0;
    while ((simvol = fgetc(fp)) != EOF) {
      if (simvol == '\n' || simvol == 13) {
        pattern[i++] = '|';
      }
      if (simvol != '\n' && simvol != 13) {
        pattern[i++] = simvol;
      }
    }
    if (pattern[i--] == '|') {
      pattern[i--] = '\0';
    }
    fclose(fp);
  } else {
    error = -1;
    fprintf(stderr, "ERROR");
  }
  return error;
}

void poscheck(int argc, char *argv[], char *pattern, flags *flag) {
  char patL[1024] = {0};
  int file_count = 0;
  int columns = 0;

  if (!flag->flgf && !flag->flge) {
    sprintf(patL, "%s", argv[optind++]);
  } else if (flag->flgf) {
    columns = patter(patL, pattern);
  } else if (!flag->flgf && flag->flge) {
    sprintf(patL, "%s", pattern);
  }
  if (columns != -1) {
    if (argc - optind > 1) {
      file_count = 1;
    }
    for (int i = optind; i < argc; i++) {
      greps(argv[i], file_count, patL, flag);
    }
  }
}

void s21_grep(int argc, char *argv[], char *pattern, flags *flag) {
  int legal = 1;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      legal = 0;
    }
    if (legal == 0 && argv[i][0] == '-') {
      legal = -1;
    }
  }
  if (legal == -1) {
    fprintf(stderr, "Error...\n");
  } else {
    poscheck(argc, argv, pattern, flag);
  }
}

void greps(char *file, int file_count, char *pattern, flags *flag) {
  int sflg = (flag->flgi == 1) ? REG_ICASE | REG_EXTENDED : REG_EXTENDED;
  regex_t reg;
  FILE *fp = fopen(file, "r");
  if (fp != NULL) {
    regcomp(&reg, pattern,
            sflg);  //компилирует наши ФЛАГИ И ПАТЕРНЫ для regexec
    output(fp, reg, file, file_count, flag);
    regfree(&reg);
    fclose(fp);
  } else if (!flag->flgs) {
    printf("%s: No such file or directory\n", file);
  }
}

void output(FILE *fp, regex_t reg, char *file, int file_count, flags *flag) {
  char text[1024];
  regmatch_t pmatch[1];  //  функция regexec который ищет совпадения в match
  int line_matchs = 0, nline = 1, status = 0;

  while (fgets(text, 1024, fp) != NULL) {
    status = regexec(&reg, text, 1, pmatch, 0);
    int match = 0;

    if (file_count && status == 0 && !flag->flgl && !flag->flgc &&
        !flag->flgh) {
      printf("%s:", file);
    }
    if ((status == 0 && !flag->flgv) || (status == REG_NOMATCH && flag->flgv)) {
      match = 1;
    }
    if (match && !flag->flgl && !flag->flgc && flag->flgn) {
      printf("%d:", nline);
    }
    if (match && !flag->flgl && !flag->flgc && !flag->flgo) {
      printf("%s", text);
      if (strrchr(text, '\n') == NULL) {
        printf("\n");
      }
    }
    if (match && flag->flgo) {
      for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
        printf("%c", text[i]);
      }
      printf("\n");
    }

    nline++;
    line_matchs += match;
  }

  if (flag->flgl && line_matchs > 0 && !flag->flgh) {
    printf("%s\n", file);  // -l | Выводит только совпадающие файлы.
  }
  if (flag->flgc && !flag->flgl &&
      file_count) {  // -c | Выводит только количество совпадающих строк.
    printf("%s:", file);
    printf("%d\n", line_matchs);
  } else if (flag->flgc && !flag->flgl) {
    printf("%d\n", line_matchs);
  }
}
