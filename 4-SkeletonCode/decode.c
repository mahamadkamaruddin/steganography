#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"



/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file
 * Output: secret file
 * Return Value: e_success or e_failure, on file errors
 */


Status open_bmp_file(DecodeInfo *decInfo)
{
	//Src Image file
	decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
	// Do Error handling
	if (decInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

		return e_failure;
	}

	return e_success;
}

Status open_secret_file(char *argv[], DecodeInfo *decInfo)
{

	if(argv[3] == NULL)
		decInfo -> secret_fname = "decode.txt";
	else
	{
		if(strcmp(strchr(argv[3],'.'),decInfo->secret_extn))
		{
			decInfo-> secret_fname = argv[3];
		}
		else
		{
			decInfo -> secret_fname = "decode.txt";	
		}

	}

	// Secret file
	decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
	// Do Error handling
	if (decInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
		return e_failure;
	}
	// No failure return e_success
	return e_success;
}





Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	if(strcmp(strchr(argv[2], '.'), ".bmp") == 0)
		decInfo -> src_image_fname = argv[2];
	else
		return e_failure;

	return e_success;
}


Status decode_lsb_to_char(DecodeInfo *decInfo, char *one_char)
{
	for(int i=0; i<8;i++)
	{
		*one_char = ((decInfo -> image_data[i]) & 1) | *one_char;

		if(i<7)
		{
			*one_char = (*one_char << 1);
		}
	}
	return e_success;
}


//function which decodes image file data into charcters or bytes.

Status decode_image_to_data(char *str, int size, DecodeInfo *decInfo)
{
	for(int i=0; i<size; i++)
	{
		unsigned char one_char = 0;

		fread(decInfo -> image_data, 8, 1, decInfo -> fptr_src_image);

		decode_lsb_to_char(decInfo, &one_char);

		str[i] = one_char;
	}
	return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{	
	//setting pointer to byte 54, from 54th byte data is encoded
	fseek(decInfo -> fptr_src_image, 54, SEEK_SET);

	int size =2;

	//string to store decoded magic string
	char magic_str[size];

	decode_image_to_data(magic_str, size, decInfo);


	//comparing magic_str which is obtained from .bmp file with actual MAGIC_STRING (#*)
	if(strcmp(magic_str,MAGIC_STRING))
		return e_success;
	else
		return e_failure;
}


Status decode_lsb_to_size(DecodeInfo *decInfo)
{
	int size =4;
	char size_extn[size];

	decode_image_to_data(size_extn, size, decInfo);

	int x = (size_extn[3]);
	decInfo->extn_size = x;
	return e_success;

}

Status decode_extn_size(DecodeInfo *decInfo)
{	
	decode_lsb_to_size(decInfo);
   return e_success;	
	
}

Status decode_secret_file_extn(int size, DecodeInfo *decInfo)
{
	char str[size];
	decode_image_to_data(decInfo->secret_extn, size, decInfo);

	return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
	int size=4;
	char scrt_file_size[size];

	decode_image_to_data(scrt_file_size, size, decInfo);

	int x = (scrt_file_size[3]);
	decInfo->size_secret_file = x;
	return e_success;
}


Status decode_secret_file_data(DecodeInfo *decInfo)
{
	int size = decInfo->size_secret_file;
	char data[size];

	decode_image_to_data(data, size, decInfo);
	
	fwrite(data, 25, 1, decInfo->fptr_secret); 
	return e_success;
}

Status do_decoding(char *argv[], DecodeInfo *decInfo)
{
	if(open_bmp_file(decInfo) == e_success)
	{
		printf("open files successfull\n");

		if(decode_magic_string(decInfo) == e_success)
		{
			printf("decoded magic string\n");

			if(decode_extn_size(decInfo) == e_success)
			{
					printf("decoded extn size\n");
					if(decode_secret_file_extn(decInfo->extn_size, decInfo) == e_success)
					{
						printf("decoded_secret_file_extn\n");
						if(open_secret_file(argv, decInfo) == e_success)
						{
							printf("opend secret file for writing\n");
							if(decode_secret_file_size(decInfo) == e_success)
							{
								printf("decoded secret file size\n");
								if(decode_secret_file_data(decInfo) == e_success)
								{
									printf("Decoded all data successfully\n");
								}
								else
								{
									printf("failed to decode the data\n");
									return e_failure;
								}
							}
							else
							{
								printf("failed to decode secret file size\n");
								return e_failure;
							}
						}
						else
						{
							printf("failed to open secret file \n");
							return e_failure;
						}
					}
					else
					{
						printf("failed to decode secret file extension\n");
						return e_failure;
					}
	
			}
			else
			{
				printf("failed to decode extn size\n");
				return e_failure;
			}
		}
		else
		{
			printf("failed to decode the magic string\n");
			return e_failure;
		}
	}
	else
	{
		printf("failed to open files\n");
		return e_failure;
	}
	return e_success;
}
