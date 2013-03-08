#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

/* These names may vary but this is typical */
extern const uint32_t zcFwImage[];
extern const uint32_t zcFwImageSize;

int main()
{
	uint32_t i;
	for (i = 0; i < zcFwImageSize/4; i++)
		write(1, &zcFwImage[i], 4);
}
