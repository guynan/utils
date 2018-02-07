

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define NFLAG   0x01
#define EFLAG   0x02

/* Manage our flags cleanly */
#define IS_FLAG_SET(i, flag)    ((i) & (flag))
#define SET_FLAG(i, flag)       ((i) |= ((i) & (flag)) ? (i) : (flag))
#define UNSET_FLAG(i, flag)     ((i) &= (!((i) & (flag))) ? (i) : (~(flag)))


/* Perhaps don't use progname as not defined by any standards */
extern char* __progname;


int8_t parse_args(int argc, char** argv);
int main(int argc, char** argv);
void usage(char* progname);


int main(int argc, char** argv)
{

        int8_t flags = parse_args(argc, argv);

        if(flags < 0){
                usage(argv[0]);
                return 1;
        }

        argv += 1;

        while(*argv){

                if(**argv == '-'){
                        argv += 1;
                        continue;
                }

                (void) fprintf(stdout, "%s", *argv);

                if(*++argv)
                        fprintf(stdout, " ");

        }

        if(! IS_FLAG_SET(flags, NFLAG)){
                (void) fprintf(stdout, "\n");
        }

        return 0;

}


int8_t parse_args(int argc, char** argv)
{
        int8_t flags = 0;
        int ch = 0;

        while((ch = getopt(argc, argv, "ne")) != -1){

                switch(ch){

                        case 'n':
                                SET_FLAG(flags, NFLAG);
                                break;

                        case 'e':
                                SET_FLAG(flags, EFLAG);
                                break;

                        default:
                                return -1;
                }

        }

        return flags;

}


void usage(char* progname)
{
        (void) fprintf(stderr, "Usage: %s [OPTIONS] [FILE]\n",
                        (__progname) ? __progname : progname);
}

