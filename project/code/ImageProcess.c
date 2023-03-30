





#include "ImageProcess.h"


void get_using_img(Uint8 (*InImg)[IMGW], Uint8 (*OutImg)[IMGW])
{

    Int16 t_i16LoopX;
    Int16 t_i16LoopY;

    for(t_i16LoopY = 0; t_i16LoopY < IMGH; t_i16LoopY++)
    {
        for(t_i16LoopX = 0; t_i16LoopX < IMGW; t_i16LoopX++)
        {
            OutImg[t_i16LoopY][t_i16LoopX] = InImg[t_i16LoopY][t_i16LoopX];
        }
    }
}




Uint8 GetThreshold(Uint8 (*Img)[IMGW])
 {
    Uint16 t_u16i, t_u16j;
    Uint32 t_u32Amount = 0;
    Uint32 t_u32PixelBack = 0;
    Uint32 t_u32PixelIntegralBack = 0;
    Uint32 t_u32PixelIntegral = 0;
    Int32 t_int32PixelIntegralFore = 0;
    Int32 t_int32PixelFore = 0;
    F32 t_F32OmegaBack, t_F32OmegaFore, t_F32MicroBack, t_F32MicroFore, t_F32SigmaB, t_F32Sigma; // 类间方差;
    Int32 t_int32MinValue, t_int32MaxValue;
    Uint8 t_u8Threshold = 0;
    Uint16 t_u16HistoGramAr[256];
    for (t_u16j = 0; t_u16j < 256; t_u16j++)
    {
        t_u16HistoGramAr[t_u16j] = 0; //初始化灰度直方图
    }
    for (t_u16j = 0; t_u16j < IMGH; t_u16j+=2)
    {
        for (t_u16i = 0; t_u16i < IMGW; t_u16i+=2)
        {
            t_u16HistoGramAr[(Uint8)Img[t_u16j][t_u16i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }
    for (t_int32MinValue = 0; t_int32MinValue < 256 && t_u16HistoGramAr[t_int32MinValue] == 0; t_int32MinValue++)
    {
        ; //获取最小灰度的值
    }
    for (t_int32MaxValue = 255; t_int32MaxValue > t_int32MinValue && t_u16HistoGramAr[t_int32MinValue] == 0; t_int32MaxValue--)
    {
        ; //获取最大灰度的值
    }
    if (t_int32MaxValue == t_int32MinValue)
    {
        return ((Uint8)(t_int32MaxValue)); // 图像中只有一个颜色
    }
    if (t_int32MinValue + 1 == t_int32MaxValue)
    {
        return ((Uint8)(t_int32MinValue)); // 图像中只有二个颜色
    }
    for (t_u16j = (Uint16)t_int32MinValue; t_u16j <= t_int32MaxValue; t_u16j++)
    {
        t_u32Amount += t_u16HistoGramAr[t_u16j]; //  像素总数
    }
    t_u32PixelIntegral = 0;
    for (t_u16j = (Uint16)t_int32MinValue; t_u16j <= t_int32MaxValue; t_u16j++)
    {
        t_u32PixelIntegral += t_u16HistoGramAr[t_u16j] * t_u16j; //灰度值总数
    }
    t_F32SigmaB = -1;
    for (t_u16j = (Uint16)t_int32MinValue; t_u16j < t_int32MaxValue; t_u16j++)
    {
        t_u32PixelBack = t_u32PixelBack + t_u16HistoGramAr[t_u16j];                                                           //前景像素点数
        t_int32PixelFore = t_u32Amount - t_u32PixelBack;                                                                      //背景像素点数
        t_F32OmegaBack = (F32)t_u32PixelBack / t_u32Amount;                                                                   //前景像素百分比
        t_F32OmegaFore = (F32)t_int32PixelFore / t_u32Amount;                                                                 //背景像素百分比
        t_u32PixelIntegralBack += t_u16HistoGramAr[t_u16j] * t_u16j;                                                          //前景灰度值
        t_int32PixelIntegralFore = t_u32PixelIntegral - t_u32PixelIntegralBack;                                               //背景灰度值
        t_F32MicroBack = (F32)t_u32PixelIntegralBack / t_u32PixelBack;                                                        //前景灰度百分比
        t_F32MicroFore = (F32)t_int32PixelIntegralFore / t_int32PixelFore;                                                    //背景灰度百分比
        t_F32Sigma = t_F32OmegaBack * t_F32OmegaFore * (t_F32MicroBack - t_F32MicroFore) * (t_F32MicroBack - t_F32MicroFore); //计算类间方差
        if (t_F32Sigma > t_F32SigmaB)                                                                                         //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            t_F32SigmaB = t_F32Sigma;
            t_u8Threshold = (Uint8)t_u16j;
        }
    }
    return t_u8Threshold; //返回最佳阈值;
 }

void ImageHandle_threshold(Uint8 (*InImg)[IMGW], Uint8 (*OutImg)[IMGW], Uint8 Threshold)
{
    Int16 t_i16i, t_i16j;

    t_i16i = -1;
    while((++t_i16i) < IMGH)
    {
        t_i16j = -1;
        while((++t_i16j) < IMGW)
        {
            OutImg[t_i16i][t_i16j] = InImg[t_i16i][t_i16j]> Threshold? B_WHITE : B_BLACK;
        }
    }
}

void ImageHandle_Copyimage(Uint8 *img_data, Uint8 *output_data)
{
    Int16 t_i16i;

    t_i16i = -1;
    while((++t_i16i) < IMGH * IMGW)
    {
        output_data[t_i16i] = img_data[t_i16i];
    }
}



void GetBinaryImage(Uint8 (*InImg)[IMGW], Uint8 (*OutImg)[IMGW], Uint8 Threshold)
 {
        Int16 t_i16i, t_i16j;
        Uint8 TempTh=Threshold;
        /*通常我们是在一个大小为188*120的图像上面截取我们所需要图像的大小*/
        /*Y_OFFSET 用来截取图像的,实际是在Y轴的偏移*/
        t_i16i = - 1;
        while ((++t_i16i) < ( IMGH))
        {
            if(t_i16i<IMGH/3)
            {
                    TempTh=Threshold+10;
            }
            else
            {
                    TempTh=Threshold;
            }

            t_i16j = IMGW;

            while((--t_i16j) > -1)
            {
                if (InImg[t_i16i][t_i16j] > (TempTh))
                    OutImg[t_i16i][t_i16j] = B_WHITE;
                else
                    OutImg[t_i16i][t_i16j] = B_BLACK;
            }
        }
 }


void BinaryImage(Uint8 (*InImg)[IMGW], Uint8 (*OutImg)[IMGW], Uint8 Threshold)
{
    Int16 t_i16i, t_i16j;

    for(t_i16i = 0; t_i16i < IMGH; t_i16i++)
    {
        for(t_i16j = 0; t_i16j < IMGW; t_i16j++)
        {
            if(InImg[t_i16i][t_i16j] > Threshold)
            {
                OutImg[t_i16i][t_i16j] = B_WHITE;
            }
            else
            {
                OutImg[t_i16i][t_i16j] = B_BLACK;
            }
        }
    }
}

