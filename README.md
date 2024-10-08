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
   
> C++의 메모리 누수 감지 라이브러리를 이용한 메모리 누수 탐지
```C++

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

...
_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //메모리 누수 검사
...
_CrtDumpMemoryLeaks();
```

> 누수된 메모리가 발생한 부분을 탐지. 메모리 블럭이 콘솔에 출력됨.
```
Detected memory leaks!
Dumping objects ->
{201} normal block at 0x04364838, 76 bytes long.
 Data: <x   @           > 78 F9 1B 04 40 F5 1B 04 18 F6 1B 04 E0 FA 1B 04 
{200} normal block at 0x04364220, 76 bytes long.
 Data: <( 7   7   7 H 7 > 28 E8 37 04 B8 E4 37 04 18 E6 37 04 48 E1 37 04
```  
> 누수된 메모리가 발생한 부분에 중단점을 발생시켜 누수 발생 위치를 탐지함  
![7](https://github.com/kisa98/Driect2DFrameWork/blob/master/Images/7.png?raw=true)

3. 메모리 풀링
> 총알을 생성하고 소멸시키는 과정을 최적화하기 위해 리스트에 넣어 재활용함
```C++
if (m_pBulletObjects.size() < 25) {  //총알 25개까지
	try {
		CGameObject* bulletObject = new CGameObject();  //새로운 총알 오브젝트 생성
		bulletObject->Initialize(m_pBulletImage.get(), true);
		bulletObject->GetTransform().SetTranslate(DX2DClasses::SVector2(dis(gen), 0));
		CRectCollider* bulletCollider = new CRectCollider();
		bulletCollider->InitCollider(bulletObject->GetTransformPtr(), SVector2(), m_pBulletImage->GetImageSize());

		m_pBulletObjects.push_back(bulletObject);  //총알 오브젝트를 리스트에 푸시
		m_pBulletColliders.push_back(bulletCollider);
	}
	catch (const std::exception& e) {
		std::cout << "Caught an exception: " << e.what() << std::endl;
		CDebugHelper::LogConsole("Bug %s", e.what());
	}
}
else {  //총알이 충분히 생성되었다면
	if (bulletIndex < 25) {
		CGameObject* bulletObject = m_pBulletObjects[bulletIndex];
		bulletObject->SetActive(true);  //총알을 재활용
		bulletObject->GetTransform().SetTranslate(DX2DClasses::SVector2(dis(gen), 0));

		++bulletIndex;
	}
}
```

```C++
for (CGameObject* gameObject : m_pBulletObjects) {
  //기존의 총알 오브젝트를 파괴하고 재생성하는 로직을 수정
	//gameObject->Release();
	//delete gameObject;
  //위치를 초기화하고 비활성화시켜 재활용할 준비
	gameObject->GetTransform().SetTranslate(DX2DClasses::SVector2(0, 0));
	gameObject->SetActive(false);
}
```
