#include "menu/menu_builder.h"
#include "oled/oled_api.h"

// 菜单显示 - 基础样式
// menuOpt - 菜单操作结构体
// oledDev - Oled设备
void MenuBuilder_Display(sMenuOpt* menuOpt, eOledDevice oledDev)
{
    uint8_t disNum; // 屏幕中显示第几个子元素
    uint8_t sonNum; // 第几个子元素
    uint8_t sonLoc; // 子元素位置

    // oledDev显示菜单界面
    o_dev(oledDev);

    o_clr();
    // 标题
    if(menuOpt->menu[menuOpt->curId].nameLang == M_ASCII){
        o_edgestr(16,0,menuOpt->menu[menuOpt->curId].name,ASCII_H16W8);
    }else if(menuOpt->menu[menuOpt->curId].nameLang == M_ZH){
        o_edgecn(16,0,menuOpt->menu[menuOpt->curId].name,ZH_H16W16);
    }
    // 子元素
    if(menuOpt->menu[menuOpt->curId].type == M_FOLDER){
        disNum = 1; sonNum = 1; sonLoc = menuOpt->curId+1;
        if(menuOpt->pointId>3){     // 计算显示偏移
            o_str(0,16*(3-1)+16,">",ASCII_H16W8);  // 指针
            while(sonNum<=menuOpt->pointId-3){
                while(sonLoc<menuOpt->endid && menuOpt->menu[sonLoc].level>menuOpt->menu[menuOpt->curId].level+1){
                    sonLoc++;
                }
                sonNum++; sonLoc++;
            }
        }else{
            o_str(0,16*(menuOpt->pointId-1)+16,">",ASCII_H16W8);  // 指针
        }
        // 绘制菜单
        while(sonNum<=menuOpt->menu[menuOpt->curId].e.folder.son && disNum<=3){
            while(sonLoc<menuOpt->endid && menuOpt->menu[sonLoc].level>menuOpt->menu[menuOpt->curId].level+1){
                sonLoc++;
            }
            if(sonLoc<menuOpt->endid){
                if(menuOpt->menu[sonLoc].nameLang == M_ASCII){
                    o_str(10,16*(disNum-1)+16,menuOpt->menu[sonLoc].name,ASCII_H16W8);
                }else if(menuOpt->menu[sonLoc].nameLang == M_ZH){
                    o_chinese(10,16*(disNum-1)+16,menuOpt->menu[sonLoc].name,ZH_H16W16);
                }
            }else{
                break;
            }
            disNum++; sonNum++; sonLoc++;
        }
    }else if(menuOpt->menu[menuOpt->curId].type == M_FILE){
        menuOpt->menu[menuOpt->curId].e.file.func();
    }
}

// 菜单切换
// scode: 切换操作
void MenuBuilder_Switch(sMenuOpt* menuOpt, eOledDevice oledDev, uint8_t scode)
{
    uint8_t fatherLoc; // 父元素位置
    uint8_t nextLoc;   // 下个同级元素位置
    uint8_t cnt;

    // oledDev显示菜单界面
    o_dev(oledDev);

    switch(scode)
    {
        /* 上一个 */
        case M_PRE:
            if(menuOpt->menu[menuOpt->curId].type == M_FOLDER){
                // 指针
                if(menuOpt->pointId>1){
                    o_partclr(0,16*(menuOpt->pointId-1)+16,8,16*menuOpt->pointId+16);
                    menuOpt->pointId--;
                    if(menuOpt->pointId>=3){
                        MenuBuilder_Display(menuOpt,oledDev);
                        o_str(0,16*(3-1)+16,">",ASCII_H16W8);
                    }else{
                        o_str(0,16*(menuOpt->pointId-1)+16,">",ASCII_H16W8);
                    }
                }
            }else if(menuOpt->menu[menuOpt->curId].type == M_FILE){
                
            }
        break;
        /* 下一个 */
        case M_NEXT:
            if(menuOpt->menu[menuOpt->curId].type == M_FOLDER){
                if(menuOpt->menu[menuOpt->curId].e.folder.son==0){
                    return;
                }
                if(menuOpt->pointId<menuOpt->menu[menuOpt->curId].e.folder.son){
                    o_partclr(0,16*(menuOpt->pointId-1)+16,8,16*menuOpt->pointId+16);
                    menuOpt->pointId++;
                    if(menuOpt->pointId>3){
                        MenuBuilder_Display(menuOpt,oledDev);
                        o_str(0,16*(3-1)+16,">",ASCII_H16W8);
                    }else{
                        o_str(0,16*(menuOpt->pointId-1)+16,">",ASCII_H16W8);
                    }
                }
            }else if(menuOpt->menu[menuOpt->curId].type == M_FILE){
                
            }
        break;
        /* 确认 */
        case M_OK:
            if(menuOpt->menu[menuOpt->curId].type == M_FOLDER){
                if(menuOpt->menu[menuOpt->curId].e.folder.son==0){
                    return;
                }
                if(menuOpt->backMaintain==M_BACKNOMT){
                }else if(menuOpt->backMaintain==M_BACKMT){  // 记忆位置模式
                    menuOpt->menuLoc[menuOpt->menu[menuOpt->curId].level].pointIdBackup = menuOpt->pointId;
                }
                nextLoc = menuOpt->curId + 1;
                cnt = menuOpt->pointId;
                while(cnt>=1){   // 找pointId次
                    while(menuOpt->menu[nextLoc++].level>menuOpt->menu[menuOpt->curId].level+1){   // 扫描直到找到level为下一级的元素
                    }
                    cnt--;
                }
                menuOpt->curId = nextLoc - 1;
                menuOpt->pointId = 1;
                MenuBuilder_Display(menuOpt,oledDev);
            }else if(menuOpt->menu[menuOpt->curId].type == M_FILE){   // 进入文件后按OK，无反应
                
            }
        break;
        /* 上一级 */
        case M_BACK:
            if(menuOpt->curId>=1){
                fatherLoc = menuOpt->curId - 1;
                while(menuOpt->menu[fatherLoc].level>menuOpt->menu[menuOpt->curId].level-1){   // 扫描直到找到level为上一级的元素
                    fatherLoc--;
                }
                menuOpt->curId = fatherLoc;
                if(menuOpt->backMaintain==M_BACKNOMT){
                    menuOpt->pointId = 1;
                }else if(menuOpt->backMaintain==M_BACKMT){      // 记忆位置模式
                    menuOpt->pointId = menuOpt->menuLoc[menuOpt->menu[menuOpt->curId].level].pointIdBackup;
                }
                MenuBuilder_Display(menuOpt,oledDev);
            }
        break;
        /* 返回主菜单 */
        case M_BACKROOT:
            menuOpt->curId = 0; menuOpt->pointId = 1;
            MenuBuilder_Display(menuOpt,oledDev);
        break;
    }
}
