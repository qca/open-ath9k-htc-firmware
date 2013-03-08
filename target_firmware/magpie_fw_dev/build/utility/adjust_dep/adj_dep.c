#include <stdio.h>

#define BUF_LEN 256


int main(int argc, char **argv)
{
   FILE *file_in, *file_out;
   char line_in_buf[BUF_LEN], line_out_buf[2 * BUF_LEN];


   if ((file_in = fopen("tmp1_file", "r")) == NULL)
   {
      fprintf(stderr, "Can not open input file: <<tmp1_file>>\n"); 
      exit(1);
   }

   if ((file_out = fopen("tmp2_file", "w")) == NULL)
   {
      fprintf(stderr, "Can not open output file: <<tmp2_file>>\n");
      fclose(file_in);
      exit(1);
   }

   while (fgets(line_in_buf, BUF_LEN, file_in) != NULL)
   {
      if (line_in_buf[0] != ' ')
      {
         strcpy(line_out_buf, argv[1]);
         strcat(line_out_buf, line_in_buf);
      }
      else
      {
         strcpy(line_out_buf, line_in_buf);
      }

      fputs(line_out_buf, file_out);
   }

   fclose(file_in);
   fclose(file_out);
}
