#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct flag_array {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} flags;

//  main

void s21_grep(int argc, char *argv[]);
void print_files(flags flag, int count_files, char *pattern, char *file_names);
void print_for_patterns(flags flag, char *pattern, char *name_of_file,
                        int count_files);

//  helpers

void applications_flags_in_reading(flags flag, int match, int lno,
                                   char *name_of_file, char *lbuf,
                                   char *pattern, regmatch_t *pmatch,
                                   int *line_matches_flag_o, int match_flag_o,
                                   int count_files);
void applying_flags_after_reading(flags flag, char *name_of_file,
                                  int count_files, int line_matches_flag_o,
                                  int line_matches);

//  helpers for helpers

void several_in_a_row_flag_o(flags flag, char *name_of_file, char *pattern,
                             int count_files);
void standatr_flag_o(flags flag, char *name_of_file, int count_files,
                     int line_matches_flag_o);

//  parsing

void parse_flags_and_pattern(int argc, char *argv[], flags *flag,
                             int *count_files, char *file_names, char *pattern);
void parse_patterns_and_file_names(flags flag, int argc, char *argv[],
                                   int count_files, char *pattern,
                                   char *file_names, char *file_f,
                                   char *regex_e, int optind);

//  helpers for parsing
int getting_patterns_from_a_file(char *pattern, char *file_name);
void separating(char *str, char *dest, int *flag_end);

#endif  // SRC_GREP_S21_GREP_H_
