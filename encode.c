#include<stdio.h>
#include<string.h>
#include"encode.h"
#include"types.h"
#include"common.h"


Status read_and_validate_encode_args(int argc,char* argv[],EncodeInfo* encInfo){
    int len1=strlen(argv[2]);
    if(argv[2][len1-4]=='.' && argv[2][len1-3]=='b' && argv[2][len1-2]=='m' && argv[2][len1-1]=='p'){
        encInfo->src_img_fname=argv[2];
    }
    else{
        return e_failure;
    }
    
    char* dot=strrchr(argv[3],'.');  //to get the last dot in the secret file
    if(dot !=NULL && (strcmp(dot,".txt")==0 || strcmp(dot,".csv")==0 || strcmp(dot,".py")==0)){
        encInfo->secret_fname=argv[3];   //secret_fname-->secret.txt
        strcpy(encInfo->extn_secret_file,dot);  // copies ".txt" file into encInfo->extn_secret_file
    }
    else{
        return e_failure;
    }
    if(argc > 4){
        int len3=strlen(argv[4]);
        if(argv[4][len3-4]=='.' && argv[4][len3-3]=='b' && argv[4][len3-2]=='m' && argv[4][len3-1]=='p'){
            encInfo->stego_img_fname=argv[4];
        }
        else{
            return e_failure;
        }
    }
    else{
        encInfo->stego_img_fname="stego_img.bmp";
        printf("Output file name not mentioned,so given a default name");
        printf("\n");
    }
    return e_success;
    }
    
    Status open_files(EncodeInfo* encInfo){
        encInfo->fptr_src_img=fopen(encInfo->src_img_fname,"rb");
                                 //fopen("beautiful.bmp","rb")
        if(encInfo->fptr_src_img==NULL){
            perror("fopen");  //eg: fopen:No such file or directory
//The operating system sets an internal error code (errno) indicating:
//No such file or directory

            fprintf(stderr,"ERROR:Unable to open file %s\n",encInfo->src_img_fname);// outputs: ERROR:Unable to open file beautiful.bmp
            return e_failure;
        }
        
        encInfo->fptr_secret=fopen(encInfo->secret_fname,"rb");
                 // fopen("secret.txt","rb")
        if(encInfo->fptr_secret==NULL){
            perror("fopen");
            fprintf(stderr,"ERROR:Unable to open file %s\n",encInfo->secret_fname);
            return e_failure;
        }
        
        encInfo->fptr_stego_img=fopen(encInfo->stego_img_fname,"wb");
        if(encInfo->fptr_stego_img == NULL){
        perror("fopen");
        fprintf(stderr,"ERROR:Unable to open file %s\n",encInfo->stego_img_fname);
            return e_failure;
        }
        return e_success;
        
    }
    
    uint get_image_size_for_bmp(FILE* fptr_image){
        uint width,height;
        
        //seek to 18th byte
        fseek(fptr_image,18,SEEK_SET); // Move the file pointer to 18 bytes from the beginning of the file.
        //fseek(FILE *stream, long offset, int origin);
        fread(&width,sizeof(int),1,fptr_image);//read the width
        fread(&height,sizeof(int),1,fptr_image); //read the height
        
        return width*height*3;
        //multiplied by 3 since every single pixel is actually made of a cluster of three bytes R G B 
    
    }
    
    uint get_file_size(FILE* fptr_secret){
        fseek(fptr_secret,0l,SEEK_END); // position the cursor at last byte of file,0 as a long integer(move 0 bytes from the end)
        long size=ftell(fptr_secret); // std c giving the byte number to which the cursor points to.
        rewind(fptr_secret); //resets the cursor back to 0 (ie begining)
        return size;
        
    }
    
    Status check_capacity(EncodeInfo* encInfo){
    encInfo->img_capacity=get_image_size_for_bmp(encInfo->fptr_src_img);
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
    printf("Image capacity = %u\n", encInfo->img_capacity);
printf("Secret file size = %ld\n", encInfo->size_secret_file);
    if(encInfo->img_capacity > 54 +strlen(MAGIC_STRING)*8 +32+strlen(encInfo->extn_secret_file)*8 +32+ encInfo->size_secret_file *8){
        return e_success;
    }
    else{
        return e_failure;
    }
    }
    
    //To copy the header part form src img to the stego img
    Status copy_bmp_header(FILE* fptr_src_img,FILE* fptr_stego_img){
        char image_buffer[54];
        rewind(fptr_src_img);
        if(fread(image_buffer,54,1,fptr_src_img)!=1){
            return e_failure;
        }
        if(fwrite(image_buffer,54,1,fptr_stego_img)!=1){
            //handing you 1 single block of data, and that block is 54 bytes long
            return e_failure;
        }
        return e_success;
    }
    
    Status encode_byte_to_lsb(char data,char* image_buffer){
        for(int i=0;i<8;i++){
        // step1:get the msb bit in data
        // step2:clear the lsb bit in image_buffer[i]
        ////step3:step1|step2
            int bit=(data >>(7-i))& 1;
            image_buffer[i]=image_buffer[i] & 0xFE;
            image_buffer[i]=image_buffer[i] | bit;
        }
        return e_success;
    }
    
    Status encode_data_to_image(const char* data,int size,FILE* src,FILE* dest){
        char image_buffer[8];
        for(int i=0;i<size;i++){
            if(fread(image_buffer,1,8,src)!=8){ //reads 8 img bytes from src and store it inside the image_buffer
                return e_failure;
            }
            
            //Modifying the LSBs of all the 8 image bytes
            if(encode_byte_to_lsb(data[i],image_buffer)!=e_success){
                return e_failure;
            }
            // writing the modified 8 bytes into output.bmp file
            if(fwrite(image_buffer,1,8,dest)!=8){
                return e_failure;
            }
        }
        return e_success;
    }
    
    Status encode_magic_string(const char* magic_str,EncodeInfo* encInfo){
        if(encode_data_to_image(magic_str,strlen(magic_str),encInfo->fptr_src_img,encInfo->fptr_stego_img)==e_success){
            return e_success;
        }
        else{
            return e_failure;
        }
    }
    Status encode_size_to_lsb(int data,char* image_buffer){
        for(int i=0;i<32;i++){
            //get the msb bit of the data
            //clear the lsb of image_buffer
            //take or operation of the two
            int bit=(data>>(31-i)) & 1;
            image_buffer[i]=image_buffer[i] & 0xFE;
            image_buffer[i]=image_buffer[i] | bit;
        }
        return e_success;
    }
    
    Status encode_secret_file_extn_size(long extn_size,EncodeInfo* encInfo){
        char image_buffer[32];
        if(fread(image_buffer,1,32,encInfo->fptr_src_img)!=32){
            return e_failure;
        }
        if(encode_size_to_lsb(extn_size,image_buffer)==e_failure){
           return e_failure;
        }
        if(fwrite(image_buffer,1,32,encInfo->fptr_stego_img)!=32){
            return e_failure;
        }
        return e_success;
    }
    
    Status encode_secret_file_extn(const char* extn,EncodeInfo* encInfo){
        if(encode_data_to_image(extn,strlen(extn),encInfo->fptr_src_img,encInfo->fptr_stego_img)==e_failure){
            return e_failure;
        }
        else{
            return e_success;
        }
    }

    Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
     char image_buffer[32];
    // step1 :read 32 bytes fromm the src
    // step2: call size_to_lsb(file_size,image_buffer);
    // step3: write 32 bytes to the dest
     if(fread(image_buffer,1,32,encInfo->fptr_src_img)!=32)
     {
        return e_failure;
     }
     if(encode_size_to_lsb(file_size,image_buffer)==e_failure)
     {
        return e_failure;
     }
     if(fwrite(image_buffer,1,32,encInfo->fptr_stego_img)!=32)
     {
        return e_failure;
     }
     return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char data[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
    // read the contents from the file
    if (fread(data, 1, encInfo->size_secret_file, encInfo->fptr_secret) != encInfo->size_secret_file)
    {
       printf("secret file contents are not read correctly\n");
        return e_failure;
    }
    if(encode_data_to_image(data, encInfo->size_secret_file, encInfo->fptr_src_img,encInfo->fptr_stego_img)==e_success)
    {
    return e_success;
    }
    else
    {
        return e_failure;
    }

    printf("Size stored = %ld\n", encInfo->size_secret_file);

rewind(encInfo->fptr_secret);

printf("Cursor after rewind = %ld\n", ftell(encInfo->fptr_secret));

size_t bytes = fread(data, 1,
                     encInfo->size_secret_file,
                     encInfo->fptr_secret);

printf("Requested = %ld\n", encInfo->size_secret_file);
printf("Read = %zu\n", bytes);
}

