

#ifndef __Perspective_H_
#define __Perspective_H_

#include "zf_common_headfile.h"

#include "FeatureProcess.h"
#include "LeastSquare.h"
#include "stdbool.h"
#include "math.h"


#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct image {
    uint8_t *data;
    uint32_t width;
    uint32_t height;
    uint32_t step;
} image_t;

typedef struct fimage {
    float *data;
    uint32_t width;
    uint32_t height;
    uint32_t step;
} fimage_t;
enum garage_type_e {
    GARAGE_NONE = 0,
    GARAGE_OUT_LEFT, GARAGE_OUT_RIGHT,
    GARAGE_FOUND_LEFT, GARAGE_FOUND_RIGHT,
    GARAGE_IN_LEFT, GARAGE_IN_RIGHT,
    GARAGE_PASS_LEFT, GARAGE_PASS_RIGHT,
    GARAGE_STOP,
    GARAGE_NUM,
};


typedef enum
{
    _anglesignal_false = 0,
    _anglesignal_max,
    _anglesignal_min,
}_anglesignal_enum;

typedef struct _element_angle_
{
    unsigned char L_edge;
    unsigned char R_edge;
}_element_angle_struct;


extern _element_angle_struct _anglesignal;

#define PI               3.14159265358979
#define ROAD_WIDTH      (0.45)
#define POINTS_MAX_LEN  (80)
#define FAR_POINTS_MAX_LEN  (POINTS_MAX_LEN)

#define AT AT_IMAGE
#define AT_IMAGE(img, x, y)          ((img)->data[(y)*(img)->step+(x)])
#define AT_IMAGE_CLIP(img, x, y)     AT_IMAGE(img, clip(x, 0, (img)->width-1), clip(y, 0, (img)->height-1))

#define DEF_IMAGE(ptr, w, h)         {.data=ptr, .width=w, .height=h, .step=w}
#define ROI_IMAGE(img, x1, y1, w, h) {.data=&AT_IMAGE(img, x1, y1), .width=w, .height=h, .step=img.width}

extern image_t img_raw;

extern float leftAngular, rightAngular;


extern int Ypt0_rpts0s_id, Ypt1_rpts1s_id;
extern bool Ypt0_found, Ypt1_found;


extern int Lpt0_rpts0s_id, Lpt1_rpts1s_id;
extern bool Lpt0_found, Lpt1_found;


extern Uint8 Element_LoopY;
extern float LeftLoopYangle, ReftLoopYangle;

//左右找到的点数
extern int ipts0_num, ipts1_num;

extern int ipts0[POINTS_MAX_LEN][2];
extern int ipts1[POINTS_MAX_LEN][2];

extern float rpts0[POINTS_MAX_LEN][2];
extern float rpts1[POINTS_MAX_LEN][2];

extern int far_ipts0[120][2];
extern int fat_ipts1[120][2];

extern int far_ipts0_num, far_ipts1_num;

extern Int16 Left_StareLine;
extern Int16 Right_StareLine;

extern Uint8 LLoopY;
extern Uint8 RLoopY;

extern Uint8 LStarePointStatus;
extern Uint8 RStarePointStatus;

void Information(void);
void measure_corners(void);

//----------------外部函数--------------------
void process_image(Uint8 Threshold);
void find_corners (void);

void ImageHandle_LineProcess(Uint8 (*InImg)[IMGW], Uint8 Threshold);

//----------------底层函数--------------------
void findline_lefthand_adaptive  (image_t *img, Uint8 local_thres,  int x, int y, int pts[][2], int *num);
void findline_righthand_adaptive (image_t *img, Uint8 local_thres,  int x, int y, int pts[][2], int *num);

void blur_points(float pts_in[][2], int num, float pts_out[][2], int kernel);
void resample_points(float pts_in[][2], int num1, float pts_out[][2], int *num2, float dist);
void local_angle_points(float pts_in[][2], int num, float angle_out[], int dist);
void nms_angle(float angle_in[], int num, float angle_out[], int kernel);
void track_leftline(float pts_in[][2], int num, float pts_out[][2], int approx_num, float dist);
void track_rightline(float pts_in[][2], int num, float pts_out[][2], int approx_num, float dist);
#endif


