#include "BulletGame.h"
#include "DX2DClasses/Driect2DFramework.h"
#include "DX2DClasses/SingletonRenderTarget.h"
#include "DX2DClasses/ColorBrush.h"
#include "DX2DClasses/Vector2.h"
#include "DX2DClasses/Image.h"
#include "DX2DClasses/InputManager.h"
#include "DX2DClasses/CollisionCheck.h"
#include "DX2DClasses/ColorBrushPalettet.h"
#include "DX2DClasses/DebugHelper.h"
#include "DX2DClasses/GameObject.h"
#include "DX2DClasses/Colliders.h"
#include <conio.h>
#include <iostream>
#include <string>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


using namespace DX2DClasses;

void RemoveGameObjectIndexOf(size_t index);

BulletGame::BulletGame()
{
	//Initialize();
}

BulletGame::~BulletGame()
{
	//Release();
}

void BulletGame::Initialize(HWND hWnd, CDriect2DFramwork* pDX2DFramework)
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();

	m_pColorBrushPalettet = std::unique_ptr<CColorBrushPalettet>(new CColorBrushPalettet());
	m_pColorBrushPalettet->Initialize(pRenderTarget);

	m_fPlayerSpeed = 15;
	m_pPlayer = std::unique_ptr<CImage>(new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 6));
	m_pPlayer.get()->ManualLoadImage(hWnd, L"Images\\player%02d.png");

	m_pBulletImage = std::unique_ptr<CImage>(new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 1));
	m_pBulletImage->ManualLoadImage(hWnd, L"Images\\death%02d.png");

	m_pPlayerObject = std::unique_ptr<CGameObject>(new CGameObject());
	m_pPlayerObject->Initialize(m_pPlayer.get(), true);
	//m_pPlayerObject->GetTransform().SetScale(DX2DClasses::SVector2(0.5, 0.5));
	m_pPlayerObject->GetTransform().SetTranslate(DX2DClasses::SVector2(275, 500));

	m_pPlayerBoxCollider = std::unique_ptr<CBoxCollider>(new CBoxCollider());
	m_pPlayerBoxCollider->InitCollider(m_pPlayerObject->GetTransformPtr(), SVector2(), m_pPlayer->GetImageSize());

	m_pNumberImage = std::shared_ptr<CImage>(new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 10));
	m_pNumberImage->ManualLoadImage(hWnd, L"Images\\Number%d.png");

	m_pNumberObject0 = std::unique_ptr<CGameObject>(new CGameObject());
	m_pNumberObject0->Initialize(m_pNumberImage.get(), true);
	m_pNumberObject0->GetTransform().SetTranslate(DX2DClasses::SVector2(100, 10));

	m_pNumberObject1 = std::unique_ptr<CGameObject>(new CGameObject());
	m_pNumberObject1->Initialize(m_pNumberImage.get(), true);
	m_pNumberObject1->GetTransform().SetTranslate(DX2DClasses::SVector2(115, 10));

	m_pGameOverImage = std::unique_ptr<CImage>(new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 1));
	m_pGameOverImage->ManualLoadImage(hWnd, L"Images\\GameOver.png");

	m_pGameOverObject = std::unique_ptr<CGameObject>(new CGameObject());
	m_pGameOverObject->Initialize(m_pGameOverImage.get(), true);
	m_pGameOverObject->GetTransform().SetTranslate(DX2DClasses::SVector2(120, 225));
	m_pGameOverObject->SetActive(false);

	m_pStartImage = std::unique_ptr<CImage>(new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 1));
	m_pStartImage->ManualLoadImage(hWnd, L"Images\\Start.png");

	m_pStartObject = std::unique_ptr<CGameObject>(new CGameObject());
	m_pStartObject->Initialize(m_pStartImage.get(), true);
	m_pStartObject->GetTransform().SetTranslate(DX2DClasses::SVector2(130, 255));

	m_pScoreImage = std::unique_ptr<CImage>(new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 1));
	m_pScoreImage->ManualLoadImage(hWnd, L"Images\\Score.png");

	m_pScoreObject = std::unique_ptr<CGameObject>(new CGameObject());
	m_pScoreObject->Initialize(m_pScoreImage.get(), true);
	m_pScoreObject->GetTransform().SetTranslate(DX2DClasses::SVector2(10, 10));




	CDebugHelper::LogConsole("\n");
	CDebugHelper::LogConsole("  _____   ____  _____   _____ ______ \n");
	CDebugHelper::LogConsole(" |  __ \\ / __ \\|  __ \\ / ____|  ____|\n");
	CDebugHelper::LogConsole(" | |  | | |  | | |  | | |  __| |__   \n");
	CDebugHelper::LogConsole(" | |  | | |  | | |  | | | |_ |  __|  \n");
	CDebugHelper::LogConsole(" | |__| | |__| | |__| | |__| | |____ \n");
	CDebugHelper::LogConsole(" |_____/ \\____/|_____/ \\_____|______|\n\n");
	CDebugHelper::LogConsole("[ Press SPACEBAR to Start Game ]\n");
}

