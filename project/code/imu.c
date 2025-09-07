
#include "imu.h"


uint8 l_border[image_h];//左线数组
uint8 r_border[image_h];//右线数组
uint8 center_line[image_h];//中线数组


int my_abs(int value)
{
if(value>=0) return value;
else return -value;
}
 
int16 limit_a_b(int16 x, int a, int b)
{
    if(x<a) x = (int16)a;
    if(x>b) x = (int16)b;
    return x;
}
 
int16 limit1(int16 x, int16 y)
{
    if (x > y)             return y;
    else if (x < -y)       return -y;
    else                return x;
}
 
 
uint8 original_image[image_h][image_w];
uint8 image_thereshold;//图像分割阈值
 
//      复制获得一副灰度图像
void Get_image(uint8(*mt9v03x_image)[image_w])
{
#define use_num     1   //1就是不压缩，2就是压缩一倍
    uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < image_h; i += use_num)         
    {
        for (j = 0; j <image_w; j += use_num)    
        {
            original_image[row][line] = mt9v03x_image[i][j];
            line++;
        }
        line = 0;
        row++;
    }
}
 
//     动态阈值大津法
 
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
{
#define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};
 
    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;
 
 
    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }
 
 
 
 
    for (MinValue = 0; MinValue < 255 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值
 
    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }
 
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }
 
    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
   return Threshold;
}
 
//    图像二值化
uint8 bin_image[image_h][image_w];//图像数组
void turn_to_bin(void)
{
  uint8 i,j;
 //image_thereshold = otsuThreshold(original_image[0], image_w, image_h);
  for(i = 0;i<image_h;i++)
  {
      for(j = 0;j<image_w;j++)
      {
          if(original_image[i][j]>125)bin_image[i][j] = white_pixel;
          else bin_image[i][j] = black_pixel;
      }
  }
}
 
 
//------------------------------------------------------
//函数名称：void get_start_point(uint8 start_row)
//功能说明：寻找两个边界的边界点作为八邻域循环的起始点
//参数说明：输入任意行数
//函数返回：无
//example：  get_start_point(image_h-2)
//-----------------------------------------------------
uint8 start_point_l[2] = { 0 };//左边起点的x，y值
uint8 start_point_r[2] = { 0 };//右边起点的x，y值

