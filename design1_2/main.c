#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096
#define BUFFER_NUM 10
#define READ_SEM 0
#define WRITE_SEM 1

int sem_id = 0;

typedef struct buffer_str {
    size_t length;
    char buffer[BUFFER_SIZE];
    struct buffer_str *next;
}shared_buffer;

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
};

shared_buffer *buf_ptr = NULL;

enum ERR_NUM {SUCCEED = 0,
        SRC_FILE_IS_DIR,
        DST_FILE_IS_NOT_DIR,
        SRC_OPEN_FAILED,
        DST_OPEN_FAILED,
        CREATE_SEM_ERROR,
        SET_SEM_VALUE_ERROR,
        };

char *get_file_name(char *filepath);
int copy_file(char *src_file_name, char *dst_file_name);
void error_catch(int rstate);
void *reader_function(void *read_fp);
void *writer_function(void *writer_fp);
bool  semaphore_P(unsigned short sum_num);
bool  semaphore_V(unsigned short sum_num);
bool  set_value_semaphore(void);
void  delete_semaphore(void);
shared_buffer *malloc_circular_list(int size);
shared_buffer *free_circular_list(shared_buffer *target, int size);

int main(int argc, char *argv[]) {
    int return_state = 0;
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
            stat(dst, &s_buf);
            if (S_ISDIR(s_buf.st_mode)) {
                filename = get_file_name(src);
//                printf("file name is %s\n", filename);
                if (dst[strlen(dst) - 1] != '/')
                    dst = strcat(dst, "/");
                char *dest_name = strcat(dst,filename);
//                printf("destination file name is %s\n", dest_name);
                copy_file(src, dest_name);
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

    error_catch(return_state);
    return return_state;
}

char *get_file_name(char *filepath) {
    char *filename = NULL;
    size_t len = strlen(filepath);
    int left_slash_pos = -1;
    for (int i = (int)len - 1; i >= 0; i--) {
        if (filepath[i] == '/') {
            left_slash_pos = i;
            break;
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
    printf("Copy %s to %s.\n", src_file_name, dst_file_name);
    FILE *src = fopen(src_file_name, "rb");
    FILE *dst = fopen(dst_file_name, "wb");
    if (src == NULL)
        return SRC_OPEN_FAILED;
    if (dst == NULL)
        return DST_OPEN_FAILED;
    sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);

    if (sem_id == -1) {
        return CREATE_SEM_ERROR;
    }
    if (set_value_semaphore() == false) {
        delete_semaphore();
        return SET_SEM_VALUE_ERROR;
    }
    buf_ptr = malloc_circular_list(BUFFER_NUM);
    pthread_t reader, writer;
    int iret1 = 0, iret2 = 0;
    /* Create 2 independent threads each of which will execute function */
    if ((iret1 = pthread_create(&reader, NULL, reader_function, src)) != 0) {
        // error creating the first thread
        perror("Create thread for reader failed!\n");
        delete_semaphore();
        exit(1);
    }

    if ((iret2 = pthread_create(&writer, NULL, writer_function, dst)) != 0) {
        // error creating the second thread
        perror("Create thread for writer failed!\n");
        delete_semaphore();
        exit(1);
    }
    pthread_join(reader, NULL);
    pthread_join(writer, NULL);
    buf_ptr = free_circular_list(buf_ptr, BUFFER_NUM);
    fclose(src);
    fclose(dst);
    return SUCCEED;
}

void *reader_function(void *read_fp) {
    FILE *fp = (FILE *) read_fp;
    shared_buffer *ptr = buf_ptr;
    fseek(read_fp, 0, SEEK_SET);
    semaphore_P(READ_SEM);
    size_t len = fread(ptr->buffer, sizeof(char), BUFFER_SIZE * sizeof(char), fp);
    ptr->length = len;
//    printf("reader %lu\t", ptr->length);
    ptr = ptr->next;
    semaphore_V(WRITE_SEM);
    while (len != 0) {
        semaphore_P(READ_SEM);
        len = fread(ptr->buffer, sizeof(char), BUFFER_SIZE * sizeof(char), fp);
        ptr->length = len;
//        printf("reader %lu\t", ptr->length);
        ptr = ptr->next;
        semaphore_V(WRITE_SEM);
    }
    return NULL;
}

void *writer_function(void *writer_fp) {
    FILE *fp = (FILE *) writer_fp;
    semaphore_P(WRITE_SEM);
    shared_buffer *ptr = buf_ptr;
    size_t len = fwrite(ptr->buffer, sizeof(char), ptr->length * sizeof(char), fp);
//    printf("writer %lu\t", len);
    ptr = ptr->next;
    semaphore_V(READ_SEM);
    while(ptr->length > 0 && ptr->length <= BUFFER_SIZE * sizeof(char)) {
        semaphore_P(WRITE_SEM);
        len = fwrite(ptr->buffer, sizeof(char), ptr->length * sizeof(char), fp);
//        printf("writer %lu\t", len);
        ptr = ptr->next;
        semaphore_V(READ_SEM);
    }
    return NULL;
}

bool semaphore_P(unsigned short sum_num) {
    struct sembuf sem;
    sem.sem_num = sum_num;
    sem.sem_op = -1;
    sem.sem_flg = 0;
    if (semop(sem_id, &sem, 1) == -1) {
        perror("semaphore P failed!\n");
        return false;
    }
    return true;
}

bool semaphore_V(unsigned short sum_num) {
    struct sembuf sem;
    sem.sem_num = sum_num;
    sem.sem_op = 1;
    sem.sem_flg = 0;
    if (semop(sem_id, &sem, 1) == -1) {
        perror("semaphore V failed!\n");
        return false;
    }
    return true;
}

bool set_value_semaphore(void) {
    union semun sem_union;

    sem_union.val = BUFFER_NUM;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
        perror("Fail to set value for semaphore!\n");
        return false;
    }
    sem_union.val = 0;
    if (semctl(sem_id, 1, SETVAL, sem_union) == -1) {
        perror("Fail to set value for semaphore!\n");
        return false;
    }
    return true;
}

void delete_semaphore(void) {
    union semun sem_union;

    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
        perror("Failed to delete semaphore!\n");
    }
}

