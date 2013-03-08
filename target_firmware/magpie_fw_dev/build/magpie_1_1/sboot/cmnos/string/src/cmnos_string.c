#include "athos_api.h"

/* Common string functions, provided by toolchain libraries. */

extern char *strcpy(char *s1, const char *s2);
extern char *strncpy(char *s1, const char *s2, unsigned int n);
extern int   strcmp(const char *s1, const char *s2);
extern int   strncmp(const char *s1, const char *s2, unsigned int n);
extern int   strlen(const char *s);

LOCAL void
cmnos_string_init(void)
{
}

void
cmnos_string_module_install(struct string_api *tbl)
{
    tbl->_string_init           = cmnos_string_init;
    tbl->_strcpy                = strcpy;
    tbl->_strncpy               = strncpy;
    tbl->_strlen                = strlen;
    tbl->_strcmp                = strcmp;
    tbl->_strncmp               = strncmp;
}