uint8 get_start_point(uint8 start_row)
{
    uint8 i = 0,l_found = 0,r_found = 0;
    //清零
    start_point_l[0] = 0;//x
    start_point_l[1] = 0;//y
 
    start_point_r[0] = 0;//x
    start_point_r[1] = 0;//y
 
                    
            
     for(i=1;i<MT9V03X_W-1;i++) 
    {
        start_point_l[0] = i;//x
        start_point_l[1] = start_row;//y
        if (bin_image[start_row][i] == 255 && bin_image[start_row][i - 1] == 0)
        {
            //printf("找到左边起点image[%d][%d]\n", start_row,i);
            l_found = 1;
            break;
        }
    }
 
    
     for(i=MT9V03X_W-2;i>0;i--)
    {
        start_point_r[0] = i;//x
        start_point_r[1] = start_row;//y
        if (bin_image[start_row][i] == 255 && bin_image[start_row][i + 1] == 0)
        {
            //printf("找到右边起点image[%d][%d]\n",start_row, i);
            r_found = 1;
            break;
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    

        //从中间往左边，先找起点
//    for (i = image_w / 2; i > border_min; i--)
//    {
//        start_point_l[0] = i;//x
//        start_point_l[1] = start_row;//y
//        if (bin_image[start_row][i] == 255 && bin_image[start_row][i - 1] == 0)
//        {
//            printf("找到左边起点image[%d][%d]\n", start_row,i);
//            l_found = 1;
//            break;
//        }
//    }
// 
//    for (i = image_w / 2; i < border_max; i++)
//    {
//        start_point_r[0] = i;//x
//        start_point_r[1] = start_row;//y
//        if (bin_image[start_row][i] == 255 && bin_image[start_row][i + 1] == 0)
//        {
//            printf("找到右边起点image[%d][%d]\n",start_row, i);
//            r_found = 1;
//            break;
//        }
//    }
 
    if(l_found && r_found)return 1;
    else {
        //printf("未找到起点\n");
        return 0;
    }
}
 
/*
函数名称：void search_l_r(uint16 break_flag, uint8(*image)[image_w],uint16 *l_stastic, uint16 *r_stastic,
                            uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y,uint8*hightest)
 
功能说明：八邻域正式开始找右边点的函数，这个是左右线一次性找完。
参数说明：
break_flag_r            ：最多需要循环的次数
(*image)[image_w]       ：需要进行找点的图像数组，必须是二值图,填入数组名称即可
                       特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
*l_stastic              ：统计左边数据，用来输入初始数组成员的序号和取出循环次数
*r_stastic              ：统计右边数据，用来输入初始数组成员的序号和取出循环次数
l_start_x               ：左边起点横坐标
l_start_y               ：左边起点纵坐标
r_start_x               ：右边起点横坐标
r_start_y               ：右边起点纵坐标
hightest                ：循环结束所得到的最高高度
函数返回：无
example：·
    search_l_r((uint16)USE_num,image,&data_stastics_l, &data_stastics_r,start_point_l[0],
                start_point_l[1], start_point_r[0], start_point_r[1],&hightest);
 */
#define USE_num image_h*3   //定义找点的数组成员个数按理说300个点能放下，但是有些特殊情况确实难顶，多定义了一点
 
 //存放点的x，y坐标
uint16 points_l[(uint16)USE_num][2] = { {  0 } };//左线
uint16 points_r[(uint16)USE_num][2] = { {  0 } };//右线
uint16 dir_r[(uint16)USE_num] = { 0 };//用来存储右边生长方向
uint16 dir_l[(uint16)USE_num] = { 0 };//用来存储左边生长方向
uint16 data_stastics_l = 0;//统计左边找到点的个数
uint16 data_stastics_r = 0;//统计右边找到点的个数
uint8 hightest = 0;//最高点
void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint8*hightest)
{
 
    uint8 i = 0, j = 0;
 
    //左边变量
    uint8 search_filds_l[8][2] = { {  0 } };
    uint8 index_l = 0;
    uint8 temp_l[8][2] = { {  0 } };
    uint8 center_point_l[2] = {  0 };
    uint16 l_data_statics;//统计左边
    //定义八个邻域
    static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是顺时针
 
    //右边变量
    uint8 search_filds_r[8][2] = { {  0 } };
    uint8 center_point_r[2] = { 0 };//中心坐标点
    uint8 index_r = 0;//索引下标
    uint8 temp_r[8][2] = { {  0 } };
    uint16 r_data_statics;//统计右边
    //定义八个邻域
    static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是逆时针
 
    l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
    r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来
 
    //第一次更新坐标点  将找到的起点值传进来
    center_point_l[0] = l_start_x;//x
    center_point_l[1] = l_start_y;//y
    center_point_r[0] = r_start_x;//x
    center_point_r[1] = r_start_y;//y
 
        //开启邻域循环
    while (break_flag--)
    {
 
        //左边
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
            search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_l[l_data_statics][0] = center_point_l[0];//x
        points_l[l_data_statics][1] = center_point_l[1];//y
        l_data_statics++;//索引加一
 
        //右边
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
            search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_r[r_data_statics][0] = center_point_r[0];//x
        points_r[r_data_statics][1] = center_point_r[1];//y
 
        index_l = 0;//先清零，后使用
        for (i = 0; i < 8; i++)
        {
            temp_l[i][0] = 0;//先清零，后使用
            temp_l[i][1] = 0;//先清零，后使用
        }
 
        //左边判断
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
                && image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
            {
                temp_l[index_l][0] = search_filds_l[(i)][0];
                temp_l[index_l][1] = search_filds_l[(i)][1];
                index_l++;
                dir_l[l_data_statics - 1] = (i);//记录生长方向
            }
 
            if (index_l)
            {
                //更新坐标点
                center_point_l[0] = temp_l[0][0];//x
                center_point_l[1] = temp_l[0][1];//y
                for (j = 0; j < index_l; j++)
                {
                    if (center_point_l[1] > temp_l[j][1])
                    {
                        center_point_l[0] = temp_l[j][0];//x
                        center_point_l[1] = temp_l[j][1];//y
                    }
                }
            }
 
        }
        if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
            && points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
            ||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
                && points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
        {
            //printf("三次进入同一个点，退出\n");
            break;
        }
        if (my_abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
            && my_abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
            )
        {
            //printf("\n左右相遇退出\n");
            *hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
            //printf("\n在y=%d处退出\n",*hightest);
            break;
        }
        if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
        {
            //printf("\n如果左边比右边高了，左边等待右边\n");
            continue;//如果左边比右边高了，左边等待右边
        }
        if (dir_l[l_data_statics - 1] == 7
            && (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
        {
            //printf("\n左边开始向下了，等待右边，等待中... \n");
            center_point_l[0] = (uint8)points_l[l_data_statics - 1][0];//x
            center_point_l[1] = (uint8)points_l[l_data_statics - 1][1];//y
            l_data_statics--;
        }
        r_data_statics++;//索引加一
 
        index_r = 0;//先清零，后使用
        for (i = 0; i < 8; i++)
        {
            temp_r[i][0] = 0;//先清零，后使用
            temp_r[i][1] = 0;//先清零，后使用
        }
 
        //右边判断
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
                && image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
            {
                temp_r[index_r][0] = search_filds_r[(i)][0];
                temp_r[index_r][1] = search_filds_r[(i)][1];
                index_r++;//索引加一
                dir_r[r_data_statics - 1] = (i);//记录生长方向
               // printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
            }
            if (index_r)
            {
 
                //更新坐标点
                center_point_r[0] = temp_r[0][0];//x
                center_point_r[1] = temp_r[0][1];//y
                for (j = 0; j < index_r; j++)
                {
                    if (center_point_r[1] > temp_r[j][1])
                    {
                        center_point_r[0] = temp_r[j][0];//x
                        center_point_r[1] = temp_r[j][1];//y
                    }
                }
 
            }
        }
 
 
    }
 
 
    //取出循环次数
    *l_stastic = l_data_statics;
    *r_stastic = r_data_statics;
}
/*
函数名称：void get_left(uint16 total_L)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_L ：找到的点的总数
函数返回：无
备    注：
example： get_left(data_stastics_l );
 */
void get_left(uint16 total_L)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;
    //初始化
    for (i = 0;i<image_h;i++)
    {
        l_border[i] = border_min;
    }
    h = image_h - 2;
    //左边
    for (j = 0; j < total_L; j++)
    {
       // printf("%d\n", j);
        if (points_l[j][1] == h)
        {
            l_border[h] = points_l[j][0]+1;
        }
        else continue; //每行只取一个点，没到下一行就不记录
        h--;
        if (h == 0)
        {
            break;//到最后一行退出
        }
    }
}
/*
函数名称：void get_right(uint16 total_R)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_R  ：找到的点的总数
函数返回：无
备    注：
example：get_right(data_stastics_r);
 */
