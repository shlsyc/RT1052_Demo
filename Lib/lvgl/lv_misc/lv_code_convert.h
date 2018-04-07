#ifndef LV_CODE_CONVERT_H
#define LV_CODE_CONVERT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>

uint16_t lv_convert (   /* Converted code, 0 means conversion error */
    uint16_t   src,     /* Character code to be converted */
    uint8_t    dir      /* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
);

uint16_t lv_wtoupper (  /* Upper converted character */
    uint16_t chr        /* Input character */
);

uint32_t lv_uni2utf8(uint32_t letter_uni);

uint32_t lv_gbk2utf8_content(uint16_t gbk_text);

void lv_gbk2utf8(uint8_t *uft8_text,const char * gbk_text);

uint8_t * lv_gbk2utf8_macro(const char * gbk_text);

#ifdef __cplusplus
}
#endif

#endif
