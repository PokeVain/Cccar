/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
* @file             zf_common_function
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#include "zf_common_function.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       获取整型数的最大公约数 九章算术之更相减损术
// @param       num1            数字1
// @param       num2            数字2
// @return      uint32          最大公约数
// Sample usage:                return get_greatest_common_divisor(144, 36);    // 获取 144 与 36 的最大公约数
//-------------------------------------------------------------------------------------------------------------------
uint32 get_greatest_common_divisor (uint32 num1, uint32 num2)
{
    while(num1 != num2)
    {
        if(num1 > num2)
            num1 = num1 -num2;
        if(num1 < num2)
            num2 = num2 - num1;
    }
    return num1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       绝对值函数 数据范围是 [-32767,32767]
// @param       dat             需要求绝对值的数
// @return      int             返回绝对值
// Sample usage:                dat = myabs(dat);                               // 将dat变成正数
//-------------------------------------------------------------------------------------------------------------------
//int  myabs (int dat)                                                          // 定义查看 zf_common_function.h

//-------------------------------------------------------------------------------------------------------------------
// @brief       限幅 数据范围是 [-32768,32767]
// @param       x               被限幅的数据
// @param       y               限幅范围(数据会被限制在-y至+y之间)
// @return      int             限幅之后的数据
// Sample usage:                int dat = limit(500,300);                       // 数据被限制在-300至+300之间  因此返回的结果是300
//-------------------------------------------------------------------------------------------------------------------
//int limit(int x, int32 y)                                                     // 定义查看 zf_common_function.h

//-------------------------------------------------------------------------------------------------------------------
// @brief       双边限幅 数据范围是 [-32768,32767]
// @param       x               被限幅的数据
// @param       a               限幅范围左边界
// @param       b               限幅范围右边界
// @return      int             限幅之后的数据
// Sample usage:                int dat = limit_ab(500,-300,400);               //数据被限制在-300至+400之间  因此返回的结果是400
//-------------------------------------------------------------------------------------------------------------------
//int limit_ab(int x, int a, int b)                                             // 定义查看 zf_common_function.h

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件延时
// @param       t               延时时间
// @return      void
// Sample usage:                my_delay(100);                                  // 初始化核心时钟为 96MHz
//-------------------------------------------------------------------------------------------------------------------
void my_delay (volatile long t)
{
    while(t--);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       字符串转整形数字 数据范围是 [-32768,32767]
// @param       str             传入字符串 可带符号
// @return      int             转换后的数据
// Sample usage:                int32 dat = str_to_int("-100");
//-------------------------------------------------------------------------------------------------------------------
int32 str_to_int (char *str)
{
    uint8 sign = 0;
    int32 temp = 0;

    if('-' == *str)
    {
        sign = 1;
        str ++;
    }
    else if('+' == *str)
    {
        str ++;
    }

    while(('0' <= *str)&&('9' >= *str))
    {
        temp = temp*10+((uint8)(*str)-0x30);
        str ++;
    }

    if(sign)
        return -temp;
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       字符串转整形数字 数据范围是 [0,65535]
// @param       str             传入字符串 无符号
// @return      int             转换后的数据
// Sample usage:                uint32 dat = str_to_uint("100");
//-------------------------------------------------------------------------------------------------------------------
uint32 str_to_uint (char *str)
{
    uint32 temp = 0;

    while(('0' <= *str)&&('9' >= *str))
    {
        temp = temp*10+((uint8)(*str)-0x30);
        str ++;
    }

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       整形数字转字符串 数据范围是 [-32768,32767]
// @param       *str            字符串指针
// @param       number          传入的数据
// @return      NULL
// Sample usage:                int32o_str(data_buffer, -300);
//-------------------------------------------------------------------------------------------------------------------
void int_to_str (char *str, int32 number)
{
    uint8 data_temp[16];
    uint8 bit = 0;

    if(0 > number)
    {
        *str++ = '-';
    }
    else if(0 == number)
    {
        *str = '0';
        return;
    }

    // int [-32768,32767]
    while(0 != number)
    {
        data_temp[bit++] = myabs(number%10);
        number /= 10;
    }
    while(0 != bit)
    {
        *str++ = (data_temp[bit-1]+0x30);
        bit--;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       整形数字转字符串 数据范围是 [0,65535]
// @param       *str            字符串指针
// @param       number          传入的数据
// @return      NULL
// Sample usage:                uint_to_str(data_buffer, 300);
//-------------------------------------------------------------------------------------------------------------------
void uint_to_str (char *str, uint32 number)
{
    int8 data_temp[16];
    uint8 bit = 0;

    if(0 == number)
    {
        *str = '0';
        return;
    }

    while(0 != number)
    {
        data_temp[bit++] = number%10;
        number /= 10;
    }
    while(0 != bit)
    {
        *str++ = (myabs(data_temp[bit-1])+0x30);
        bit--;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       字符串转浮点数
// @param       str             传入字符串 可带符号
// @return      float           转换后的数据
// Sample usage:                float dat = str_to_float("-100.2");
//-------------------------------------------------------------------------------------------------------------------
float str_to_float (char *str)
{
    uint8 sign = 0;
    float temp = 0.0;
    float temp_point = 0.0;
    uint8 point_bit = 0;

    if('-' == *str)
    {
        sign = 1;
        str ++;
    }
    else if('+' == *str)
    {
        str ++;
    }

    while(('0' <= *str)&&('9' >= *str))
    {
        temp = temp*10+((uint8)(*str)-0x30);
        str ++;
    }
    if('.' == *str)
    {
        str++;
        while(('0' <= *str)&&('9' >= *str))
        {
            temp_point = temp_point*10+((uint8)(*str)-0x30);
            point_bit++;
            str ++;
        }
        while(point_bit--)
        {
            temp_point /= 10;
        }
    }
    temp += temp_point;

    if(sign)
        return -temp;
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       浮点数字转字符串
// @param       *str            字符串指针
// @param       number          传入的数据
// @param       point_bit       小数点精度
// @return      NULL
// Sample usage:                float_to_str(data_buffer, 3.1415, 2);           // 结果输出 data_buffer = "3.14"
//-------------------------------------------------------------------------------------------------------------------
void float_to_str (char *str, float number, uint8 point_bit)
{
    int data_int = 0;
    int data_float = 0.0;
    int data_temp[5];
    int data_temp_point[5];
    uint8 bit = 0;

    data_int = (int)number;
    if(0 > number)
    {
        *str++ = '-';
    }
    else if(0.0 == number)
    {
        *str++ = '0';
        *str++ = '.';
        *str = '0';
        return;
    }

    number = number - data_int;
    while(point_bit--)
    {
        number = number*10;
    }
    data_float = (int)number;

    while(0 != data_int)
    {
        data_temp[bit++] = data_int%10;
        data_int /= 10;
    }
    while(0 != bit)
    {
        *str++ = (myabs(data_temp[bit-1])+0x30);
        bit--;
    }
    *str++ = '.';
    if(0 == data_float)
        *str = '0';
    else
    {
        while(0 != data_float)
        {
            data_temp_point[bit++] = data_float%10;
            data_float /= 10;
        }
        while(0 != bit)
        {
            *str++ = (myabs(data_temp_point[bit-1])+0x30);
            bit--;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       字符串转浮点数
// @param       str             传入字符串 可带符号
// @return      double          转换后的数据
// Sample usage:                double dat = str_to_double("-100.2");
//-------------------------------------------------------------------------------------------------------------------
double str_to_double (char *str)
{
    uint8 sign = 0;
    double temp = 0.0;
    double temp_point = 0.0;
    uint8 point_bit = 0;

    if('-' == *str)
    {
        sign = 1;
        str ++;
    }
    else if('+' == *str)
    {
        str ++;
    }

    while(('0' <= *str)&&('9' >= *str))
    {
        temp = temp*10+((uint8)(*str)-0x30);
        str ++;
    }
    if('.' == *str)
    {
        str++;
        while(('0' <= *str)&&('9' >= *str))
        {
            temp_point = temp_point*10+((uint8)(*str)-0x30);
            point_bit++;
            str ++;
        }
        while(point_bit--)
        {
            temp_point /= 10;
        }
    }
    temp += temp_point;

    if(sign)
        return -temp;
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       浮点数字转字符串
// @param       *str            字符串指针
// @param       number          传入的数据
// @param       point_bit       小数点精度
// @return      NULL
// Sample usage:                double_to_str(data_buffer, 3.1415, 2);           // 结果输出 data_buffer = "3.14"
//-------------------------------------------------------------------------------------------------------------------
void double_to_str (char *str, double number, uint8 point_bit)
{
    int data_int = 0;
    int data_float = 0.0;
    int data_temp[5];
    int data_temp_point[5];
    uint8 bit = 0;

    data_int = (int)number;
    if(0 > number)
    {
        *str++ = '-';
    }
    else if(0.0 == number)
    {
        *str++ = '0';
        *str++ = '.';
        *str = '0';
        return;
    }

    number = number - data_int;
    while(point_bit--)
    {
        number = number*10;
    }
    data_float = (int)number;

    while(0 != data_int)
    {
        data_temp[bit++] = data_int%10;
        data_int /= 10;
    }
    while(0 != bit)
    {
        *str++ = (myabs(data_temp[bit-1])+0x30);
        bit--;
    }
    *str++ = '.';
    if(0 == data_float)
        *str = '0';
    else
    {
        while(0 != data_float)
        {
            data_temp_point[bit++] = data_float%10;
            data_float /= 10;
        }
        while(0 != bit)
        {
            *str++ = (myabs(data_temp_point[bit-1])+0x30);
            bit--;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       数字转换为 ASCII 值 内部调用
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static uint8 number_conversion_ascii (uint32 dat, int8 *p, uint8 neg_type, uint8 radix)
{
    int32   neg_dat;
    uint32  pos_dat;
    uint8   temp_data = 0;
    uint8   valid_num = 0;

    if(neg_type)
    {
        neg_dat = (int32)dat;
        if(0 > neg_dat)   neg_dat = -neg_dat;
        while(1)
        {
            *p = neg_dat%radix + '0';
            neg_dat = neg_dat/radix;
            valid_num++;

            if(!neg_dat) break;
            p++;
        }
    }
    else
    {
        pos_dat = dat;
        while(1)
        {
            temp_data = pos_dat%radix;
            if(10 <= temp_data) temp_data += 'A'-10;
            else                temp_data += '0';

            *p = temp_data;

            pos_dat = pos_dat/radix;
            valid_num++;

            if(!pos_dat) break;
            p++;
        }
    }
    return valid_num;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       printf 显示转换 内部调用
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static void printf_reverse_order (int8 *d_buff, uint32 len)
{
    uint32 i;
    int8  temp_data;
    for(i=0;len/2>i;i++)
    {
        temp_data = d_buff[len-1-i];
        d_buff[len-1-i] = d_buff[i];
        d_buff[i] = temp_data; 
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       sprintf 函数实现
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 zf_sprintf (int8 *buff, const int8 *format, ...)
{
    uint32 buff_len=0;
    va_list arg;
    va_start(arg, format);

    while (*format)
    {
        int8 ret = *format;
        if ('%' == ret)
        {
            switch (*++format)
            {
                case 'a':// 十六进制p计数法输出浮点数 暂未实现
                    {
                    }
                    break;

                case 'c':// 一个字符
                    {
                        int8 ch = (int8)va_arg(arg, uint32);
                        *buff = ch;
                        buff++;
                        buff_len++;
                    }
                    break;

                case 'd':
                case 'i':// 有符号十进制整数
                    {
                        int8 vstr[33];
                        int32 ival = (int32)va_arg(arg, int32);
                        uint8 vlen = number_conversion_ascii((uint32)ival, vstr, 1, 10);

                        if(0>ival)
                        {
                            vstr[vlen] = '-';
                            vlen++;
                        }
                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;
                    }
                    break;

                case 'f':// 浮点数，输出小数点后六位  不能指定输出精度
                case 'F':// 浮点数，输出小数点后六位  不能指定输出精度
                {
                    int8 vstr[33];
                    double ival = (double)va_arg(arg, double);
                    uint8 vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);

                    if(0>ival)
                    {
                        vstr[vlen] = '-';
                        vlen++;
                    }
                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff += vlen;
                    buff_len += vlen;

                    ival = ((double)ival - (int32)ival)*1000000;
                    if(ival)
                    {
                        vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);
                    }
                    else
                    {
                        vstr[0] = vstr[1] = vstr[2] = vstr[3] = vstr[4] = vstr[5] = '0';
                        vlen = 6;
                    }

                    while(6>vlen)
                    {
                        vstr[vlen] = '0';
                        vlen++;
                    }

                    vstr[vlen] = '.';
                    vlen++;

                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff_len += vlen;
                }
                break;

                case 'u':// 无符号十进制整数
                    {
                        int8 vstr[33];
                        uint32 ival = (uint32)va_arg(arg, uint32);
                        uint8 vlen = number_conversion_ascii(ival, vstr, 0, 10);

                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;
                    }
                    break;

                case 'o':// 无符号八进制整数
                    {
                        int8 vstr[33];
                        uint32 ival = (uint32)va_arg(arg, uint32);
                        uint8 vlen = number_conversion_ascii(ival, vstr, 0, 8);

                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;

                    }
                    break;

                case 'x':// 无符号十六进制整数
                case 'X':// 无符号十六进制整数
                    {
                        int8 vstr[33];
                        uint32 ival = (uint32)va_arg(arg, uint32);
                        uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);

                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;
                    }
                    break;

                case 's':// 字符串
                    {
                        int8 *pc = va_arg(arg, int8 *);
                        while (*pc)
                        {
                            *buff = *pc;
                            buff++;
                            buff_len++;
                            pc++;
                        }
                    }
                    break;

                case 'p':// 以16进制形式输出指针
                    {
                        int8 vstr[33];
                        //uint32 ival = (uint32)va_arg(arg, uint32);
                        //uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);

                        printf_reverse_order(vstr,8);
                        memcpy(buff,vstr,8);
                        buff += 8;
                        buff_len += 8;
                    }
                    break;

                case '%':// 输出字符%
                    {
                        *buff = '%';
                        buff++;
                        buff_len++;
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            *buff = (int8)(*format);
            buff++;
            buff_len++;
        }
        format++;
    }
    va_end(arg);

    return buff_len;
}