void get_right(uint16 total_R)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;
    for (i = 0; i < image_h; i++)
    {
        r_border[i] = border_max;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据
    }
    h = image_h - 2;
    //右边
    for (j = 0; j < total_R; j++)
    {
        if (points_r[j][1] == h)
        {
            r_border[h] = points_r[j][0] - 1;
        }
        else continue;//每行只取一个点，没到下一行就不记录
        h--;
        if (h == 0)break;//到最后一行退出
    }
}
 
//定义膨胀和腐蚀的阈值区间
#define threshold_max   255*5//此参数可根据自己的需求调节
#define threshold_min   255*2//此参数可根据自己的需求调节
void image_filter(uint8(*bin_image)[image_w])//形态学滤波，简单来说就是膨胀和腐蚀的思想
{
    uint16 i, j;
    uint32 num = 0;
 
 
    for (i = 1; i < image_h - 1; i++)
    {
        for (j = 1; j < (image_w - 1); j++)
        {
            //统计八个方向的像素值
            num =
                bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
                + bin_image[i][j - 1] + bin_image[i][j + 1]
                + bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];
 
 
            if (num >= threshold_max && bin_image[i][j] == 0)
            {
 
                bin_image[i][j] = 255;//白  可以搞成宏定义，方便更改
 
            }
            if (num <= threshold_min && bin_image[i][j] == 255)
            {
 
                bin_image[i][j] = 0;//黑
 
            }
 
        }
    }
 
}
 
