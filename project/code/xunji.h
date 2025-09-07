#ifndef xunji_h_
#define xunji_h_

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"
#include "zf_device_mt9v03x.h"

#define GrayScale 256

extern uint8 weight_midline[120];

extern uint16 mid_line;
extern int16 error;

extern uint8 base_image[MT9V03X_H][MT9V03X_W];
extern uint8 img_threshold;
extern uint8 image[MT9V03X_H][MT9V03X_W];

extern float zhuanjiao_sum;

extern uint16 left_line_list[MT9V03X_H];
extern uint16 right_line_list[MT9V03X_H];
extern uint16 mid_line_list[MT9V03X_H];
extern uint16 track_wide[MT9V03X_H];
extern uint16 up_line_list[MT9V03X_W];
extern uint16 down_line_list[MT9V03X_W];
extern uint16 track_wide_high[MT9V03X_W];


void towzhi(uint8 value);

void zhuanxiang(int W);

void find_mid_Line_tow(void);

void draw_line(void);

void yanshi(int Y);

uint8 Ostu(uint8 index[MT9V03X_H][MT9V03X_W]);

//detect_corner(void);

void find_mid_Line_weight(void);

uint8 qi_shi_dian(uint8 qi_shi_row);

void find_mid_Line_four(void);

void qing_kong_shu_zu(void);

void find_mid_Line_three(void);

void qing_kong_shu_zu_tow(void);


#endif
