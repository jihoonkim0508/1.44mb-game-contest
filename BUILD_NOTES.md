# Windows x64 빌드 메모

## 현재 설치 상태 (2026-08-20 확인)

- Visual Studio Community 2026 18.9.1
- MSVC x64 컴파일러 `cl.exe` 19.51.36256
- MSVC linker/library tools `link.exe`, `lib.exe`, `dumpbin.exe` 14.51.36256
- 경로: `C:\Program Files\Microsoft Visual Studio\18\Community`
- 미설치/미발견: Windows SDK, CMake, Ninja, GCC/MinGW, Zig, Clang 컴파일러
  (`clang-format`/`clang-tidy`만 있고 `clang.exe`는 없음)

MSVC는 설치됐지만 Windows SDK가 없어서 `windows.h`, `kernel32.lib`,
`user32.lib`를 쓰는 일반 Win32 빌드는 현재 그대로는 실패한다. Visual Studio
Installer에서 **Windows 11 SDK** 구성 요소 하나만 추가하면 된다. 별도 빌드 시스템은
필요 없다.

## 두 게임이 공통으로 쓸 최소 빌드

각 프로젝트에서 아래처럼 Developer Command Prompt 환경을 불러온 뒤 `cl` 하나로
컴파일과 링크를 끝낸다. SDK 설치 후 사용할 명령이다.

```bat
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
cl /nologo /TC /O1 /Os /GS- /W4 src\main.c /Fe:game.exe /link /subsystem:windows /machine:x64 /incremental:no /opt:ref /opt:icf /filealign:512
```

- 공통 프레임워크, CMake, 패키지 관리자는 두지 않는다.
- 배포 빌드에는 `/O1 /Os /OPT:REF /OPT:ICF`를 유지한다.
- CRT까지 제거할 때만 소스에 자체 entry point를 두고 `/Zl`과
  `/NODEFAULTLIB /ENTRY:mainCRTStartup`을 추가한다. 처음부터 필요한 최적화는 아니다.
- `/ALIGN`은 필요 없고 잘못 쓰면 실행 불가 경고가 나므로 생략한다.

## 용량 검사 (제출 전 필수)

공모전 한도는 1,474,560바이트다. 압축 파일 크기가 아니라 최종 제출 EXE 자체를
확인하고, 빌드가 한도를 넘으면 실패시키는 가장 짧은 PowerShell 검사는 다음과 같다.

```powershell
$limit = 1474560
$bytes = (Get-Item -LiteralPath .\game.exe).Length
"$bytes / $limit bytes"
if ($bytes -gt $limit) { throw "game.exe exceeds the 1.44 MB limit" }
```

형식 확인:

```bat
dumpbin /headers game.exe | findstr /i "machine subsystem"
```

`machine (x64)`와 `subsystem (Windows GUI)`가 보여야 한다.

## 실제 프로브 결과

`C:\Dev\1.44\toolchain_probe`에서 SDK 없이도 검증할 수 있도록 Win32 함수 선언과
두 개의 최소 `.def` import library를 직접 만들었다. 이 방식으로 빌드한
`hello.exe`가 실제 메시지 창을 표시하고 정상적으로 닫히는 것을 확인했다.

- 형식: PE32+ / x64 / Windows GUI
- 크기: **3,072바이트**
- file alignment: 512바이트

이 `.def` 우회는 툴체인 확인용일 뿐이다. 게임용 Win32 API 선언을 직접 유지하지
말고 Windows SDK를 설치해 표준 헤더와 import library를 사용한다.
