/*
 * @Description: AD9834波形发生器驱动文件
 * @Author: MALossov
 * @Date: 2022-06-28 16:37:55
 * @LastEditTime: 2022-10-06 02:56:10
 * @LastEditors: MALossov
 * @Reference:
 */
#ifndef __AD9834_H
#define __AD9834_H
#include "main.h"

//------------------AD9834系统参数定义------------------//
/**
 * @brief 系统参数区，定义AD9834的工作参数及硬件连接信息
 * @notice 使用AD9834前需确认硬件连接与本定义一致
 */

 /* AD9834系统时钟 */
#define AD9834_SYSTEM_COLCK     75000000UL  // 系统时钟75MHz

/* AD9834 硬件连接定义 - 直接使用main.h中的宏定义 */

// 封装AD9834专用控制引脚操作 - 直接使用main.h中的宏定义
#define AD9834_FSYNC(x)   HAL_GPIO_WritePin(AD9834_FSYNC_GPIO_Port, AD9834_FSYNC_Pin, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define AD9834_SCK(x)     HAL_GPIO_WritePin(AD9834_SCK_GPIO_Port, AD9834_SCK_Pin, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define AD9834_SDA(x)     HAL_GPIO_WritePin(AD9834_SDA_GPIO_Port, AD9834_SDA_Pin, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define AD9834_RST(x)     HAL_GPIO_WritePin(AD9834_RST_GPIO_Port, AD9834_RST_Pin, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define AD9834_FS(x)      HAL_GPIO_WritePin(AD9834_FS_GPIO_Port, AD9834_FS_Pin, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define AD9834_PS(x)      HAL_GPIO_WritePin(AD9834_PS_GPIO_Port, AD9834_PS_Pin, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)

 // 频率/相位寄存器选择位
#define FREQ_0      0  // 选择频率寄存器0
#define FREQ_1      1  // 选择频率寄存器1
#define PHASE_0     0  // 选择相位寄存器0
#define PHASE_1     1  // 选择相位寄存器1

 /*--------------------- AD9834函数声明 ---------------------*/
extern void AD9834_Init(void);   // 初始化AD9834
extern void AD9834_Set_Freq(unsigned char freq_number, unsigned long freq); // 设置指定寄存器的频率值
extern void AD9834_Select_Wave(unsigned int initdata); // 选择输出波形类型

/*--------------------- AD9834波形选择函数及相关宏定义 ---------------------*/
/**
 * @brief AD9834_Select_Wave函数相关的波形选择宏定义
 * @usage 在调用AD9834_Select_Wave()函数时使用以下宏定义作为参数
 * @example AD9834_Select_Wave(Square_Wave);  // 输出方波
 *          AD9834_Select_Wave(Sine_Wave);    // 输出正弦波
 *          AD9834_Select_Wave(Triangle_Wave); // 输出三角波
 */

// 寄存器0的波形定义（默认使用频率寄存器0）
#define Triangle_Wave    0x2002  // 三角波
#define Sine_Wave       0x2008  // 正弦波
#define Square_Wave     0x2028  // 方波

// 寄存器1的波形定义
#define Sine_Wave_Reg1   0x2808  // 正弦波（寄存器1）
#define Square_Wave_Reg1 0x2828  // 方波（寄存器1）
#define Triangle_Wave_Reg1 0x2802  // 三角波（寄存器1）

// 其他功能控制定义
#define SelectByPin 0x2208  // 通过PIN_SW位选择寄存器（该位决定使用哪个频率/相位寄存器）
#define RESET_AD9834 0x2100  // 复位AD9834
#define STOP_DAC 0x2040      // 停止DAC输出


/*--------------------- AD9834寄存器控制位定义（高级用法）---------------------*/
/**
 * @brief 寄存器控制位定义，用于高级功能配置
 * @notice 一般用户直接使用上面的波形宏即可，以下定义供高级用户使用
 */

// 工作模式控制位
#define AD9834_B28    0x2000  // 配置为28位频率模式
#define AD9834_HLB    0x0100  // 配置为保持模式（需配合其他位使用）
#define AD9834_RESET  0x0100  // 复位AD9834
#define AD9834_SLEEP1 0x0080  // 进入睡眠模式1
#define AD9834_SLEEP12 0x0040 // 进入睡眠模式2
#define AD9834_OPBITEN 0x0020 // 使能输出缓冲
#define AD9834_DIV2   0x0010  // 配置为二分频模式
#define AD9834_MODE   0x0002  // 配置为斜坡模式
#define AD9834_SIGN_PIBIT 0x0001  // 配置为符号/π模式
#define AD9834_PHASE 0xC000      // 相位寄存器0（高2位选择相位寄存器）
#define AD9834_PHASE1 0xE000     // 相位寄存器1


/*保留部分
// 控制寄存器位定义（默认值为0）
#define DB15        0
#define DB14        0
#define DB13        B28  // 28位频率模式选择位
#define DB12        HLB  // 保持模式选择位
#define DB11        FSEL  // 频率寄存器选择位
#define DB10        PSEL  // 相位寄存器选择位
#define DB9         PIN_SW  // 引脚选择模式位
#define DB8         RESET  // 复位位
#define DB7         SLEEP1  // 睡眠模式1位
#define DB6         SLEEP12  // 睡眠模式2位
#define DB5         OPBITEN  // 输出使能位
#define DB4         SIGN_PIB  // 符号/π模式位
#define DB3         DIV2  // 二分频模式位
#define DB2         0
#define DB1         MODE  // 工作模式位
#define DB0         0
#define CONTROL_REGISTER    (DB15<<15)|(DB14<<14)|(DB13<<13)|(DB12<<12)|(DB11<<11)|(DB10<<10)|(DB9<<9)|(DB8<<8)|(DB7<<7)|(DB6<<6)|(DB5<<5)|(DB4<<4)|(DB3<<3)|(DB2<<2)|(DB1<<1)|(DB0<<0)
// 组合控制寄存器值（按位拼接各控制位）
*/
#endif /* AD9834_H */