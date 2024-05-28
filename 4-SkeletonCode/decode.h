#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding txt file from bmp file
 * Info about output and intermediate data is
 * also stored
 */


#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4


typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* text File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char secret_extn[MAX_FILE_SUFFIX];
    int extn_size;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    int size_secret_file;


} DecodeInfo;

/* Decoding function prototype */

/* read_and_validate_decode_args */

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(char *argv[],DecodeInfo *decInfo);

/* Open bmp file */
Status open_bmp_file(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extenstion size */
Status decode_extn_size(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(int size, DecodeInfo *decInfo);

/* Open .txt file */
Status open_secret_file(char *argv[], DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode function, which does the real decoding */
Status decode_image_to_data(char *str, int size, DecodeInfo *decInfo);

/* Decode LSB of image data array into character or byte */
Status decode_lsb_to_char(DecodeInfo *decInfo, char *one_char);

/*Decode LSB's of images data in to size */

Status decode_lsb_to_size(DecodeInfo *decInfo);


















#endif
