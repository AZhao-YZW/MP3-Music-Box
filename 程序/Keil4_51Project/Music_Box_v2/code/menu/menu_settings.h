#ifndef __MENU_SETTINGS_H__
#define __MENU_SETTINGS_H__

#include "config.h"
/*********************************************
 *          以下为菜单通用设置.h文件
 *          包含建立菜单通用的配置              
 *
 *          by AZhao, B站 @阿釗同学
 *********************************************/

/*------------------菜单通用宏定义-------------------*/
// 元素类型
#define M_FOLDER        0
#define M_FILE          1
// 元素名称语言
#define M_ASCII         0
#define M_ZH            1
// 菜单切换操作码
#define M_PRE           0
#define M_NEXT          1
#define M_OK            2
#define M_BACK          3
#define M_BACKROOT      4       // 返回主菜单
// 返回上一级时指针位置
#define M_BACKMT        0
#define M_BACKNOMT      1
// 元素参数赋值
#define M_SETARGS(_menu,_nameLang,_type,_level,_self,_operate,_name)  {\
                            i++;\
                            _menu[i].id = i;\
                            _menu[i].nameLang = _nameLang;\
                            _menu[i].type = _type;\
                            _menu[i].level = _level;\
                            _menu[i].self = _self;\
                            if(_menu[i].type==M_FOLDER){_menu[i].e.folder.son = _operate;}\
                            else if(_menu[i].type==M_FILE){_menu[i].e.file.func = _operate;}\
                            _menu[i].name = _name;\
                        }
// 设置记忆模式 - 输入记忆位置
#define M_SETBACKMT(_menu,_curId,_pointId,_backmt)  {\
                        if(_menu.curId==_curId && _menu.pointId==_pointId){\
                            _menu.backMaintain = _backmt;\
                        }\
                    }
/*------------------菜单通用结构体定义-------------------*/
// 菜单结构体
typedef struct
{
    uint8_t id;         // 菜单元素ID 
    uint8_t type;       // 类型: 0:folder  1:file
    uint8_t* name;      // 元素显示名称
    uint8_t nameLang;   // 元素名称语言
    uint8_t self;       // 自身为第几个子节点
    uint8_t level;      // 自身所在目录层级
    union {
        struct {
            uint8_t son;    // 包含子节点的数目
        }folder;
        struct {
            void (*func)(void); // 进入文件跳转函数
        }file;
    }e; // 两种类型元素特性
}sMenu;
// 菜单位置记忆结构体
typedef struct
{
    uint8_t pointIdBackup;  // 记忆返回位置
}sMenuLoc;
// 菜单操作结构体
typedef struct
{
    sMenu* menu;    // 菜单结构体指针
    uint8_t endid;  // 菜单元素个数
    uint8_t curId;  // 当前所在元素ID
    uint8_t pointId;// 当前指针选择子元素ID -偏移
    uint8_t backMaintain;   // 返回上一级时指针是否在进入时位置
    sMenuLoc* menuLoc;  // 菜单位置记忆结构体指针
    uint8_t disFont;// 显示字体(暂时未使用)
}sMenuOpt;

#endif
