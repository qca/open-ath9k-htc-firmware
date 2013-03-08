#ifndef __EEPROM_API_H__
#define __EEPROM_API_H__

typedef enum {
	RET_SUCCESS = 0, 
	RET_NOT_INIT,
	RET_NOT_EXIST,
	RET_EEP_CORRUPT,
	RET_EEP_OVERFLOW,
    
	// add return code from here    
	RET_UNKNOWN
} T_EEP_RET;


/*!- interface of eeprom access
 *
 */
struct eep_api {
	void (* _eep_init)(void);
	T_EEP_RET (* _eep_read)(uint16_t, uint16_t, uint16_t *);
	T_EEP_RET (* _eep_write)(uint16_t, uint16_t, uint16_t *);
	T_EEP_RET (*_eep_is_exist)(void);
};

#endif /* __EEPROM_API_H__ */

