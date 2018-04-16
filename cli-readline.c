#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <readline/readline.h>

FILE* file = NULL;

void usage(char* arg0) {
    fprintf(stderr,
        "Usage: %s [-f file] [-n name] [-p prompt]\n"
        "   -f      file to dump buffer info\n"
        "   -n      readline application name\n"
        "   -p      readline prompt\n"
    , arg0);

}

void line_handler(char *line)
{
    if (! line) exit(0); // eof
    if (file) fprintf(file, "%i %s\n", -1, line);
    free(line);
}

int main(int argc, char *argv[])
{
    int opt;
    char* prompt = NULL;
    while ((opt = getopt(argc, argv, "hf:n:p:")) != -1) {
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
