#include "bsp_bmp.h"
#include "ff.h"
#include "bsp_ra8875_lcd.h"
#include<stdio.h>

#define RGB24TORGB16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3)))


/*
* M_FileAlignAdj�꣬�����Ż��ļ�ϵͳ��ȡ����512�ֽ�����ʱ���ֵĵ�ַ��������
* �ο����ӣ�http://www.amobbs.com/thread-5488537-1-1.html
* ÿ�ζ��Ľ���ָ��ptr��m_FileIo_Buf[]��ǰ4Byte֮���ƶ�������f_read()֮��DMA����SD Sector����ʱһ����(u32 *)��ַ��
*/
#define M_FileAlignAdj(pFile)       (4 - (512 - ((uint32_t)((pFile)->fptr) & 0x01FF)) & 0x0003)  


#define BMP_BUFF_SIZE  1024*2*10      //ͼƬ���ݻ�������С  

__align(4) BYTE ColorData[BMP_BUFF_SIZE];      //���ջ�����
BYTE * pColorData = (BYTE *)ColorData;					/* ����ָ�򻺳�����ָ�룬���M_FileAlignAdj��ʹ�� */

FATFS bmpfs; 
FIL bmpfsrc; 
FRESULT bmpres;

/* �������Ҫ��ӡbmp��ص���ʾ��Ϣ,��printfע�͵�����
 * ��Ҫ��printf()���轫���������ļ���������
 */
#define BMP_DEBUG_PRINTF(FORMAT,...)  //printf(FORMAT,##__VA_ARGS__)	 

/* ��ӡBMP�ļ���ͷ��Ϣ�����ڵ��� */
static void showBmpHead(BITMAPFILEHEADER* pBmpHead)
{
    BMP_DEBUG_PRINTF("λͼ�ļ�ͷ:\r\n");
    BMP_DEBUG_PRINTF("�ļ���С:%d\r\n",(*pBmpHead).bfSize);
    BMP_DEBUG_PRINTF("������:%d\r\n",(*pBmpHead).bfReserved1);
    BMP_DEBUG_PRINTF("������:%d\r\n",(*pBmpHead).bfReserved2);
    BMP_DEBUG_PRINTF("ʵ��λͼ���ݵ�ƫ���ֽ���:%d\r\n",(*pBmpHead).bfOffBits);
		BMP_DEBUG_PRINTF("\r\n");	
}

/* ��ӡBMP�ļ���ͷ��Ϣ�����ڵ��� */
static void showBmpInforHead(tagBITMAPINFOHEADER* pBmpInforHead)
{
    BMP_DEBUG_PRINTF("λͼ��Ϣͷ:\r\n");
    BMP_DEBUG_PRINTF("�ṹ��ĳ���:%d\r\n",(*pBmpInforHead).biSize);
    BMP_DEBUG_PRINTF("λͼ��:%d\r\n",(*pBmpInforHead).biWidth);
    BMP_DEBUG_PRINTF("λͼ��:%d\r\n",(*pBmpInforHead).biHeight);
    BMP_DEBUG_PRINTF("biPlanesƽ����:%d\r\n",(*pBmpInforHead).biPlanes);
    BMP_DEBUG_PRINTF("biBitCount������ɫλ��:%d\r\n",(*pBmpInforHead).biBitCount);
    BMP_DEBUG_PRINTF("ѹ����ʽ:%d\r\n",(*pBmpInforHead).biCompression);
    BMP_DEBUG_PRINTF("biSizeImageʵ��λͼ����ռ�õ��ֽ���:%d\r\n",(*pBmpInforHead).biSizeImage);
    BMP_DEBUG_PRINTF("X����ֱ���:%d\r\n",(*pBmpInforHead).biXPelsPerMeter);
    BMP_DEBUG_PRINTF("Y����ֱ���:%d\r\n",(*pBmpInforHead).biYPelsPerMeter);
    BMP_DEBUG_PRINTF("ʹ�õ���ɫ��:%d\r\n",(*pBmpInforHead).biClrUsed);
    BMP_DEBUG_PRINTF("��Ҫ��ɫ��:%d\r\n",(*pBmpInforHead).biClrImportant);
		BMP_DEBUG_PRINTF("\r\n");
}

/*
 * ��ʾbmpͼƬ, 24λ���ɫ
 * ͼƬ��Ⱥ͸߶ȸ���ͼƬ��С����
 */
