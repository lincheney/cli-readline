#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <readline/readline.h>

FILE* file = NULL;
unsigned int max_lines = 0;

void usage(char* arg0) {
    fprintf(stderr,
        "Usage: %s [-f FILE] [-n NAME] [-p PROMPT] [-l LINES]\n"
        "   -f FILE     dump buffer info into FILE\n"
        "   -n NAME     readline application name\n"
        "   -p PROMPT   readline prompt\n"
        "   -l LINES    exit after LINES of input\n"
    , arg0);

}

void line_handler(char *line)
{
    if (! line) exit(0); // eof
    if (file) fprintf(file, "%i %s\n", -1, line);
    if (max_lines == 1) exit(0);
    max_lines--;
    free(line);
}

int main(int argc, char *argv[])
{
    int opt;
    char* prompt = NULL;
    while ((opt = getopt(argc, argv, "hf:n:p:l:")) != -1) {
        switch (opt) {
            case 'f':
                file = fopen(optarg, "w");
                if (!file) {
                    fprintf(stderr, "Failed to open %s: %s\n", optarg, strerror(errno));
                    exit(1);
                }
                break;
            case 'n':
                rl_readline_name = optarg;
                break;
            case 'p':
                prompt = optarg;
                break;
            case 'l':
                max_lines = atoi(optarg);
                break;
            default:
                usage(argv[0]);
                exit(opt == 'h' ? 0 : 1);
        }
    }

    rl_initialize();
    rl_callback_handler_install(prompt, &line_handler);

    while (1)
    {
        rl_callback_read_char();
        if (file) fprintf(file, "%i %s\n", rl_point, rl_line_buffer);
    }
}
