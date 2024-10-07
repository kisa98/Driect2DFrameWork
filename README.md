# DirectX 2D Framework를 활용한 총알 피하기 게임

![MainImage2](https://github.com/kisa98/Driect2DFrameWork/blob/master/Images/2.png?raw=true)

## 게임 소개
![MainImage](https://github.com/kisa98/Driect2DFrameWork/blob/master/Images/1.png?raw=true)  
키보드 스페이스 바 키를 눌러 게임을 시작합니다.  
![3](https://github.com/kisa98/Driect2DFrameWork/blob/master/Images/3.png?raw=true)  
하늘에서 떨어지는 검은 총알을 피하는 게임입니다.  
![5](https://github.com/kisa98/Driect2DFrameWork/blob/master/Images/5.png?raw=true)  
키보드 좌우 화살표( ← → ) 키를 이용하여 이동하여 캐릭터를 움직입니다.  
![4](https://github.com/kisa98/Driect2DFrameWork/blob/master/Images/4.png?raw=true)  
캐릭터의 충돌 박스가 총알의 충돌 박스에 부딪히면 게임 오버되며 스페이스 바 키를 눌러 게임을 재시작 할 수 있습니다.  
![6](https://github.com/kisa98/Driect2DFrameWork/blob/master/Images/6.png?raw=true)  
좌상단에 점수가 표시됩니다.  
계속된 도전을 통해 최고점에 도전해보세요!

## 프로젝트 목적
- DirectX 코드 체험
- WINAPI의 콜백 함수 구조를 이해하며 UNITY 이벤트 함수 싸이클에 대해 간접적으로 이해
- C++ 프로그래밍 기술 향상

## 프로젝트 구조
> main.cpp  
> ISceneManager.h (Scene Interface)  
> BulletGame.h BulletGame.cpp  
> DX2DClasses (Directory)

## 코드
1. Window API의 콜백 함수 구조를 이용한 게임 이벤트 시스템  
> UNITY 이벤트 함수와 유사한 역할을 하는 함수들이 아래의 Window API의 메시지 처리 콜백 함수에서 작동함.
```C++
main.cpp
void Initialize(HWND hWnd); //초기화 (UNITY의 Start()에 대응)
void Update();              //프레임별 업데이트 (Update()에 대응)
void Release();             //씬 파괴 (씬이 파괴될 때 호출하며, 각 객체에 포함된 OnDestroy()를 호출하여 메모리를 해제함)
void Draw();                //그래픽 렌더링
```

```C++
main.cpp
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  ...
  case WM_CREATE:
    Initialize(hWnd);
    break;
  case WM_PAINT:
    Update();
    hdc = BeginPaint(hWnd, &ps);
    Draw();
  ...
}
```


      
2. 메모리 관리 (스마트 포인터 사용, 누수 탐지)
> 동적 할당하는 객체를 안전하게 관리하기위해 C++의 STL인 스마트포인터를 활용함.
```C++
BulletGame.cpp
...
//단 하나만 존재하는 플레이어 게임오브젝트를 Unique Pointer로 관리
m_pPlayerObject = std::unique_ptr<CGameObject>(new CGameObject());  
m_pPlayerObject->Initialize(m_pPlayer.get(), true);

...
//점수 숫자 이미지를 Shared Pointer로 관리
m_pNumberImage = std::shared_ptr<CImage>(new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 10)); 
m_pNumberImage->ManualLoadImage(hWnd, L"Images\\Number%d.png");
```
   
> 메모리 누수 감지 라이브러리를 이용한 메모리 누수 탐지
```C++
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
```
```
Detected memory leaks!
Dumping objects ->
{201} normal block at 0x04364838, 76 bytes long.
 Data: <x   @           > 78 F9 1B 04 40 F5 1B 04 18 F6 1B 04 E0 FA 1B 04 
{200} normal block at 0x04364220, 76 bytes long.
 Data: <( 7   7   7 H 7 > 28 E8 37 04 B8 E4 37 04 18 E6 37 04 48 E1 37 04
```
![7](https://github.com/kisa98/Driect2DFrameWork/blob/master/Images/7.png?raw=true)
