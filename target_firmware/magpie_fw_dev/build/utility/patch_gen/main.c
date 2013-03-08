#include <stdio.h>
#include <time.h>
#include <string.h>
#include "dt_defs.h"
#include "patch.h"

#define SIZE_HASH_BUFFER	4096

#define ROM_SIZE	32

uint8_t pBuf[SIZE_HASH_BUFFER];

//unsigned short m_crc16ccitt;

void print_help()
{
	printf("\n\rThis utility is used to prefix a header to the patch code!\n\r");
	printf("For loading into the bootcode and the integrity crc header!\n\r"); 
	printf("    - patch_hdr [patch binary] [load address]\n\r");
	printf("\n\r   e.g. patch_hdr patch.bin 0x8e0000\n\r");
}


#define _ROMP_SIZE_	64

int main(int argc, char* argv[] )
{
	FILE *out;
	FILE *in;

	struct rom_patch_st patch_item;
	uint8_t *pBufPtr = NULL;
	uint32_t ran_patch_size = 0;
	uint8_t *ld_addr;
	uint8_t *fun_addr;
	uint8_t name_of_patch[32];

	// init the patch 
	_patch_init(&patch_item);

	if(argc==2)
	{
		_patch_oepn(pBuf, argv[1]);
		_patch_dump(pBuf);

		goto ERROR;
	}
	else if (argc<3)
		goto ERROR;
	else
		printf("%s %s %s\n\r", argv[0], argv[1], argv[2]);



	/***************************************************************/
	// allocate the buffer space for func address
	if( !(patch_item.fun = (uint8_t *)malloc(SIZE_HASH_BUFFER)) )
		goto ERROR;

	// check the function address of the patch code
	ld_addr = argv[2];
	if( db_ascii_to_hex((ld_addr+2), &(patch_item.ld_addr))!=0 )
		goto ERROR;

	// check the function address of the patch code
	fun_addr = argv[3];
	if( db_ascii_to_hex((fun_addr+2), &(patch_item.fun_addr))!=0 )
		goto ERROR;

	/***************************************************************/

	if( gen_patch_item(&patch_item, argv[1])!=TRUE )
		goto ERROR;

	printf(" - prefix to the patch binary -\n\r");
	_patch_dump(&patch_item);

	sprintf(name_of_patch, "h_%s", argv[1]);

	printf("%s is to be outputed\n\r", name_of_patch);

	_patch_append(pBuf, &patch_item);

	out = fopen(name_of_patch, "wb");

	// ( struct size -4 ) + patch_code size + 4-%4( make it alignment ) 
	printf(" write %d bytes to h_%s as an output file", (sizeof(struct rom_patch_st)-4)+patch_item.len+(4-(patch_item.len%4)), argv[1]);
	fwrite((uint8_t *)pBuf, sizeof(uint8_t), (sizeof(struct rom_patch_st)-4)+patch_item.len+(4-(patch_item.len%4)), out);

	fclose(out);

	goto DONE;

ERROR:
	print_help();

DONE:
	if( patch_item.fun )
		free(patch_item.fun);

	return 0;
}
