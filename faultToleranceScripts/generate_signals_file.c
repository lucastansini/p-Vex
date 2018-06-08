#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char file_list_0[] = "signals_list_0.txt";
char file_list_1[] = "signals_list_1.txt";
char file_list_2[] = "signals_list_2.txt";
char file_list_3[] = "signals_list_3.txt";
char file_list_out[] = "signals_injfaults.do";

int main()
{
  int line_counter = 0;
  FILE * fp;
  FILE * fp_out;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp_out = fopen(file_list_out, "w");
  if (fp_out == NULL)
    return 1;

  fprintf(fp_out, "#List of signals to inject faults\n");
  fprintf(fp_out, "array set signal {\n");

//0
  fp = fopen(file_list_0, "r");
  if (fp == NULL)
    return 1;
  while ((read = getline(&line, &len, fp)) != -1) {
    if( line[read-1] == '\n' )
      line[read-1] = '\0';
    fprintf(fp_out, "%d %s\n", line_counter, line);
    line_counter++;
  }
  fclose(fp);

//1
  fp = fopen(file_list_1, "r");
  if (fp == NULL)
    return 1;
  while ((read = getline(&line, &len, fp)) != -1) {
    if( line[read-1] == '\n' )
      line[read-1] = '\0';
    fprintf(fp_out, "%d %s\n", line_counter, line);
    line_counter++;
  }
  fclose(fp);

//2
  fp = fopen(file_list_2, "r");
  if (fp == NULL)
    return 1;
  while ((read = getline(&line, &len, fp)) != -1) {
    if( line[read-1] == '\n' )
      line[read-1] = '\0';
    fprintf(fp_out, "%d %s\n", line_counter, line);
    line_counter++;
  }
  fclose(fp);

//3
  fp = fopen(file_list_3, "r");
  if (fp == NULL)
    return 1;
  while ((read = getline(&line, &len, fp)) != -1) {
    if( line[read-1] == '\n' )
      line[read-1] = '\0';
    fprintf(fp_out, "%d %s\n", line_counter, line);
    line_counter++;
  }
  fclose(fp);
 
//
  if (line)
    free(line);
  fprintf(fp_out, "}\n");
  fclose(fp_out);

  return 0;
}
