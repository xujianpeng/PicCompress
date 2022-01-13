const int16 PMAX = 440 ;       //大于440的值将被当成 440
const int16 PMIN = 40 ;       //小于PMIN的值将被当成      赛道越简单，速度越快，否则越是复杂就越慢
const int16 DMAX = 55;       //这个值表示图像后DMAX行空的多少，空的越多，速度越慢
const int16 DMIN = 0;       //
const int16 SMAX = 100;      //两个参数都统一到0~100的值（归一化）
const int16 SMIN = 0 ; 
const int16 FMAX = 100 ;     //语言值的满幅值
              // 0  1   2   3   4   5   6
int16 PFF[7] = { 0, 16, 32, 48, 64, 80, 96 };     //统一到最大值； 最小值；
int16 DFF[7] = { 0, 16, 32, 48, 64, 80, 96 };     //

/*输出量U语言值特征点    0    1    2    3    4    5    6 */
/*输出量U语言值特征点    0    1    2    3    4    5    6 */
int16 UFF1[7] =     {   220, 235, 240, 255, 275, 305, 340};   //数组里面的数值为速度
int16 UFF2[7] =     {   130, 140, 145, 150, 155, 190, 220};     
int16 UFF3[7] =     {   135, 145, 150, 155, 160, 200, 260};     
int16 UFF4[7] =     {   140, 150, 155, 160, 165, 220, 270};  //preSee   
 
uint16 rule[7][7] = 
{
  /*Track_complexity */  //  0,  1,  2,  3,  4,  5,  6    Prospect_See
  /* 0 */                   {  6,  5,  4,  3,  2,  1,  1,} , //U语言数组里面的下标
  /* 1 */                   {  5,  5,  4,  3,  2,  1,  1,} ,      
  /* 2 */                   {  4,  4,  3,  2,  1,  1,  0,} ,
  /* 3 */                   {  3,  3,  3,  2,  1,  1,  0,} ,
  /* 4 */                   {  2,  2,  2,  1,  1,  0,  0,} ,
  /* 5 */                   {  2,  1,  1,  0,  0,  0,  0,} , 
  /* 6 */                   {  1,  1,  0,  0,  0,  0,  0,}
};

int16 *UFF = UFF1 ;  //这样可以选择不用的U语言数组



