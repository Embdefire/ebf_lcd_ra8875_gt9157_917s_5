#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

//#include <inttypes.h>

#include "bsp_ra8875_lcd.h"

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */



/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GTP_I2C_PORT    	    GPIOF			/* GPIO�˿� */
#define GTP_I2C_GPIO_CLK 	    RCC_APB2Periph_GPIOF		/* GPIO�˿�ʱ�� */
#define GTP_I2C_SCL_PIN		    GPIO_Pin_9			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define GTP_I2C_SDA_PIN		    GPIO_Pin_11			/* ���ӵ�SDA�����ߵ�GPIO */

/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define I2C_SCL_1()  GPIO_SetBits(GTP_I2C_PORT, GTP_I2C_SCL_PIN)		/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(GTP_I2C_PORT, GTP_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(GTP_I2C_PORT, GTP_I2C_SDA_PIN)		/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(GTP_I2C_PORT, GTP_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GTP_I2C_PORT, GTP_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define I2C_SCL_1()  GTP_I2C_PORT->BSRR = GTP_I2C_SCL_PIN				/* SCL = 1 */
	#define I2C_SCL_0()  GTP_I2C_PORT->BRR = GTP_I2C_SCL_PIN				/* SCL = 0 */
	
	#define I2C_SDA_1()  GTP_I2C_PORT->BSRR = GTP_I2C_SDA_PIN				/* SDA = 1 */
	#define I2C_SDA_0()  GTP_I2C_PORT->BRR = GTP_I2C_SDA_PIN				/* SDA = 0 */
	
	#define I2C_SDA_READ()  ((GTP_I2C_PORT->IDR & GTP_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif


#define GTP_ADDRESS            0xBA

#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


/* Ӳ��IIC*/
//#define GTP_I2C                          I2C1
//#define GTP_I2C_CLK                      RCC_APB1Periph_I2C1

//#define GTP_I2C_SCL_PIN                  GPIO_Pin_6                  
//#define GTP_I2C_SCL_GPIO_PORT            GPIOB                       
//#define GTP_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
//#define GTP_I2C_SCL_SOURCE               GPIO_PinSource6
//#define GTP_I2C_SCL_AF                   GPIO_AF_I2C1

//#define GTP_I2C_SDA_PIN                  GPIO_Pin_7                  
//#define GTP_I2C_SDA_GPIO_PORT            GPIOB                     
//#define GTP_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
//#define GTP_I2C_SDA_SOURCE               GPIO_PinSource7
//#define GTP_I2C_SDA_AF                   GPIO_AF_I2C1



#define GTP_RST_GPIO_PORT                GPIOG
#define GTP_RST_GPIO_CLK                 RCC_APB2Periph_GPIOG
#define GTP_RST_GPIO_PIN                 GPIO_Pin_7

#define GTP_INT_GPIO_PORT                GPIOF
#define GTP_INT_GPIO_CLK                 RCC_APB2Periph_GPIOF
#define GTP_INT_GPIO_PIN                 GPIO_Pin_6
#define GTP_INT_EXTI_PORTSOURCE          GPIO_PortSourceGPIOF
#define GTP_INT_EXTI_PINSOURCE           GPIO_PinSource6
#define GTP_INT_EXTI_LINE                EXTI_Line6
#define GTP_INT_EXTI_IRQ                 EXTI9_5_IRQn

#define GTP_IRQHandler                   EXTI9_5_IRQHandler








uint8_t I2C_CheckDevice(uint8_t Address);

uint8_t I2C_ReadBytes( uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
uint8_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);

void I2C_GTP_IRQEnable(void);
void I2C_GTP_IRQDisable(void);
void I2C_ResetChip(void);
void I2C_Touch_Init(void);




#endif
