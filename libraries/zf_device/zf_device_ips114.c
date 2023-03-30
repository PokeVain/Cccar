/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_ips114
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�                                       ��Ƭ���ܽ�
*                   SCL                 �鿴 zf_device_ips114.h �� IPS114_SCL_PIN �궨��
*                   SDA                 �鿴 zf_device_ips114.h �� IPS114_SDA_PIN �궨��
*                   RST                 �鿴 zf_device_ips114.h �� IPS114_RST_PIN �궨��
*                   DC                  �鿴 zf_device_ips114.h �� IPS114_DC_PIN �궨��
*                   CS                  �鿴 zf_device_ips114.h �� IPS114_CS_PIN �궨��
*                   BLK                 �鿴 zf_device_ips114.h �� IPS114_BLK_PIN �궨��
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ���ֱ���135*240
*                   ------------------------------------
********************************************************************************************************************/


#include "zf_device_ips114.h"

uint16 ips114_pencolor = IPS114_DEFAULT_PENCOLOR;
uint16 ips114_bgcolor = IPS114_DEFAULT_BGCOLOR;

ips114_dir_enum ips114_display_dir = IPS114_DEFAULT_DISPLAY_DIR;
uint8 ips114_x_max = 240;
uint8 ips114_y_max = 135;

#if IPS114_USE_SOFT_SPI
static soft_spi_info_struct             ips114_spi;
#define ips114_write_8bit_data(data)    soft_spi_write_8bit(&ips114_spi, data)
#define ips114_write_16bit_data(data)   soft_spi_write_16bit(&ips114_spi, data)
#else
#define ips114_write_8bit_data(data)    spi_write_8bit(IPS114_SPI, data)
#define ips114_write_16bit_data(data)   spi_write_16bit(IPS114_SPI, data)


//#define ips114_write_8bit_data(data)    (IPS114_CS(0),IPS114_DC(1),spi_write_8bit(IPS114_SPI, data),IPS114_CS(1))
//#define ips114_write_16bit_data(data)   (IPS114_CS(0),IPS114_DC(1),spi_write_16bit(IPS114_SPI, data),IPS114_CS(1))
#endif

