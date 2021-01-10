/*
	课程内容:贪吃蛇
	知识点:结构体.控制语句.循环.+easyx图形库
*/

#include<stdio.h>

//Easyx图形库中的头文件
#include<graphics.h> 

//conio是Console Input/Output（控制台输入输出）的简写
//其中定义了通过控制台进行数据输入和数据输出的函数
//主要是一些用户通过按键盘产生的对应操作，比如getch()函数等等
#include<conio.h>

/*在程序中加入背景音乐
首先要包含
#include<MMSystem.h> //播放音乐头文件
#pragma comment（lib.“win mm.lib”） //告诉编译器，加载winmm. lib库文件

函数PlaySound(TEXT(“音乐文件名.wav”).，NULL，SND_FILMNAME | SND_ASYNC | SND_LOOP） //只能用于wav文件
*/

//window控制台
#include<windows.h>

#include<mmsystem.h>  

#pragma comment(lib,"winmm.lib")

#define WIN_WIDTH 800  //窗口宽
#define WIN_HEIGHT 600 //窗口高
#define MAX_SNAKE 300  //蛇的最大长度 


/*
(0.0) - (1.0) - (2.0) - (3.0)
(0.1) - (1.1) - (2.1) - (3.1)
(0.2) - (1.2) - (2.2) - (3.2)
(0.3) - (1.3) - (2.3) - (3.3)
*/


//枚举蛇的方向
enum DIR  //枚举蛇的方向
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

struct Snake_tlg  //蛇的结构体
{
	int num;  //长度
	int dir;  //方向
	int score;  //分数
	int size;  //蛇的宽度和高度(每一节的方块大小)
	POINT coor[MAX_SNAKE];  //坐标

	/*
	typedef struct tagPOINT
   {
	LONG  x;
	LONG  y;
	} POINT;
   */

}snake;  //定义蛇

struct Food_tlg
{
	POINT fd;  //食物坐标
	int flag;  //个数
	DWORD color;  //颜色

}food;  //定义食物结构体变量


//游戏开始界面函数声明
void GameFrameWork();

//游戏输入函数声明
void GameInit();

//游戏绘图函数声明
void GameDraw();

//蛇的移动函数声明
void SnakeMove();

//键盘输入函数声明
void KeyControl();

//吃食物函数声明
void EatFood();

//不能吃自己函数声明
void DontEatSelf();

//游戏暂停函数声明
void Pause();

int main()
{
	void GameFrameWork();
	initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);  //初始化一个图形窗口

	GameInit();  //蛇初始化数值

	DWORD t1, t2;
	//取当前时刻
	t1 = t2 = GetTickCount();

	//控制闪屏
	BeginBatchDraw();
	while (1)
	{
		//控制蛇的移动
		if (t2 - t1 > 40)
		{
			SnakeMove();
			t1 = t2;
		}
		//取当前时刻
		t2 = GetTickCount();

		//控制闪屏
		FlushBatchDraw();

		//游戏暂停函数
		Pause();

		//画蛇
		GameDraw();

		//吃食物
		EatFood();

		//读取键盘输入
		KeyControl();

		//Sleep(50);  //(影响蛇的速度并且也会影响控制速度)

		//判断是否吃到自己(如果撞到则暂停游戏)
		DontEatSelf();

	}

	system("pause");
	closegraph();  //关掉窗口图
	return(0);
}

//游戏输入函数(蛇的初始化)
void GameInit()
{
	void GameFrameWork();

	srand(GetTickCount());  //将电脑时间变成随机数种子

	//初始化蛇

	snake.num = 3;  //初始长度
	snake.dir = RIGHT;  //初始方向
	snake.score = 0;  //初始分数
	snake.size = 10;  //初始宽和高(每一节蛇的高度和宽度)
	//尾
	snake.coor[2].x = 0;
	snake.coor[2].y = 0;

	//身体
	snake.coor[1].x = snake.size;
	snake.coor[1].y = 0;

	//头
	snake.coor[0].x = 2 * snake.size;
	snake.coor[0].y = 0;


	//初始化食物

	//随机食物坐标
	food.fd.x = rand() % (WIN_WIDTH / 10) * 10;
	food.fd.y = rand() % (WIN_HEIGHT / 10) * 10;

	//食物个数
	food.flag = 1;

	//随机颜色
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);


	//循环播放音乐
	mciSendString("open music.mp3 alias MyLove", NULL, 0, NULL);
	mciSendString("play MyLove repeat", NULL, 0, NULL);



	//检测是否有按键
	//kbhit();s

}

