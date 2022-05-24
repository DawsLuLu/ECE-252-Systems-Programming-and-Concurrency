#include "util.h"

int check_png_header(U8 * out, FILE * fp) {
    rewind(fp);
    if (fread(out, sizeof(U8), 8, fp) != 8)
        return -1;

    if (out[0] == 0x89 && out[1] == 'P' && out[2] == 'N' && out[3] == 'G' && out[4] == 0x0D && out[5] == 0x0A && out[6] == 0x1A && out[7] == 0x0A)
        return 0;
    
    return -1;
}

int get_chunk(chunk_p out, FILE * fp, long offset, int whence) {

    if (fseek(fp, offset, whence) == -1)
        return -1;

    if (fread(&out->length, 4, 1, fp) == 0)
        return -1;
    out->length = ntohl(out->length);

    U8 type_and_data[out->length + 4];
    if (fread(&type_and_data, 1, 4 + out->length, fp) == 0)
        return -1;

    for (int i = 0; i < 4; i++) out->type[i] = type_and_data[i];

    U8 * data = malloc(out->length * sizeof(U8));

    if (data == NULL && out->length != 0)
        return -1;

    memcpy(data, type_and_data+4, out->length * sizeof(U8));
    out->p_data = data;

    fread(&out->crc, sizeof(U32), 1, fp);
    out->crc = ntohl(out->crc);

    return 0;
}

U32 check_crc(chunk_p const chunk) {
    U8 *buf =  malloc((chunk->length + 4) * sizeof(U8));

    memcpy(buf, chunk->type, 4);
    memcpy(buf + 4, chunk->p_data, chunk->length);

    U32 res = crc(buf, 4 + chunk->length);
    free(buf);

    return res;
}

void print_chunk(chunk_p const chunk) {
    printf("Length: %d\n", chunk->length);
    printf("Type: %c %c %c %c\n", chunk->type[0], chunk->type[1], chunk->type[2], chunk->type[3]);
    printf("Data: ");
    for (int i = 0; i < chunk->length; i++) {
        printf("%x ", chunk->p_data[i]);
    }
    printf("\n");
    printf("CRC: %x\n", chunk->crc);
}

int is_dir(char *dir)
{

    struct stat buf;

    if (stat(dir, &buf) < 0) {
        perror("stat error");
        return 1;
    }   

    if (S_ISDIR(buf.st_mode)) {
        return 0;
    }

    return 1;

}

int is_regular(char *dir)
{

    struct stat buf;

    if (stat(dir, &buf) < 0) {
        perror("stat error");
        return 1;
    }   

    if (S_ISREG(buf.st_mode)) {
        return 0;
    }

    return 1;

}

int findPng(char *directory, int *foundPng, char *fullPath) 
{
    DIR *p_dir;
    struct dirent *p_dirent;
    char str[64];
    U8 header[8];
    FILE * pngStream;

    if ((p_dir = opendir(directory)) == NULL) {
        sprintf(str, "opendir(%s)", directory);
        perror(str);
        exit(2);
    }

    p_dirent = readdir(p_dir);
    char *str_path = p_dirent->d_name;
    p_dirent = readdir(p_dir);
    str_path = p_dirent->d_name;

    chdir(directory);

    while ((p_dirent = readdir(p_dir)) != NULL) {
        str_path = p_dirent->d_name;  /* relative path name! */
        pngStream = fopen(str_path, "rb");

        if (pngStream == NULL)
            continue;

        if (str_path == NULL) {
            return 0;
        }

        if (is_dir(str_path) == 0) {
            char newPath[strlen(fullPath) + strlen(str_path) + 2];
            newPath[0] = 0;

            strcat(newPath, fullPath);
            newPath[strlen(fullPath) + 1] = '\0';
            newPath[strlen(fullPath)] = '/';
            strcat(newPath, str_path);

            findPng(str_path, foundPng, newPath);

            chdir("..");
        }

        else if (is_regular(str_path) == 0 && check_png_header(header, pngStream) == 0) {

            printf("%s/%s\n", fullPath, str_path);

            *foundPng = 1;
        }

        fclose(pngStream);

    }

    if ( closedir(p_dir) != 0 ) {
        perror("closedir");
        exit(3);
    }

    return 0;
}

int concat_data_chunks(chunk_p out, struct chunk * data, U64 width, U64 total_h, size_t imgc) {

    // allocate buffer for inflated memory
    U8 * data_inf = malloc(total_h * (width * 4 + 1));

    size_t offset = 0;
    size_t length = 0;
    for (size_t i = 0; i < imgc; i++) {
        if (mem_inf(data_inf + offset, &length, data[i].p_data, data[i].length) != 0) {
            fprintf(stderr,"Failed to inflate data for the chunk %ld\n", i);
            free(data_inf);
            return -1;  
        }
        offset+=length;
    }

    // offset now contains the total number of bytes inflated, compress the data 
    U8 * compressed = malloc(offset + 4);
    U64 compress_length;
    int ret = mem_def(compressed + 4, &compress_length, data_inf, offset, Z_DEFAULT_COMPRESSION);
    if (ret != 0 || compress_length > offset) {
        fprintf(stderr,"Failed to compress data for the stacked chunks\n");
        free(compressed);
        return -1;
    }

    // copy over memory
    U8 * data_p = malloc(compress_length*sizeof(U8));
    memcpy(data_p, compressed + 4, compress_length);
    out->p_data = data_p;
    out->length = compress_length;

    // calulcate crc
    compressed[0] = 'I';
    compressed[1] = 'D';
    compressed[2] = 'A';
    compressed[3] = 'T';
    out->crc = (U32)crc(compressed, compress_length + 4);
    memcpy(out->type, compressed, 4);

    free(compressed);
    free(data_inf);

    return 0;
}
