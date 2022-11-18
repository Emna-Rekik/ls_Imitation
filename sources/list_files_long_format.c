#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>
#include<inttypes.h>
#include <limits.h>
#include <stdlib.h>

/** From man 2 stat */
#if 0

	struct stat {
	       dev_t     st_dev;         /* ID of device containing file */
	       ino_t     st_ino;         /* Inode number */
	       mode_t    st_mode;        /* File type and mode */
	       nlink_t   st_nlink;       /* Number of hard links */
	       uid_t     st_uid;         /* User ID of owner */
	       gid_t     st_gid;         /* Group ID of owner */
	       dev_t     st_rdev;        /* Device ID (if special file) */
	       off_t     st_size;        /* Total size, in bytes */
	       blksize_t st_blksize;     /* Block size for filesystem I/O */
	       blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

	       /* Since Linux 2.6, the kernel supports nanosecond
		  precision for the following timestamp fields.
		  For the details before Linux 2.6, see NOTES. */

	       struct timespec st_atim;  /* Time of last access */
	       struct timespec st_mtim;  /* Time of last modification */
	       struct timespec st_ctim;  /* Time of last status change */

	   #define st_atime st_atim.tv_sec      /* Backward compatibility */
	   #define st_mtime st_mtim.tv_sec
	   #define st_ctime st_ctim.tv_sec
	   };


#endif

/** From man 3 getpwuid */
#if 0

	struct passwd {
               char   *pw_name;       /* username */
               char   *pw_passwd;     /* user password */
               uid_t   pw_uid;        /* user ID */
               gid_t   pw_gid;        /* group ID */
               char   *pw_gecos;      /* user information */
               char   *pw_dir;        /* home directory */
               char   *pw_shell;      /* shell program */
           };

#endif

/** From man 3 ctime */
#if 0

	struct tm {
               int tm_sec;    /* Seconds (0-60) */
               int tm_min;    /* Minutes (0-59) */
               int tm_hour;   /* Hours (0-23) */
               int tm_mday;   /* Day of the month (1-31) */
               int tm_mon;    /* Month (0-11) */
               int tm_year;   /* Year - 1900 */
               int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
               int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
               int tm_isdst;  /* Daylight saving time */
           };

#endif

/** From man 7 inode */
#if 0
	   S_IFMT     0170000   bit mask for the file type bit field
	   
	   S_IFSOCK   0140000   socket		//12 en decimal
           S_IFLNK    0120000   symbolic link		//10 en decimal
           S_IFREG    0100000   regular file		//8 en decimal
           S_IFBLK    0060000   block device		//6 en decimal
           S_IFDIR    0040000   directory		//4 en decimal
           S_IFCHR    0020000   character device	//2 en decimal
           S_IFIFO    0010000   FIFO			//1 en decimal

#endif

typedef struct {
	uint16_t others_x:1;
	uint16_t others_w:1;
	uint16_t others_r:1;
	uint16_t group_x:1;
	uint16_t group_w:1;
	uint16_t group_r:1;
	uint16_t owner_x:1;
	uint16_t owner_w:1;
	uint16_t owner_r:1;
	uint16_t sticky:1;
	uint16_t sgid:1;
	uint16_t suid:1;
	uint16_t fileType:4;
} mode_s;

typedef union {
	mode_s mode;
	uint16_t value;
} mode_u;

