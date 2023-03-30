





#include "Perspective.h"



#define         DeadSection         65


int delta_x = 187;
int delta_y = 22;

float line_blur_kernel = 7;
float pixel_per_meter =64.444444;
float sample_dist = 0.02;
float angle_dist = 0.2;

image_t img_raw = DEF_IMAGE(0, 188, 60);
float rot[3][3] ={{2.9588,0,-1.9462},
                  {3.4999,1.0072,-3.4945},
                  {0.0369,0,0.9757}};

float leftAngular, rightAngular;



Uint8 Element_LoopY;
float LeftLoopYangle, ReftLoopYangle;

_element_angle_struct _anglesignal;

//int Time_ms;


/**
 *  二位数组 存储坐标
 **/
int ipts0[POINTS_MAX_LEN][2];
int ipts1[POINTS_MAX_LEN][2];
int ipts0_num, ipts1_num;

float rpts0[POINTS_MAX_LEN][2];
float rpts1[POINTS_MAX_LEN][2];
int rpts0_num, rpts1_num;

 float rpts0b[POINTS_MAX_LEN][2];
 float rpts1b[POINTS_MAX_LEN][2];
int rpts0b_num, rpts1b_num;

 float rpts0s[POINTS_MAX_LEN][2];
 float rpts1s[POINTS_MAX_LEN][2];
int rpts0s_num, rpts1s_num;

 float rpts0a[POINTS_MAX_LEN];
 float rpts1a[POINTS_MAX_LEN];
int rpts0a_num, rpts1a_num;

 float rpts0an[POINTS_MAX_LEN];
 float rpts1an[POINTS_MAX_LEN];
int rpts0an_num, rpts1an_num;

 float rptsc0[POINTS_MAX_LEN][2];
 float rptsc1[POINTS_MAX_LEN][2];
int rptsc0_num, rptsc1_num;

int far_ipts0[120][2];
int fat_ipts1[120][2];
int far_ipts0_num, far_ipts1_num;

float far_rpts0[POINTS_MAX_LEN][2];
float far_rpts1[POINTS_MAX_LEN][2];
int far_rpts0_num, far_rpts1_num;

float far_rpts0b[POINTS_MAX_LEN][2];
float far_rpts1b[POINTS_MAX_LEN][2];
int far_rpts0b_num, far_rpts1b_num;

float far_rpts0s[POINTS_MAX_LEN][2];
float far_rpts1s[POINTS_MAX_LEN][2];
int far_rpts0s_num, far_rpts1s_num;

float far_rpts0a[POINTS_MAX_LEN];
float far_rpts1a[POINTS_MAX_LEN];
int far_rpts0a_num, far_rpts1a_num;

float far_rpts0an[POINTS_MAX_LEN];
float far_rpts1an[POINTS_MAX_LEN];
int far_rpts0an_num, far_rpts1an_num;
bool far_Lpt0_found, far_Lpt1_found;


//跟踪中线
float (*rpts)[2];
int rpts_num;

float rptsn[POINTS_MAX_LEN][2];
int rptsn_num;


int Ypt0_rpts0s_id, Ypt1_rpts1s_id;
bool Ypt0_found, Ypt1_found;


int Lpt0_rpts0s_id, Lpt1_rpts1s_id;
bool Lpt0_found, Lpt1_found;


bool is_turn0, is_turn1;



int clip(int x, int low, int up)
{
    return x > up ? up : x < low ? low : x;
}

/*
 *   0
 * 3   1
 *   2
 */
const int dir_front[4][2] = {
                                { 0, -1 },
                                { 1,  0 },
                                { 0,  1 },
                                {-1,  0 }
                            };

const int dir_frontleft[4][2]= {
                                    { -1, -1 },
                                    {  1, -1 },
                                    {  1,  1 },
                                    { -1,  1 }
                                };

const int dir_frontright[4][2] = {
                                    {  1, -1 },
                                    {  1,  1 },
                                    { -1,  1 },
                                    { -1, -1 }
                                 };


