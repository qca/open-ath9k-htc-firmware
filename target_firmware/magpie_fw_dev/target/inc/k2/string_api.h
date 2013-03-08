#ifndef __STRING_API_H__
#define __STRING_API_H__

struct string_api {
	void (* _string_init)(void);
	char *(* _strcpy)(char *, const char *);
	char *(* _strncpy)(char *, const char *, unsigned int);
	int (* _strlen)(const char *);
	int (* _strcmp)(const char *, const char *);
	int (* _strncmp)(const char *, const char *, unsigned int);
};

#endif /* __STRING_API_H__ */