void Lcd_show_bmp(unsigned short int x, unsigned short int y, char *pic_name)
{
  
	int i, j ;
	int width, height, l_width;
  
	BYTE red,green,blue;
	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;
	WORD fileType;

	unsigned int read_num;
	unsigned char tmp_name[20];
	sprintf((char*)tmp_name,"%s",pic_name);

	bmpres = f_open(&bmpfsrc,(char *)tmp_name, FA_OPEN_EXISTING|FA_READ);	
/*-------------------------------------------------------------------------------------------------------*/
	if(bmpres == FR_OK)
	{
		BMP_DEBUG_PRINTF("Open file success\r\n");

		/* ��ȡ�ļ�ͷ��Ϣ  �����ֽ�*/         
		f_read(&bmpfsrc,&fileType,sizeof(WORD),&read_num);     

		/* �ж��ǲ���bmp�ļ� "BM"*/
		if(fileType != 0x4d42)
		{
			BMP_DEBUG_PRINTF("file is not .bmp file!\r\n");
			return;
		}
		else
		{
			BMP_DEBUG_PRINTF("Ok this is .bmp file\r\n");	
		}        

		/* ��ȡBMP�ļ�ͷ��Ϣ*/
		f_read(&bmpfsrc,&bitHead,sizeof(tagBITMAPFILEHEADER),&read_num);        
		showBmpHead(&bitHead);

		/* ��ȡλͼ��Ϣͷ��Ϣ */
		f_read(&bmpfsrc,&bitInfoHead,sizeof(BITMAPINFOHEADER),&read_num);        
		showBmpInforHead(&bitInfoHead);
	}    
	else
	{
		BMP_DEBUG_PRINTF("file open fail!\r\n");
		return;
	}    
/*-------------------------------------------------------------------------------------------------------*/
	width = bitInfoHead.biWidth;  
 
  /* bmpͼ��ĸ�Ϊ������ʾͼ��洢�����Ǵ��ϵ��£�Ϊ������ʾ���µ��� */
  height = (bitInfoHead.biHeight>0 ? bitInfoHead.biHeight:-bitInfoHead.biHeight);
  
 
  l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);	


	if(l_width > 800*480*3)
	{
		BMP_DEBUG_PRINTF("\n SORRY, PIC IS TOO BIG (<=800)\n");
		return;
	}
	
  
  /* ����ͼƬ��λ�úʹ�С�� ��������ʾ���� ��Ϊy����ת�ˣ���Ҫ���¼���y��������*/
	RA8875_SetDispWin(x, y, height, width);
  
   /* BMPͼƬ����д�뷽�� */
  RA8875_SetWriteDirection(0,0);

  LCD_RA8875_REG(0x02); 		/* ׼����д�Դ� */
	
	/* �ж��Ƿ���24bit ��16bits ͼƬ */
	if(bitInfoHead.biBitCount == 24 || bitInfoHead.biBitCount == 16)
	{
    uint8_t rowNum = BMP_BUFF_SIZE/l_width  ;     //ÿ�ζ�ȡ��������һ�ζ����пɼӿ�ͼƬ��ʾ�ٶȣ�
    uint8_t readCount = height/rowNum;             //��rowNum��ȡ�Ĵ���  
    uint8_t restRow = height%rowNum;                //����rowNum�У�ʣ������� 
    
    uint8_t *ptr;                                     //��ʾʱ���ڵ���buff˳���ָ��
    uint8_t row;                                      //��ʾʱ���м���
    
  
    for(i=0;i< readCount ||(restRow && i == readCount); i++)  //��i=readcound ��restRow����ʱ����ȡʣ�಻��rownum�е�����
		{	 
      
      if(bitInfoHead.biHeight>0)                    //���������ݷ���Ϊ����
      {
        if(i != readCount)                             
        {
          /* bmp�ļ����������ǵ���ģ����ﶨλbmp�ļ�λ�ã�ʹ������������˳���*/
          f_lseek(&bmpfsrc,bitHead.bfOffBits+l_width*(height-(i+1)*rowNum));        //һ�ζ�ȡrowNum��
        }
        else        
        {        
          //���һ�Σ�һ�ζ�ȡrestRow��
          f_lseek(&bmpfsrc,bitHead.bfOffBits+l_width*(height-readCount*rowNum-restRow));  
          
          /*��rowNumֵ�޸ĳ�restRow���������һ�ζ�����*/
          rowNum = restRow;           
        }
      }
      
      pColorData = (BYTE *)ColorData;  
      pColorData += M_FileAlignAdj(&bmpfsrc);           //4�ֽڵ�ַ�����Ż�����������ݶ�ȡ�ٶȣ��������ײ��memcpy����
      
      /* һ�ζ�ȡrowNum�л�restRow������(���һ��ʱ��ȡrestRow) */
      f_read(&bmpfsrc,pColorData,l_width*rowNum,&read_num); 

      /* 24λͼ����� */
      if(bitInfoHead.biBitCount == 24)
      {
        for(row=0;row<rowNum;row++)
        {
          if(bitInfoHead.biHeight>0)
            ptr=pColorData+l_width*(rowNum-(row+1));       //���¶���ʹ��ʾ���ݰ�˳������
          else
            ptr=pColorData+l_width*row;
            
          for(j=0; j<l_width;j+=3) 											   //һ����Ч��Ϣ
          {           
            red = ptr[j+2];
            green = ptr[j+1];
            blue = 	ptr[j];
            LCD_RA8875_WRITE_DATA(RGB24TORGB16(red,green,blue)); //д��LCD-GRAM 
          } 
         }
      }
      else if(bitInfoHead.biBitCount == 16) /*16λͼ�����*/
      {           
        for(row=0;row<rowNum;row++)
        {
          if(bitInfoHead.biHeight>0)
            ptr=pColorData+l_width*(rowNum-(row+1));       //���¶���ʹ��ʾ���ݰ�˳������
          else
            ptr=pColorData+l_width*row;
          
        for(j=0; j<l_width; j++) 											   //һ����Ч��Ϣ
        {          
          LCD_RA8875_WRITE_DATA(ptr[j]|ptr[++j]<<8);   //д��LCD-GRAM      
        } 
        }
      }      
    }

       
	}    
	else 
	{        
		BMP_DEBUG_PRINTF("SORRY, THIS  IS NOT A 24 NOR A 16 BITS  PIC");
		return ;
	}
	f_close(&bmpfsrc);    
  
  	/* �˳����ڻ�ͼģʽ */
	RA8875_QuitWinMode();
  


}



