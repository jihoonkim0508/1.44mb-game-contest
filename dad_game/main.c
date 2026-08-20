/* SDK-free Win32 game: only declarations used by this file. */
#define API __declspec(dllimport)
#define CALL __stdcall
typedef unsigned long DWORD; typedef unsigned int UINT; typedef unsigned __int64 ULONG_PTR; typedef __int64 LONG_PTR;
typedef LONG_PTR LPARAM,LRESULT; typedef ULONG_PTR WPARAM; typedef int BOOL; typedef unsigned short ATOM;
typedef unsigned short wchar_t; typedef void *HANDLE,*HINSTANCE,*HWND,*HDC,*HGDIOBJ,*HBITMAP,*HBRUSH,*HFONT,*HCURSOR,*HMONITOR; typedef const wchar_t *LPCWSTR; typedef wchar_t *LPWSTR;
typedef struct{long x,y;}POINT; typedef struct{long left,top,right,bottom;}RECT;
typedef struct{HWND hwnd;UINT message;WPARAM wParam;LPARAM lParam;DWORD time;POINT pt;DWORD private_;}MSG;
typedef struct{HDC hdc;BOOL erase;RECT paint;BOOL restore,incUpdate;unsigned char reserved[32];}PAINTSTRUCT;
typedef LRESULT(CALL*WNDPROC)(HWND,UINT,WPARAM,LPARAM);
typedef struct{UINT style;WNDPROC proc;int clsExtra,wndExtra;HINSTANCE instance;HANDLE icon;HCURSOR cursor;HBRUSH background;LPCWSTR menu,className;}WNDCLASSW;
typedef struct{DWORD size;RECT monitor,work;DWORD flags;}MONITORINFO;
API DWORD CALL GetTickCount(void); API HINSTANCE CALL GetModuleHandleW(LPCWSTR); API LPWSTR CALL GetCommandLineW(void); API __declspec(noreturn) void CALL ExitProcess(UINT);
API ATOM CALL RegisterClassW(const WNDCLASSW*); API HWND CALL CreateWindowExW(DWORD,LPCWSTR,LPCWSTR,DWORD,int,int,int,int,HWND,HANDLE,HINSTANCE,void*);
API BOOL CALL ShowWindow(HWND,int); API ULONG_PTR CALL SetTimer(HWND,ULONG_PTR,UINT,void*); API int CALL GetMessageW(MSG*,HWND,UINT,UINT);
API BOOL CALL TranslateMessage(const MSG*); API LRESULT CALL DispatchMessageW(const MSG*); API LRESULT CALL DefWindowProcW(HWND,UINT,WPARAM,LPARAM);
API void CALL PostQuitMessage(int); API HCURSOR CALL LoadCursorW(HINSTANCE,LPCWSTR); API BOOL CALL DestroyWindow(HWND); API BOOL CALL InvalidateRect(HWND,const RECT*,BOOL);
API HDC CALL BeginPaint(HWND,PAINTSTRUCT*); API BOOL CALL EndPaint(HWND,const PAINTSTRUCT*); API BOOL CALL GetClientRect(HWND,RECT*); API int CALL DrawTextW(HDC,LPCWSTR,int,RECT*,UINT);
API BOOL CALL MessageBeep(UINT); API int CALL wsprintfW(LPWSTR,LPCWSTR,...); API int CALL FillRect(HDC,const RECT*,HBRUSH);
API LONG_PTR CALL GetWindowLongPtrW(HWND,int); API LONG_PTR CALL SetWindowLongPtrW(HWND,int,LONG_PTR); API BOOL CALL GetWindowRect(HWND,RECT*);
API BOOL CALL SetWindowPos(HWND,HWND,int,int,int,int,UINT); API HMONITOR CALL MonitorFromWindow(HWND,DWORD); API BOOL CALL GetMonitorInfoW(HMONITOR,MONITORINFO*);
API HDC CALL CreateCompatibleDC(HDC); API HBITMAP CALL CreateCompatibleBitmap(HDC,int,int); API HGDIOBJ CALL SelectObject(HDC,HGDIOBJ); API HBRUSH CALL CreateSolidBrush(DWORD);
API BOOL CALL DeleteObject(HGDIOBJ); API BOOL CALL DeleteDC(HDC); API int CALL SetBkMode(HDC,int); API DWORD CALL SetTextColor(HDC,DWORD);
API HFONT CALL CreateFontW(int,int,int,int,int,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,LPCWSTR); API BOOL CALL BitBlt(HDC,int,int,int,int,HDC,int,int,DWORD);
API BOOL CALL StretchBlt(HDC,int,int,int,int,HDC,int,int,int,int,DWORD); API BOOL CALL Ellipse(HDC,int,int,int,int);
#define RGB(r,g,b)((DWORD)((r)|((g)<<8)|((b)<<16)))
#define WM_DESTROY 2
#define WM_PAINT 15
#define WM_KEYDOWN 256
#define WM_KEYUP 257
#define WM_TIMER 275
#define VK_RETURN 13
#define VK_SPACE 32
#define VK_ESCAPE 27
#define VK_F11 122
#define WS_WINDOW 0x00CA0000
#define WS_POPUP 0x80000000
#define CW_USEDEFAULT 0x80000000
#define IDC_ARROW ((LPCWSTR)32512)
#define GWL_STYLE -16
#define MONITOR_DEFAULTTONEAREST 2
#define SWP_FRAMECHANGED 0x20
#define SWP_SHOWWINDOW 0x40
#define DT_CENTER 1
#define DT_VCENTER 4
#define DT_SINGLELINE 32
#define TRANSPARENT 1
#define FW_NORMAL 400
#define FW_BOLD 700
#define HANGUL_CHARSET 129
#define ANTIALIASED_QUALITY 4
#define SRCCOPY 0x00CC0020
#define MB_ICONHAND 0x10
#define MB_ICONEXCLAMATION 0x30
enum{INTRO,PLAY,WIN,LOSE};
enum{QUIET,FOOTSTEPS,DOOR_OPEN};
static int mode=INTRO,score,caught,sleeping,j_down,monitor_on=1,arrival,fullscreen,f11_down;
static DWORD started,last_tick,next_warning,door_at,door_close; static UINT rng_state; static RECT window_rect;
static UINT rnd(void){rng_state=rng_state*1664525u+1013904223u;return rng_state;}
static int safe_at_door(int sleep,int monitor,int playing){return sleep&&!monitor&&!playing;}
static void game_key(void){j_down=1;monitor_on=1;sleeping=0;}
static void monitor_key(void){j_down=0;monitor_on=0;sleeping=0;}
static void bed_key(void){j_down=0;sleeping=1;}
static int state_test(void){monitor_on=0;sleeping=1;j_down=0;game_key();if(!j_down||!monitor_on||sleeping)return 0;monitor_key();if(j_down||monitor_on||sleeping)return 0;bed_key();return sleeping&&!monitor_on&&!j_down;}
static int contains(LPCWSTR s,LPCWSTR q){int i,j;for(i=0;s[i];++i){for(j=0;q[j]&&s[i+j]==q[j];++j){}if(!q[j])return 1;}return 0;}
static void reset_game(void){DWORD now=GetTickCount();mode=PLAY;score=caught=sleeping=j_down=arrival=0;monitor_on=1;started=last_tick=now;next_warning=now+3200+rnd()%2500;}
static void fill(HDC dc,int l,int t,int r,int b,DWORD color){RECT box={l,t,r,b};HBRUSH brush=CreateSolidBrush(color);FillRect(dc,&box,brush);DeleteObject(brush);}
static void oval(HDC dc,int l,int t,int r,int b,DWORD color){HBRUSH brush=CreateSolidBrush(color);HGDIOBJ old=SelectObject(dc,brush);Ellipse(dc,l,t,r,b);SelectObject(dc,old);DeleteObject(brush);}
static void text(HDC dc,int l,int t,int r,int b,int size,int weight,DWORD color,LPCWSTR s){RECT line={l,t,r,b};HFONT f=CreateFontW(size,0,0,0,weight,0,0,0,HANGUL_CHARSET,0,0,ANTIALIASED_QUALITY,0,L"Malgun Gothic");HGDIOBJ old=SelectObject(dc,f);SetBkMode(dc,TRANSPARENT);SetTextColor(dc,color);DrawTextW(dc,s,-1,&line,DT_CENTER|DT_SINGLELINE|DT_VCENTER);SelectObject(dc,old);DeleteObject(f);}
static void character_at_desk(HDC dc){oval(dc,270,415,470,700,RGB(52,65,200));oval(dc,295,335,445,495,RGB(42,39,36));}
static void character_in_bed(HDC dc){fill(dc,55,445,665,680,RGB(83,175,192));fill(dc,75,470,645,655,RGB(86,103,175));oval(dc,145,500,540,635,RGB(52,65,200));oval(dc,485,500,610,625,RGB(42,39,36));}
static void scene(HDC dc){wchar_t hud[128];DWORD now=GetTickCount();DWORD screen_color=j_down?RGB(205,55,48):(monitor_on?RGB(245,245,235):RGB(115,115,115));
 fill(dc,0,0,720,720,RGB(242,239,225));fill(dc,0,500,720,720,RGB(79,169,187));
 if(arrival==DOOR_OPEN){fill(dc,520,20,690,215,RGB(38,31,25));fill(dc,665,20,705,215,RGB(137,101,56));text(dc,520,80,675,145,22,FW_BOLD,RGB(245,220,180),L"아빠");}
 else fill(dc,520,20,690,215,RGB(137,101,56));
 fill(dc,150,300,570,455,RGB(137,101,56));fill(dc,235,165,555,355,RGB(55,55,55));fill(dc,255,185,535,335,screen_color);fill(dc,510,370,550,430,RGB(20,20,20));
 text(dc,245,195,545,325,28,FW_BOLD,j_down?RGB(255,235,225):RGB(35,35,35),j_down?L"게임 중":(monitor_on?L"켜짐":L"꺼짐"));
 if(sleeping)character_in_bed(dc);else character_at_desk(dc);
 wsprintfW(hud,L"점수 %05d   남은 시간 %02d   들킴 %d/3",score/10,60-(now-started)/1000,caught);text(dc,20,12,500,55,20,FW_BOLD,RGB(30,30,30),hud);
 text(dc,20,670,700,710,17,FW_NORMAL,RGB(20,25,30),L"J 게임(누르기)   K 모니터 끄기   Space 자는 척   F11 전체화면");
 if(arrival==FOOTSTEPS)text(dc,100,70,620,145,38,FW_BOLD,RGB(190,40,35),L"쿵...  쿵...");
 if(arrival==DOOR_OPEN)text(dc,100,70,500,145,38,FW_BOLD,RGB(190,40,35),L"끼익—");
}
static void paint(HWND hwnd,HDC target){RECT r;HDC dc;HBITMAP bmp;HGDIOBJ old;HBRUSH black;int w,h,size,ox,oy;GetClientRect(hwnd,&r);w=r.right;h=r.bottom;dc=CreateCompatibleDC(target);bmp=CreateCompatibleBitmap(target,720,720);old=SelectObject(dc,bmp);fill(dc,0,0,720,720,RGB(20,20,20));
 if(mode==INTRO){fill(dc,0,0,720,720,RGB(20,24,35));text(dc,40,95,680,165,48,FW_BOLD,RGB(245,235,205),L"아빠 온다!");text(dc,40,210,680,255,22,FW_NORMAL,RGB(235,235,225),L"J를 누르는 동안만 점수를 얻는다");text(dc,40,260,680,305,22,FW_NORMAL,RGB(235,235,225),L"K로 모니터를 끄고 Space로 자는 척");text(dc,40,310,680,355,22,FW_NORMAL,RGB(235,235,225),L"둘 중 하나라도 빼먹으면 들킨다");text(dc,40,440,680,500,28,FW_BOLD,RGB(105,205,160),L"Enter로 시작");text(dc,40,540,680,590,18,FW_NORMAL,RGB(175,180,190),L"F11 창 모드 / 전체화면");}
 else if(mode==WIN||mode==LOSE){fill(dc,0,0,720,720,mode==WIN?RGB(24,67,55):RGB(80,30,30));text(dc,40,130,680,205,48,FW_BOLD,RGB(245,235,205),mode==WIN?L"무사히 버텼다!":L"컴퓨터 압수!");{wchar_t result[64];wsprintfW(result,L"최종 점수 %d",score/10);text(dc,40,265,680,325,30,FW_BOLD,RGB(240,240,230),result);}text(dc,40,420,680,480,24,FW_NORMAL,RGB(220,225,215),L"Enter로 다시 하기");}
 else scene(dc);
 black=CreateSolidBrush(RGB(15,15,15));FillRect(target,&r,black);DeleteObject(black);size=w<h?w:h;ox=(w-size)/2;oy=(h-size)/2;StretchBlt(target,ox,oy,size,size,dc,0,0,720,720,SRCCOPY);SelectObject(dc,old);DeleteObject(bmp);DeleteDC(dc);}
