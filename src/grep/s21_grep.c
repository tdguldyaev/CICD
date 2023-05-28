#include "s21_grep.h"

int main(int argc, char *argv[]) {
  s21_grep(argc, argv);
  return 0;
}

void s21_grep(int argc, char *argv[]) {
  if (argc >= 3) {
    flags flag = {0};
    int count_files = 0;
    char pattern[2048] = {0};
    char file_names[2048] = {0};
    parse_flags_and_pattern(argc, argv, &flag, &count_files, file_names,
                            pattern);
    print_files(flag, count_files, pattern, file_names);
  }
}

void parse_flags_and_pattern(int argc, char *argv[], flags *flag,
                             int *count_files, char *file_names,
                             char *pattern) {
  char regex_e[2048] = {0}, file_f[2048] = {0};
  int rez = 0;
  const char *short_options = "e:ivclnhsof:";
  while ((rez = getopt(argc, argv, short_options)) != -1) {
    switch (rez) {
      case 'e':
        flag->e += 1;
        strcat(regex_e, optarg);
        if (flag->e > 0) strcat(regex_e, "\\|");
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag->f = 1;
        strcat(file_f, optarg);
        break;
      case 'o':
        flag->o = 1;
        break;
    }
  }
  if (flag->f || flag->e)
    *count_files = argc - optind;
  else
    *count_files = argc - optind - 1;
  parse_patterns_and_file_names(*flag, argc, argv, *count_files, pattern,
                                file_names, file_f, regex_e, optind);
}

void parse_patterns_and_file_names(flags flag, int argc, char *argv[],
                                   int count_files, char *pattern,
                                   char *file_names, char *file_f,
                                   char *regex_e, int optind) {
  int k = 0, flag_f_e = 0;
  if (flag.f) {
    k = getting_patterns_from_a_file(pattern, file_f);
    if (k != -1)
      flag_f_e = 1;
    else
      printf("Can not open file.\n");
  }
  if (flag.e && flag_f_e == 1) {
    if (k != -2) strcat(pattern, "\\|");
    strcat(pattern, regex_e);
  } else {
    strcat(pattern, regex_e);
  }
  if (!flag.e) {
    if (flag.f && k != -2) strcat(pattern, "\\|");
    strcat(pattern, argv[optind]);
  }
  if (flag.e > 0) pattern[(int)strlen(pattern) - 2] = '\0';
  if (count_files == 0) {
    printf("Cannot open file.\n");
  } else if (count_files == 1) {
    strcat(file_names, argv[argc - 1]);
  } else {
    for (int i = 0; i < count_files; i++) {
      if (flag_f_e || (flag.e && !flag.f))
        strcat(file_names, argv[optind + i]);
      else
        strcat(file_names, argv[optind + 1 + i]);
      strcat(file_names, "|");
    }
  }
  if (count_files == 1) strcat(file_names, "|");
}

void print_files(flags flag, int count_files, char *pattern, char *file_names) {
  char name_of_file[512] = "\0";
  int flag_end = 0;
  separating(file_names, name_of_file, &flag_end);
  while (flag_end != 2) {
    print_for_patterns(flag, pattern, name_of_file, count_files);
    memset(name_of_file, 0, sizeof(name_of_file));
    separating(file_names, name_of_file, &flag_end);
  }
}

void print_for_patterns(flags flag, char *pattern, char *name_of_file,
                        int count_files) {
  FILE *file = NULL;
  int cflags = 0, lno = 0, k = 0, line_matches = 0, line_matches_flag_o = 0;
  size_t len = 0;
  ssize_t read = 0;
  regex_t reg;
  regmatch_t f_match[100];
  while (k != 1) {
    k++;
    file = fopen(name_of_file, "r");
    if (flag.i) cflags = REG_ICASE;
    if (file != NULL) {
      regcomp(&reg, pattern, cflags);
      char *lbuf = NULL;
      int flag_exit = 0;
      while ((read = getline(&lbuf, &len, file)) != -1 && flag_exit == 0) {
        ++lno;
        int match = 0, match_flag_o = 0;
        int success = regexec(&reg, lbuf, 1, f_match, 0);
        if (strchr(lbuf, '\n') == NULL) strcat(lbuf, "\n");
        if (success == 0 && !flag.v) match = 1;
        if (success == REG_NOMATCH && flag.v && !flag.o) match = 1;
        if (success == REG_NOMATCH && flag.v) match_flag_o = 1;
        applications_flags_in_reading(flag, match, lno, name_of_file, lbuf,
                                      pattern, f_match, &line_matches_flag_o,
                                      match_flag_o, count_files);
        if ((!flag.v && !flag.c && !flag.l) && (flag.e + flag.f > 1))
          flag_exit = 1;
        line_matches += match;
      }
      applying_flags_after_reading(flag, name_of_file, count_files,
                                   line_matches_flag_o, line_matches);
      regfree(&reg);
      fclose(file);
      if (lbuf) free(lbuf);
    } else {
      if (!flag.s) printf("Can't open file\n");
    }
    lno = 0;
  }
}

