#ifndef __MISC_API_H__
#define __MISC_API_H__

enum hostif_s{
    HIF_USB = 0,
    HIF_PCIE,
    HIF_GMAC,
    HIF_PCI,

    // HIF should be added above here
    HIF_NUM,
    HIF_NONE

};

typedef enum hostif_s A_HOSTIF;

struct register_dump_s;

struct misc_api {
    void (* _system_reset)(void);
    void (* _mac_reset)(void);
    void (* _assfail)(struct register_dump_s *);
    void (* _misaligned_load_handler)(struct register_dump_s *);

    void (* _report_failure_to_host)(struct register_dump_s *, int);
    int (* _target_id_get)(void);
    A_HOSTIF (* _is_host_present)(void);

    uint8_t (*_kbhit)(uint8_t);

    uint16_t (* _rom_version_get)(void);
};
#endif /* __MISC_API_H__ */
