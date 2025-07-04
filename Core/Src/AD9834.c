/*
 * @Description: AD9834波形发生器驱动实现文件
 * @Author: MALossov
 * @Date: 2022-07-03 15:41:32
 * @LastEditTime: 2025-07-04 11:00:00
 * @LastEditors: ablewangee
 * @Reference: 康威电子
 */
/**********************************************************
 功能：75MHz时钟， AD9834正弦波点频输出，范围0-30M(可编辑0-100M)，
            方波：50k-5M，三角波：0-10M，扫频默认正弦波 。
 接口：控制接口请参照AD9834.h
 时间：2022/10/08
 版本：0.9
 作者：康威电子（原作者），MALossov（修改）
 **********************************************************/
#include "AD9834.h" 

/****************************************************************
 函数名称: AD9834_Write_16Bits
 功    能: 向AD9834写入16位数据（内部函数）
 参    数: data --  要写入的16位数据
 返回值  : 无
 *****************************************************************/
static void AD9834_Write_16Bits(unsigned int data)
{
    unsigned char i = 0;

    AD9834_SCK(1);
    AD9834_FSYNC(0);

    for (i = 0; i < 16; i++)
    {
        if (data & 0x8000)
            AD9834_SDA(1);
        else
            AD9834_SDA(0);

        AD9834_SCK(0);
        data <<= 1;
        AD9834_SCK(1);
    }
    AD9834_SDA(1);
    AD9834_FSYNC(1);
}

/***********************************************************************************
函数名称：AD9834_Select_Wave
功    能：写控制位控制，选择输出波形类型
参    数：initdata -- 要输入的命令，可使用以下宏定义

// 寄存器1波形宏定义：
#define Square_Wave_Reg1   0x2828  // 方波（寄存器1）
#define Sine_Wave_Reg1     0x2808  // 正弦波（寄存器1）
#define Triangle_Wave_Reg1 0x2802  // 三角波（寄存器1）

    使用示例：
    AD9834_Select_Wave(Square_Wave);   // 输出方波
    AD9834_Select_Wave(Sine_Wave);     // 输出正弦波
    AD9834_Select_Wave(Triangle_Wave); // 输出三角波
    也可以使用其他的控制命令 ;

返回值  ：无
注    意： 大部分情况下，只需要写入一次，除非要改变波形类型。
          同样可以通过改变控制位来改变波形类型，但是这样会导致频率和相位的改变。
************************************************************************************/
void AD9834_Select_Wave(unsigned int initdata)
{
    AD9834_FSYNC(1);
    AD9834_SCK(1);
    AD9834_RST(1);
    AD9834_RST(1);
    AD9834_RST(0);

    AD9834_Write_16Bits(initdata);
}

/****************************************************************
函数名称: AD9834_Init
功    能: 初始化后将FS和PS引脚设置为低。
参    数: 无
返回值  : 无
*****************************************************************/
void AD9834_Init(void)
{
    AD9834_FS(0);  // 将FS引脚设置为低
    AD9834_PS(0);  // 将PS引脚设置为低
}

/****************************************************************
函数名称: AD9834_Set_Freq
功    能: 设置频率值
参    数: freq_number -- 要写入的频率寄存器(FREQ_0或FREQ_1)
          freq -- 频率值，直接填写赫兹，换算公式为： (Freq_value(value)=Freq_data(data)*FCLK/2^28)
                                                //此时FCLK=75MHz，所以Freq_value=Freq_data*75/2^28
返回值  : 无
*****************************************************************/
void AD9834_Set_Freq(unsigned char freq_number, unsigned long freq)
{
    unsigned long FREQREG = (unsigned long)(268435456.0 / AD9834_SYSTEM_COLCK * freq);

    unsigned int FREQREG_LSB_14BIT = (unsigned int)FREQREG;
    unsigned int FREQREG_MSB_14BIT = (unsigned int)(FREQREG >> 14);

    //设置要配置哪个频率寄存器
    if (freq_number == FREQ_0)
    {
        FREQREG_LSB_14BIT &= ~(1U << 15);
        FREQREG_LSB_14BIT |= 1 << 14;
        FREQREG_MSB_14BIT &= ~(1U << 15);
        FREQREG_MSB_14BIT |= 1 << 14;
    }
    else
    {
        FREQREG_LSB_14BIT &= ~(1 << 14);
        FREQREG_LSB_14BIT |= 1U << 15;
        FREQREG_MSB_14BIT &= ~(1 << 14);
        FREQREG_MSB_14BIT |= 1U << 15;
    }
    //配置频率寄存器
    AD9834_Write_16Bits(FREQREG_LSB_14BIT);
    AD9834_Write_16Bits(FREQREG_MSB_14BIT);
}