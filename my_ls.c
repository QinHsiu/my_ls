#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
int  fun1(char *dir,char *filename)
{
    struct stat buf;
    char out[100];
    if(stat(dir,&buf)<0)
    {
        perror("stat");
        return (-1);

    }

    switch(buf.st_mode & S_IFMT)
    {
    case S_IFREG:
        printf("-");
        break;
    case S_IFDIR:
        printf("d");
        break;
    case S_IFCHR:
        printf("c");
        break;
    case S_IFBLK:
        printf("b");
        break;
    case S_IFIFO:
        printf("p");
        break;
    case S_IFLNK:
        printf("l");
        break;
    case S_IFSOCK:
        printf("s");
        break;
    }

    int n;
    for(n=8;n>=0;n--)
    {
        if(buf.st_mode&(1<<n))
        {
            switch(n%3)
            {
            case 2:
                printf("r");
                break;
            case 1:
                printf("w");
                break;
            case 0:
                printf("x");
                    break;
            default:
                break;

            }
        }
        else
        {
            printf("-");
        }
    }


    printf(" %d",buf.st_nlink);

    struct passwd *pw;
    pw = getpwuid(buf.st_uid);
    printf(" %s",pw->pw_name);

    struct group *gr;
    gr = getgrgid(buf.st_gid);
    printf(" %s",gr->gr_name);

    printf(" %ld",buf.st_size);


    struct tm *t;
    t = localtime(&buf.st_atime);
    printf(" %d-%d-%d %d:%d",t->tm_year+1900,
                            t->tm_mon+1,
                            t->tm_mday,
                            t->tm_hour,
                            t->tm_min);
   printf(" %s ",filename);



    if(S_ISLNK(buf.st_mode))
    {
        printf(" -> ");
        if(readlink(filename,out,100)==-1)
        {

        }
        printf("%s",out);

    }
    printf("\n");
    return 0;
}


int main(int argc ,char **argv)
{
    char w[100];
    memset(w,0,100);

    if(argc<2)
        strcpy(w,"./");

    else
        strcpy(w,argv[1]);
    struct stat buf;
    char name[100];
    if(stat(w,&buf)<0)
    {
        fprintf(stderr,"stat error:%s\n",strerror(errno));
        return -1;
    }

    if(S_ISDIR(buf.st_mode))
    {
        DIR *dir;
        dir = opendir(w);
        struct dirent *pdr;
        while ((pdr = readdir(dir))!=NULL)
        {
            if(pdr->d_name[0]=='.')
            {
            }
            else
            {
                printf(" dir:%s \n",pdr->d_name);
                memset(name,0,100);
                strcpy(name,w);
            //  strcat(name,"/");
                strcat(name,pdr->d_name);
                fun1(name,pdr->d_name);
            }

        }
    }
    else
    {
        fun1(w,w);
    }
    return 0;
}
