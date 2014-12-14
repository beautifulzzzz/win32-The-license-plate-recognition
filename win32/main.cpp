#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                                // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];                                // The title bar text
HBITMAP bg,dra;
HDC mdc;

const int Width=600;
const int Length=799;

const int xstart=50;
const int ystart=20;
// Foward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                MyPaint(HDC hdc);
//========================================================================================
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	// TODO: Place code here.
	MSG msg;

	MyRegisterClass(hInstance);//调用函数向系统注册窗口类别，输入参数hInstance是目前运行程序的对象代码；

	// 调用InitInstance函数，进行初始化操作；
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// 消息循环（通过消息循环来获取信息，
	//进行必要的键盘信息转换而后将控制权交给操作系统，
	//有操作系统决定哪个程序的消息处理函数处理消息
	while (GetMessage(&msg, NULL, 0, 0)) //获取程序消息
	{
		TranslateMessage(&msg);//转换伪码及字符
		DispatchMessage(&msg);//将控制权交给系统，再有系统决定负责处理消息的程序；
	}

	return msg.wParam;
}
//=====================================================================================



//=============================================================================================
//在建立程序窗口实体之前，必须先定义一个窗口类别，其中包含所要建立窗口的信息，
//并向系统注册，这里的MyRegisterClass函数就是进行定义及注册窗口类别的函数。
//==============================================================================================
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;            //申请一个窗口类别“WNDCLASSEX”和结构”wcex“
	//--------------------------------------------------------------
	//定义vcex结构的各项信息，其中设定信息处理函数（lpfnWndProc）
	//为WNDPROC，类别名称为（lpszClassName）为”fe";
	//--------------------------------------------------------------
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style            = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = (WNDPROC)WndProc;
	wcex.cbClsExtra        = 0;
	wcex.cbWndExtra        = 0;
	wcex.hInstance        = hInstance;
	wcex.hIcon            = NULL;
	wcex.hCursor        = NULL;
	wcex.hCursor        = LoadCursor(NULL,IDC_ARROW);
	wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName    = NULL;
	wcex.lpszClassName    = "fe";
	wcex.hIconSm        = NULL;

	return RegisterClassEx(&wcex);//调用RegisterClassEx函数注册类别，返回一个“ATOM"形态的字符串
	//此字符串即为类别名称”fe";
}
//============================================================================================