static void toggle_fullscreen(HWND hwnd){MONITORINFO info;if(!fullscreen){GetWindowRect(hwnd,&window_rect);SetWindowLongPtrW(hwnd,GWL_STYLE,(LONG_PTR)WS_POPUP);info.size=sizeof(info);if(GetMonitorInfoW(MonitorFromWindow(hwnd,MONITOR_DEFAULTTONEAREST),&info))SetWindowPos(hwnd,0,info.monitor.left,info.monitor.top,info.monitor.right-info.monitor.left,info.monitor.bottom-info.monitor.top,SWP_FRAMECHANGED|SWP_SHOWWINDOW);fullscreen=1;}else{SetWindowLongPtrW(hwnd,GWL_STYLE,(LONG_PTR)WS_WINDOW);SetWindowPos(hwnd,0,window_rect.left,window_rect.top,window_rect.right-window_rect.left,window_rect.bottom-window_rect.top,SWP_FRAMECHANGED|SWP_SHOWWINDOW);fullscreen=0;}}
static void update(HWND hwnd){DWORD now,dt;if(mode!=PLAY)return;now=GetTickCount();dt=now-last_tick;last_tick=now;if(j_down&&!sleeping)score+=(int)dt;
 if(arrival==QUIET&&now>=next_warning){arrival=FOOTSTEPS;door_at=now+1300+rnd()%700;MessageBeep(MB_ICONEXCLAMATION);}
 if(arrival==FOOTSTEPS&&now>=door_at){arrival=DOOR_OPEN;door_close=now+950;if(!safe_at_door(sleeping,monitor_on,j_down)){caught++;MessageBeep(MB_ICONHAND);}if(caught>=3)mode=LOSE;}
 if(arrival==DOOR_OPEN&&now>=door_close){arrival=QUIET;next_warning=now+2600+rnd()%3200;}
 if(mode==PLAY&&now-started>=60000)mode=WIN;InvalidateRect(hwnd,0,0);}