void applications_flags_in_reading(flags flag, int match, int lno,
                                   char *name_of_file, char *lbuf,
                                   char *pattern, regmatch_t *f_match,
                                   int *line_matches_flag_o, int match_flag_o,
                                   int count_files) {
  if (match && count_files > 1 && !flag.h && !flag.c && !flag.l &&
      !(flag.o && flag.c)) {
    if (flag.l)
      printf("%s", name_of_file);
    else
      printf("%s:", name_of_file);
  }
  if (match && flag.n && !flag.l && !flag.c &&
      !((!flag.v && !flag.c && !flag.l) && (flag.e + flag.f > 1)))
    printf("%d:", lno);
  if (match && !flag.c && !flag.l && !flag.o) printf("%s", lbuf);
  if (flag.o) {
    if ((!flag.v && !flag.c && !flag.l) && (flag.e + flag.f > 1)) {
      several_in_a_row_flag_o(flag, name_of_file, pattern, count_files);
    } else if (match) {
      for (; f_match[0].rm_so < f_match[0].rm_eo; f_match[0].rm_so++) {
        if (!flag.c && !flag.l) printf("%c", lbuf[f_match[0].rm_so]);
      }
      *line_matches_flag_o += 1;
      if (!flag.c && !flag.l) printf("\n");
    }
  }
  if (flag.v && flag.o) *line_matches_flag_o += match_flag_o;
}

void applying_flags_after_reading(flags flag, char *name_of_file,
                                  int count_files, int line_matches_flag_o,
                                  int line_matches) {
  if (flag.o)
    standatr_flag_o(flag, name_of_file, count_files, line_matches_flag_o);
  if (flag.c && !flag.l && !flag.o) {
    if (count_files > 1 && !flag.h)
      printf("%s:%d\n", name_of_file, line_matches);
    else
      printf("%d\n", line_matches);
  }
  if (flag.l && line_matches > 0 && !flag.o) printf("%s\n", name_of_file);
}

void several_in_a_row_flag_o(flags flag, char *name_of_file, char *pattern,
                             int count_files) {
  regmatch_t f_match_1[100];
  if ((!flag.v && !flag.c && !flag.l) && (flag.e + flag.f > 1)) {
    FILE *file_1 = NULL;
    regex_t reg_1;
    char *lbuf_1 = NULL, *string = NULL;
    size_t len_1 = 0;
    ssize_t read_1 = 0;
    int match_1 = 0, line_matches_1 = 0, lno_1 = 0, eflag = 0, status = 0;
    file_1 = fopen(name_of_file, "r");
    if (file_1 != NULL) {
      if (flag.i) eflag = REG_ICASE;
      while ((read_1 = getline(&lbuf_1, &len_1, file_1)) != -1) {
        ++lno_1;
        string = lbuf_1;
        status = regcomp(&reg_1, pattern, eflag);
        if (status == 0) match_1 = 1;
        while ((status = regexec(&reg_1, string, 1, f_match_1, 0)) == 0) {
          if (count_files > 1) {
            if (!flag.h) printf("%s:", name_of_file);
            if (flag.n) printf("%d:", lno_1);
          } else {
            if (flag.n) printf("%d:", lno_1);
          }
          char tmp[512] = "\0";
          strcat(tmp, string);
          for (int i = f_match_1[0].rm_so; i < f_match_1[0].rm_eo; i++) {
            printf("%c", tmp[i]);
          }
          printf("\n");
          string += f_match_1[0].rm_eo;
        }
        regfree(&reg_1);
        line_matches_1 += match_1;
      }
      if (lbuf_1) free(lbuf_1);
    } else {
      if (!flag.s) printf("Can't open file\n");
    }
    fclose(file_1);
  }
}

void standatr_flag_o(flags flag, char *name_of_file, int count_files,
                     int line_matches_flag_o) {
  if (count_files > 1) {
    if (flag.c && !flag.l) {
      if (flag.h) {
        printf("%d\n", line_matches_flag_o);
      } else {
        printf("%s:%d\n", name_of_file, line_matches_flag_o);
      }
    }
    if (flag.l && line_matches_flag_o > 0) printf("%s\n", name_of_file);
  } else {
    if (flag.c && !flag.l) {
      printf("%d\n", line_matches_flag_o);
    }
    if (flag.l && line_matches_flag_o > 0) printf("%s\n", name_of_file);
  }
}

void separating(char *str, char *dest, int *flag_end) {
  int sep = 124, pos = 0;
  char *sym_copy = strchr(str, sep);
  if (sym_copy != NULL) pos = (int)(sym_copy - str + 1);
  strncat(dest, str, (size_t)(pos - 1));
  char *tmp = str;
  int i = 0;
  for (i = 0; i < (int)strlen(str) - pos; i++) {
    str[i] = tmp[(int)(pos) + i];
  }
  str[((int)strlen(str) - (pos))] = '\0';
  if ((int)strlen(str) == 0) *flag_end += 1;
}

int getting_patterns_from_a_file(char *pattern, char *file_name) {
  FILE *file;
  int k = 0;
  file = fopen(file_name, "r");
  int i = 0;
  if (file != NULL) {
    int c;
    while ((c = fgetc(file)) != EOF) {
      if (c == 13 || c == 10) {
        pattern[i++] = '\\';
        pattern[i++] = '|';
      }
      if (c != 13 && c != 10) pattern[i++] = c;
      k++;
    }
  } else {
    printf("Can't open file.\n");
    i = -1;
  }
  if (pattern[i - 1] == '|') pattern[i - 1] = '\0';
  if (k == 0) i = -2;
  fclose(file);
  return i;
}
