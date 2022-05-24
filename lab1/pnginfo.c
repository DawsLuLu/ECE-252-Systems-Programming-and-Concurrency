#include "util.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Usage: %s <file name>\n", argv[0]);
        exit(1);
    }

    FILE * pngStream;
    char str[64];

    if ((pngStream = fopen(argv[1], "rb")) == NULL) {
        sprintf(str, "fopen(%s)", argv[1]);
        perror(str);
        exit(2);
    }

    U8 header[8];
    if (check_png_header(header, pngStream) != 0) {
        printf("%s is not a PNG file\n", argv[1]);
        fclose(pngStream);
        exit(0);
    }
    
    struct chunk ihdr;
    struct chunk idat;
    struct chunk iend;
    
    if (get_chunk(&ihdr, pngStream, 0, SEEK_CUR) == -1 || get_chunk(&idat, pngStream, 0, SEEK_CUR) == -1 || get_chunk(&iend, pngStream, 0, SEEK_CUR) == -1)
        exit(1);

    struct data_IHDR ihdr_data;
    memcpy(&ihdr_data, ihdr.p_data, 13);

    printf("%s: %d x %d\n", argv[1], ntohl(ihdr_data.width), ntohl(ihdr_data.height));

    U32 res = check_crc(&ihdr);
    if (ihdr.crc != res) {
        printf("IHDR chunk CRC error: computed %x, expected %x\n", res, ihdr.crc);
    }

    res = check_crc(&idat);
    if (idat.crc != res) {
        printf("IDAT chunk CRC error: computed %x, expected %x\n", res, idat.crc);
    }

    res = check_crc(&iend);
    if (iend.crc != res) {
        printf("IEND chunk CRC error: computed %x, expected %x\n", res, iend.crc);
    }

    free(ihdr.p_data);
    free(idat.p_data);
    free(iend.p_data);

    fclose(pngStream);

    return 0;
}