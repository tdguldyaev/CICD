#include "s21_cat.h"

int main(int argc, char *argv[]) {
  s21_cat(argc, argv);
  return 0;
}

void s21_cat(int argc, char *argv[]) {
  if (argc >= 2) {
    flags f;
    f.b = 0;
    f.e = 0;
    f.n = 0;
    f.s = 0;
    f.t = 0;
    f.v = 0;
    f.not_a_v = 0;
    int file_counter = 0, file_arr[argc];
    parse_flags(argc, argv, &f, file_arr, &file_counter);
    proc_files(argc, argv, file_arr, f);
  }
}

void parse_flags(int argc, char *argv[], flags *f, int *file_arr,
                 int *file_counter) {
  int i;
  int flag_E = 0, flag_T = 0;
  for (i = 1; i < argc; i++) {
    file_arr[i] = 0;
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-b") == 0 ||
          strcmp(argv[i], "--number-nonblank") == 0) {
        f->b = 1;
      } else if (strcmp(argv[i], "-e") == 0) {
        f->e = 1;
        f->v = 1;
      } else if (strcmp(argv[i], "-E") == 0) {
        f->e = 1;
        flag_E++;
      } else if (strcmp(argv[i], "-n") == 0 ||
                 strcmp(argv[i], "--number") == 0) {
        f->n = 1;
      } else if (strcmp(argv[i], "-s") == 0 ||
                 strcmp(argv[i], "--squeeze-blank") == 0) {
        f->s = 1;
      } else if (strcmp(argv[i], "-t") == 0) {
        f->t = 1;
        f->v = 1;
      } else if (strcmp(argv[i], "-T") == 0) {
        f->t = 1;
        flag_T++;
      } else if (strcmp(argv[i], "-v") == 0) {
        f->v = 1;
      }
    } else {
      *file_counter += 1;
      file_arr[i] = 1;
    }
  }
  if ((flag_E == 1 && flag_T == 1) || (flag_E == 1 && f->t == 0) ||
      (flag_T == 1 && f->e == 0))
    f->not_a_v = 1;
}

void proc_files(int argc, char *argv[], int *file_arr, flags f) {
  FILE *file;
  int num_line = 1;
  for (int i = 1; i < argc; i++) {
    if (file_arr[i]) {  //  if file_arr == 1: it is file
      file = fopen(argv[i], "r");
      if (file == NULL) {
        printf("cat: %s: No such file or directory\n", argv[i]);
      } else {
        fclose(file);
        print_file(argv[i], f, &num_line);
      }
    }
  }
}

void print_file(char *argv, flags f, int *num_line) {
  int lines_in_file = lines_num(argv);
  int flag = 1, flag_empty_prev_line = 0;
  char line[BUFF], ch;
  FILE *file;
  file = fopen(argv, "r");
  ch = fgetc(file);
  while (flag) {
    int i = 0;
    while (((int)ch) != EOF && ch != '\n') {  //  collect line char by char
      line[i++] = ch;
      ch = getc(file);
    }
    if (((int)ch) == EOF) {  // exit
      flag = 0;
    } else {
      if (ch == '\n') line[i++] = ch;
    }
    ch = fgetc(file);  //  first sym of next line
    if (!line_is_empty(line) ||
        !EOF_ends(line))  //  print lines and num of line
      print_lines_and_num(line, f, *num_line, flag_empty_prev_line,
                          lines_in_file);
    if ((!f.b || !line_is_empty(line)) &&
        (!f.s || !flag_empty_prev_line || !line_is_empty(line)))
      *num_line = *num_line +
                  1;  //  counting lines, if flag b || flag s need skip a step
    flag_empty_prev_line = line_is_empty(line);  //  prev line is empty?
    for (int k = 0; k < BUFF; k++) {
      line[k] = EOF;
    }
  }
  fclose(file);
}

void print_lines_and_num(char line[BUFF], flags f, int num_line,
                         int flag_empty_prev_line, int lines_in_file) {
  int last_symbole;
  if (num_line == lines_in_file)
    last_symbole = 0;
  else
    last_symbole = 1;
  if (((!f.s || !line_is_empty(line) || !flag_empty_prev_line) && f.n &&
       !f.b) ||
      (f.b && !line_is_empty(line)))
    printf("%6d\t", num_line);  //  if flags: b, n, print line number
  if (!f.s || !line_is_empty(line) ||
      !flag_empty_prev_line) {  //  processing flags: e, t, v
    if (f.t)
      print_line_with_tab(line, f);
    else
      print_line(line, f, last_symbole);
  }
}

void print_line_with_tab(char line[BUFF], flags f) {
  char ch;
  int i = 0;
  ch = line[i++];
  while (ch != '\n' && ch != EOF) {
    if (ch == '\t') {
      printf("^I");
    } else {
      if (f.v) {
        print_special(&ch);
      } else {
        printf("%c", ch);
      }
    }
    ch = line[i++];
  }
  if (ch != EOF && f.e) printf("$");
  if (ch != EOF) printf("%c", ch);
}

void print_line(char line[BUFF], flags f, int last_symbole) {
  char ch;
  int i = 0;
  ch = line[i++];
  while (ch != '\n' && ch != EOF) {
    if (f.v) {
      print_special(&ch);
    } else {
      printf("%c", ch);
    }
    ch = line[i++];
  }
  if (ch != EOF && f.e) printf("$");
  if (ch != EOF && last_symbole) printf("%c", ch);
}

void print_special(char *ch) {
  if (spec_sym(*ch)) {
    printf("%s", s21_cat_spec_symbols[(int)*ch]);
  } else {
    if (*ch != 127) {
      printf("%c", *ch);
    } else {
      printf("%c", '^');
      printf("%c", '?');
    }
  }
}

int spec_sym(unsigned char ch) {
  int res = 0;
  if (ch < 32 || ch > 128) res = 1;
  if (ch == 127) res = 0;
  return res;
}

int line_is_empty(char line[BUFF]) {
  int i = 0, flag = 1;
  char c = line[i++];
  while (flag && i < BUFF && c != '\n' && c != EOF) {
    if (c != ' ' && c != '\n' && c != EOF) flag = 0;
    c = line[i++];
  }
  return flag;
}

int lines_num(char *argv) {
  FILE *file;
  char ch = '0';
  int count = 1;
  file = fopen(argv, "r");
  while (ch != EOF) {
    ch = fgetc(file);
    if (ch == '\n') count++;
  }
  fclose(file);
  return count;
}

int EOF_ends(char line[BUFF]) {
  int k = 0;
  while (line[k] != '\n' && line[k] != EOF) {
    k++;
  }
  return line[k] == EOF;
}