void list_files_long_format(int counter,int argc, char **argv, int nbre_arg, int n, int show_hidden_files)
{
	struct stat mystat;
	DIR* dirptr;
	struct dirent* entry;
	mode_u mode;
	
	char directory[50];
	int i;
	
	i = counter;
	if (argc == nbre_arg)
	{
		strcpy(directory,"./");
	}
	else
	{
		i = nbre_arg;
	}
	for(;i < argc; i++)
	{
		if (i != counter)
		{
			strcpy(directory, argv[i]);
		}
		chdir(directory);
		dirptr = opendir(directory);
		if (dirptr == NULL)
		{
			perror("dirptr");
			return;
		}
		if (argc > n)
		{
			printf("%s:\n", argv[i]);
		}
		
		struct dirent* entry_for_total;
		DIR* dirptr_for_total = opendir(directory);
		int total_used_blocs = 0;
		while((entry_for_total = readdir(dirptr_for_total)) != NULL)
		{
			if(entry_for_total->d_name[0] != '.' || show_hidden_files)
			{
				if(lstat(entry_for_total->d_name, &mystat) < 0)
				{
					printf("lstat failes %s\n", entry->d_name);
					return;
				}
				total_used_blocs += mystat.st_blocks;
			}
		}
		printf("total %d\n", total_used_blocs/2);
		
		while((entry = readdir(dirptr)) != NULL)
		{
			if(entry->d_name[0] != '.' || show_hidden_files)
			{
				if(lstat(entry->d_name, &mystat) < 0)
				{
					printf("lstat failes %s\n", entry->d_name);
					return;
				}
				
				mode.value = mystat.st_mode;
				
				char file_type;
				switch(mystat.st_mode & S_IFMT)
				{
					case S_IFSOCK:
						file_type = 's';
						break;
					case S_IFLNK:
						file_type = 'l';
						break;
					case S_IFREG:
						file_type = '-';
						break;
					case S_IFBLK:
						file_type = 'b';
						break;
					case S_IFDIR:
						file_type = 'd';
						break;
					case S_IFCHR:
						file_type = 'c';
						break;
					case S_IFIFO:
						file_type = 'p';
						break;
					default:
						file_type = '-';
						break;
				}
				
				time_t now = time(NULL);
				struct tm* current_time = gmtime(&now);
				int current_year = current_time->tm_year + 1900;
				
				struct tm* mytm = gmtime(&mystat.st_mtim.tv_sec);
				char month[20];
				int month_number = mytm->tm_mon;
				switch(month_number)
				{
					case 0:
						strcpy(month, "Jan");
						break;
					case 1:
						strcpy(month, "Feb");
						break;
					case 2:
						strcpy(month, "Mar");
						break;
					case 3:
						strcpy(month, "Apr");
						break;
					case 4:
						strcpy(month, "May");
						break;
					case 5:
						strcpy(month, "Jun");
						break;
					case 6:
						strcpy(month, "Jul");
						break;
					case 7:
						strcpy(month, "Aug");
						break;
					case 8:
						strcpy(month, "Sep");
						break;
					case 9:
						strcpy(month, "Oct");
						break;
					case 10:
						strcpy(month, "Nov");
						break;
					case 11:
						strcpy(month, "Dec");
						break;
					default:
						strcpy(month, "Jan");
						break;
				}
				
				char time[20] = {0};
				char tmp[10];
				if(!(strcmp(month, "Jan") == 0 || strcmp(month, "Feb") == 0 ||
				   strcmp(month, "Mar") == 0 || strcmp(month, "Apr") == 0) &&
				   ((mytm->tm_year + 1900) == current_year))
				{
					int t = mytm->tm_hour +1;
					if (t == 24)
					{
						t = 0;
					}
					sprintf(tmp, "%i", t);
					strcat(time, tmp);
					strcat(time, ":");
					sprintf(tmp, "%i", mytm->tm_min);
					strcat(time, tmp);
				}
				else
				{
					sprintf(tmp, "%i", mytm->tm_year + 1900);
					strcpy(time, tmp);
				}
				
				char buf[PATH_MAX];
				char file_name[PATH_MAX];
				realpath(entry->d_name, buf);
				if(file_type == 'l')
				{
					strcpy(file_name, entry->d_name);
					strcat(file_name, " -> ");
					strcat(file_name, buf);
				}
				else
				{
					strcpy(file_name, entry->d_name);
				}
				
				printf("%c%c%c%c%c%c%c%c%c%c %3ld %4s %4s %5ld %3s %2d %5s %s\n",
					file_type,
					(mode.mode.owner_r == 1)? 'r': '-',
					(mode.mode.owner_w == 1)? 'w': '-',
					(mode.mode.owner_x == 1)? 'x': '-',
					(mode.mode.group_r == 1)? 'r': '-',
					(mode.mode.group_w == 1)? 'w': '-',
					(mode.mode.group_x == 1)? 'x': '-',
					(mode.mode.others_r == 1)? 'r': '-',
					(mode.mode.others_w == 1)? 'w': '-',
					(mode.mode.others_x == 1)? 'x': '-',
					mystat.st_nlink,
					(getpwuid(mystat.st_uid) != NULL)? getpwuid(mystat.st_uid)->pw_name : "Unkn", 
					(getpwuid(mystat.st_gid) != NULL)? getpwuid(mystat.st_uid)->pw_name : "Unkn", 
					mystat.st_size,
					month,
					mytm->tm_mday,
					time,
					file_name
					);
			}
		}
		if (i != argc-1)
		{
			printf("\n");
		}
		closedir(dirptr);
	}
}
