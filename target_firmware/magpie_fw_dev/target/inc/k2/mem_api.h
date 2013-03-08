#ifndef __MEM_API_H__
#define __MEM_API_H__

struct mem_api {
	void (* _mem_init)(void);
	void *(* _memset)(void *, int, unsigned int);
	void *(* _memcpy)(void *, const void *, unsigned int);
	void *(* _memmove)(void *, const void *, unsigned int);
	int (* _memcmp)(const void *, const void *, unsigned int);
};

#endif /* __MEM_API_H__ */
