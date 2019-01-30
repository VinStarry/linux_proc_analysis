#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 4096
#define BUFFER_NUM 10

enum ERR_NUM {SUCCEED = 0,
        SRC_FILE_IS_DIR,
        DST_FILE_IS_NOT_DIR};

inline FILE* open_file(FILE *fp, char *filepath);
char *get_file_name(char *filepath);
int copy_file(char *src_file_name, char *dst_file_name);
void error_catch(int rstate);

int main(int argc, char *argv[]) {
    int return_state = 0;
    FILE *fp_read = NULL;
    FILE *fp_write = NULL;
    char *src = NULL;
    char *dst = NULL;
    char *opt = NULL;
    char *filename = NULL;
    struct stat s_buf;
    if (argc < 3 || argc > 4) {
        perror("Invalid input!");
    }
    else if (argc == 3) {
        src = argv[1];
        dst = argv[2];
        stat(src, &s_buf);
        if (S_ISDIR(s_buf.st_mode)) {
            printf("%s is a directory\n", src);
            return_state = SRC_FILE_IS_DIR;
        }
        else {
            open_file(fp_read, src);
            stat(dst, &s_buf);
            if (S_ISDIR(s_buf.st_mode)) {
                filename = get_file_name(src);
                printf("file name is %s\n", filename);
                if (dst[strlen(dst) - 1] != '/')
                    dst = strcat(dst, "/");
                char *dest_name = strcat(dst,filename);
                printf("destination file name is %s\n", dest_name);

            }
            else {
                printf("%s is not a directory\n", dst);
                return_state = DST_FILE_IS_NOT_DIR;
            }
        }
    }
    else {
        opt = argv[1];
        src = argv[2];
        dst = argv[3];
    }

    return return_state;
}

FILE* open_file(FILE *fp, char *filepath) {
    fp = fopen(filepath, "rb");
    return fp;
}

char *get_file_name(char *filepath) {
    char *filename = NULL;
    size_t len = strlen(filepath);
    int left_slash_pos = -1;
    for (int i = (int)len - 1; i >= 0; i--) {
        if (filepath[i] == '/') {
            left_slash_pos = i;
        }
    }
    filename = (char *)malloc(sizeof(char) * (len - left_slash_pos));
    for (int i = 0; i < len - left_slash_pos; i++) {
        filename[i] = filepath[left_slash_pos + 1 + i];
    }
    filename[len-left_slash_pos-1] = '\0';
    return filename;
}

int copy_file(char *src_file_name, char *dst_file_name) {

}

void error_catch(int rstate) {
    switch (rstate) {
        case SRC_FILE_IS_DIR:
            printf("")
            break;
        default:
            break;
    }
}