void measure_corners(void)
{

    int im1 = clip(Element_LoopY - 10, 0, rpts0s_num - 1);
    int ip1 = clip(Element_LoopY + 10, 0, rpts0s_num - 1);
    LeftLoopYangle = ABS(rpts0a[Element_LoopY]) - (ABS(rpts0a[im1]) + ABS(rpts0a[ip1])) / 2;
    ReftLoopYangle = ABS(rpts1a[Element_LoopY]) - (ABS(rpts1a[im1]) + ABS(rpts1a[ip1])) / 2;
}



void find_corners(void)
{
    Ypt0_found = Ypt1_found = Lpt0_found = Lpt1_found = false;

    _anglesignal.L_edge = _anglesignal_false;
    _anglesignal.R_edge = _anglesignal_false;

    for (int i = 0;  i < rpts0s_num && i < 50; i++) {
        if (rpts0an[i] == 0) continue;

        int im1 = clip(i - 20, 0, rpts0s_num - 1);
        int ip1 = clip(i + 20, 0, rpts0s_num - 1);
        float conf = ABS(rpts0a[i]) - (ABS(rpts0a[im1]) + ABS(rpts0a[ip1])) / 2;

        if (Ypt0_found == false && 0.732 < conf && conf < 1.206)
        {
            Ypt0_rpts0s_id = i;
            Ypt0_found = true;
        }
        if (Lpt0_found == false && 1.226 < conf && conf < 2.093 )
        {
            Lpt0_rpts0s_id = i;
            Lpt0_found = true;
        }

        if(_anglesignal.L_edge == _anglesignal_false && 1.0 < conf && conf < 2.326)
        {
            leftAngular = conf;
            _anglesignal.L_edge = _anglesignal_max;
        }


        if (Ypt0_found == true && Lpt0_found == true)
            break;
    }
    for (int i = 0; i < rpts1s_num && i < 50; i++) {
        if (rpts1an[i] == 0) continue;
        int im1 = clip(i - 20, 0, rpts1s_num - 1);
        int ip1 = clip(i + 20, 0, rpts1s_num - 1);
        float conf = ABS(rpts1a[i]) - (ABS(rpts1a[im1]) + ABS(rpts1a[ip1])) / 2;


        if (Ypt1_found == false && 0.732 < conf && conf < 1.206)
        {
            Ypt1_rpts1s_id = i;
            Ypt1_found = true;
        }

        if (Lpt1_found == false && 1.226 < conf && conf < 2.093 )
        {
            Lpt1_rpts1s_id = i;
            Lpt1_found = true;
        }

        if(_anglesignal.R_edge == _anglesignal_false && 1.0 < conf && conf < 2.326)
        {
            rightAngular = conf;
            _anglesignal.R_edge = _anglesignal_max;
        }
        if (Ypt1_found == true && Lpt1_found == true)
            break;
    }

    if (Ypt0_found && Ypt1_found) {
        float dx = rpts0s[Ypt0_rpts0s_id][0] - rpts1s[Ypt1_rpts1s_id][0];
        float dy = rpts0s[Ypt0_rpts0s_id][1] - rpts1s[Ypt1_rpts1s_id][1];
        float dn = my_sqrt(dx * dx + dy * dy);
        if (ABS(dn - 0.45 * pixel_per_meter) < 0.15 * pixel_per_meter) {
            float dwx = rpts0s[clip(Ypt0_rpts0s_id + 20, 0, rpts0s_num - 1)][0] -
                        rpts1s[clip(Ypt1_rpts1s_id + 20, 0, rpts1s_num - 1)][0];
            float dwy = rpts0s[clip(Ypt0_rpts0s_id + 20, 0, rpts0s_num - 1)][1] -
                        rpts1s[clip(Ypt1_rpts1s_id + 20, 0, rpts1s_num - 1)][1];
            float dwn = my_sqrt(dwx * dwx + dwy * dwy);
            if (!(dwn > 0.7 * pixel_per_meter &&
                  rpts0s[clip(Ypt0_rpts0s_id + 20, 0, rpts0s_num - 1)][0] < rpts0s[Ypt0_rpts0s_id][0] &&
                  rpts1s[clip(Ypt1_rpts1s_id + 20, 0, rpts1s_num - 1)][0] > rpts1s[Ypt1_rpts1s_id][0])) {
                Ypt0_found = Ypt1_found = false;
            }
        } else {
            Ypt0_found = Ypt1_found = false;
        }
    }

        if (Lpt0_found && Lpt1_found) {
            float dx = rpts0s[Lpt0_rpts0s_id][0] - rpts1s[Lpt1_rpts1s_id][0];
            float dy = rpts0s[Lpt0_rpts0s_id][1] - rpts1s[Lpt1_rpts1s_id][1];
            float dn = my_sqrt(dx * dx + dy * dy);
            if (ABS(dn - 0.45 * pixel_per_meter) < 0.15 * pixel_per_meter) {
                float dwx = rpts0s[clip(Lpt0_rpts0s_id + 20, 0, rpts0s_num - 1)][0] -
                            rpts1s[clip(Lpt1_rpts1s_id + 20, 0, rpts1s_num - 1)][0];
                float dwy = rpts0s[clip(Lpt0_rpts0s_id + 20, 0, rpts0s_num - 1)][1] -
                            rpts1s[clip(Lpt1_rpts1s_id + 20, 0, rpts1s_num - 1)][1];
                float dwn = my_sqrt(dwx * dwx + dwy * dwy);
                if (!(dwn > 0.7 * pixel_per_meter &&
                      rpts0s[clip(Lpt0_rpts0s_id + 20, 0, rpts0s_num - 1)][0] < rpts0s[Lpt0_rpts0s_id][0] &&//涓や晶杈圭晫鍚戜袱杈规墿鍏�
                      rpts1s[clip(Lpt1_rpts1s_id + 20, 0, rpts1s_num - 1)][0] > rpts1s[Lpt1_rpts1s_id][0])) {
                    Lpt0_found = Lpt1_found = false;
                }
            } else {
                Lpt0_found = Lpt1_found = false;
            }
        }
}