/*
函数名称：void image_draw_rectan(uint8(*image)[image_w])
功能说明：给图像画一个黑框
参数说明：uint8(*image)[image_w] 图像首地址
函数返回：无
example： image_draw_rectan(bin_image);
 */
void image_draw_rectan(uint8(*image)[image_w])
{
 
    uint8 i = 0;
    for (i = 0; i < image_h; i++)
    {
        image[i][0] = 0;
        image[i][1] = 0;
        image[i][image_w - 1] = 0;
        image[i][image_w - 2] = 0;
 
    }
    for (i = 0; i < image_w; i++)
    {
        image[0][i] = 0;
        image[1][i] = 0;
        //image[image_h-1][i] = 0;
 
    }
}
 
/*
函数名称：void image_process(void)
功能说明：最终处理函数
example： image_process();
 */

//int zhongxian=94;
//int error_tow=0;
//int row_error[MT9V03X_H];
//int16 sum_row_error;
//int16 sum_QZ;
uint8 QZ[120]={
                               1,1,1,1,1,1,1,1,1,1,//图像0-10行权重
                               1,1,1,1,1,1,1,1,1,1,//图像10-20行权重  23--60cm
                               1,1,1,1,1,1,1,1,1,1,//图像20-30行权重
                               1,1,1,1,1,1,1,1,1,1,//图像30-40行权重
                               20,19,19,18,18,15,15,14,13,12,//图像40-50行权重
                               10,10,10,9,9,9,9,8,8,8,//图像50-60行权重
                               8,8,7,7,7,7,6,6,6,6,//图像60-70行权重
                               5,5,5,4,4,1,1,1,1,1,//图像70-80行权重
                               1,1,1,1,1,1,1,1,1,1,
                               1,1,1,1,1,1,1,1,1,1,
                               1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,
};


