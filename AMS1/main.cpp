#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "menu.h"
int main() {
    int nSelection = -1;      //����˵�����

    printf("***��ӭ����Ʒѹ���ϵͳ***\n");      //���ϵͳ����

    do {
        outputMenu();     //����outputMenu����,���ϵͳ�˵�
        fflush(stdin);    //�������������
        scanf("%d", &nSelection);
        switch (nSelection) {
        case 1:    //��ӿ�
        {
            add();
            break;
        }
        case 2:    //��ѯ��
        {
            query();
            break;
        }
        case 3:    
        {
            logon();   //�ϻ�
            break;
        }
        case 4:
        {
            settle();       //�»�
            break;
        }
        case 5:
        {
            addMoney();     //��ֵ
            break;
        }
        case 6:
        {
            refundMoney();  //�˷�
            break;
        }
        case 7:
        {
            statistics();   //��ѯͳ��
            break;
        }
        case 8:
        {
            annul();         //ע����
            break;
        }
        case 9:
        {
            renew();         //���
            break;
        }
        case 10:
        {
            changepwd();     //�޸�����
            break;
        }
        case 11:
        {
            vip();           //ע��VIP
            break;
        }
        case 0:
        {
            exitApp();       //�˳�����
            break;
        }
        default:
        {
            printf("����Ĳ˵�����Ŵ���\n");
            break;
        }

        }
    } while (nSelection != 0);
    return 0;
}
