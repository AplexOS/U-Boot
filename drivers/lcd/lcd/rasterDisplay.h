
#ifndef _RASTER_DISPLAY_H_
#define _RASTER_DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

void Lcd_Init(unsigned int image,unsigned int image_size);
void Lcd_reset(void);
void Lcd_off(void);
void Lcd_on(void);

#ifdef __cplusplus
}
#endif

#endif 