/**********************************************************
 * ��������Screen_shot
 * ����  ����ȡLCDָ��λ��  ָ����ߵ����� ����Ϊ24λ���ɫbmp��ʽͼƬ
 * ����  : 	x								---ˮƽλ�� 
 *					y								---��ֱλ��  
 *					Width						---ˮƽ���   
 *					Height					---��ֱ�߶�  	
 *					filename				---�ļ���
 * ���  ��	0 		---�ɹ�
 *  				-1 		---ʧ��
 *	    		8			---�ļ��Ѵ���
 * ����  ��Screen_shot(0, 0, 320, 240, "/myScreen");-----ȫ����ͼ
 * ע��  ��x��Χ[0,319]  y��Χ[0,239]  Width��Χ[0,320-x]  Height��Χ[0,240-y]
 *					����ļ��Ѵ���,��ֱ�ӷ���	
 **************************************************************/ 
/*
 * bmp�ļ�ͷ��54���ֽڣ�����ǰ14���ֽ����ļ�ͷ��Ϣ����40���ֽ���λͼ��Ϣͷ��Ϣ
 * bmp�ļ�ͷ֮����Ǿ����������Ϣ
 * 0x42 0x4d :bm
 * 54        :ʵ��λͼ���ݵ�ƫ���ֽ���
 * 40        :λͼ��Ϣͷ�ṹ��ĳ���
 * 1         :ƽ����Ϊ1
 * 24        :24bit���ɫ
 */
int Screen_Shot(unsigned short int x,
                  unsigned short int y,
                  unsigned short int Width,
                  unsigned short int Height,
                  unsigned char *filename)
{
	/* bmp  �ļ�ͷ 54���ֽ� */
	unsigned char header[54] =
	{
		0x42, 0x4d, 0, 0, 0, 0, 
		0, 0, 0, 0, 54, 0, 
		0, 0, 40,0, 0, 0, 
		0, 0, 0, 0, 0, 0, 
		0, 0, 1, 0, 24, 0, 
		0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 
		0, 0, 0
	};
	
	int i;
	int j;
	long file_size;     
	long width;
	long height;
	unsigned char r,g,b;	
	unsigned char tmp_name[30];
	unsigned int mybw;
	unsigned int read_data;
	char kk[4]={0,0,0,0};
	
	/* ��*�� +������ֽ� + ͷ����Ϣ */
	file_size = (long)Width * (long)Height * 3 + Height*(Width%4) + 54;		

	/* �ļ���С 4���ֽ� */
	header[2] = (unsigned char)(file_size &0x000000ff);
	header[3] = (file_size >> 8) & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;
	
	/* λͼ�� 4���ֽ� */
	width=Width;	
	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) &0x000000ff;
	header[20] = (width >> 16) &0x000000ff;
	header[21] = (width >> 24) &0x000000ff;
	
	/* λͼ�� 4���ֽ� */
	height = Height;
	header[22] = height &0x000000ff;
	header[23] = (height >> 8) &0x000000ff;
	header[24] = (height >> 16) &0x000000ff;
	header[25] = (height >> 24) &0x000000ff;
	
	/* ��filename ����һ���ĸ�ʽ������ tmp_name */
	sprintf((char*)tmp_name,"1:%s.bmp",filename);
	