Status copy_remaining_img_data(FILE *src, FILE *dest)
{
    // char ch;
    // while(//read byte by byte upto eof)
    //     {
    //     // write byte  by byte to the dest_img
    //     }

    int ch;
    while ((ch = fgetc(src)) != EOF)  // read one byte
    {
        if (fputc(ch, dest) == EOF)   // write one byte
        {
            return e_failure;         // write error
        }
    }
    return e_success;
}
    
    Status do_encoding(EncodeInfo* encInfo){
        printf("DEBUG: opening files in binary mode\n");
        if(open_files(encInfo)==e_failure){
            printf("error\n");
            return e_failure;
        }
        else{
            printf("Files are opened successfully\n");
            printf("\n");
        }
        
        if(check_capacity(encInfo)==e_failure){
        printf("checking file capacity is failed\n");
        return e_failure;
        }
        else{
            printf("checking files have been successfully completed\n");
        }
        
        if(copy_bmp_header(encInfo->fptr_src_img,encInfo->fptr_stego_img)==e_failure){
            printf("header is not copied \n");
            return e_failure;
        }
        else{
            printf("Copy_header is successfully completed \n");
        }
        
        if(encode_magic_string(MAGIC_STRING,encInfo)==e_failure){
            printf("magic string encoding failed \n");
        }
        else{
            printf("magic string encoding successfull \n");
            printf("\n");
        }
        
        if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_failure){
            printf("secret file extn size not encoded \n");
            printf("\n");
        }
        else{
            printf("secret file extn successfully encoded \n");
            printf("\n");
        }
        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)== e_failure){
            printf("Secret file could not be encoded \n");
            printf("\n");
        }
        else{
            printf("Secret file extention encoded succesfully \n");
            printf("\n");
        }

        if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
{
    printf("Secret file size could not be encoded \n");
    return e_failure;
}
else
{
    printf("Secret file size encoded successfully \n");
}

if (encode_secret_file_data(encInfo) == e_failure)
{
    printf("Secret file data could not be encoded \n");
    return e_failure;
}
else
{
    printf("Secret file data encoded successfully \n");
}

if (copy_remaining_img_data(encInfo->fptr_src_img,
                            encInfo->fptr_stego_img) == e_failure)
{
    printf("Remaining image data could not be copied \n");
    return e_failure;
}
else
{
    printf("Remaining image data copied successfully \n");
}

return e_success;
        
    }