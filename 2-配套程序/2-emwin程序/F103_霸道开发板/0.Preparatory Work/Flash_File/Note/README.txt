一、FSMC_SRAM(外部SRAM)使用情况说明：
		文件名		  变量名		   功能				地址起始位置		长度	   
1.bsp_fsmc_sram.c    TxBuffer	    SRAM测试函数        0x68008000    	 	0x400
2.bsp_bmp.c			 bmpinfo	    大图片缓存		 	0x680c0000			0x38402
3.mp3.c				 buffer			文件缓冲区		  	0x68000000			0x1000
4.mp3.c				 outBuf			PCM流缓冲			0x68002000			0x1388



二、外部Flash使用情况
	文件名							功能			地址起始位置        长度
1.fatfs_flash_spi.c(655行)		  Flash测试函数		0			        512BYTE	
2.bsp_touch.c					  触摸校准			1024				10BYTE	
3.fatfs_flash_spi.c(800行)		  字库				4096				53*4096(212KB)	
4.fatfs_flash_spi.c(107行)		  FATFS文件系统		512*4096			1536*4096(6MB)	
