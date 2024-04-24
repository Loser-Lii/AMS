#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "menu.h"
int main() {
    int nSelection = -1;      //输入菜单项编号

    printf("***欢迎进入计费管理系统***\n");      //输出系统名称

    do {
        outputMenu();     //调用outputMenu函数,输出系统菜单
        fflush(stdin);    //清除输入流缓存
        scanf("%d", &nSelection);
        switch (nSelection) {
        case 1:    //添加卡
        {
            add();
            break;
        }
        case 2:    //查询卡
        {
            query();
            break;
        }
        case 3:    
        {
            logon();   //上机
            break;
        }
        case 4:
        {
            settle();       //下机
            break;
        }
        case 5:
        {
            addMoney();     //充值
            break;
        }
        case 6:
        {
            refundMoney();  //退费
            break;
        }
        case 7:
        {
            statistics();   //查询统计
            break;
        }
        case 8:
        {
            annul();         //注销卡
            break;
        }
        case 9:
        {
            renew();         //激活卡
            break;
        }
        case 10:
        {
            changepwd();     //修改密码
            break;
        }
        case 11:
        {
            vip();           //注册VIP
            break;
        }
        case 0:
        {
            exitApp();       //退出程序
            break;
        }
        default:
        {
            printf("输入的菜单项序号错误！\n");
            break;
        }

        }
    } while (nSelection != 0);
    return 0;
}
