#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

//#include <inttypes.h>

#include "bsp_ra8875_lcd.h"

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GTP_I2C_PORT    	    GPIOE			/* GPIO�˿� */
#define GTP_I2C_GPIO_CLK 	    RCC_APB2Periph_GPIOE		/* GPIO�˿�ʱ�� */
#define GTP_I2C_SCL_PIN		    GPIO_Pin_4			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define GTP_I2C_SDA_PIN		    GPIO_Pin_2			/* ���ӵ�SDA�����ߵ�GPIO */

/*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
#define I2C_SCL_1()  GTP_I2C_PORT->BSRR = GTP_I2C_SCL_PIN				/* SCL = 1 */
#define I2C_SCL_0()  GTP_I2C_PORT->BRR = GTP_I2C_SCL_PIN				/* SCL = 0 */

#define I2C_SDA_1()  GTP_I2C_PORT->BSRR = GTP_I2C_SDA_PIN				/* SDA = 1 */
#define I2C_SDA_0()  GTP_I2C_PORT->BRR = GTP_I2C_SDA_PIN				/* SDA = 0 */

#define I2C_SDA_READ()  ((GTP_I2C_PORT->IDR & GTP_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */

#define GTP_ADDRESS            	0xBA

#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


#define GTP_RST_GPIO_PORT                GPIOE
#define GTP_RST_GPIO_CLK                 RCC_APB2Periph_GPIOE
#define GTP_RST_GPIO_PIN                 GPIO_Pin_0

#define GTP_INT_GPIO_PORT                GPIOE
#define GTP_INT_GPIO_CLK                 RCC_APB2Periph_GPIOE
#define GTP_INT_GPIO_PIN                 GPIO_Pin_3
#define GTP_INT_EXTI_PORTSOURCE          GPIO_PortSourceGPIOE
#define GTP_INT_EXTI_PINSOURCE           GPIO_PinSource3


uint8_t I2C_CheckDevice(uint8_t Address);

uint8_t I2C_ReadBytes( uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
uint8_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);

void I2C_ResetChip(void);
void I2C_Touch_Init(void);




#endif
