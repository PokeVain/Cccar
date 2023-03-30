



#include "DSPFunction.h"



void ClearScreen_DSPFunction(
                                DSP_Uint16Type X,
                                DSP_Uint16Type Y,
                                DSP_Uint16Type Width,
                                DSP_Uint16Type Hight,
                                DSP_Uint16Type Color
                            )
{
    DSP_Uint32Type LoopX = (Width - X) * (Hight - Y);

    DSP_Set_Region(X,Y,Width-1,Hight-1);

    while((LoopX--) > 0)
    {
        DSP_Write_16Bit(Color);
    }
}

void ShowLoge_DSPFunction(  const unsigned char *Sprt,
                            DSP_Uint16Type X,
                            DSP_Uint16Type Y,
                            DSP_Uint16Type Width,
                            DSP_Uint16Type Hight    )
{
    DSP_Uchar8Type Loge_L, Loge_H;
    DSP_Uint16Type Colour;
    DSP_Uint32Type LoopX;
    DSP_Clear(DSP_BLACK);

    DSP_Set_Region(X, Y, X+Width-1, Y+Hight-1);

    for (LoopX = 0; LoopX < Width * Hight; LoopX++)
    {
        Loge_L=*(Sprt+LoopX*2);
        Loge_H=*(Sprt+LoopX*2+1);
        Colour = Loge_H << 8 | Loge_L;
        DSP_Write_16Bit(Colour);
    }
}


void ShowChiese_DSPFunction(
                                DSP_Uint32Type x,
                                DSP_Uint32Type y,
                                DSP_Uint32Type fc,
                                DSP_Uint32Type bc,
                                DSP_Uchar8Type size,
                                DSP_Uchar8Type *p   )
{


    DSP_Uchar8Type LoopX, LoopY, LoopN, LoopS;
    DSP_Uchar8Type *p_data;

    DSP_Set_Region(x,y,x+16*size-1,y+15);

    for(LoopS = 0; LoopS < 16; LoopS++)
    {
        LoopN = size;
        p_data = p + LoopS * 2;
        while(LoopN--)
        {
            for(LoopX = 0; LoopX < 2; LoopX++)
            {
                for(LoopY = 8; LoopY > 0; LoopY--)
                {
                    if((*p_data>>(LoopY-1)) & 0x01)
                        DSP_Write_16Bit(fc);
                    else
                        DSP_Write_16Bit(bc);
                }
                p_data++;
            }
            p_data += 30;
        }
    }

}


void GuideChese_DSPFunction(    DSP_Uint32Type x,
                                DSP_Uint32Type y,
                                DSP_Uint32Type fc,
                                DSP_Uint32Type bc,
                                DSP_Uchar8Type *s   )
{
    DSP_Uint16Type LoopX, LoopY, LoopN;
    DSP_Uchar8Type Temp;

    while(*s)
    {
        if((*s) >= 128)
        {
            for(LoopN = 0; LoopN < CharactersNumber; LoopN++)
            {
                if(Characters[LoopN].Guide[0] == *s && Characters[LoopN].Guide[1] == *(s+1))
                {
                    DSP_Set_Region(x,y,x+15,y+15);
                    for(LoopX = 0; LoopX < 32; LoopX++)
                    {
                        for(LoopY = 0; LoopY < 8; LoopY++)
                        {
                            if(Characters[LoopN].Code[LoopX] & (0x80 >> LoopY))
                                DSP_Write_16Bit(fc);
                            else
                                DSP_Write_16Bit(bc);
                        }
                    }
                }

            }
            s+=2; x+=16;
        }
        else
        {
            DSP_Set_Region(x,y,x+7,y+15);
            for(LoopX = 0; LoopX < 16; LoopX++)
            {
                Temp = tft_ascii[(*s)-32][LoopX];
                for(LoopY = 0; LoopY < 8; LoopY++)
                {
                    if(Temp&0x01)
                        DSP_Write_16Bit(fc);
                    else
                        DSP_Write_16Bit(bc);
                    Temp>>=1;
                }
            }
            s+=1; x+=8;
        }
    }
}









