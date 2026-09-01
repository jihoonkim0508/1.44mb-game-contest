/* SDK-free Win32 game: only declarations used by this file. */
#define API __declspec(dllimport)
#define CALL __stdcall
typedef unsigned long DWORD; typedef unsigned int UINT; typedef unsigned __int64 ULONG_PTR; typedef __int64 LONG_PTR;
typedef LONG_PTR LPARAM,LRESULT; typedef ULONG_PTR WPARAM; typedef int BOOL; typedef unsigned short ATOM;
typedef unsigned short wchar_t; typedef void *HANDLE,*HINSTANCE,*HWND,*HDC,*HGDIOBJ,*HBITMAP,*HBRUSH,*HCURSOR,*HMONITOR; typedef const wchar_t *LPCWSTR; typedef wchar_t *LPWSTR;
#include "font_atlas_data.h"
typedef struct{long x,y;}POINT; typedef struct{long left,top,right,bottom;}RECT;
typedef struct{HWND hwnd;UINT message;WPARAM wParam;LPARAM lParam;DWORD time;POINT pt;DWORD private_;}MSG;
typedef struct{HDC hdc;BOOL erase;RECT paint;BOOL restore,incUpdate;unsigned char reserved[32];}PAINTSTRUCT;
typedef LRESULT(CALL*WNDPROC)(HWND,UINT,WPARAM,LPARAM);
typedef struct{UINT style;WNDPROC proc;int clsExtra,wndExtra;HINSTANCE instance;HANDLE icon;HCURSOR cursor;HBRUSH background;LPCWSTR menu,className;}WNDCLASSW;
typedef struct{DWORD size;RECT monitor,work;DWORD flags;}MONITORINFO;
typedef struct{unsigned char operation,flags,alpha,format;}BLENDFUNCTION;
API DWORD CALL GetTickCount(void); API HINSTANCE CALL GetModuleHandleW(LPCWSTR); API LPWSTR CALL GetCommandLineW(void); API __declspec(noreturn) void CALL ExitProcess(UINT);
API DWORD CALL GetModuleFileNameW(HINSTANCE,LPWSTR,DWORD); API HANDLE CALL CreateFileW(LPCWSTR,DWORD,DWORD,void*,DWORD,DWORD,HANDLE); API DWORD CALL SetFilePointer(HANDLE,long,long*,DWORD); API BOOL CALL ReadFile(HANDLE,void*,DWORD,DWORD*,void*); API BOOL CALL CloseHandle(HANDLE);
API ATOM CALL RegisterClassW(const WNDCLASSW*); API HWND CALL CreateWindowExW(DWORD,LPCWSTR,LPCWSTR,DWORD,int,int,int,int,HWND,HANDLE,HINSTANCE,void*);
API BOOL CALL AdjustWindowRectEx(RECT*,DWORD,BOOL,DWORD);
API BOOL CALL ShowWindow(HWND,int); API ULONG_PTR CALL SetTimer(HWND,ULONG_PTR,UINT,void*); API int CALL GetMessageW(MSG*,HWND,UINT,UINT);
API BOOL CALL TranslateMessage(const MSG*); API LRESULT CALL DispatchMessageW(const MSG*); API LRESULT CALL DefWindowProcW(HWND,UINT,WPARAM,LPARAM);
API void CALL PostQuitMessage(int); API HCURSOR CALL LoadCursorW(HINSTANCE,LPCWSTR); API BOOL CALL DestroyWindow(HWND); API BOOL CALL InvalidateRect(HWND,const RECT*,BOOL);
API HDC CALL BeginPaint(HWND,PAINTSTRUCT*); API BOOL CALL EndPaint(HWND,const PAINTSTRUCT*); API BOOL CALL GetClientRect(HWND,RECT*);
API BOOL CALL MessageBeep(UINT); API int CALL wsprintfW(LPWSTR,LPCWSTR,...); API int CALL FillRect(HDC,const RECT*,HBRUSH);
API LONG_PTR CALL GetWindowLongPtrW(HWND,int); API LONG_PTR CALL SetWindowLongPtrW(HWND,int,LONG_PTR); API BOOL CALL GetWindowRect(HWND,RECT*);
API BOOL CALL SetWindowPos(HWND,HWND,int,int,int,int,UINT); API HMONITOR CALL MonitorFromWindow(HWND,DWORD); API BOOL CALL GetMonitorInfoW(HMONITOR,MONITORINFO*);
API HDC CALL CreateCompatibleDC(HDC); API HBITMAP CALL CreateCompatibleBitmap(HDC,int,int); API HGDIOBJ CALL SelectObject(HDC,HGDIOBJ); API HBRUSH CALL CreateSolidBrush(DWORD);
API BOOL CALL DeleteObject(HGDIOBJ); API BOOL CALL DeleteDC(HDC); API BOOL CALL BitBlt(HDC,int,int,int,int,HDC,int,int,DWORD);
API BOOL CALL StretchBlt(HDC,int,int,int,int,HDC,int,int,int,int,DWORD);
API int CALL SetStretchBltMode(HDC,int);
API int CALL StretchDIBits(HDC,int,int,int,int,int,int,int,int,const void*,const void*,UINT,DWORD);
API HBITMAP CALL CreateDIBitmap(HDC,const void*,DWORD,const void*,const void*,UINT); API BOOL CALL TransparentBlt(HDC,int,int,int,int,HDC,int,int,int,int,UINT);
API BOOL CALL AlphaBlend(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
API BOOL CALL PlaySoundW(LPCWSTR,HANDLE,DWORD);
#define RGB(r,g,b)((DWORD)((r)|((g)<<8)|((b)<<16)))
#define WM_DESTROY 2
#define WM_PAINT 15
#define WM_ERASEBKGND 20
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
#define FW_NORMAL 400
#define FW_BOLD 700
#define SRCCOPY 0x00CC0020
#define COLORONCOLOR 3
#define MB_ICONHAND 0x10
#define GENERIC_READ 0x80000000
#define OPEN_EXISTING 3
#define FILE_ATTRIBUTE_NORMAL 0x80
#define FILE_END 2
#define CBM_INIT 4
#define SND_ASYNC 1
#define SND_NODEFAULT 2
#define SND_MEMORY 4
#define SND_LOOP 8
enum{INTRO,PLAY,WIN,LOSE};
enum{QUIET,DAD_PRESENT};
static int mode=INTRO,score,caught,visits,sleeping,j_down,monitor_on=1,arrival,visit_caught,fullscreen,f11_down,qa_freeze;
static DWORD started,last_tick,next_warning,warning_until,door_close,caught_fx_until,lose_at,audio_resume_at,found_sound_at; static UINT rng_state; static RECT window_rect;
static unsigned char room_bmp[ROOM_BMP_SIZE],sheet_bmp[SHEET_BMP_SIZE],founded_wav[FOUNDED_WAV_SIZE],game_wav[GAME_WAV_SIZE],stomp_wav[STOMP_WAV_SIZE],door_wav[DOOR_WAV_SIZE]; static int room_loaded,audio_loaded; static HBITMAP sheet_bitmap;
static UINT rnd(void){rng_state=rng_state*1664525u+1013904223u;return rng_state;}
static int safe_at_door(int sleep,int monitor,int playing){return sleep&&!monitor&&!playing;}
static DWORD warning_ms(DWORD elapsed,int visit){return visit==0?900:elapsed>=40000?600:700;}
static void game_key(void){j_down=1;monitor_on=1;sleeping=0;}
static void monitor_key(void){j_down=0;monitor_on=0;sleeping=0;}
static void bed_key(void){j_down=0;sleeping=1;}
static int state_test(void){monitor_on=0;sleeping=1;j_down=0;game_key();if(!j_down||!monitor_on||sleeping)return 0;monitor_on=1;sleeping=j_down=0;bed_key();if(!sleeping||!monitor_on||j_down)return 0;monitor_key();if(j_down||monitor_on||sleeping)return 0;bed_key();return sleeping&&!monitor_on&&!j_down&&warning_ms(0,0)==900&&warning_ms(10000,1)==700&&warning_ms(39999,4)==700&&warning_ms(40000,1)==600;}
static int contains(LPCWSTR s,LPCWSTR q){int i,j;for(i=0;s[i];++i){for(j=0;q[j]&&s[i+j]==q[j];++j){}if(!q[j])return 1;}return 0;}
static void load_room(HINSTANCE inst){wchar_t path[520];HANDLE file;DWORD got=0,offset,soffset;if(!GetModuleFileNameW(inst,path,520))return;file=CreateFileW(path,GENERIC_READ,1,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);if(file==(HANDLE)(LONG_PTR)-1)return;SetFilePointer(file,-(long)(ROOM_BMP_SIZE+SHEET_BMP_SIZE+FOUNDED_WAV_SIZE+GAME_WAV_SIZE+STOMP_WAV_SIZE+DOOR_WAV_SIZE),0,FILE_END);if(ReadFile(file,room_bmp,ROOM_BMP_SIZE,&got,0)&&got==ROOM_BMP_SIZE&&ReadFile(file,sheet_bmp,SHEET_BMP_SIZE,&got,0)&&got==SHEET_BMP_SIZE&&ReadFile(file,founded_wav,FOUNDED_WAV_SIZE,&got,0)&&got==FOUNDED_WAV_SIZE&&ReadFile(file,game_wav,GAME_WAV_SIZE,&got,0)&&got==GAME_WAV_SIZE&&ReadFile(file,stomp_wav,STOMP_WAV_SIZE,&got,0)&&got==STOMP_WAV_SIZE&&ReadFile(file,door_wav,DOOR_WAV_SIZE,&got,0)&&got==DOOR_WAV_SIZE&&room_bmp[0]=='B'&&room_bmp[1]=='M'&&sheet_bmp[0]=='B'&&sheet_bmp[1]=='M'){offset=*(DWORD*)(room_bmp+10);soffset=*(DWORD*)(sheet_bmp+10);room_loaded=offset<ROOM_BMP_SIZE&&soffset<SHEET_BMP_SIZE;audio_loaded=founded_wav[0]=='R'&&game_wav[0]=='R'&&stomp_wav[0]=='R'&&door_wav[0]=='R';}CloseHandle(file);}
static void play_wav(unsigned char *wav,int loop){if(audio_loaded)PlaySoundW((LPCWSTR)wav,0,SND_ASYNC|SND_MEMORY|SND_NODEFAULT|(loop?SND_LOOP:0));}
static void play_base_audio(void){if(j_down&&!sleeping)play_wav(game_wav,1);else PlaySoundW(0,0,0);}
static void reset_game(void){DWORD now=GetTickCount();mode=PLAY;score=caught=visits=sleeping=j_down=arrival=visit_caught=0;monitor_on=1;warning_until=caught_fx_until=lose_at=audio_resume_at=found_sound_at=0;started=last_tick=now;next_warning=now+3200+rnd()%2500;}
static void fill(HDC dc,int l,int t,int r,int b,DWORD color){RECT box={l,t,r,b};HBRUSH brush=CreateSolidBrush(color);FillRect(dc,&box,brush);DeleteObject(brush);}
static void alpha_fill(HDC dc,int l,int t,int r,int b,DWORD color,unsigned char alpha){HDC layer=CreateCompatibleDC(dc);HBITMAP pixel=CreateCompatibleBitmap(dc,1,1);HGDIOBJ old=SelectObject(layer,pixel);BLENDFUNCTION blend={0,0,alpha,0};fill(layer,0,0,1,1,color);AlphaBlend(dc,l,t,r-l,b-t,layer,0,0,1,1,blend);SelectObject(layer,old);DeleteObject(pixel);DeleteDC(layer);}
static void ensure_sheet(HDC dc){if(room_loaded&&!sheet_bitmap){DWORD offset=*(DWORD*)(sheet_bmp+10);sheet_bitmap=CreateDIBitmap(dc,sheet_bmp+14,CBM_INIT,sheet_bmp+offset,sheet_bmp+14,0);}}
static int glyph_index(wchar_t c){int i;for(i=0;font_chars[i];++i)if(font_chars[i]==c)return i;return -1;}
static int glyph_width(wchar_t c,int size){int index;if(c==' ')return (font_space_width*size+10)/20;index=glyph_index(c);if(index<0)index=glyph_index('?');return (font_widths[index]*size+10)/20;}
static void text(HDC dc,int l,int t,int r,int b,int size,int weight,DWORD color,LPCWSTR s){HDC sprites;HGDIOBJ old;int i,index,bank=0,width=0,count=0,x,y,dw,tracking=(size+10)/20;(void)weight;if(!sheet_bitmap)return;for(i=0;s[i];++i){width+=glyph_width(s[i],size);count++;}if(count>1)width+=tracking*(count-1);x=l+((r-l)-width)/2;y=t+((b-t)-size)/2;if(color==RGB(255,185,55))bank=1;else if(color==RGB(105,205,160))bank=2;else if(color==RGB(12,15,20))bank=3;sprites=CreateCompatibleDC(dc);old=SelectObject(sprites,sheet_bitmap);for(i=0;s[i];++i,x+=dw+(s[i+1]?tracking:0)){dw=glyph_width(s[i],size);if(s[i]==' ')continue;index=glyph_index(s[i]);if(index<0)index=glyph_index('?');TransparentBlt(dc,x,y,dw,size,sprites,(index%32)*16,272+bank*60+(index/32)*20,font_widths[index],20,RGB(255,0,255));}SelectObject(sprites,old);DeleteDC(sprites);}
static void outlined_text(HDC dc,int l,int t,int r,int b,int size,DWORD color,LPCWSTR s){int d=size>=40?3:2;DWORD dark=RGB(12,15,20);text(dc,l-d,t-d,r-d,b-d,size,FW_NORMAL,dark,s);text(dc,l,t-d,r,b-d,size,FW_NORMAL,dark,s);text(dc,l+d,t-d,r+d,b-d,size,FW_NORMAL,dark,s);text(dc,l-d,t,r-d,b,size,FW_NORMAL,dark,s);text(dc,l+d,t,r+d,b,size,FW_NORMAL,dark,s);text(dc,l-d,t+d,r-d,b+d,size,FW_NORMAL,dark,s);text(dc,l,t+d,r,b+d,size,FW_NORMAL,dark,s);text(dc,l+d,t+d,r+d,b+d,size,FW_NORMAL,dark,s);text(dc,l,t,r,b,size,FW_BOLD,color,s);}
static void draw_room(HDC dc,int x,int y){if(room_loaded){DWORD offset=*(DWORD*)(room_bmp+10);StretchDIBits(dc,x,y,720,720,0,0,360,360,room_bmp+offset,room_bmp+14,0,SRCCOPY);}else fill(dc,x,y,x+720,y+720,RGB(45,42,38));}
static void world(HDC dc,int sx,int sy,int phase,int j_fx){HDC sprites;HGDIOBJ old;DWORD now=GetTickCount();int father_visible=arrival==DAD_PRESENT||(caught_fx_until&&now<caught_fx_until);
 if(sx||sy)draw_room(dc,0,0);draw_room(dc,sx,sy);
 if(sheet_bitmap){sprites=CreateCompatibleDC(dc);old=SelectObject(sprites,sheet_bitmap);
  if(monitor_on)TransparentBlt(dc,sx+168,sy+154,328,190,sprites,310,105,164,95,RGB(255,0,255));
  if(j_fx){int pulse=phase&1;DWORD orange=RGB(255,105,18),yellow=RGB(255,225,92);fill(dc,sx+158,sy+144,sx+506,sy+150,orange);fill(dc,sx+158,sy+348,sx+506,sy+354,orange);fill(dc,sx+158,sy+144,sx+164,sy+354,orange);fill(dc,sx+500,sy+144,sx+506,sy+354,orange);fill(dc,sx+425-pulse*5,sy+211,sx+478+pulse*5,sy+223,orange);fill(dc,sx+445,sy+194-pulse*5,sx+458,sy+246+pulse*5,yellow);fill(dc,sx+438,sy+207,sx+466,sy+234,RGB(255,246,190));}
  if(father_visible)TransparentBlt(dc,sx+520,sy+18,196,328,sprites,208,105,98,164,RGB(255,0,255));
  if(sleeping)TransparentBlt(dc,sx+180,sy+500,440,200,sprites,190,0,220,100,RGB(255,0,255));else TransparentBlt(dc,sx+140,sy+310,370,380,sprites,0,0,185,190,RGB(255,0,255));
  if(sleeping){alpha_fill(dc,0,0,720,720,RGB(3,7,18),175);if(monitor_on)TransparentBlt(dc,sx+168,sy+154,328,190,sprites,310,105,164,95,RGB(255,0,255));}
  SelectObject(sprites,old);DeleteDC(sprites);}
 if(j_fx){static const int px[6]={28,500,48,520,170,392};static const int py[6]={170,160,330,315,112,360};static LPCWSTR labels[6]={L"+1",L"+2",L"+3",L"+5",L"+10",L"+2"};int i,n=phase&1;DWORD orange=RGB(255,105,18);
  fill(dc,sx+88+n*5,sy+414,sx+116+n*5,sy+424,orange);fill(dc,sx+101+n*5,sy+426,sx+132+n*5,sy+436,RGB(255,225,92));fill(dc,sx+88+n*5,sy+438,sx+116+n*5,sy+448,orange);
  fill(dc,sx+502-n*5,sy+400,sx+530-n*5,sy+410,orange);fill(dc,sx+486-n*5,sy+412,sx+517-n*5,sy+422,RGB(255,225,92));fill(dc,sx+502-n*5,sy+424,sx+530-n*5,sy+434,orange);
  for(i=0;i<6;++i){DWORD stamp=now+i*83;int burst=(int)(stamp/420),life=(int)(stamp%420),slot=(i+burst)%6,dx=(burst*17+i*11)%37-18,dy=(burst*13+i*7)%21-10,lift=life/10;LPCWSTR label=labels[(burst+i*2)%6];outlined_text(dc,sx+px[slot]+dx,sy+py[slot]+dy-lift,sx+px[slot]+dx+120,sy+py[slot]+dy+56-lift,40,i&1?RGB(245,240,220):RGB(255,185,55),label);}}
}
static void scene(HDC dc){wchar_t hud[128];DWORD now=GetTickCount();int phase=(int)((now/55)&3),sx=0,sy=0,caught_fx=caught_fx_until&&now<caught_fx_until,j_fx=!caught_fx&&j_down;
 if(caught_fx){switch((now/45)&3){case 0:sx=-12;sy=7;break;case 1:sx=10;sy=-9;break;case 2:sx=-8;sy=12;break;default:sx=12;sy=-7;}}else if(j_fx){switch(phase){case 0:sx=0;sy=0;break;case 1:sx=-6;sy=3;break;case 2:sx=6;sy=-3;break;default:sx=-4;sy=2;}}
 world(dc,sx,sy,phase,j_fx);
 if(warning_until&&now<warning_until){fill(dc,152,78,568,170,RGB(22,25,31));fill(dc,152,78,160,170,RGB(255,105,18));fill(dc,560,78,568,170,RGB(255,105,18));outlined_text(dc,170,88,550,160,48,RGB(255,185,55),L"쿵.. 쿵..");}
 fill(dc,0,0,620,72,RGB(20,24,28));
 wsprintfW(hud,L"점수 %05d   남은 시간 %02d   들킴 %d/3",score/10,60-(now-started)/1000,caught);text(dc,20,8,600,66,28,FW_BOLD,RGB(245,240,220),hud);
 fill(dc,0,674,720,720,RGB(20,24,28));text(dc,20,674,700,720,20,FW_BOLD,RGB(245,240,220),L"J 게임(누르기)   K 모니터 끄기   Space 자는 척   F11 전체화면");
 if(caught_fx){DWORD red=RGB(215,35,28);fill(dc,0,0,720,14,red);fill(dc,0,706,720,720,red);fill(dc,0,0,14,720,red);fill(dc,706,0,720,720,red);fill(dc,168,205,496,295,RGB(90,20,20));outlined_text(dc,168,205,496,295,60,RGB(255,235,210),L"들켰다!");}
}
static void paint(HWND hwnd,HDC target){RECT r;HDC dc;HBITMAP bmp;HGDIOBJ old;int w,h,size,ox,oy;GetClientRect(hwnd,&r);w=r.right;h=r.bottom;dc=CreateCompatibleDC(target);bmp=CreateCompatibleBitmap(target,720,720);old=SelectObject(dc,bmp);ensure_sheet(dc);fill(dc,0,0,720,720,RGB(20,20,20));
 if(mode==INTRO){DWORD accent=RGB(241,153,45);fill(dc,0,0,720,720,RGB(20,24,35));fill(dc,0,0,720,9,accent);fill(dc,0,711,720,720,accent);fill(dc,0,0,9,720,accent);fill(dc,711,0,720,720,accent);outlined_text(dc,40,88,680,180,60,RGB(245,235,205),L"몰컴하기");text(dc,40,205,680,255,24,FW_BOLD,RGB(235,235,225),L"J를 누르는 동안만 점수를 얻는다");text(dc,40,260,680,310,24,FW_BOLD,RGB(235,235,225),L"K로 모니터를 끄고 Space로 자는 척");text(dc,40,315,680,365,24,FW_BOLD,RGB(235,235,225),L"둘 중 하나라도 빼먹으면 들킨다");outlined_text(dc,40,425,680,510,40,RGB(105,205,160),L"Enter로 시작");text(dc,40,540,680,600,20,FW_BOLD,RGB(245,240,220),L"F11 창 모드 / 전체화면");}
 else if(mode==WIN||mode==LOSE){DWORD accent=mode==WIN?RGB(105,205,160):RGB(215,35,28);fill(dc,0,0,720,720,mode==WIN?RGB(24,67,55):RGB(80,30,30));fill(dc,0,0,720,9,accent);fill(dc,0,711,720,720,accent);fill(dc,0,0,9,720,accent);fill(dc,711,0,720,720,accent);outlined_text(dc,40,120,680,220,60,RGB(245,235,205),mode==WIN?L"무사히 버텼다!":L"컴퓨터 압수!");{wchar_t result[64];wsprintfW(result,L"최종 점수 %d",score/10);outlined_text(dc,40,255,680,335,40,RGB(240,240,230),result);}text(dc,40,410,680,490,32,FW_BOLD,RGB(220,225,215),L"Enter로 다시 하기");}
 else scene(dc);
 size=w<h?w:h;ox=(w-size)/2;oy=(h-size)/2;fill(target,0,0,ox,h,RGB(15,15,15));fill(target,ox+size,0,w,h,RGB(15,15,15));fill(target,ox,0,ox+size,oy,RGB(15,15,15));fill(target,ox,oy+size,ox+size,h,RGB(15,15,15));SetStretchBltMode(target,COLORONCOLOR);StretchBlt(target,ox,oy,size,size,dc,0,0,720,720,SRCCOPY);SelectObject(dc,old);DeleteObject(bmp);DeleteDC(dc);}
static void toggle_fullscreen(HWND hwnd){MONITORINFO info;if(!fullscreen){GetWindowRect(hwnd,&window_rect);SetWindowLongPtrW(hwnd,GWL_STYLE,(LONG_PTR)WS_POPUP);info.size=sizeof(info);if(GetMonitorInfoW(MonitorFromWindow(hwnd,MONITOR_DEFAULTTONEAREST),&info))SetWindowPos(hwnd,0,info.monitor.left,info.monitor.top,info.monitor.right-info.monitor.left,info.monitor.bottom-info.monitor.top,SWP_FRAMECHANGED|SWP_SHOWWINDOW);fullscreen=1;}else{SetWindowLongPtrW(hwnd,GWL_STYLE,(LONG_PTR)WS_WINDOW);SetWindowPos(hwnd,0,window_rect.left,window_rect.top,window_rect.right-window_rect.left,window_rect.bottom-window_rect.top,SWP_FRAMECHANGED|SWP_SHOWWINDOW);fullscreen=0;}}
static void update(HWND hwnd){DWORD now,dt;if(mode!=PLAY||qa_freeze)return;now=GetTickCount();dt=now-last_tick;last_tick=now;if(found_sound_at&&now>=found_sound_at){found_sound_at=0;play_wav(founded_wav,0);audio_resume_at=now+454;}else if(audio_resume_at&&now>=audio_resume_at){audio_resume_at=0;play_base_audio();}if(j_down&&!sleeping)score+=(int)dt;
 if(arrival==QUIET&&now>=next_warning&&!warning_until){warning_until=now+warning_ms(now-started,visits);visits++;play_wav(stomp_wav,0);audio_resume_at=now+540;}
 if(arrival==QUIET&&warning_until&&now>=warning_until){warning_until=0;arrival=DAD_PRESENT;door_close=now+950;visit_caught=0;play_wav(door_wav,0);audio_resume_at=now+850;}
 if(arrival==DAD_PRESENT&&!visit_caught&&!safe_at_door(sleeping,monitor_on,j_down)){visit_caught=1;caught++;caught_fx_until=now+750;found_sound_at=now+350;if(caught>=3){lose_at=caught_fx_until;audio_resume_at=0;}}
 if(lose_at&&now>=lose_at){mode=LOSE;j_down=0;PlaySoundW(0,0,0);InvalidateRect(hwnd,0,0);return;}
 if(arrival==DAD_PRESENT&&now>=door_close){arrival=QUIET;next_warning=now+2600+rnd()%3200;}
 if(!lose_at&&now-started>=60000&&(!caught_fx_until||now>=caught_fx_until)){mode=WIN;j_down=0;audio_resume_at=0;PlaySoundW(0,0,0);}InvalidateRect(hwnd,0,0);}
static LRESULT CALL wndproc(HWND hwnd,UINT msg,WPARAM w,LPARAM l){(void)l;
 if(msg==WM_KEYDOWN){if(w==VK_F11&&!f11_down){f11_down=1;toggle_fullscreen(hwnd);}if(w==VK_RETURN&&(mode==INTRO||mode==WIN||mode==LOSE))reset_game();if(mode==PLAY&&w=='J'&&!j_down){game_key();if(!audio_resume_at)play_base_audio();}if(mode==PLAY&&w=='K'){monitor_key();if(!audio_resume_at)play_base_audio();}if(mode==PLAY&&w==VK_SPACE){bed_key();if(!audio_resume_at)play_base_audio();}if(w==VK_ESCAPE)DestroyWindow(hwnd);return 0;}
 if(msg==WM_KEYUP){if(w=='J'&&j_down){j_down=0;if(!audio_resume_at)play_base_audio();}if(w==VK_F11)f11_down=0;return 0;}if(msg==WM_TIMER){update(hwnd);return 0;}if(msg==WM_ERASEBKGND)return 1;if(msg==WM_PAINT){PAINTSTRUCT ps;HDC dc=BeginPaint(hwnd,&ps);paint(hwnd,dc);EndPaint(hwnd,&ps);return 0;}if(msg==WM_DESTROY){PlaySoundW(0,0,0);if(sheet_bitmap)DeleteObject(sheet_bitmap);PostQuitMessage(0);return 0;}return DefWindowProcW(hwnd,msg,w,l);}
void mainCRTStartup(void){HINSTANCE inst;static WNDCLASSW wc;HWND hwnd;MSG msg;RECT client={0,0,720,720};int rc;LPCWSTR command=GetCommandLineW();DWORD qa_now;inst=GetModuleHandleW(0);load_room(inst);if(contains(command,L"--self-test"))ExitProcess(room_loaded&&audio_loaded&&state_test()&&safe_at_door(1,0,0)&&!safe_at_door(0,0,0)&&!safe_at_door(1,1,0)&&!safe_at_door(1,0,1)?0:1);
 rng_state=GetTickCount();if(contains(command,L"--qa-")){reset_game();qa_freeze=1;qa_now=GetTickCount();started=last_tick=qa_now;next_warning=0xffffffffu;if(contains(command,L"--qa-j")){j_down=1;score=12340;}else if(contains(command,L"--qa-warning"))warning_until=qa_now+600000;else if(contains(command,L"--qa-caught")){arrival=DAD_PRESENT;caught=1;caught_fx_until=qa_now+600000;}else if(contains(command,L"--qa-sleep-on")){sleeping=1;monitor_on=1;}else if(contains(command,L"--qa-sleep-off")){sleeping=1;monitor_on=0;}}
 wc.proc=wndproc;wc.instance=inst;wc.cursor=LoadCursorW(0,IDC_ARROW);wc.className=L"DadGameWindow";if(!RegisterClassW(&wc))ExitProcess(2);
 AdjustWindowRectEx(&client,WS_WINDOW,0,0);hwnd=CreateWindowExW(0,wc.className,L"몰컴하기",WS_WINDOW,CW_USEDEFAULT,CW_USEDEFAULT,client.right-client.left,client.bottom-client.top,0,0,inst,0);if(!hwnd)ExitProcess(3);ShowWindow(hwnd,5);SetTimer(hwnd,1,16,0);
 while((rc=GetMessageW(&msg,0,0,0))>0){TranslateMessage(&msg);DispatchMessageW(&msg);}ExitProcess(rc<0?4:(UINT)msg.wParam);}
