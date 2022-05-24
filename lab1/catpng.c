#include "util.h"

// read every file from the commandline and load their chunks
// make sure they are all the same width
// create chunks for resulting file
// iteratively concat all chunks to idat for result file
// set crcs, ihdr, iend
// write the file

void write_chunk(FILE * fp, chunk_p chunk);

int open_png(const char * file, chunk_p ihdr, chunk_p idat, chunk_p iend, U8 * png_header);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Usage: %s <file name>(s)\n", argv[0]);
        exit(1);
    }

    // structs to hold data for the resulting png
    U8 png_header[8];
    struct chunk cat_ihdr;
    struct chunk cat_idat;
    struct chunk cat_iend;
    struct data_IHDR cat_ihdr_data;
    
    if (open_png(argv[1], &cat_ihdr, &cat_idat, &cat_iend, png_header) != 0) {
        perror("Failed to open first file\n");
        exit(1);
    }
    memcpy(&cat_ihdr_data, cat_ihdr.p_data, cat_ihdr.length);
    cat_ihdr_data.height = 0;

    // structs to hold data for each png
    struct chunk ihdr;
    struct chunk * idats = malloc((argc-1)*sizeof(struct chunk));
    struct chunk iend;
    struct data_IHDR ihdr_data;

    for (int i = 1; i < argc; i++) {
        // open file
        if (open_png(argv[i], &ihdr, &idats[i-1], &iend, png_header) != 0) {
            perror("Failed to open file\n");
            exit(1);
        }
        memcpy(&ihdr_data, ihdr.p_data, ihdr.length);
        cat_ihdr_data.height = htonl(ntohl(cat_ihdr_data.height) + ntohl(ihdr_data.height)); 

        // free p_data arrays
        free(ihdr.p_data);
        free(iend.p_data);
    }

    //concat
    free(cat_idat.p_data);
    int res = concat_data_chunks(&cat_idat, idats, ntohl(ihdr_data.width), ntohl(cat_ihdr_data.height), argc-1);
    if (res != 0) {
        fprintf(stderr,"Failed to concat files\n");
        exit(1);
    }

    // update crc and data
    memcpy(cat_ihdr.p_data, (void *)&cat_ihdr_data, 13);
    U8 buffer [13+4];
    memcpy(buffer + 4, cat_ihdr.p_data, 13);
    memcpy(buffer, cat_ihdr.type, 4);
    cat_ihdr.crc = (U32)crc(buffer, 13 + 4);

    // write the result file
    FILE * result = fopen("./all.png", "wb");
    fwrite(png_header, 8, 1, result);
    write_chunk(result, &cat_ihdr);
    write_chunk(result, &cat_idat);
    write_chunk(result, &cat_iend);
    fclose(result);

    // free pdata
    free(cat_ihdr.p_data);
    free(cat_idat.p_data);
    free(cat_iend.p_data);

    // free pdata
    for (int i = 1; i < argc; i++) { 
        free(idats[i-1].p_data);
    }
    free(idats);

    // printf("Successfully concatenated all files to ./all.png\n");

    return 0;
}

void write_chunk(FILE * fp, chunk_p chunk) {
    U32 network_len = htonl(chunk->length);
    fwrite(&network_len, 4, 1, fp);
    fwrite(chunk->type, 1, 4, fp);
    fwrite(chunk->p_data, 1, chunk->length, fp);
    U32 network_crc = htonl(chunk->crc);
    fwrite(&network_crc, 4, 1, fp);
}

int open_png(const char * file, chunk_p ihdr, chunk_p idat, chunk_p iend, U8 * png_header) {
    FILE * pngStream;

     if ((pngStream = fopen(file, "rb")) == NULL) {
        return -1;
    }
    if (check_png_header(png_header, pngStream) != 0) {
        return -1;
    }

    if (get_chunk(ihdr, pngStream, 0, SEEK_CUR) == -1 || get_chunk(idat, pngStream, 0, SEEK_CUR) == -1 || get_chunk(iend, pngStream, 0, SEEK_CUR) == -1)
        return -1;

    fclose(pngStream);

    return 0;
}