void BulletGame::Release()
{
	m_pPlayerObject->Release();
	m_pPlayerObject.release();
	m_pPlayerBoxCollider.release();
	m_pBulletImage.release();
	m_pPlayer.release();

	m_pColorBrushPalettet->Release();
	m_pColorBrushPalettet.release();

	for (auto col : m_pBulletColliders) {
		delete col;
	}
	for (auto obj : m_pBulletObjects) {
		obj->Release();
		delete obj;
	}

	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();
}

void BulletGame::Update()
{
	//DX2DClasses::CTransform playerTransform = m_pPlayerObject->GetTransform();


	if (isPlayerAlive) {
		CTransform& cTrnasform = m_pPlayerObject->GetTransform();
		SVector2 vPos = cTrnasform.GetTranslate();
		SVector2 vSize = m_pPlayerObject->GetImage()->GetImageSize();
		SVector2 vScale = cTrnasform.GetScale();
		//CDebugHelper::LogConsole("(%3.f, %3.f)\n", vPos.x, vPos.y);

		bool isRightPressed = CInputManager::GetAsyncKeyStatePress(VK_RIGHT);
		bool isLeftPressed = CInputManager::GetAsyncKeyStatePress(VK_LEFT);
		bool isUpPressed = CInputManager::GetAsyncKeyStatePress(VK_UP);
		bool isDownPressed = CInputManager::GetAsyncKeyStatePress(VK_DOWN);

		if (isRightPressed && vPos.x < 540) {
			cTrnasform.Translate(SVector2::right() * m_fPlayerSpeed);//vPlayerPos.x -= fPlayerSpeed;
		}
		if (isLeftPressed && vPos.x > 0) {
			cTrnasform.Translate(SVector2::left() * m_fPlayerSpeed);//vPlayerPos.x -= fPlayerSpeed;
		}
		if (isDownPressed && vPos.y < 500) {
			cTrnasform.Translate(SVector2::down() * m_fPlayerSpeed);//vPlayerPos.x -= fPlayerSpeed;
		}
		if (isUpPressed && vPos.y > 10) {
			cTrnasform.Translate(SVector2::up() * m_fPlayerSpeed);//vPlayerPos.x -= fPlayerSpeed;
		}

		/*
		if (CInputManager::GetAsyncKeyStatePress(VK_RIGHT))
			vPlayerPos.x += fPlayerSpeed; //vPlayerPos = vPlayerPos + SVector2::right() * fPlayerSpeed;
		if (CInputManager::GetAsyncKeyStatePress(VK_LEFT))
			vPlayerPos = vPlayerPos + SVector2::left() * fPlayerSpeed;//vPlayerPos.x -= fPlayerSpeed;
		if (CInputManager::GetAsyncKeyStatePress(VK_DOWN))
			vPlayerPos = vPlayerPos + SVector2::down() * fPlayerSpeed;//vPlayerPos.y += fPlayerSpeed;
		if (CInputManager::GetAsyncKeyStatePress(VK_UP))
			vPlayerPos.y -= fPlayerSpeed;//vPlayerPos = vPlayerPos + SVector2::up() * fPlayerSpeed;
		*/


		if (isRightPressed || isLeftPressed || isUpPressed || isDownPressed) {
			m_pPlayerObject->Update();
		}
		else {
			m_pPlayerObject->GetAnimator()->SetFrame();
		}
	}

	if (isPlayerAlive) {
		for (CGameObject* gameObject : m_pBulletObjects)
		{
			if (gameObject->GetActive())
				gameObject->GetTransform().Translate(SVector2::down() * 25);
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 550);
		if (m_pBulletObjects.size() < 25) {
			try {
				CGameObject* bulletObject = new CGameObject();
				bulletObject->Initialize(m_pBulletImage.get(), true);
				bulletObject->GetTransform().SetTranslate(DX2DClasses::SVector2(dis(gen), 0));
				CRectCollider* bulletCollider = new CRectCollider();
				bulletCollider->InitCollider(bulletObject->GetTransformPtr(), SVector2(), m_pBulletImage->GetImageSize());

				m_pBulletObjects.push_back(bulletObject);
				m_pBulletColliders.push_back(bulletCollider);
			}
			catch (const std::exception& e) {
				std::cout << "Caught an exception: " << e.what() << std::endl;
				CDebugHelper::LogConsole("Bug %s", e.what());
			}
		}
		else {
			if (bulletIndex < 25) {
				CGameObject* bulletObject = m_pBulletObjects[bulletIndex];
				bulletObject->SetActive(true);
				bulletObject->GetTransform().SetTranslate(DX2DClasses::SVector2(dis(gen), 0));

				++bulletIndex;
			}
		}

		/*
		int dist = -1;
		for (auto it = m_pGameObjects.begin(); it != m_pGameObjects.end(); ++it) {
			if ((*it)->GetTransform().GetTranslate().y > 550) {
				dist = std::distance(m_pGameObjects.begin(), it);
				break;
			}
		}*/

		for (auto go : m_pBulletObjects) {
			if (go->GetTransform().GetTranslate().y > 550) {
				go->GetTransform().SetTranslate(DX2DClasses::SVector2(dis(gen), 0));
				++score;
				CDebugHelper::LogConsole("Score: %d\n", score);

				m_pNumberObject0->GetAnimator()->SetFrame(score / 10);
				m_pNumberObject1->GetAnimator()->SetFrame(score % 10);
			}
		}
		/*
		if (dist != -1) {
			CRectCollider* rect = m_pRectColliders.at(dist);
			CGameObject* gameObject = m_pGameObjects.at(dist);
			m_pRectColliders.erase(m_pRectColliders.begin() + dist);
			m_pGameObjects.erase(m_pGameObjects.begin() + dist);
			delete rect;
			gameObject->Release();
			delete gameObject;
			dist = -1;
		}*/
	}

	if (!isPlayerAlive && CInputManager::GetAsyncKeyStatePress(VK_SPACE)) {
		m_pGameOverObject->SetActive(false);
		m_pStartObject->SetActive(false);
		CDebugHelper::LogConsole("\nGame Restarted\n");
		CDebugHelper::LogConsole("\n[ !!!SURVIVE AS LONG AS YOU CAN!!! ]\n");

		bulletIndex = 0;

		//for (CRectCollider* rect : m_pRectColliders) {
		//	delete rect;
		//}
		//m_pRectColliders.clear();
		for (CGameObject* gameObject : m_pBulletObjects) {
			gameObject->GetTransform().SetTranslate(DX2DClasses::SVector2(0, 0));
			gameObject->SetActive(false);
			//gameObject->Release();
			//delete gameObject;
		}
		m_pBulletObjects.clear();
		score = 0;
		isPlayerAlive = true;
		m_pPlayerObject->GetTransform().SetTranslate(DX2DClasses::SVector2(275, 500));

		m_pNumberObject0->GetAnimator()->SetFrame(score / 10);
		m_pNumberObject1->GetAnimator()->SetFrame(score % 10);
	}
}