//============================================================================================
//按照前面所定义的窗口类别来建立并显示实际的程序窗口
//============================================================================================
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	HDC hdc;
	hInst = hInstance; // 把instance handle 储存在全局变量中；

	hWnd = CreateWindow("fe","绘图窗口",WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	//-----------------------------------------------
	//调用CreateWindow函数来建立一个窗口对象
	//第一个参数就是窗口建立依据的类别名称
	//-----------------------------------------------
	if (!hWnd)
	{
		return FALSE;
	}
	//------------------------------------------------
	//设定窗口的位置及窗口的大小，然后绘制显示在设备上
	//-------------------------------------------------
	MoveWindow(hWnd,10,10,Width,Length,true);//位置及大小
	ShowWindow(hWnd, nCmdShow);//改定窗口显示时的状态
	UpdateWindow(hWnd);//将窗口绘制在显示设备上

	hdc=GetDC(hWnd);
	mdc=CreateCompatibleDC(hdc);

	BITMAP bm1;
	unsigned char *px1;//图像矩阵
	int fenBu_Y[1000],fenBu_X[1000];//每行白色统计,每列白色统计
	for(int i=0;i<1000;i++)fenBu_Y[i]=fenBu_X[i]=0;

	bg=(HBITMAP)LoadImage(NULL,"bg.bmp",IMAGE_BITMAP,Width,Length,LR_LOADFROMFILE);//名、类型、大小、加载方式；
	GetObject(bg,sizeof(BITMAP),&bm1);
	px1=new unsigned char[bm1.bmHeight * bm1.bmWidthBytes];
	GetBitmapBits(bg,bm1.bmHeight*bm1.bmWidthBytes,px1);


	int xend,yend;
	int x,y,i;
	int rgb_b;
	int PxBytes=bm1.bmBitsPixel/8;
	for(y=0;y<(bm1.bmHeight);y++)
	{
		for(x=0;x<bm1.bmWidth;x++)
		{
			rgb_b=y*bm1.bmWidthBytes+x*PxBytes;   
			int temp=px1[rgb_b]-px1[rgb_b+2];//b-r
			/*灰度图*/
			px1[rgb_b+2]=temp;//红
			px1[rgb_b+1]=temp;//绿
			px1[rgb_b]=temp;//蓝
			/*二值化图*/
			if(temp>40)
				px1[rgb_b+2]=px1[rgb_b+1]=px1[rgb_b]=255;
			else
				px1[rgb_b+2]=px1[rgb_b+1]=px1[rgb_b]=0;
			/*统计每行白点数*/
			fenBu_Y[y]+=(px1[rgb_b+2]==255);
			fenBu_X[x]+=(px1[rgb_b+2]==255);
		}
	}

	/*左右上下边界*/
	int Y_shang4_fen1_jie4_xian4=0;
	int Y_xia4_fen1_jie4_xian4=0;
	for(y=0;y<(bm1.bmHeight);y++)
	{
		if(Y_xia4_fen1_jie4_xian4!=0)break;
		if(Y_shang4_fen1_jie4_xian4==0 && fenBu_Y[y]>150)
		{
			Y_shang4_fen1_jie4_xian4=y;
		}
		if(Y_shang4_fen1_jie4_xian4==0)continue;
		if(Y_xia4_fen1_jie4_xian4==0 && fenBu_Y[y]<150)
		{
			Y_xia4_fen1_jie4_xian4=y;
		}
	}
	int X_Zuo3_fen1_jie4_xian4[300];
	int X_You4_fen1_jie4_xian4[300];
	int num=0;
	bool left=false;
	for(x=0;x<bm1.bmWidth;x++)
	{
		if(fenBu_X[x]>70)
		{
			if(left==false)
			{
				X_Zuo3_fen1_jie4_xian4[num]=x;
				left=true;
			}
			else
			{
				X_You4_fen1_jie4_xian4[num]=x;
				left=false;
				num++;
			}
		}
	}

	/*绘制函数*/
	SetBitmapBits(bg,bm1.bmHeight*bm1.bmWidthBytes,px1);
	SelectObject(mdc,bg);
	BitBlt(hdc,0,0,1366,768,mdc,0,0,SRCCOPY);//在窗口位置、大小、原图剪切位
	HPEN hPen=CreatePen(2,1,RGB(0,0,255));//笔型；粗细；颜色
	SelectObject(hdc,hPen);
	MoveToEx(hdc,0,Y_shang4_fen1_jie4_xian4,NULL);//移到线条起点//-----GDI两个画线函数
	LineTo(hdc,1000,Y_shang4_fen1_jie4_xian4);
	MoveToEx(hdc,0, Y_xia4_fen1_jie4_xian4,NULL);//移到线条起点//-----GDI两个画线函数
	LineTo(hdc,1000, Y_xia4_fen1_jie4_xian4);
	for(i=0;i<num;i++)
	{
		if(X_You4_fen1_jie4_xian4[i]-X_Zuo3_fen1_jie4_xian4[i]<2)continue;
		MoveToEx(hdc,X_Zuo3_fen1_jie4_xian4[i],0,NULL);//移到线条起点//-----GDI两个画线函数
		LineTo(hdc,X_Zuo3_fen1_jie4_xian4[i],1000);
		MoveToEx(hdc,X_You4_fen1_jie4_xian4[i],0,NULL);//移到线条起点//-----GDI两个画线函数
		LineTo(hdc,X_You4_fen1_jie4_xian4[i],1000);
	}

	DeleteObject(hPen);
	ReleaseDC(hWnd,hdc);
	delete [] px1;

	return TRUE;
}
//============================================================================================


//============================================================================================
//
//============================================================================================
void MyPaint(HDC hdc)
{

}
//============================================================================================


//============================================================================================
//在前面定义类别的时候把WndProc定义为消息处理函数（当某些外部消息发生时，会按消息的类型
//来决定该如何进行处理。此外该函数也是一个回叫函数（CALLBACK)（windows系统函数）每一个
//程序都会接收信息，选择性接受、处理；
//============================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)                   //判断消息类型
	{
	case WM_PAINT:                //窗口重绘制
		hdc = BeginPaint(hWnd, &ps);
		MyPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:              //处理窗口结束消息
		DeleteDC(mdc);
		DeleteObject(bg);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//============================================================================================