float Length;

void Information(void)
{
    Length = ABS(rpts0[50][0] - rpts1[50][0]);
    //打印数据
    printf("%f,%f,%f\r\n",rpts0[50][0], rpts1[50][0],Length);


    Length = ABS(rpts0[10][0] - rpts1[10][0]);
    printf("%f,%f,%f\r\n",rpts0[10][0], rpts1[10][0],Length);
}


Int16 Left_StareLine;
Int16 Right_StareLine;

Uint8 LLoopY;
Uint8 RLoopY;

Uint8 LStarePointStatus;
Uint8 RStarePointStatus;


void ImageHandle_LineProcess(Uint8 (*InImg)[IMGW], Uint8 Threshold)
{

    Int16 Temp_StareLine;//, Temp_ErrorLine;
    Int16 Begined_LoopY;

    memset(ipts0, 0, sizeof(ipts0));
    memset(ipts1, 0, sizeof(ipts1));

    Temp_StareLine = IMGH;

    while((--Temp_StareLine) > -1)
    {
        m_stLPnt[Temp_StareLine].m_i16x = 0;
        m_stLPnt[Temp_StareLine].m_i16y = 0;

        m_stRPnt[Temp_StareLine].m_i16x = 0;
        m_stRPnt[Temp_StareLine].m_i16y = 0;

        m_stCPnt[Temp_StareLine].m_i16x = 0;
        m_stCPnt[Temp_StareLine].m_i16y = 0;

        m_stTPnt[Temp_StareLine].m_i16x = 0;
        m_stTPnt[Temp_StareLine].m_i16y = 0;

        m_i8LMonotonicity[Temp_StareLine] = 0;
        m_i8RMonotonicity[Temp_StareLine] = 0;
    }

    Left_StareLine  = 0;
    Right_StareLine = 0;

    LLoopY = IMGH - 1;
    RLoopY = IMGH - 1;

    ipts0_num = 0;
    ipts1_num = 0;


    Begined_LoopY = IMGH;
    LStarePointStatus = 0;
    RStarePointStatus = 0;

    while((--Begined_LoopY) > 55)
    {
        Temp_StareLine = m_stOptimalPoint.m_i16x + 6;
        while((--Temp_StareLine) > 2)
        {
            if(
                    InImg[Begined_LoopY][Temp_StareLine - 2] == B_BLACK &&
                    InImg[Begined_LoopY][Temp_StareLine - 1] == B_BLACK &&
                    InImg[Begined_LoopY][Temp_StareLine    ] == B_WHITE &&
                    InImg[Begined_LoopY][Temp_StareLine + 1] == B_WHITE
                    )
            {
                m_stLPnt[Begined_LoopY].m_i16x = Temp_StareLine;
                m_stLPnt[Begined_LoopY].m_i16y = Begined_LoopY;
                LStarePointStatus++;
            }
        }

        Temp_StareLine = m_stOptimalPoint.m_i16x - 6;
        while((++Temp_StareLine) < IMGW - 2)
        {
            if(
                    InImg[Begined_LoopY][Temp_StareLine - 1] == B_WHITE &&
                    InImg[Begined_LoopY][Temp_StareLine    ] == B_WHITE &&
                    InImg[Begined_LoopY][Temp_StareLine + 1] == B_BLACK &&
                    InImg[Begined_LoopY][Temp_StareLine + 2] == B_BLACK
                    )
            {

                m_stRPnt[Begined_LoopY].m_i16x = Temp_StareLine;
                m_stRPnt[Begined_LoopY].m_i16y = Begined_LoopY;
                RStarePointStatus++;
            }
        }
    }

    Begined_LoopY = 59;
    while((--Begined_LoopY) > 55)
    {
        if(m_stLPnt[Begined_LoopY].m_i16y && m_stLPnt[Begined_LoopY - 1].m_i16y)
        {
            Left_StareLine = m_stLPnt[Begined_LoopY].m_i16x;
            LLoopY = m_stLPnt[Begined_LoopY].m_i16y;
            break;
        }
    }

    Begined_LoopY = 59;
    while((--Begined_LoopY) > 55)
    {
        if(m_stRPnt[Begined_LoopY].m_i16y && m_stRPnt[Begined_LoopY - 1].m_i16y)
        {
            Right_StareLine = m_stRPnt[Begined_LoopY].m_i16x;
            RLoopY = m_stRPnt[Begined_LoopY].m_i16y;
            break;
        }
    }

    if (AT_IMAGE(&img_raw, Left_StareLine, LLoopY) > Threshold && LStarePointStatus != 0)
    {
        ipts0_num = 80;
        findline_lefthand_adaptive(&img_raw, Threshold, Left_StareLine, LLoopY, ipts0, &ipts0_num);
    }

    if (AT_IMAGE(&img_raw, Right_StareLine, RLoopY) > Threshold && RStarePointStatus != 0)
    {
        ipts1_num = 80;
        findline_righthand_adaptive(&img_raw, Threshold, Right_StareLine, RLoopY, ipts1, &ipts1_num);
    }


    /*透视变换*/
    for (int i = 0; i < ipts0_num; i++) {
        rpts0[i][0] = (rot[1][0]*ipts0[i][1]+rot[1][1]*ipts0[i][0]+rot[1][2])/(rot[2][0]*ipts0[i][1]+rot[2][1]*ipts0[i][0]+rot[2][2]);
        rpts0[i][1] = (rot[0][0]*ipts0[i][1]+rot[0][1]*ipts0[i][0]+rot[0][2])/(rot[2][0]*ipts0[i][1]+rot[2][1]*ipts0[i][0]+rot[2][2]);
    }
    rpts0_num = ipts0_num;
    for (int i = 0; i < ipts1_num; i++) {
        rpts1[i][0] = (rot[1][0]*ipts1[i][1]+rot[1][1]*ipts1[i][0]+rot[1][2])/(rot[2][0]*ipts1[i][1]+rot[2][1]*ipts1[i][0]+rot[2][2]);
        rpts1[i][1] = (rot[0][0]*ipts1[i][1]+rot[0][1]*ipts1[i][0]+rot[0][2])/(rot[2][0]*ipts1[i][1]+rot[2][1]*ipts1[i][0]+rot[2][2]);
    }
    rpts1_num = ipts1_num;

    // 边线滤波
    blur_points(rpts0, rpts0_num, rpts0b, 6);
    rpts0b_num = rpts0_num;
    blur_points(rpts1, rpts1_num, rpts1b, 6);
    rpts1b_num = rpts1_num;

    // 边线等距采样
    rpts0s_num = sizeof(rpts0s) / sizeof(rpts0s[0]);
    resample_points(rpts0b, rpts0b_num, rpts0s, &rpts0s_num, sample_dist * pixel_per_meter);
    rpts1s_num = sizeof(rpts1s) / sizeof(rpts1s[0]);
    resample_points(rpts1b, rpts1b_num, rpts1s, &rpts1s_num, sample_dist * pixel_per_meter);

    // 边线局部角度变化率
    //(int)round(angle_dist / sample_dist)
    local_angle_points(rpts0s, rpts0s_num, rpts0a, 20);
    rpts0a_num = rpts0s_num;
    local_angle_points(rpts1s, rpts1s_num, rpts1a, 20);
    rpts1a_num = rpts1s_num;

    // 角度变化率非极大抑制
    //(int) round(angle_dist / sample_dist) * 2 + 1
    nms_angle(rpts0a, rpts0a_num, rpts0an, 51);
    rpts0an_num = rpts0a_num;
    nms_angle(rpts1a, rpts1a_num, rpts1an, 51);
    rpts1an_num = rpts1a_num;

    //计算曲率
    LeftLoopYangle = -Slope_Calculate(0, ipts0_num,ipts0);
    ReftLoopYangle =  Slope_Calculate(0, ipts1_num,ipts1);

    find_corners();

}





