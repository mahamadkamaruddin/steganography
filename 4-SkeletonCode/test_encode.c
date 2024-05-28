#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char **argv)
{
	if (argc > 1)
	{
		if(check_operation_type(argv) == e_encode )
		{
			printf("Selected encoding\n");
			
			EncodeInfo encinfo;

			if(read_and_validate_encode_args(argv, &encinfo) == e_success)
			{
				printf("Read and validate enocode args is success\n");

				if(do_encoding(&encinfo) == e_success)
				{
					printf("Do encoding is successfull\n");
								
				}
				else
				{
					printf("do encoding isn't successfull\n");
					return 1;
				}

			}
			else
			{
				printf("read and validate encode args has failed\n");
				return 1;
			}

		}
		else if(check_operation_type(argv) == e_decode )
		{
			printf("Selected decoding\n");
			DecodeInfo decinfo;

			if(read_and_validate_decode_args(argv, &decinfo) == e_success)
			{
				printf("Read and validate of decode args in successfull\n");

				if(do_decoding(argv, &decinfo) == e_success)
				{
					printf("Do decoding is successfull\n");
								
				}
				else
				{
					printf("failed in do decoding\n");
					return 1;
				}
			}
			else
			{
				printf("Failed to read and valiadte decode args\n");
				return 1;
			}

		}
		else
		{
			printf("Invalid input format !!\n For encoding: \n ./a.out -e beautiful.bmp secret.txt stego.bmp\nFor decoding:\n ./a.out -d stego.bmp decode.txt\n");
		}
		

	}
	else
	{
			printf("Invalid input format !!\n For encoding: \n ./a.out -e beautiful.bmp secret.txt stego.bmp\nFor decoding:\n ./a.out -d stego.bmp decode.txt\n");
	}

    return 0;
}

