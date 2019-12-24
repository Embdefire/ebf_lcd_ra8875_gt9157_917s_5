/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  fire 
  * @version v1.0
  * @date    2015-xx-xx
  * @brief   ���ļ�ר��������GT9xx.h������оƬ��ʹ��ģ��IIC
  *           ��gpioģ��I2C����, ������STM32ϵ��CPU��
  *          ��ģ�鲻����Ӧ�ò�����֡��������I2C���߻�������������
  *   
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "stm32f10x.h"
#include "bsp_i2c_gpio.h"
#include "gt9xx.h"

/***ģ��IICͨ�ú���****/


static void I2C_GPIO_Config(void);
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_SendByte(uint8_t ucByte);
static uint8_t I2C_ReadByte(void);
static uint8_t I2C_WaitAck(void);
static void I2C_Ack(void);
static void I2C_NAck(void);


/**
  * @brief   I2C_Delay I2C����λ�ӳ٣����400KHz
  * @param   ��
  * @retval  ��
  */
static void I2C_Delay(void)
{
	uint8_t i;

	/*��
    ��ʹ���߼����������۲�IICͨѶ��Ƶ��
		CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
        
	*/
	for (i = 0; i < 10; i++);
}


/**
  * @brief   Delay ��λ����оƬʹ�õ��ӳٺ���
  * @param   ��
  * @retval  ��
  */
static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


/**
  * @brief   I2C_Start CPU����I2C���������ź�
  * @param   ��
  * @retval  ��
  */
void I2C_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	I2C_SDA_1();
	I2C_SCL_1();
	I2C_Delay();
	I2C_SDA_0();
	I2C_Delay();
	I2C_SCL_0();
	I2C_Delay();
}


/**
  * @brief   I2C_Stop CPU����I2C����ֹͣ�ź�
  * @param   ��
  * @retval  ��
  */
void I2C_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	I2C_SDA_0();
	I2C_SCL_1();
	I2C_Delay();
	I2C_SDA_1();
}





/**
  * @brief   I2C_Ack ����һ��ACK�ź�
  * @param   ��
  * @retval  ��
  */
static uint8_t I2C_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	I2C_Delay();
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	I2C_Delay();
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	I2C_Delay();
	return re;
}


/**
  * @brief   I2C_Ack ����һ��ACK�ź�
  * @param   ��
  * @retval  ��
  */
static void I2C_Ack(void)
{
	I2C_SDA_0();	/* CPU����SDA = 0 */
	I2C_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	I2C_Delay();
	I2C_SCL_0();
	I2C_Delay();
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}



/**
  * @brief   I2C_NAck ����1��NACK�ź�
  * @param   ��
  * @retval  ��
  */
static void I2C_NAck(void)
{
	I2C_SDA_1();	/* CPU����SDA = 1 */
	I2C_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	I2C_Delay();
	I2C_SCL_0();
	I2C_Delay();	
}


/**
  * @brief   I2C_SendByte CPU�������豸����8bit����
  * @param   �ȴ����͵��ֽ�
  * @retval  ��
  */
static void I2C_SendByte(uint8_t ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		I2C_Delay();
		I2C_SCL_1();
		I2C_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // �ͷ�����
		}
		ucByte <<= 1;	/* ����һ��bit */
		I2C_Delay();
	}
}



/**
  * @brief   I2C_ReadByte CPU��I2C�����豸��ȡ8bit����
  * @param   ��
  * @retval  ����������
  */
static uint8_t I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		I2C_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		I2C_Delay();
	}
	return value;
}




/**
  * @brief  I2C_CheckDevice ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
  * @param  ���豸�ص�ַ
  * @retval ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
  */
uint8_t I2C_CheckDevice(uint8_t Address)
{
	uint8_t ucAck;

	I2C_GPIO_Config();		/* ����GPIO */
	
	I2C_Start();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	I2C_SendByte(Address | I2C_WR);
	ucAck = I2C_WaitAck();	/* ����豸��ACKӦ�� */

	I2C_Stop();			/* ����ֹͣ�ź� */

	return ucAck;
}

/*****GT9xx����оƬר�ú���*****/

/**
  * @brief   ʹ��IIC��ȡ����
  * @param   
  * 	@arg ClientAddr:���豸��ַ
  *		@arg pBuffer:����ɴӻ���ȡ�����ݵĻ�����ָ��
  *		@arg NumByteToRead:��ȡ�����ݳ���
  * @retval  ��
  */
