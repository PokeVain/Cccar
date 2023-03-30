#ifndef __DSPFunction_H_
#define __DSPFunction_H_

#include "zf_common_font.h"
#include "FontFunction.h"

#include "zf_device_ips114.h"
#include "zf_driver_spi.h"



typedef unsigned char      DSP_Uchar8Type;
typedef unsigned short int DSP_Uint16Type;
typedef unsigned int       DSP_Uint32Type;


#define DSP_WHITE           0xFFFF  // 白色
#define DSP_BLACK           0x0000  // 黑色
#define DSP_BLUE            0x001F  // 蓝色
#define DSP_PURPLE          0XF81F  // 紫色
#define DSP_PINK            0XFE19  // 粉色
#define DSP_RED             0xF800  // 红色
#define DSP_MAGENTA         0xF81F  // 品红
#define DSP_GREEN           0x07E0  // 绿色
#define DSP_CYAN            0x07FF  // 青色
#define DSP_YELLOW          0xFFE0  // 黄色
#define DSP_BROWN           0XBC40  // 棕色
#define DSP_GRAY            0X8430  // 灰色


#define         DSP_Clear(color)                ips114_clear(color)
#define         DSP_Write_8Bit(Data)            spi_write_8bit(SPI_2, Data)
#define         DSP_Write_16Bit(Data)           spi_write_16bit(SPI_2, Data)
#define         DSP_Set_Region(X1,Y1,X2,Y2)     ips114_set_region(X1,Y1,X2,Y2)



void ClearScreen_DSPFunction(
                                DSP_Uint16Type X,
                                DSP_Uint16Type Y,
                                DSP_Uint16Type Width,
                                DSP_Uint16Type Hight,
                                DSP_Uint16Type Color
                            );

void ShowLoge_DSPFunction(  const unsigned char *Sprt,
                            DSP_Uint16Type X,
                            DSP_Uint16Type Y,
                            DSP_Uint16Type Width,
                            DSP_Uint16Type Hight);

void ShowChiese_DSPFunction(
                                DSP_Uint32Type x,
                                DSP_Uint32Type y,
                                DSP_Uint32Type fc,
                                DSP_Uint32Type bc,
                                DSP_Uchar8Type size,
                                DSP_Uchar8Type *p   );

void GuideChese_DSPFunction(    DSP_Uint32Type x,
                                DSP_Uint32Type y,
                                DSP_Uint32Type fc,
                                DSP_Uint32Type bc,
                                DSP_Uchar8Type *s   );

#endif







