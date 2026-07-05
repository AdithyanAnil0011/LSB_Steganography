#include<stdio.h>
#include "types.h"
#ifndef ENCODE_H
#define ENCODE_H

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMG_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct EncodeInfo{
    
    //src image informations
    
    char* src_img_fname;   //stores the src image bmp file
    FILE* fptr_src_img;    // stores the file pointer ,which helps to read the                                    //image file
    uint img_capacity;   // how much data image can hold
    uint bits_per_pixel;     // its read from bmp header
    char img_data [MAX_IMG_BUF_SIZE];  // 
    
    // secret file info
    
    char* secret_fname;    //stores secret.txt 
    FILE* fptr_secret;      //pointer to secret.txt ,used to read secret data
    char extn_secret_file[MAX_FILE_SUFFIX];   // stores the .txt(for decoded img)
    char secret_data[MAX_SECRET_BUF_SIZE];     //temp storage for secret characters
    long size_secret_file;   // stores the size of secret file
    
    // stego img info
    
    char* stego_img_fname;
    FILE* fptr_stego_img;
} EncodeInfo;

// check operation type
OperationType check_operation_type(char* argv);

// read and validate encode args from argv
Status read_and_validate_encode_args(int argc,char* argv[],EncodeInfo *encInfo);

// perform the encoding
Status do_encoding(EncodeInfo* encInfo);

// get file pointers for i/p and o/p files
Status open_files(EncodeInfo* encInfo);

// check capacity
Status check_capacity(EncodeInfo* encInfo);

// get image size
uint get_image_size_for_bmp(FILE* fptr_image);

// get file size
uint get_file_size(FILE* fptr);

// copy bmp image header
Status copy_bmp_header(FILE* fptr_src_image,FILE* fptr_dest_image);

// store magic string
Status encode_magic_string(const char* magic_string,EncodeInfo* encInfo);

// encode secret file extention
Status encode_secret_file_extn(const char* file_extn,EncodeInfo* encInfo);

// encode secret file size
Status encode_secret_file_size(long file_size,EncodeInfo* encInfo);

Status encode_secret_file_extn_size(long file_size,EncodeInfo* encInfo);

//Encode secret file data
Status encode_secret_file_data(EncodeInfo* encode);
//Encode function which does the real encoding
Status encode_data_to_image(const char* data,int size,FILE* fptr_src_image,FILE* fptr_stego_image);
//Encode a byte into lsb of image data array
Status encode_byte_to_lsb(char data,char* image_buffer);
Status encode_size_to_lsb(int data,char* image_buffer);

//copy remaining image bytes fromsrc to stego image after encoding
Status copy_remaining_img_data(FILE* fptr_src,FILE*fptr_dest);

#endif