void BulletGame::Draw()
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
	m_pPlayerObject->Draw();
	ColliderDraw();
	for (CGameObject* gameObject : m_pBulletObjects)
	{
		if (gameObject->GetActive())
			gameObject->Draw();
	}
	m_pNumberObject0->Draw();
	m_pNumberObject1->Draw();
	m_pGameOverObject->Draw();
	m_pScoreObject->Draw();
	m_pStartObject->Draw();
}

void BulletGame::ColliderDraw() {
	CColorBrush* pRedBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::RED);
	m_pPlayerBoxCollider->DrawOutline(pRedBrush);

	if (isPlayerAlive) {
		int dist = -1;
		for (auto it = m_pBulletColliders.begin(); it != m_pBulletColliders.end(); ++it) {
			if (m_pPlayerBoxCollider->ToRect(*it)) {
				dist = std::distance(m_pBulletColliders.begin(), it);
				break;
			}
		}

		if (dist != -1) {
			dist = -1;
			isPlayerAlive = false;
			m_pGameOverObject->SetActive(true);
			CDebugHelper::LogConsole("################# You Dead!! #################\n");
			CDebugHelper::LogConsole("[ Score : %d ]\n", score);
			if (score > highScore) {
				highScore = score;
				CDebugHelper::LogConsole("!!!!!!!!! New High Score !!!!!!!!!\n");
				CDebugHelper::LogConsole("!!!!!!!!! New High Score !!!!!!!!!\n");
				CDebugHelper::LogConsole("!!!!!!!!! New High Score !!!!!!!!!\n");
			}
			CDebugHelper::LogConsole("[ High Score : %d ]\n", highScore);
			CDebugHelper::LogConsole("[ Press SPACEBAR to Restart ]\n", score);
		}
	}
}