//	/* ע��һ�����������߼���Ϊ0 */
//	f_mount(0, &bmpfs[0]);

   /* BMPͼƬ����д�뷽�� */
  RA8875_SetReadDirection(0,0);
	
	/* �½�һ���ļ� */
	bmpres = f_open( &bmpfsrc , (char*)tmp_name, FA_CREATE_ALWAYS | FA_WRITE );
	
	/* �½��ļ�֮��Ҫ�ȹر��ٴ򿪲���д�� */
	f_close(&bmpfsrc);
		
	bmpres = f_open( &bmpfsrc , (char*)tmp_name,  FA_OPEN_EXISTING | FA_WRITE);

	if ( bmpres == FR_OK || bmpres ==FR_EXIST )
	{    
		/* ��Ԥ�ȶ���õ�bmpͷ����Ϣд���ļ����� */
		bmpres = f_write(&bmpfsrc, header,sizeof(unsigned char)*54, &mybw);		
		
		/* �����ǽ�ָ�����ڵ����ݶ�����д���ļ�����ȥ */
		for(i=0; i<Height; i++)					
		{
     /* bmp�ļ����������ǵ���ģ����ﶨλbmp�ļ�λ�ã�ʹ������������˳���*/
//     f_lseek(&bmpfsrc,bitHead.bfOffBits+l_width*(height-(i+1)*rowNum));        //һ�ζ�ȡrowNum��
      #if 0
			if( !(Width%4) )				/* �պ���4�ֽڶ��� */
			{
       
        
				for(j=0; j<Width; j++)  
				{					
					read_data = RA8875_GetPixel(x+j, y+i);				
					
					r =  GETR_FROM_RGB16(read_data);
					g =  GETG_FROM_RGB16(read_data);
					b =  GETB_FROM_RGB16(read_data);

					bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
					bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
					bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
				}
			}
			else
			{
				for(j=0;j<Width;j++)
				{					
					read_data = RA8875_GetPixel(x+i,y+j );
					
					r =  GETR_FROM_RGB16(read_data);
					g =  GETG_FROM_RGB16(read_data);
					b =  GETB_FROM_RGB16(read_data);

					bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
					bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
					bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
				}
				/* ����4�ֽڶ�������Ҫ���� */	
				bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(Width%4), &mybw);
			}

      #else
        if( !(Width%4) )				/* �պ���4�ֽڶ��� */
			{
       
        
				for(j=0; j<Width; j++)  
				{					
					read_data = RA8875_GetPixel(x+j, y+i);				
					
					r =  GETR_FROM_RGB16(read_data);
					g =  GETG_FROM_RGB16(read_data);
					b =  GETB_FROM_RGB16(read_data);

					bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
					bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
					bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
				}
			}
			else
			{
				for(j=0;j<Width;j++)
				{					
					read_data = RA8875_GetPixel(x+i,y+j );
					
					r =  GETR_FROM_RGB16(read_data);
					g =  GETG_FROM_RGB16(read_data);
					b =  GETB_FROM_RGB16(read_data);

					bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
					bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
					bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
				}
				/* ����4�ֽڶ�������Ҫ���� */	
				bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(Width%4), &mybw);
			}

      #endif      
		}/* ������� */

		f_close(&bmpfsrc); 
    
      RA8875_SetReadDirection(0,0);
		return 0;
	}
	else if ( bmpres == FR_EXIST )  //����ļ��Ѿ�����
	{
    
    printf("exist");
    
      RA8875_SetReadDirection(0,0);
		return FR_EXIST;	 					//8
	}
	else/* ����ʧ�� */
	{
        printf("error");
  RA8875_SetReadDirection(0,0);
		return -1;
	}    
}


/************************end of file******************/