static LRESULT CALL wndproc(HWND hwnd,UINT msg,WPARAM w,LPARAM l){(void)l;
 if(msg==WM_KEYDOWN){if(w==VK_F11&&!f11_down){f11_down=1;toggle_fullscreen(hwnd);}if(w==VK_RETURN&&(mode==INTRO||mode==WIN||mode==LOSE))reset_game();if(mode==PLAY&&w=='J')game_key();if(mode==PLAY&&w=='K')monitor_key();if(mode==PLAY&&w==VK_SPACE)bed_key();if(w==VK_ESCAPE)DestroyWindow(hwnd);return 0;}
 if(msg==WM_KEYUP){if(w=='J')j_down=0;if(w==VK_F11)f11_down=0;return 0;}if(msg==WM_TIMER){update(hwnd);return 0;}if(msg==WM_PAINT){PAINTSTRUCT ps;HDC dc=BeginPaint(hwnd,&ps);paint(hwnd,dc);EndPaint(hwnd,&ps);return 0;}if(msg==WM_DESTROY){PostQuitMessage(0);return 0;}return DefWindowProcW(hwnd,msg,w,l);}
void mainCRTStartup(void){HINSTANCE inst;static WNDCLASSW wc;HWND hwnd;MSG msg;int rc;if(contains(GetCommandLineW(),L"--self-test"))ExitProcess(state_test()&&safe_at_door(1,0,0)&&!safe_at_door(0,0,0)&&!safe_at_door(1,1,0)&&!safe_at_door(1,0,1)?0:1);
 rng_state=GetTickCount();inst=GetModuleHandleW(0);wc.proc=wndproc;wc.instance=inst;wc.cursor=LoadCursorW(0,IDC_ARROW);wc.className=L"DadGameWindow";if(!RegisterClassW(&wc))ExitProcess(2);
 hwnd=CreateWindowExW(0,wc.className,L"아빠 온다!",WS_WINDOW,CW_USEDEFAULT,CW_USEDEFAULT,760,790,0,0,inst,0);if(!hwnd)ExitProcess(3);ShowWindow(hwnd,5);SetTimer(hwnd,1,16,0);
 while((rc=GetMessageW(&msg,0,0,0))>0){TranslateMessage(&msg);DispatchMessageW(&msg);}ExitProcess(rc<0?4:(UINT)msg.wParam);}