//-------------------------------------------------------------------------------------------------------------------
// @brief       д���� �ڲ�����
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
void ips114_write_index (uint8 dat)
{
    IPS114_CS(1);
    IPS114_CS(0);
    IPS114_DC(0);
    ips114_write_8bit_data(dat);
    IPS114_DC(1);
    IPS114_CS(1);
    IPS114_CS(0);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ���� �ڲ�����
// @param       x1              ��ʼx������
// @param       y1              ��ʼy������
// @param       x2              ����x������
// @param       y2              ����y������
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
void ips114_set_region (uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
//    zf_assert(x1 < ips114_x_max);
//    zf_assert(y1 < ips114_y_max);
//    zf_assert(x2 < ips114_x_max);
//    zf_assert(y2 < ips114_y_max);

    if(ips114_display_dir==IPS114_PORTAIT)
    {
        ips114_write_index(0x2a);                                               // �е�ַ����
        ips114_write_16bit_data(x1+52);
        ips114_write_16bit_data(x2+52);
        ips114_write_index(0x2b);                                               // �е�ַ����
        ips114_write_16bit_data(y1+40);
        ips114_write_16bit_data(y2+40);
        ips114_write_index(0x2c);                                               // ������д
    }
    else if(ips114_display_dir==IPS114_PORTAIT_180)
    {
        ips114_write_index(0x2a);                                               // �е�ַ����
        ips114_write_16bit_data(x1+53);
        ips114_write_16bit_data(x2+53);
        ips114_write_index(0x2b);                                               // �е�ַ����
        ips114_write_16bit_data(y1+40);
        ips114_write_16bit_data(y2+40);
        ips114_write_index(0x2c);                                               // ������д
    }
    else if(ips114_display_dir==IPS114_CROSSWISE)
    {
        ips114_write_index(0x2a);                                               // �е�ַ����
        ips114_write_16bit_data(x1+40);
        ips114_write_16bit_data(x2+40);
        ips114_write_index(0x2b);                                               // �е�ַ����
        ips114_write_16bit_data(y1+53);
        ips114_write_16bit_data(y2+53);
        ips114_write_index(0x2c);                                               // ������д
    }
    else
    {
        ips114_write_index(0x2a);                                               // �е�ַ����
        ips114_write_16bit_data(x1+40);
        ips114_write_16bit_data(x2+40);
        ips114_write_index(0x2b);                                               // �е�ַ����
        ips114_write_16bit_data(y1+52);
        ips114_write_16bit_data(y2+52);
        ips114_write_index(0x2c);                                               // ������д
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����������
// @param       color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @return      void
// Sample usage:                ips114_clear(YELLOW);
//-------------------------------------------------------------------------------------------------------------------
void ips114_clear (uint16 color)
{
    uint32 i = ips114_x_max*ips114_y_max;

    ips114_set_region(0,0,ips114_x_max-1,ips114_y_max-1);
    for( ; i>0; i--)
    {
        ips114_write_16bit_data(color);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ���� �������ֻ���ڳ�ʼ����Ļ֮ǰ���ò���Ч
// @param       dir             ��ʾ����  ���� zf_device_ips114.h �� ips114_dir_enum ö���嶨��
// @return      void
// Sample usage:                ips114_set_dir(IPS114_CROSSWISE);
//-------------------------------------------------------------------------------------------------------------------
void ips114_set_dir (ips114_dir_enum dir)
{
    ips114_display_dir = dir;
    if(dir < 2)
    {
        ips114_x_max = 135;
        ips114_y_max = 240;
    }
    else
    {
        ips114_x_max = 240;
        ips114_y_max = 135;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ��ɫ
// @param       pen             ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @param       bgcolor         ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @return      void
// Sample usage:                ips114_set_color(WHITE,BLACK);
//-------------------------------------------------------------------------------------------------------------------
void ips114_set_color (uint16 pen, uint16 bgcolor)
{
    ips114_pencolor = pen;
    ips114_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ������
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ����ɫ
// @return      void
// Sample usage:                ips114_draw_point(0,0,RED);                     // ���� 0,0 ��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void ips114_draw_point (uint16 x,uint16 y,uint16 color)
{
//    zf_assert(x < ips114_x_max);
//    zf_assert(y < ips114_y_max);

    ips114_set_region(x,y,x,y);
    ips114_write_16bit_data(color);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ������
// @param       x_start         ����x��������
// @param       y_start         ����y��������
// @param       x_end           ����x������յ�
// @param       y_end           ����y������յ�
// @param       dat             ��Ҫ��ʾ����ɫ
// @return      void
// Sample usage:                ips114_drawpoint(0,0,RBB565_RED);  //����0,0��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void ips114_draw_line (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color)
{
    zf_assert(x_start < ips114_x_max);
    zf_assert(y_start < ips114_y_max);
    zf_assert(x_end < ips114_x_max);
    zf_assert(y_end < ips114_y_max);

    int16 x_dir = (x_start<x_end ? 1:-1);
    int16 y_dir = (y_start<y_end ? 1:-1);
    float temp_rate = 0;
    float temp_b = 0;

    if(x_start != x_end)
    {
        temp_rate = (float)(y_start-y_end)/(float)(x_start-x_end);
        temp_b = (float)y_start-(float)x_start*temp_rate;
    }
    else
    {
        while(y_start != y_end)
        {
            ips114_draw_point(x_start, y_start, color);
            y_start += y_dir;
        }
        return;
    }
    if(myabs(y_start-y_end)>myabs(x_start-x_end))
    {
        while(y_start != y_end)
        {
            ips114_draw_point(x_start, y_start, color);
            y_start += y_dir;
            x_start = (int16)(((float)y_start-temp_b)/temp_rate);
        }
    }
    else
    {
        while(x_start != x_end)
        {
            ips114_draw_point(x_start, y_start, color);
            x_start += x_dir;
            y_start = (int16)((float)x_start*temp_rate+temp_b);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ�ַ�
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ���ַ�
// @return      void
// Sample usage:                ips114_show_char(0,0,'x');                      // ���� 0,0 дһ���ַ� x
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_char (uint16 x, uint16 y, const char dat)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint8 i,j;
    uint8 temp;

    for(i=0; i<16; i++)
    {
        ips114_set_region(x,y+i,x+7,y+i);
        temp = tft_ascii[dat-32][i];                                            // �� 32 ��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո��� ascii ������� 32
        for(j=0; j<8; j++)
        {
            if(temp&0x01)
                ips114_write_16bit_data(ips114_pencolor);
            else
                ips114_write_16bit_data(ips114_bgcolor);
            temp>>=1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ�ַ���
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ���ַ���
// @return      void
// Sample usage:                ips114_show_string(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_string (uint16 x, uint16 y, const char dat[])
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);
    
    uint16 j = 0;
    while(dat[j] != '\0')
    {
        ips114_show_char(x+8*j,y,dat[j]);
        j++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ32λ�з���(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint32
// @param       num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// @return      void
// Sample usage:                ips114_showuint32(0,0,x,3);                     // x ����Ϊ int32 uint16 int16 uint8 int8 ����
// note:                        ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_int (uint16 x, uint16 y, const int32 dat, uint8 num)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);
    zf_assert(num > 0);
    zf_assert(num <= 10);

    int32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num+1);

    if(num < 10)
    {
        for(; num>0; num--)
            offset *= 10;
        dat_temp %= offset;
    }
    int_to_str(data_buffer, dat_temp);
    ips114_show_string(x,y,(const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ32λ�з���(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint32
// @param       num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// @return      void
// Sample usage:                ips114_show_uint32(0,0,x,3);                    // x ����Ϊ int32 uint16 int16 uint8 int8 ����
// note:                        ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_uint (uint16 x, uint16 y, const uint32 dat, uint8 num)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);
    zf_assert(num > 0);
    zf_assert(num <= 10);

    uint32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];
    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num);

    if(num < 10)
    {
        for(; num>0; num--)
            offset *= 10;
        dat_temp %= offset;
    }
    uint_to_str(data_buffer, dat_temp);
    ips114_show_string(x,y,(const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ������(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������float��double
// @param       num             ����λ��ʾ����   ���10λ
// @param       pointnum        С��λ��ʾ����   ���6λ
// @return      void
// Sample usage:                ips114_showfloat(0,0,x,2,3);                    // ��ʾ������   ������ʾ2λ   С����ʾ��λ
// @note                        �ر�ע�⵱����С��������ʾ��ֵ����д���ֵ��һ����ʱ��
//                              ���������ڸ��������ȶ�ʧ���⵼�µģ��Ⲣ������ʾ���������⣬
//                              �й���������飬�����аٶ�ѧϰ   ���������ȶ�ʧ���⡣
//                              ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_float (uint16 x, uint16 y, const float dat, uint8 num, uint8 pointnum)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);
    zf_assert(num > 0);
    zf_assert(num <= 8);
    zf_assert(pointnum > 0);
    zf_assert(pointnum <= 6);

    float dat_temp = dat;
    float offset = 1.0;
    char data_buffer[17];
    memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num+pointnum+2);

    if(num < 10)
    {
        for(; num>0; num--)
            offset *= 10;
        dat_temp = dat_temp-((int)dat_temp/(int)offset)*offset;
    }
    float_to_str(data_buffer, dat_temp, pointnum);
    ips114_show_string(x,y,data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ��ֵͼ�� ����ÿ�˸������һ���ֽ�����
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       *image          ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ���
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ���� ������Χ [0, ips114_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips114_y_max]
// @return      void
// Sample usage:                ips114_show_binary_image(0,0,ov7725_image_binary[0],OV7725_W,OV7725_H,OV7725_W,OV7725_H);
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_binary_image (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint32 i = 0, j = 0;
    uint8 temp = 0;
    uint32 width_index = 0, height_index = 0;

    ips114_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(image+height_index*width/8+width_index/8);                 // ��ȡ���ص�
            if(0x80 & (temp << (width_index%8)))
                ips114_write_16bit_data(RGB565_WHITE);
            else
                ips114_write_16bit_data(RGB565_BLACK);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ 8bit �Ҷ�ͼ�� ����ֵ����ֵ
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       *image          ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ���
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ���� ������Χ [0, ips114_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips114_y_max]
// @param       threshold       ��ֵ����ʾ��ֵ 0-��������ֵ��
// @return      void
// Sample usage:                ips114_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,MT9V03X_W,MT9V03X_H,0);
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_gray_image (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint32 i = 0, j = 0;
    uint16 color = 0,temp = 0;
    uint32 width_index = 0, height_index = 0;

    ips114_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(image+height_index*width+width_index);                     // ��ȡ���ص�
            if(threshold == 0)
            {
                color=(0x001f&((temp)>>3))<<11;
                color=color|(((0x003f)&((temp)>>2))<<5);
                color=color|(0x001f&((temp)>>3));
                ips114_write_16bit_data(color);
            }
            else if(temp < threshold)
                ips114_write_16bit_data(RGB565_BLACK);
            else
                ips114_write_16bit_data(RGB565_WHITE);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ RGB565 ��ɫͼ��
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       *image          ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ���
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ���� ������Χ [0, ips114_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips114_y_max]
// @param       color_mode      ɫ��ģʽ 0-��λ��ǰ 1-��λ��ǰ
// @return      void
// Sample usage:                ips114_show_rgb565_image(0,0,camera_buffer_addr,SCCB8660_W,SCCB8660_H,SCCB8660_W,SCCB8660_H,1);
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_rgb565_image (uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint32 i = 0, j = 0;
    uint16 color = 0;
    uint32 width_index = 0, height_index = 0;

    ips114_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            color = *(image+height_index*width+width_index);                    // ��ȡ���ص�
            if(color_mode)
                color = ((color&0xff)<<8) | (color>>8);
            ips114_write_16bit_data(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ����
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       *wave           ��������ָ��
// @param       width           ����ʵ�ʿ���
// @param       value_max       ����ʵ�����ֵ
// @param       dis_width       ������ʾ���� ������Χ [0, ips114_x_max]
// @param       dis_value_max   ������ʾ���ֵ ������Χ [0, ips114_y_max]
// @return      void
// Sample usage:                ips114_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,240,135,128);
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_wave (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint32 i = 0, j = 0;
    uint32 width_index = 0, value_max_index = 0;

    ips114_set_region(x, y, x+dis_width-1, y+dis_value_max-1);                  // ������ʾ����
    for(i=0;i<dis_value_max;i++)
    {
        for(j=0;j<dis_width;j++)
        {
            ips114_write_16bit_data(ips114_bgcolor); 
        }
    }

    for(i=0;i<dis_width;i++)
    {
        width_index = i*width/dis_width;
        value_max_index = *(wave+width_index)*(dis_value_max-1)/value_max;
        ips114_draw_point(i+x, (dis_value_max-1)-value_max_index+y, ips114_pencolor);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       size            ȡģ��ʱ�����õĺ��������С Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��� ȡģ��ʱ����Ҫ������һ����
// @param       *chinese_buffer ��Ҫ��ʾ�ĺ�������
// @param       number          ��Ҫ��ʾ����λ
// @param       color           ��ʾ��ɫ
// @return      void
// Sample usage:                ips114_show_chinese(0,0,16,chinese_test[0],4,RBB565_RED);//��ʾfont�ļ������ ʾ��
// @Note                        ʹ��PCtoLCD2002����ȡģ           ���롢����ʽ��˳��   16*16
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_chinese (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    int i, j, k; 
    uint8 temp, temp1, temp2;
    const uint8 *p_data;
    
    temp2 = size/8;
    
    ips114_set_region(x,y,number*size-1+x,y+size-1);
    
    for(i=0;i<size;i++)
    {
        temp1 = number;
        p_data = chinese_buffer+i*temp2;
        while(temp1--)
        {
            for(k=0;k<temp2;k++)
            {
                for(j=8;j>0;j--)
                {
                    temp = (*p_data>>(j-1)) & 0x01;
                    if(temp)    ips114_write_16bit_data(color);
                    else        ips114_write_16bit_data(ips114_bgcolor);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2*size;
        }   
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       1.14�� IPSҺ����ʼ��
// @return      void
// Sample usage:                ips114_init();
//-------------------------------------------------------------------------------------------------------------------
void ips114_init (void)
{
#if IPS114_USE_SOFT_SPI
    soft_spi_init(&ips114_spi, 0, IPS114_SOFT_SPI_DELAY, IPS114_SCL_PIN, IPS114_SDA_PIN, SOFT_SPI_PIN_NULL, SOFT_SPI_PIN_NULL);
#else

    spi_init(IPS114_SPI, SPI_MODE0, IPS114_SPI_SPEED, IPS114_SCL_PIN, IPS114_SDA_PIN, SPI_MISO_NULL, SPI_CS_NULL);
#endif

    gpio_init(IPS114_DC_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(IPS114_RST_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(IPS114_CS_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(IPS114_BLK_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);

    ips114_set_dir(ips114_display_dir);
    ips114_set_color(ips114_pencolor, ips114_bgcolor);

    IPS114_RST(0);
    system_delay_ms(200);

    IPS114_RST(1);
    system_delay_ms(100);

    ips114_write_index(0x36);
    system_delay_ms(100);
    if      (ips114_display_dir==0) ips114_write_8bit_data(0x00);
    else if (ips114_display_dir==1) ips114_write_8bit_data(0xC0);
    else if (ips114_display_dir==2) ips114_write_8bit_data(0x70);
    else                            ips114_write_8bit_data(0xA0);

    ips114_write_index(0x3A);
    ips114_write_8bit_data(0x05);

    ips114_write_index(0xB2);
    ips114_write_8bit_data(0x0C);
    ips114_write_8bit_data(0x0C);
    ips114_write_8bit_data(0x00);
    ips114_write_8bit_data(0x33);
    ips114_write_8bit_data(0x33);

    ips114_write_index(0xB7);
    ips114_write_8bit_data(0x35);

    ips114_write_index(0xBB);
    ips114_write_8bit_data(0x37);

    ips114_write_index(0xC0);
    ips114_write_8bit_data(0x2C);

    ips114_write_index(0xC2);
    ips114_write_8bit_data(0x01);

    ips114_write_index(0xC3);
    ips114_write_8bit_data(0x12);

    ips114_write_index(0xC4);
    ips114_write_8bit_data(0x20);

    ips114_write_index(0xC6);
    ips114_write_8bit_data(0x0F);

    ips114_write_index(0xD0);
    ips114_write_8bit_data(0xA4);
    ips114_write_8bit_data(0xA1);

    ips114_write_index(0xE0);
    ips114_write_8bit_data(0xD0);
    ips114_write_8bit_data(0x04);
    ips114_write_8bit_data(0x0D);
    ips114_write_8bit_data(0x11);
    ips114_write_8bit_data(0x13);
    ips114_write_8bit_data(0x2B);
    ips114_write_8bit_data(0x3F);
    ips114_write_8bit_data(0x54);
    ips114_write_8bit_data(0x4C);
    ips114_write_8bit_data(0x18);
    ips114_write_8bit_data(0x0D);
    ips114_write_8bit_data(0x0B);
    ips114_write_8bit_data(0x1F);
    ips114_write_8bit_data(0x23);

    ips114_write_index(0xE1);
    ips114_write_8bit_data(0xD0);
    ips114_write_8bit_data(0x04);
    ips114_write_8bit_data(0x0C);
    ips114_write_8bit_data(0x11);
    ips114_write_8bit_data(0x13);
    ips114_write_8bit_data(0x2C);
    ips114_write_8bit_data(0x3F);
    ips114_write_8bit_data(0x44);
    ips114_write_8bit_data(0x51);
    ips114_write_8bit_data(0x2F);
    ips114_write_8bit_data(0x1F);
    ips114_write_8bit_data(0x1F);
    ips114_write_8bit_data(0x20);
    ips114_write_8bit_data(0x23);

    ips114_write_index(0x21);

    ips114_write_index(0x11);
    system_delay_ms(120);

    ips114_write_index(0x29);

    ips114_clear(ips114_bgcolor);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�з���
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
//  @param      dat             ��Ҫ��ʾ�ı�������������int8_t
//  @return     void
//  Sample usage:               ips114_show_int8(0,0,x);                        // xΪ int8 ����
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_int8(uint16_t x,uint16_t y,int8_t dat)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint8_t a[3];
    uint8_t i;
    if(dat<0)
    {
        ips114_show_char(x,y,'-');
        dat = -dat;
    }
    else
        ips114_show_char(x,y,' ');

    a[0] = dat/100;
    a[1] = dat/10%10;
    a[2] = dat%10;
    i = 0;
    while(i<3)
    {
        ips114_show_char(x+(8*(i+1)),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ8λ�޷���
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı��� ��������uint8_t
// @return      void
// Sample usage:                ips114_show_uint8(0,0,x);                       // x Ϊ uint8_t ����
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_uint8(uint16_t x,uint16_t y,uint8_t dat)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint8_t a[3];
    uint8_t i;

    a[0] = dat/100;
    a[1] = dat/10%10;
    a[2] = dat%10;
    i = 0;
    while(i<3)
    {
        ips114_show_char(x+(8*i),y,'0' + a[i]);
        i++;
    }

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ16λ�з���
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı��� ��������int16_t
// @return      void
// Sample usage:                ips114_show_int16(0,0,x);                       // x Ϊ int16_t ����
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_int16(uint16_t x,uint16_t y,int16_t dat)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint8_t a[5];
    uint8_t i;

    if(dat<0)
    {
        ips114_show_char(x,y,'-');
        dat = -dat;
    }
    else    ips114_show_char(x,y,' ');

    a[0] = dat/10000;
    a[1] = dat/1000%10;
    a[2] = dat/100%10;
    a[3] = dat/10%10;
    a[4] = dat%10;

    i = 0;
    while(i<5)
    {
        ips114_show_char(x+(8*(i+1)),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ16λ�޷���
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint16_t
// @return      void
// Sample usage:                ips114_show_uint16(0,0,x);                      // x Ϊ uint16_t ����
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_uint16(uint16_t x,uint16_t y,uint16_t dat)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint8_t a[5];
    uint8_t i;

    a[0] = dat/10000;
    a[1] = dat/1000%10;
    a[2] = dat/100%10;
    a[3] = dat/10%10;
    a[4] = dat%10;

    i = 0;
    while(i<5)
    {
        ips114_show_char(x+(8*i),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ32λ�з���(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint32_t
// @param       num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// @return      void
// Sample usage:                ips114_showuint32(0,0,x,3);                     // x ����Ϊ int32_t uint16_t int16_t uint8_t int8_t ����
// note:                        ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_int32(uint16_t x,uint16_t y,int32_t dat,uint8_t num)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    int8_t    buff[34];
    uint8_t   length;

    if(10<num)      num = 10;

    num++;
    if(0>dat)   length = zf_sprintf( &buff[0],(const int8_t *)"%d",dat);        // ����
    else
    {
        buff[0] = ' ';
        length = zf_sprintf( &buff[1],(const int8_t *)"%d",dat);
        length++;
    }
    while(length < num)
    {
        buff[length] = ' ';
        length++;
    }
    buff[num] = '\0';

    ips114_show_string(x, y, (const char *)buff);                               // ��ʾ����
}