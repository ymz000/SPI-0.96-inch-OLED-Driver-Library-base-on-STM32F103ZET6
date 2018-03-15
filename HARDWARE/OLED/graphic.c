#include "graphic.h"
#include "oled.h"

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
u8 OLED_GRAM[128][8];

//��ͼ����,���½�Ϊԭ������,����x,����y

void OLED_RefreshGRAM(void)		//ˢ��������Ļ12864,�ɸ�����Ҫ����
{
    u8 i, n;
    for(i = 0; i < 8; i++)		//ҳ 0-7
    {
        OLED_WriteCmd(0xb0 + i); //����ҳ��ַ��0~7��
        OLED_WriteCmd(0x00);     //������ʾλ�á��е͵�ַ
        OLED_WriteCmd(0x10);     //������ʾλ�á��иߵ�ַ
        for(n = 0; n < 128; n++)
            OLED_WriteData(OLED_GRAM[n][i]);
    }
}
//����
//x:0~127
//y:0~63
void OLED_DrawPoint(u8 x, u8 y, u8 dot)
{
    u8 pos, bx, temp = 0;
    if(x > 127 || y > 63)return; //������Χ��.
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    if(dot)
        OLED_GRAM[x][pos] |= temp;
    else
        OLED_GRAM[x][pos] &= ~temp;
}

//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2,x1<=127 ,0<=y1<=63
//dot:0,���;1,���
void OLED_FillArea(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot)
{
    u8 x, y;
    for(x = x1; x <= x2; x++)
    {
        for(y = y1; y <= y2; y++)
            OLED_DrawPoint(x, y, dot);
    }
}
//��ֱ��
//�������,�յ�����,ע�����½�Ϊԭ��
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //������������
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if(delta_x > 0)
        incx = 1; 			//���õ�������
    else if(delta_x == 0)
        incx = 0; //��ֱ��
    else			//С��0
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if(delta_y > 0)
        incy = 1;
    else if(delta_y == 0)
        incy = 0; //ˮƽ��
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if( delta_x > delta_y)
        distance = delta_x; //ѡȡ��������������
    else
        distance = delta_y;

    for(t = 0; t <= distance + 1; t++ ) //�������
    {
        OLED_DrawPoint(uRow, uCol, dot); //����
        xerr += delta_x ;
        yerr += delta_y ;
        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

//������
//(x1,y1),(x2,y2):���εĶԽ�����
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u8 dot)
{
    //������ֱ��
    OLED_DrawLine(x1, y1, x2, y1, dot);
    OLED_DrawLine(x1, y1, x1, y2, dot);
    OLED_DrawLine(x1, y2, x2, y2, dot);
    OLED_DrawLine(x2, y1, x2, y2, dot);
}

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
//ʹ��Bresenham�㷨��Բ
void OLED_DrawCircle(u16 x0, u16 y0, u8 r, u8 dot)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);         //�ж��¸���λ�õı�־
    while(a <= b)
    {
        OLED_DrawPoint(x0 + a, y0 - b, dot);       //5
        OLED_DrawPoint(x0 + b, y0 - a, dot);       //0
        OLED_DrawPoint(x0 + b, y0 + a, dot);       //4
        OLED_DrawPoint(x0 + a, y0 + b, dot);       //6
        OLED_DrawPoint(x0 - a, y0 + b, dot);
        OLED_DrawPoint(x0 - b, y0 + a, dot);
        OLED_DrawPoint(x0 - a, y0 - b, dot);
        OLED_DrawPoint(x0 - b, y0 - a, dot);
        a++;
        //ʹ��Bresenham�㷨��Բ
        if(di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}