void findline_lefthand_adaptive(image_t *img, Uint8 local_thres, int x, int y, int pts[][2], int *num)
{


    int step = 0, dir = 0, turn = 0;
    while (step < *num  && 2 < x && x < img->width - 2 && 0 < y && y < img->height && turn < 4)
    {
        int front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);
        int frontleft_value = AT(img, x + dir_frontleft[dir][0], y + dir_frontleft[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 1) % 4;
            turn++;
        } else if (frontleft_value < local_thres) {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        } else {

            x += dir_frontleft[dir][0];
            y += dir_frontleft[dir][1];
            dir = (dir + 3) % 4;
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;
}


void findline_righthand_adaptive(image_t *img, Uint8 local_thres,  int x, int y, int pts[][2], int *num)
{

    int step = 0, dir = 0, turn = 0;
    while (step < *num && 2 < x && x < img->width - 2  && 0 < y && y < img->height && turn < 4)
    {

        int front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);
        int frontright_value = AT(img, x + dir_frontright[dir][0], y + dir_frontright[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 3) % 4;
            turn++;
        } else if (frontright_value < local_thres) {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        } else {
            x += dir_frontright[dir][0];
            y += dir_frontright[dir][1];
            dir = (dir + 1) % 4;
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;
}

void blur_points(float pts_in[][2], int num, float pts_out[][2], int kernel)
{
    int half = kernel / 2;
    for (int i = 0; i < num; i++) {
        pts_out[i][0] = pts_out[i][1] = 0;
        for (int j = -half; j <= half; j++) {
            pts_out[i][0] += pts_in[clip(i + j, 0, num - 1)][0] * (half + 1 - ABS(j));
            pts_out[i][1] += pts_in[clip(i + j, 0, num - 1)][1] * (half + 1 - ABS(j));
        }
        pts_out[i][0] /= (2 * half + 2) * (half + 1) / 2;
        pts_out[i][1] /= (2 * half + 2) * (half + 1) / 2;
    }
}

void resample_points(float pts_in[][2], int num1, float pts_out[][2], int *num2, float dist)
{
    if (num1 < 0) {
        *num2 = 0;
        return;
    }
    pts_out[0][0] = pts_in[0][0];
    pts_out[0][1] = pts_in[0][1];
    int len = 1;
    for (int i = 0; i < num1 - 1 && len < *num2; i++) {
        float x0 = pts_in[i][0];
        float y0 = pts_in[i][1];
        float x1 = pts_in[i + 1][0];
        float y1 = pts_in[i + 1][1];

        do {
            float x = pts_out[len - 1][0];
            float y = pts_out[len - 1][1];

            float dx0 = x0 - x;
            float dy0 = y0 - y;
            float dx1 = x1 - x;
            float dy1 = y1 - y;

            float dist0 = my_sqrt(dx0 * dx0 + dy0 * dy0);
            float dist1 = my_sqrt(dx1 * dx1 + dy1 * dy1);

            float r0 = (dist1 - dist) / (dist1 - dist0);
            float r1 = 1 - r0;

            if (r0 < 0 || r1 < 0) break;
            x0 = x0 * r0 + x1 * r1;
            y0 = y0 * r0 + y1 * r1;
            pts_out[len][0] = x0;
            pts_out[len][1] = y0;
            len++;
        } while (len < *num2);

    }
    *num2 = len;
}

void local_angle_points(float pts_in[][2], int num, float angle_out[], int dist)
{
    for (int i = 0; i < num; i++) {
        if (i <= 0 || i >= num - 1) {
            angle_out[i] = 0;
            continue;
        }
        float dx1 = pts_in[i][0] - pts_in[clip(i - dist, 0, num - 1)][0];
        float dy1 = pts_in[i][1] - pts_in[clip(i - dist, 0, num - 1)][1];
        float dn1 = my_sqrt(dx1 * dx1 + dy1 * dy1);
        float dx2 = pts_in[clip(i + dist, 0, num - 1)][0] - pts_in[i][0];
        float dy2 = pts_in[clip(i + dist, 0, num - 1)][1] - pts_in[i][1];
        float dn2 = my_sqrt(dx2 * dx2 + dy2 * dy2);
        float c1 = dx1 / dn1;
        float s1 = dy1 / dn1;
        float c2 = dx2 / dn2;
        float s2 = dy2 / dn2;
        angle_out[i] = atan2f(c1 * s2 - c2 * s1, c2 * c1 + s2 * s1);
    }
}


void nms_angle(float angle_in[], int num, float angle_out[], int kernel)
{
    int half = kernel / 2;
    for (int i = 0; i < num; i++) {
        angle_out[i] = angle_in[i];
        for (int j = -half; j <= half; j++) {
            if (ABS(angle_in[clip(i + j, 0, num - 1)]) > ABS(angle_out[i])) {
                angle_out[i] = 0;
                break;
            }
        }
    }
}


void track_leftline(float pts_in[][2], int num, float pts_out[][2], int approx_num, float dist) {
    for (int i = 0; i < num; i++) {
        float dx = pts_in[clip(i + approx_num, 0, num - 1)][0] - pts_in[clip(i - approx_num, 0, num - 1)][0];
        float dy = pts_in[clip(i + approx_num, 0, num - 1)][1] - pts_in[clip(i - approx_num, 0, num - 1)][1];
        float dn = my_sqrt(dx * dx + dy * dy);
        dx /= dn;
        dy /= dn;
        pts_out[i][0] = pts_in[i][0] - dy * dist;
        pts_out[i][1] = pts_in[i][1] + dx * dist;
    }
}

void track_rightline(float pts_in[][2], int num, float pts_out[][2], int approx_num, float dist) {
    for (int i = 0; i < num; i++) {
        float dx = pts_in[clip(i + approx_num, 0, num - 1)][0] - pts_in[clip(i - approx_num, 0, num - 1)][0];
        float dy = pts_in[clip(i + approx_num, 0, num - 1)][1] - pts_in[clip(i - approx_num, 0, num - 1)][1];
        float dn = my_sqrt(dx * dx + dy * dy);
        dx /= dn;
        dy /= dn;
        pts_out[i][0] = pts_in[i][0] + dy * dist;
        pts_out[i][1] = pts_in[i][1] - dx * dist;
    }
}
