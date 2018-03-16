#include "sys.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "wdg.h"
#include "timer.h"
#include "tpad.h"
#include "lcd.h"
#include "qr_encode.h"

#define QRCODE_Y 	80		//TFT��ά����ʾ����y

void DISPLAY_RENCODE_TO_TFT(u8 *qrcode_data)
{
    u8 i, j;
    u16 x, y, p;
    u8 qrencode_buff[12];			//���LCD ID�ַ���
    EncodeData((char *)qrcode_data);
    //LCD_Fill(10,110,15,115,BLACK);
    LCD_Fill(0, 40, 240, 320, WHITE);
    sprintf((char*)qrencode_buff, "size:%d", m_nSymbleSize); //��LCD ID��ӡ��lcd_id���顣
    LCD_ShowString(10, 40, 200, 16, 16, qrencode_buff);		//��ʾLCD ID
    if(m_nSymbleSize * 2 > 240)
    {
        LCD_ShowString(10, 60, 200, 16, 16, (u8 *)"The QR Code is too large!"); //̫����ʾ����
        return;
    }
    for(i = 0; i < 10; i++)
    {
        if((m_nSymbleSize * i * 2) > 240)	break;
    }
    p = (i - 1) * 2; //���С
    x = (240 - m_nSymbleSize * p) / 2;
    y = QRCODE_Y;
    sprintf((char*)qrencode_buff, "piont:%d", p); //��LCD ID��ӡ��lcd_id���顣
    LCD_ShowString(10, 60, 200, 16, 16, qrencode_buff);
    for(i = 0; i < m_nSymbleSize; i++)
    {
        for(j = 0; j < m_nSymbleSize; j++)
        {
            //USART1_SendData(m_byModuleData[j][i]);
            if(m_byModuleData[i][j] == 1)
                LCD_Fill(x + p * i, y + p * j, x + p * (i + 1) - 1, y + p * (j + 1) - 1, BLACK);

        }

    }
}
//0 0 5 5
//6 6 11 11
const u8 codetest[] = //΢����Ƭ������ȥ�ٶ���������ά����Ƭ �鿴
{
    "BEGIN:VCARD\r\n"
    "VERSION:3.0\r\n"
    "N:������\r\n"//����
    "TEL:13888888888\r\n"//�ֻ���
    "TEL;TYPE=WORK,VOICE:0571-88888888\r\n"//��������
    "TEL;TYPE=HOME,VOICE:0571-66666666\r\n"//��ͥ����
    "NOTE:QQ:12345\r\n"//QQ����
    "EMAIL:12345@qq.com\r\n"//�����ַ
    "ORG:��˾����\r\n"//��˾
    "TITLE:ְλ����\r\n"
    "URL:http://������ҳ\r\n"
    "ADR;TYPE=WORK:������ַ\r\n"//��ַ
    "ADR;TYPE=HOME:��ͥסַ\r\n"
    "END:VCARD"
};

int main(void)
{
    u16 i, len;
    Stm32_Clock_Init(9);	//ϵͳʱ������
    uart_init(72, 256000);	//���ڳ�ʼ��Ϊ9600
    delay_init(72);	   	 	//��ʱ��ʼ��
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    BEEP_Init();			//��������ʼ��
    KEY_Init();				//������ʼ��
    LCD_Init();				//TFT Һ����ʼ��
    LCD_Clear(WHITE);
    POINT_COLOR = RED;
    LCD_ShowString(10, 0, 200, 16, 16, (u8 *)"WarShip STM32 ^_^");
    LCD_ShowString(10, 20, 200, 16, 16, (u8 *)"QR_ENCODE TEST");
    DISPLAY_RENCODE_TO_TFT((u8 *)codetest);
    memset(USART_RX_BUF, '\0', USART_REC_LEN);

    while(1)
    {
        if(USART_RX_STA & 0x8000)
        {
            len = USART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
            USART_RX_STA = 0;
            printf("���յ������ݳ���Ϊ:%d\r\n", len);
            printf("���յ�������Ϊ:%d\r\n", len);
            USART1_SendPtr(USART_RX_BUF, len);
            printf("\r\n");
            DISPLAY_RENCODE_TO_TFT(USART_RX_BUF);
            memset(USART_RX_BUF, '\0', USART_REC_LEN);

        }
        else
        {
            if(++i >= 100)
            {
                i = 0;
                LED0 = !LED0;
            }
            delay_ms(10);
        }

    }

}









