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


/************************end of file******************/
