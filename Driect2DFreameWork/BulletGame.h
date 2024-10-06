#pragma once
#include "DX2DClasses/SceneManager.h"
#include "DX2DClasses/Vector2.h"
#include <vector>
#include <random>
#include <iostream>

namespace DX2DClasses
{
	class CImage;
	class CGameObject;
	class CBoxCollider;
	class CRectCollider;
	class CColorBrush;
	class CColorBrushPalettet;

	class BulletGame : public ISceneManager
	{
		int highScore = 0;
		int score = 0;
		bool isPlayerAlive;

		std::unique_ptr<CColorBrushPalettet> m_pColorBrushPalettet;

		std::unique_ptr<CImage> m_pPlayer;
		std::unique_ptr<CGameObject> m_pPlayerObject;
		std::unique_ptr<CBoxCollider> m_pPlayerBoxCollider;

		std::unique_ptr<CImage> m_pBulletImage;

		std::shared_ptr<CImage> m_pNumberImage;
		std::unique_ptr<CGameObject> m_pNumberObject0;
		std::unique_ptr<CGameObject> m_pNumberObject1;

		std::unique_ptr<CImage> m_pScoreImage;
		std::unique_ptr<CGameObject> m_pScoreObject;

		std::unique_ptr<CImage> m_pStartImage;
		std::unique_ptr<CGameObject> m_pStartObject;

		std::unique_ptr<CImage> m_pGameOverImage;
		std::unique_ptr<CGameObject> m_pGameOverObject;

		SVector2 m_vPos;

		std::vector<CGameObject*> m_pGameObjects;
		std::vector<CImage*> m_pImages;
		std::vector<CRectCollider*> m_pRectColliders;
	public:
		float m_fPlayerSpeed;
		BulletGame();
		~BulletGame();

		void Initialize(HWND hWnd, CDriect2DFramwork* pDX2DFramework) override; //초기화
		void Release() override; //동적객체 제거

		void Update() override; //정보변경
		void Draw() override; //객체 그리기
		void ColliderDraw();
	};
}