


#include "GUIControl.h"


static void LCD_Point(ST_2D_INT_POINT_INFO Pnt,Uint16 Color)
{
    ips114_draw_point(Pnt.m_i16x,Pnt.m_i16y,Color);
}

void DisPalyImg(Uint8 *Img, Uint16 width, Uint16 height)
{
    Int16 Loop_X = -1;
    Int16 Loop_Y = -1;

    ST_2D_INT_POINT_INFO temp;

    ips114_displayimage032(Img, width, height);


    /*显示列*/
    while((++Loop_Y) < IMGH)
    {
        /*显示线*/
        if (m_stLPnt[Loop_Y].m_i16y)
        {
            LCD_Point(m_stLPnt[Loop_Y], DSP_MAGENTA);
        }

        if (m_stRPnt[Loop_Y].m_i16y)
        {
            LCD_Point(m_stRPnt[Loop_Y], DSP_BLUE);
        }

//        LCD_Point(m_stTPnt[Loop_Y], DSP_PINK);

        LCD_Point(m_stCPnt[Loop_Y], DSP_RED);

        /*显示拐点*/
        temp.m_i16y = Loop_Y;

        temp.m_i16x = LTOP.m_i16x;
        LCD_Point(temp, DSP_YELLOW);

        temp.m_i16x = RTOP.m_i16x;
        LCD_Point(temp, DSP_YELLOW);

        temp.m_i16x = LBOT.m_i16x;
        LCD_Point(temp, DSP_YELLOW);

        temp.m_i16x = RBOT.m_i16x;
        LCD_Point(temp, DSP_YELLOW);


        temp.m_i16x = MSPOT.m_i16x;
        LCD_Point(temp, DSP_BROWN);

        temp.m_i16x = m_stOptimalPoint.m_i16x;
        LCD_Point(temp, DSP_BLUE);
    }



    /*显示行*/

    Loop_X = 0;
    while((++Loop_X) < far_ipts1_num)
    {
        //占时用来显示前瞻
        ips114_draw_point(fat_ipts1[Loop_X][0],fat_ipts1[Loop_X][1],DSP_GREEN);
    }

    Loop_X = 0;
    while((++Loop_X) < far_ipts0_num)
    {
        //占时用来显示前瞻
        ips114_draw_point(far_ipts0[Loop_X][0],far_ipts0[Loop_X][1],DSP_YELLOW);
    }

    Loop_X = 0;
    while((++Loop_X) < far_ipts0_num)
    {
        //占时用来显示前瞻
        ips114_draw_point(ipts0[Loop_X][0],ipts0[Loop_X][1],DSP_BLUE);
    }

    Loop_X = 0;
    while((++Loop_X) < far_ipts0_num)
    {
        //占时用来显示前瞻
        ips114_draw_point(ipts1[Loop_X][0],ipts1[Loop_X][1],DSP_BLUE);
    }
}
