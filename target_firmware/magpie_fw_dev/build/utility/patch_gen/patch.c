#include <stdio.h>
#include "dt_defs.h"
#include "patch.h"

//extern unsigned short m_crc16ccitt;
uint16_t uRead = 0;
uint8_t patchBuf[SIZE_HASH_BUFFER];


int db_ascii_to_hex(char* num_str, unsigned long* hex_num)
{
    int i = 0;

    *hex_num = 0;
    while (num_str[i])
    {
        if ((num_str[i] >= '0') && (num_str[i] <= '9'))
        {
            *hex_num <<= 4;
            *hex_num += (num_str[i] - '0');
        }
        else if ((num_str[i] >= 'A') && (num_str[i] <= 'F'))
        {
            *hex_num <<= 4;
            *hex_num += (num_str[i] - 'A' + 10);
        }
        else if ((num_str[i] >= 'a') && (num_str[i] <= 'f'))
        {
            *hex_num <<= 4;
            *hex_num += (num_str[i] - 'a' + 10);
        }
        else
        {
            return -1;
        }
        i++;
    }
    return 0;
}


#if 0
void dump_hex(uint8_t *buf, uint16_t size)
{
	uint16_t i=0;


	for(i=0; i<size; i++)
	{
		if( (i%16==0) )
			printf("\n\r");
		else if( (i>0) && (i%8==0) )
			printf("- ");

		printf("%02x ", buf[i]);
	}
}
#endif


BOOLEAN gen_patch_item(struct rom_patch_st *patch, uint8_t *file_name)
{
	BOOLEAN retVal = FALSE;
	FILE *in;
	uint8_t *ptr;
	uint16_t size = 0;

  	if((in = fopen(file_name,"rb")) != NULL)
		retVal = TRUE;
	else
		goto ERR_DONE;

	ptr = patchBuf;
	while (1)
	{
		ptr += uRead;
		uRead = fread(ptr, 1, SIZE_HASH_BUFFER, in);
		
		// debug to dump the data we read
		//dump_hex(ptr, uRead);

		patch->len += uRead;

		if(uRead != SIZE_HASH_BUFFER) 
			break;
    }

	memcpy(patch->fun, patchBuf, patch->len);
	patch->crc16 = patch->len; // bugs? workaround?
	
	printf("\n\n");

	fclose(in);
ERR_DONE:

	return retVal;
}

BOOLEAN _patch_init(struct rom_patch_st *patch)
{
	// init the pact_pack
	//memset((uint8_t *)patch_patck, 0x0, sizeof(struct rom_patch_pack_st));
	patch->crc16 = 0;
	patch->len = 0;
	patch->ld_addr = 0;
	patch->fun_addr = 0;
	patch->fun = NULL;

	return TRUE;
}

void _patch_dump(struct rom_patch_st *patch)
{
	printf(" -----------------------------\n\r");
	printf(" patch code crc: 0x%04x\n\r", patch->crc16);
	printf(" patch code size: %d\n\r", patch->len);
	printf(" patch ld_addr: 0x%08x\n\r", patch->ld_addr);
	printf(" patch fun_addr: 0x%08x\n\r", patch->fun_addr);
	printf(" -----------------------------\n\r");
}


BOOLEAN _patch_oepn(uint8_t *buf, uint8_t *mFile)
{
	FILE *in;
	BOOLEAN retVal = FALSE;
	uint16_t offset = 0;

  	if((in = fopen(mFile,"rb")) != NULL)
	{
		printf("%s is opened successful!\n\r", mFile);
		retVal = TRUE;
	}
	else
		goto ERROR;

	while (1)
	{
		uRead = fread(buf+offset, 1, SIZE_HASH_BUFFER, in);

		// debug to dump the data we read
		//dump_hex(buf+offset, uRead);

		offset += uRead;
		if(uRead != SIZE_HASH_BUFFER) break;

	}
ERROR:
	if(in)
		close(in);

	return retVal;
}

uint32_t _patch_append(uint8_t *buf, struct rom_patch_st *patch)
{

	memcpy(buf, (uint8_t *)patch, (sizeof(struct rom_patch_st)-4));
	memcpy(buf+(sizeof(struct rom_patch_st)-4), patch->fun, patch->len);

	return 0;
}