/*  P 代表赛道复杂程度
D 表示前瞻
模糊控制算法通过这两个跟定量以及规则表给出当前应该输出的速度
*/
int16 FuzzySet_Speed(int16 P, int16 D)          //模糊运算引擎，返回速度值 
{
   int16 U;       /*偏差，以及输出值的精确量 */
   uint16 PF[2];
   uint16 DF[2]; 
   uint16 UF[4];  /*偏差，偏差微分以及输出值的隶属度PF[1]是P的隶属度，PF[0]是隶属度的补集 */
   int16 Pn = 0, Dn = 0;
   int16 Un[4];
   int32 temp1,temp2;
   
   if(P < PMIN)
     P = PMIN;
   else
     if(P > PMAX)
       P = PMAX;
   
   P = (int16)((double)(P - PMIN) / (PMAX - PMIN) * (SMAX - SMIN) + SMIN); //归一化到SMIN ~ SMAX
   
   
   if(D < DMIN)
     D = DMIN;
   else 
     if( D > DMAX)
       D = DMAX;
    
   D = (int16)(((double)D )/ (DMAX - DMIN) * (SMAX - SMIN) + SMIN) ;    //归一化到SMIN ~ SMAX

   /*隶属度的确定*/
   /*根据PD的指定语言获得有效的隶属度*/
   
   if(P > PFF[0] && P < PFF[6])
   {
        if (P <= PFF[1])
        {
          Pn = 1;
          PF[0] = (uint16)(FMAX * ((float)(PFF[1] - P) / (PFF[1] - PFF[0])));
        }
        else if (P <= PFF[2])
        {
          Pn = 2;
          PF[0] = (uint16)(FMAX * ((float)(PFF[2] - P) / (PFF[2] - PFF[1])));
        }
        else if (P <= PFF[3])
        {
          Pn = 3;
          PF[0] = (uint16)(FMAX * ((float)(PFF[3] - P) / (PFF[3] - PFF[2])));
        }
        else if (P <= PFF[4])
        {
          Pn = 4;
          PF[0] = (uint16)(FMAX * ((float)(PFF[4] - P) / (PFF[4] - PFF[3])));
        }
        else if (P <= PFF[5])
        {
          Pn = 5;
          PF[0] = (uint16)(FMAX * ((float)(PFF[5] - P) / (PFF[5] - PFF[4])));
        }
        else if (P <= PFF[6])
        {
          Pn = 6;
          PF[0] = (uint16)(FMAX * ((float)(PFF[6] - P) / (PFF[6] - PFF[5])));
        }
   }
   else if (P <= PFF[0])
   {
        Pn = 1;
        PF[0] = (uint16)(FMAX);
    }
   else if (P <= PFF[6])
   {
        Pn = 6; 
        PF[0] = 0; 
    }
   PF[1] = (uint16)(FMAX - PF[0]);
     
     
     if (D > DFF[0] && D < DFF[6])
      {
        if (D <= DFF[1])
        {
          Dn = 1; DF[0] = (uint16)(FMAX * ((float)(DFF[1] - D) / (DFF[1] - DFF[0])));
        }
        else if (D <= DFF[2])
        {
          Dn = 2;
          DF[0] = (uint16)(FMAX * ((float)(DFF[2] - D) / (DFF[2] - DFF[1])));
        }
        else if (D <= DFF[3])
        {
          Dn = 3;
          DF[0] = (uint16)(FMAX * ((float)(DFF[3] - D) / (DFF[3] - DFF[2])));
        }
        else if (D <= DFF[4])
        {
          Dn = 4;
          DF[0] = (uint16)(FMAX * ((float)(DFF[4] - D) / (DFF[4] - DFF[3])));
        }
        else if (D <= DFF[5])
        {
          Dn = 5;
          DF[0] = (uint16)(FMAX * ((float)(DFF[5] - D) / (DFF[5] - DFF[4])));
        }
        else if (D <= DFF[6])
        {
          Dn = 6;
          DF[0] = (uint16)(FMAX * ((float)(DFF[6] - D) / (DFF[6] - DFF[5])));
        }
      }
      else if (D <= DFF[0])
      {
        Dn = 1;
        DF[0] = (uint16)(FMAX);
      }
      else if (D >= DFF[6])
      {
        Dn = 6;
        DF[0] = 0;
      }
      DF[1] = (uint16)(FMAX - DF[0]);
    
   
       /*使用误差范围优化后的规则表rule[7][7]*/
      /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
      /*一般都是四个规则有效*/
      Un[0] = rule[Pn - 1][ Dn - 1];
      Un[1] = rule[Pn][ Dn - 1];
      Un[2] = rule[Pn - 1][ Dn];
      Un[3] = rule[Pn][ Dn];
      if (PF[0] <= DF[0])//计算R表，隶属函数求交集
        UF[0] = PF[0];
      else
        UF[0] = DF[0];
      if (PF[1] <= DF[0])
        UF[1] = PF[1];
      else
        UF[1] = DF[0];
      if (PF[0] <= DF[1])
        UF[2] = PF[0];
      else
        UF[2] = DF[1];
      if (PF[1] <= DF[1])
        UF[3] = PF[1];
      else
        UF[3] = DF[1];
      /*同隶属函数输出语言值求大*/
      
      if (Un[0] == Un[1])
      {
        if (UF[0] > UF[1])
          UF[1] = 0;
        else
          UF[0] = 0;
      }
      if (Un[0] == Un[2])
      {
        if (UF[0] > UF[2])
          UF[2] = 0;
        else
          UF[0] = 0;
      }
      if (Un[0] == Un[3])
      {
        if (UF[0] > UF[3])
          UF[3] = 0;
        else
          UF[0] = 0;
      }
      if (Un[1] == Un[2])
      {
        if (UF[1] > UF[2])
          UF[2] = 0;
        else
          UF[1] = 0;
      }
      if (Un[1] == Un[3])
      {
        if (UF[1] > UF[3])
          UF[3] = 0;
        else
          UF[1] = 0;
      }
      if (Un[2] == Un[3])
      {
        if (UF[2] > UF[3])
          UF[3] = 0;
        else
          UF[2] = 0;
      }
      
      /*重心法(按隶属度加权平均)反模糊*/
      /*Un[]原值为输出隶属函数标号，转换为隶属函数值*/
      // if (Un[0] >= 0)
      Un[0] = UFF[Un[0]];
      // else
      //     Un[0] = (int16)(-UFF[-Un[0]]);
      // if (Un[1] >= 0)
      Un[1] = UFF[Un[1]];
      //  else
      //    Un[1] = (int16)(-UFF[-Un[1]]);
      // if (Un[2] >= 0)
      Un[2] = UFF[Un[2]];
      //  else
      //     Un[2] = (int16)(-UFF[-Un[2]]);
      // if (Un[3] >= 0)
      Un[3] = UFF[Un[3]];
      // else
      //     Un[3] = (int16)(-UFF[-Un[3]]);
      
      temp1 = UF[0] * Un[0] + UF[1] * Un[1] + UF[2] * Un[2] + UF[3] * Un[3];
      temp2 = UF[0] + UF[1] + UF[2] + UF[3];
      U = (int16)(temp1 / temp2);
      return U;
}
   