shared_buffer *malloc_circular_list(int size) {
    shared_buffer *head = NULL, *current = NULL;
    for (int i = 0; i < size; i++) {
        if (i == 0) {
            head = (shared_buffer *)malloc(sizeof(shared_buffer));
            head->next = NULL;
            head->length = 0;
            current = head;
        }
        else if (i != size - 1) {
            current->next = (shared_buffer *)malloc(sizeof(shared_buffer));
            current = current->next;
            current->next = NULL;
            current->length = 0;
        }
        else {
            current->next = (shared_buffer *)malloc(sizeof(shared_buffer));
            current = current->next;
            current->next = head;
            current->length = 0;
        }
    }
    return head;
}

shared_buffer *free_circular_list(shared_buffer *target, int size) {
    shared_buffer *current = target, *previous = target;
    for (int i = 0; i < size; i++) {
        current = ( i == size - 1 ) ? NULL : current->next;
        free(previous);
        previous = NULL;
    }
    return NULL;
}

void error_catch(int rstate) {
    switch (rstate) {
        case SRC_FILE_IS_DIR:
            printf("source file is a directory!\n");
            break;
        case DST_FILE_IS_NOT_DIR:
            printf("destination file is not a directory!\n");
        case SRC_OPEN_FAILED:
            printf("open source file failed!\n");
        case DST_OPEN_FAILED:
            printf("open destination file failed!\n");
        case CREATE_SEM_ERROR:
            printf("error creating semaphores!\n");
        case SET_SEM_VALUE_ERROR:
            printf("error setting value for semaphores!\n");
        default:
            break;
    }
}