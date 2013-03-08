#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

#define FILE_NAME_LEN 256


int main(int argc, char **argv)
{
    char ar_file_name[FILE_NAME_LEN], obj_file_name[FILE_NAME_LEN];
   
    struct stat ar_file_stat, obj_file_stat;
   
    struct utimbuf obj_file_utb;
   
    if (argc != 3)
    {
        printf("\nUsage: adj_time archive_file obj_file\n");    	
    }    	
    else
    {    	
        strcpy(ar_file_name, argv[1]);
        strcpy(obj_file_name, argv[2]);
   
        stat(ar_file_name, &ar_file_stat);
        stat(obj_file_name, &obj_file_stat);
   
        if (ar_file_stat.st_mtime == obj_file_stat.st_mtime)
        {
            obj_file_utb.actime = obj_file_stat.st_atime;
            obj_file_utb.modtime = obj_file_stat.st_mtime + 1;
      
            utime(obj_file_name, &obj_file_utb);
        }
    }       
   
    exit(0);
}