int image_process(void)
{
uint16 i;
uint8 hightest = 0;//定义一个最高行，tip：这里的最高指的是y值的最小

/*这是离线调试用的*/
memcpy(original_image[0],mt9v03x_image[0],MT9V03X_IMAGE_SIZE);                        //Get_image(mt9v03x_image);
turn_to_bin();

/*提取赛道边界*/
image_filter(bin_image);//滤波
image_draw_rectan(bin_image);//预处理
//清零
data_stastics_l = 0;
data_stastics_r = 0;
if (get_start_point(image_h - 2))//找到起点了，再执行八领域，没找到就一直找
{
    //printf("正在开始八领域\n");
    search_l_r((uint16)USE_num, bin_image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &hightest);
    //printf("八邻域已结束\n");
    // 从爬取的边界线内提取边线 ， 这个才是最终有用的边线
    get_left(data_stastics_l);
    get_right(data_stastics_r);
    //处理函数放这里，不要放到if外面去了，不要放到if外面去了，不要放到if外面去了
 
}

//显示图像   
tft180_show_gray_image(0, 0, bin_image[0], MT9V03X_W, MT9V03X_H, 94, 60, 0);
 
    //根据最终循环次数画出边界点
    for (i = 0; i < data_stastics_l; i++)
    {
       tft180_draw_point((int)(0.5*points_l[i][0]), (int)(0.5*points_l[i][1]), uesr_RED);//显示起点
    }
    for (i = 0; i < data_stastics_r; i++)
    {
       tft180_draw_point((int)(0.5*points_r[i][0]), (int)(0.5*points_r[i][1]), uesr_BLUE);//显示起点
    }
 
    for (i = hightest; i < image_h-1; i++)
    {
        center_line[i] = (l_border[i] + r_border[i])/2;//求中线
        //求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
       tft180_draw_point((int)(0.5*center_line[i]),(int)(0.5*i), uesr_RED);//显示起点 显示中线
       tft180_draw_point((int)(0.5*l_border[i]), (int)(0.5*i), uesr_BLUE);//显示起点 显示左边线
       tft180_draw_point((int)(0.5*r_border[i]), (int)(0.5*i), uesr_GREEN);//显示起点 显示右边线
         
    }
    
//    for(int y=image_h-1;y>=hightest;y--){
//       sum_row_error+=center_line[y];
//       //sum_QZ+=QZ[i];
//    }
//    zhongxian=sum_row_error/187;
//    error_tow=94-zhongxian;
    
//    zhongxian=sum_row_error/sum_QZ;
//    error_tow=94-zhongxian;
    
//    for(uint8 i=MT9V03X_H-1;i>0;i--){
//      row_error[i]=94-center_line[i];
//    } 
//    for(uint8 i=MT9V03X_H-1;i>0;i--){
//    sum_row_error+=(row_error[i]*QZ[i]);
//    sum_QZ+=QZ[i];
//    }
//    
//    error_tow=(int)(sum_row_error/sum_QZ);
    
    int sun_QZ=0;
    int sum_errorT=0;
    int errorT=0;
    for(int y=image_h-1;y>=hightest;y--)
    {
      sum_errorT+=(94-center_line[y])*QZ[y];
      sun_QZ+=QZ[y];
    }
    
    errorT=sum_errorT/sun_QZ;
    
    return errorT;
    
}
 
 
























 
 
 
// 
//int image_process(void)
//{
//uint16 i;
//uint8 hightest = 0;//定义一个最高行，tip：这里的最高指的是y值的最小
//
//*这是离线调试用的*/          //加个/
//
//
//*提取赛道边界*/               //加个/
//image_filter(image);//滤波
//image_draw_rectan(image);//预处理
////清零
//data_stastics_l = 0;
//data_stastics_r = 0;
//if (get_start_point(image_h - 2))//找到起点了，再执行八领域，没找到就一直找
//{
//    //printf("正在开始八领域\n");
//    search_l_r((uint16)USE_num, image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &hightest);
//    //printf("八邻域已结束\n");
//    // 从爬取的边界线内提取边线 ， 这个才是最终有用的边线
//    get_left(data_stastics_l);
//    get_right(data_stastics_r);
//    //处理函数放这里，不要放到if外面去了，不要放到if外面去了，不要放到if外面去了
// 
//}
// 
// 
////显示图像   
//tft180_show_gray_image(0, 0, image[0], MT9V03X_W, MT9V03X_H, 128, 81, 0);
// 
//    //根据最终循环次数画出边界点
//    for (i = 0; i < data_stastics_l; i++)
//    {
//       tft180_draw_point(points_l[i][0], points_l[i][1], uesr_RED);//显示起点
//    }
//    for (i = 0; i < data_stastics_r; i++)
//    {
//       tft180_draw_point(points_r[i][0], points_r[i][1], uesr_BLUE);//显示起点
//    }
// 
//    for (i = hightest; i < image_h-1; i++)
//    {
//        center_line[i] = (l_border[i] + r_border[i]) >> 1;//求中线
//        //求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
//       tft180_draw_point((int)(0.68*center_line[i]),(int)(0.68*i), uesr_RED);//显示起点 显示中线
//       tft180_draw_point((int)(0.68*l_border[i]), (int)(0.68*i), uesr_BLUE);//显示起点 显示左边线
//       tft180_draw_point((int)(0.68*r_border[i]), (int)(0.68*i), uesr_GREEN);//显示起点 显示右边线
//    }
//     
//    //测试
//    int sum_error_tow=0;
//    int error_tow=0;
//    for(int y=image_h-1;y>=hightest;y--)
//    {
//      sum_error_tow+=94-center_line[y];
//    }
//    
//    error_tow=sum_error_tow/(image_h-1-hightest);
//    
//    return error_tow;
// 
// 
//}
// 
// 