uint8_t I2C_ReadBytes( uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead)
{
	uint16_t i;
	
	
	/* ��1��������I2C���������ź� */
	I2C_Start();

	/* ��2�������Ͷ��豸��ַ��������ȡ����*/
	I2C_SendByte(ClientAddr|I2C_RD);
	
	/* ��3�����ȴ�ACK */
	if (I2C_WaitAck() != 0)
	{
		goto cmd_fail;	/* ������Ӧ�� */
	}	
	
	/* ��4����ѭ����ȡ���� */
	for (i = 0; i < NumByteToRead; i++)
	{
		pBuffer[i] = I2C_ReadByte();	/* ��1���ֽ� */     
		
		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if (i != NumByteToRead - 1)
		{
			I2C_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			I2C_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}  
	}
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();
	return 0;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();

	return 0xff;
}

/**
  * @brief   ʹ��IICд������
  * @param   
  * 	@arg ClientAddr:���豸��ַ
  *		@arg pBuffer:������ָ��
  *   @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
uint8_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite)
{
	uint16_t i;
  
  /* ��1��������I2C���������ź� */
  I2C_Start();

  /* ��2��������д�豸��ַ������д������ */
  I2C_SendByte(ClientAddr);
  
  /* ��3�����ȴ�ACK */
  if (I2C_WaitAck() != 0)
  {
    goto cmd_fail;	/*������Ӧ�� */
  }
  
	for (i = 0; i < NumByteToWrite; i++)
	{
	
		/* ��4������ʼд������ */
		I2C_SendByte(pBuffer[i]);
	
		/* ��5�����ȴ�ACK */
		if (I2C_WaitAck() != 0)
		{
			goto cmd_fail;	/* ������Ӧ�� */
		}

	}
	
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	I2C_Stop();
	return 0;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();
  
	return 0xff;
}





/**
  * @brief   I2C_GPIO_Config ��ʼ�����ƴ���оƬ��GPIO IIC RST INT
  * @param   ��
  * @retval  ��
  */
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(GTP_I2C_GPIO_CLK|GTP_RST_GPIO_CLK|GTP_INT_GPIO_CLK|RCC_APB2Periph_AFIO, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin = GTP_I2C_SCL_PIN | GTP_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* ��©��� */
	GPIO_Init(GTP_I2C_PORT, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	I2C_Stop();  
 
  /*!< Configure RST */   
  GPIO_InitStructure.GPIO_Pin = GTP_RST_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GTP_RST_GPIO_PORT, &GPIO_InitStructure);
  
  /*!< Configure INT */   
  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);

}




/**
  * @brief  ʹ��GT91xx�ж�
  * @param  ��
  * @retval ��
  */
void I2C_GTP_IRQEnable(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;  
  /*!< Configure INT */   
  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);  
  
  /* ���� EXTI �ж�Դ ��INT ���� */
  GPIO_EXTILineConfig(GTP_INT_EXTI_PORTSOURCE, GTP_INT_EXTI_PINSOURCE);

  /* ѡ�� EXTI �ж�Դ */
  EXTI_InitStructure.EXTI_Line = GTP_INT_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);  
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = GTP_INT_EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


/**
  * @brief  ��ֹGT91xx�ж�
  * @param  ��
  * @retval ��
  */
void I2C_GTP_IRQDisable(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /*!< Configure INT */
  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);

  /* ���� EXTI �ж�Դ ��INT ���� */
  GPIO_EXTILineConfig(GTP_INT_EXTI_PORTSOURCE, GTP_INT_EXTI_PINSOURCE);

  /* ѡ�� EXTI �ж�Դ */
  EXTI_InitStructure.EXTI_Line = GTP_INT_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = GTP_INT_EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

}




/**
  * @brief  ��GT91xxоƬ���и�λ
  * @param  ��
  * @retval ��
  */
void I2C_ResetChip(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  /*!< Configure INT */
	  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);

	  /*��ʼ��GT9157,rstΪ�ߵ�ƽ��intΪ�͵�ƽ����gt9157���豸��ַ������Ϊ0xBA*/

	  /*��λΪ�͵�ƽ��Ϊ��ʼ����׼��*/
    GPIO_ResetBits (GTP_INT_GPIO_PORT,GTP_INT_GPIO_PIN);
	  GPIO_ResetBits (GTP_RST_GPIO_PORT,GTP_RST_GPIO_PIN);
	  Delay(0x0FFFFF);

	  /*����һ��ʱ�䣬���г�ʼ��*/
	  GPIO_SetBits (GTP_RST_GPIO_PORT,GTP_RST_GPIO_PIN);
	  Delay(0x0FFFFF);

	  /*��INT��������Ϊ��������ģʽ*/
	  /*!< Configure INT */
	  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);
}


/**
  * @brief  I2C ����(GT91xx)��ʼ��
  * @param  ��
  * @retval ��
  */
void I2C_Touch_Init(void)
{
  I2C_GPIO_Config(); 
 
  /*����Ҫ��λоƬ������IIC��ַ*/
  I2C_ResetChip();

  I2C_GTP_IRQEnable();
}



/* ------------------------------------------end of file---------------------------------------- */