//游戏绘图函数
void GameDraw()
{
	//设置背景颜色
	setbkcolor(RGB(105, 160, 141));
	cleardevice();  //清理设备

	//画蛇

	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(BLACK);
		setfillcolor(YELLOW);
		fillrectangle(snake.coor[i].x, snake.coor[i].y, snake.coor[i].x + snake.size, snake.coor[i].y + snake.size);
		/*
		fillrectangle函数共有四个参数
		横坐标snake.coor[i].x到snake.coor[i].x + snake.size
		纵坐标snake.coor[i].y到snake.coor[i].y + snake.size
		画一个正方形
		*/
		printf("%d    %d\n", snake.coor[i].x, snake.coor[i].y);  //坐标
	}

	//画食物(如果食物个数为1则会画出食物)
	if (food.flag == 1)
	{
		setfillcolor(food.color);
		fillellipse(food.fd.x, food.fd.y, food.fd.x + snake.size, food.fd.y + snake.size);
	}

	//显示分数
	char temp[20] = "";
	sprintf(temp, "分数:%d", snake.score);  //将格式化数据写入字符串
	setbkmode(TRANSPARENT);
	outtextxy(20, 20, temp);  //将UNICODE字符集转化为多字节字符集(在项目属性字符集中更改)

	//提示信息
	char Ttemp[50] = "按回车暂停游戏!(任意键即可继续游戏)";
	setbkmode(TRANSPARENT);
	outtextxy(200, 20, Ttemp);

}

//蛇的移动函数
void SnakeMove()
{
	//遍历从大到小,从尾部到头部
	for (int i = snake.num - 1; i > 0; i--)  //从最后一节蛇开始,每一节蛇都等于前一节蛇的上次坐标
	{
		//将前一个元素的坐标赋值给后面元素的坐标数组中
		snake.coor[i].x = snake.coor[i - 1].x;
		snake.coor[i].y = snake.coor[i - 1].y;
	}
	switch (snake.dir)
	{
	case UP:
		snake.coor[0].y -= 10;  //头向上
		if (snake.coor[0].y + 10 <= 0)
		{
			printf("You bit the wall");
			//snake.coor[0].y = WIN_HEIGHT;
		}
		break;
	case DOWN:
		snake.coor[0].y += 10;  //头向下
		if (snake.coor[0].y - 10 >= WIN_HEIGHT)
		{
			printf("You bit the wall");
			//snake.coor[0].y = 0;
		}
		break;
	case LEFT:
		snake.coor[0].x -= 10;  //头向左
		if (snake.coor[0].x + 10 <= 0)
		{
			printf("You bit the wall");
			//snake.coor[0].x = WIN_WIDTH;
		}
		break;
	case RIGHT:
		snake.coor[0].x += 10;  //头向右
		if (snake.coor[0].x - 10 >= WIN_WIDTH)
		{
			printf("You bit the wall");
			//snake.coor[0].x = 0;
		}
		break;
	default:
		break;
	}
}

//读取键盘输入函数
void KeyControl()
{
	//使用win32API获取键盘消息
	if (GetAsyncKeyState(VK_UP) && snake.dir != DOWN)
	{
		snake.dir = UP;
	}
	if (GetAsyncKeyState(VK_DOWN) && snake.dir != UP)
	{
		snake.dir = DOWN;
	}
	if (GetAsyncKeyState(VK_LEFT) && snake.dir != RIGHT)
	{
		snake.dir = LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT) && snake.dir != LEFT)
	{
		snake.dir = RIGHT;
	}
}

//吃食物函数
void EatFood()
{
	//如果蛇头坐标与食物坐标相同
	if (snake.coor[0].x == food.fd.x && snake.coor[0].y == food.fd.y && food.flag == 1)
	{
		snake.num++;
		snake.score += 10;
		food.flag = 0;
	}
	if (food.flag == 0)
	{
		//初始化食物

		//随机食物坐标
		food.fd.x = rand() % (WIN_WIDTH / 10) * 10;
		food.fd.y = rand() % (WIN_HEIGHT / 10) * 10;

		//食物个数
		food.flag = 1;

		//随机颜色
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
}

//不能吃自己函数
void DontEatSelf()
{
	//理论上蛇的前三节不可能撞到,只可能从第四节开始才有可能
	for (int i = 4; i < snake.num; i++)
	{
		if (snake.coor[0].x == snake.coor[i].x && snake.coor[0].y == snake.coor[i].y)
		{
			outtextxy(200, 200, "上分王(sfw) 给爷爬！");
			system("pause");
			exit(666);
		}
	}
}

//游戏暂停函数
void Pause()
{
	if (GetAsyncKeyState(VK_RETURN))
	{
		system("pause");
	}
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		exit(-1);
	}
}


//游戏开始界面
void GameFrameWork()
{
	printf("***************************");
	printf("Press Space go on this game");
	printf("***************************");
	printf("Press ESC run out this game");
	printf("***************************");

	Sleep(300);
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		exit(-1);
	}


	system("pause");
}