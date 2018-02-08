

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

/* 
 * FLAGS structure
 *   
 * | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
 * ^-----------------^   ^   ^   ^
 *       unused          h   n   c
 *
 * -a is the summation of -vet
 *
 * */
#define CFLAG   0x01
#define NFLAG   0x02
#define HFLAG   0x04

/* Manage our flags cleanly */
#define IS_FLAG_SET(i, flag)    ((i) & (flag))
#define SET_FLAG(i, flag)       ((i) |= ((i) & (flag)) ? (i) : (flag))
#define UNSET_FLAG(i, flag)     ((i) &= (!((i) & (flag))) ? (i) : (~(flag)))


extern char* __progname;


char* get_filename(int argc, char** argv);
int8_t parse_args(int argc, char** argv);
int cat_file(FILE* stream, int8_t flags);
int main(int argc, char** argv);
void usage(char* progname);
void display_help(void);


int main(int argc, char** argv)
{
        int8_t flags = parse_args(argc, argv);

        if(flags < 0){
                usage(argv[0]);
                return 1;
        }

        if(!flags){
                

        FILE* fp = NULL;
        const char* filename = get_filename(argc, argv);

        if(!filename){
                filename = "stdin";
                fp = stdin;
                goto use_stdin;
        }

        fp = fopen(filename, "r");

use_stdin:

        if(!fp){
                (void) fprintf(stderr, "%s: %s: Could not open file\n",
                        (__progname) ? __progname : argv[0], filename);
        }

        cat_file(fp, flags);
        fclose(fp);

        return 0;

}



/* Here we have two options for head, we can navigate using bytes as the
 * counter, or using the newline character. Note that the -n and -c are in
 * conflict, this implementation will take the last flag */
int8_t parse_args(int argc, char** argv)
{
        int8_t flags = 0;
        int ch = 0;

        while((ch = getopt(argc, argv, "cnh")) != -1){

                switch(ch){

                        case 'c':
                                SET_FLAG(flags, CFLAG);
                                UNSET_FLAG(flags, NFLAG);
                                break;

                        case 'n':
                                SET_FLAG(flags, NFLAG);
                                UNSET_FLAG(flags, CFLAG);
                                break;

                        case 'h':
                                SET_FLAG(flags, HFLAG);
                                return flags;

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


char* get_filename(int argc, char** argv)
{
        for(int32_t i = 1; i < argc; i++){

                if(*argv[i] != '-'){
                        return argv[i];
                }

        }

        /* Haven't found a valid *possible* filename */
        return NULL;

}

int cat_file(FILE* stream, int8_t flags)
{
        int c = 0;

        if(IS_FLAG_SET(flags, HFLAG)){
                display_help();
                return 1;
        }

        while((c = fgetc(stream)) != EOF){

                if(c == '\n' && IS_FLAG_SET(flags, EFLAG)){
                        fprintf(stdout, "$\n");
                        continue;
                }

                if(c == '\t' && IS_FLAG_SET(flags, TFLAG)){
                        fprintf(stdout, "^I");
                        continue;
                }
                               
                fprintf(stdout, "%c", c);
        }

        return 1;
}

void display_help(void)
{
        (void) fprintf(stdout, "You need help\n");
}

