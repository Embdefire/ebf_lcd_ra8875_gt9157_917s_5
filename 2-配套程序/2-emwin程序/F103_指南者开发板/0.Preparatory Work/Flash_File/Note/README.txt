һ��FSMC_SRAM(�ⲿSRAM)ʹ�����˵����
		�ļ���		  ������		   ����				��ַ��ʼλ��		����	   
1.bsp_fsmc_sram.c    TxBuffer	    SRAM���Ժ���        0x68008000    	 	0x400
2.bsp_bmp.c			 bmpinfo	    ��ͼƬ����		 	0x680c0000			0x38402
3.mp3.c				 buffer			�ļ�������		  	0x68000000			0x1000
4.mp3.c				 outBuf			PCM������			0x68002000			0x1388



�����ⲿFlashʹ�����
	�ļ���							����			��ַ��ʼλ��        ����
1.fatfs_flash_spi.c(655��)		  Flash���Ժ���		0			        512BYTE	
2.bsp_touch.c					  ����У׼			1024				10BYTE	
3.fatfs_flash_spi.c(800��)		  �ֿ�				4096				53*4096(212KB)	
4.fatfs_flash_spi.c(107��)		  FATFS�ļ�ϵͳ		512*4096			1536*4096(6MB)	
