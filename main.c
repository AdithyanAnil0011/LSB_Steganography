#include"stdio.h"
#include"encode.h"
#include"decode.h"
#include"types.h"
#include"common.h"
#include<string.h>

OperationType check_operation_type(char* argv){
    if(strcmp(argv,"-e")==0){
        return e_encode;
    }
    else if(strcmp(argv,"-d")==0){
        return e_decode;
    }
    else{
        return e_unsupported;
    }
}
int main(int argc,char* argv[]){
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if(argc <2){
        printf("enter a valid argument");
        printf("for encoding: min 4 arguments");
        printf("for decoding: min 3 argumenst");
        return 1;
    }
    
    OperationType res=check_operation_type(argv[1]);  // operationtype is userdefined datatype declared inside the type.c file as a enum
    if(res==e_encode){

          if(argc < 4)   
        {
            printf("ERROR: Encoding requires at least 4 arguments.\n");
            printf("\n");
            printf("INFO  :  Please enter like this %s -e <source_image> <secret_file> <stego_image>\n", argv[0]);
            return 1;
        }

        printf("Selected Operation :Encoding ");
        printf("\n");
        
        if(read_and_validate_encode_args(argc,argv,&encInfo)==e_success){
            printf("Run and Validation is successfully completed for encoding");
            printf("\n");
            if(do_encoding(&encInfo)==e_success){
                printf("----------\n");
                printf("Encoding is completed\n");
                printf("-------\n");
            }
            else{
                printf("encoding is not done\n");
            }
        }
    }
   else if( res==e_decode){
       printf("Selected Operation: Decoding\n");
       if(read_and_validate_decode_args(argc,argv,&decInfo)==d_success){
           printf("Run and Validate is successfully completed for decoding\n");
           if(do_decoding(&decInfo)==d_success){
               printf("------\n");
               printf("Decoding is completed\n");
               printf("-------\n");
           }
           else{
               printf("decoding is not done\n");
           }
       }
   }
    
    return 0;
}