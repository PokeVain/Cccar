





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
    F32 t_F32OmegaBack, t_F32OmegaFore, t_F32MicroBack, t_F32MicroFore, t_F32SigmaB, t_F32Sigma; // ��䷽��;
    Int32 t_int32MinValue, t_int32MaxValue;
    Uint8 t_u8Threshold = 0;
    Uint16 t_u16HistoGramAr[256];
    for (t_u16j = 0; t_u16j < 256; t_u16j++)
    {
        t_u16HistoGramAr[t_u16j] = 0; //��ʼ���Ҷ�ֱ��ͼ
    }
    for (t_u16j = 0; t_u16j < IMGH; t_u16j+=2)
    {
        for (t_u16i = 0; t_u16i < IMGW; t_u16i+=2)
        {
            t_u16HistoGramAr[(Uint8)Img[t_u16j][t_u16i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
        }
    }
    for (t_int32MinValue = 0; t_int32MinValue < 256 && t_u16HistoGramAr[t_int32MinValue] == 0; t_int32MinValue++)
    {
        ; //��ȡ��С�Ҷȵ�ֵ
    }
    for (t_int32MaxValue = 255; t_int32MaxValue > t_int32MinValue && t_u16HistoGramAr[t_int32MinValue] == 0; t_int32MaxValue--)
    {
        ; //��ȡ���Ҷȵ�ֵ
    }
    if (t_int32MaxValue == t_int32MinValue)
    {
        return ((Uint8)(t_int32MaxValue)); // ͼ����ֻ��һ����ɫ
    }
    if (t_int32MinValue + 1 == t_int32MaxValue)
    {
        return ((Uint8)(t_int32MinValue)); // ͼ����ֻ�ж�����ɫ
    }
    for (t_u16j = (Uint16)t_int32MinValue; t_u16j <= t_int32MaxValue; t_u16j++)
    {
        t_u32Amount += t_u16HistoGramAr[t_u16j]; //  ��������
    }
    t_u32PixelIntegral = 0;
    for (t_u16j = (Uint16)t_int32MinValue; t_u16j <= t_int32MaxValue; t_u16j++)
    {
        t_u32PixelIntegral += t_u16HistoGramAr[t_u16j] * t_u16j; //�Ҷ�ֵ����
    }
    t_F32SigmaB = -1;
    for (t_u16j = (Uint16)t_int32MinValue; t_u16j < t_int32MaxValue; t_u16j++)
    {
        t_u32PixelBack = t_u32PixelBack + t_u16HistoGramAr[t_u16j];                                                           //ǰ�����ص���
        t_int32PixelFore = t_u32Amount - t_u32PixelBack;                                                                      //�������ص���
        t_F32OmegaBack = (F32)t_u32PixelBack / t_u32Amount;                                                                   //ǰ�����ذٷֱ�
        t_F32OmegaFore = (F32)t_int32PixelFore / t_u32Amount;                                                                 //�������ذٷֱ�
        t_u32PixelIntegralBack += t_u16HistoGramAr[t_u16j] * t_u16j;                                                          //ǰ���Ҷ�ֵ
        t_int32PixelIntegralFore = t_u32PixelIntegral - t_u32PixelIntegralBack;                                               //�����Ҷ�ֵ
        t_F32MicroBack = (F32)t_u32PixelIntegralBack / t_u32PixelBack;                                                        //ǰ���ҶȰٷֱ�
        t_F32MicroFore = (F32)t_int32PixelIntegralFore / t_int32PixelFore;                                                    //�����ҶȰٷֱ�
        t_F32Sigma = t_F32OmegaBack * t_F32OmegaFore * (t_F32MicroBack - t_F32MicroFore) * (t_F32MicroBack - t_F32MicroFore); //������䷽��
        if (t_F32Sigma > t_F32SigmaB)                                                                                         //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
        {
            t_F32SigmaB = t_F32Sigma;
            t_u8Threshold = (Uint8)t_u16j;
        }
    }
    return t_u8Threshold; //���������ֵ;
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
        /*ͨ����������һ����СΪ188*120��ͼ�������ȡ��������Ҫͼ��Ĵ�С*/
        /*Y_OFFSET ������ȡͼ���,ʵ������Y���ƫ��*/
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

