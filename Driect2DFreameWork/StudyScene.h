#pragma once
#include "DX2DClasses/SceneManager.h"
#include "DX2DClasses/Vector2.h"
#include <vector>
#include <random>

namespace DX2DClasses
{
	class CImage;
	class CGameObject;
	class CBoxCollider;
	class CRectCollider;
	class CColorBrush;
	class CColorBrushPalettet;

	class StudyScene : public ISceneManager
	{
		int highScore = 0;
		int score = 0;
		bool isPlayerAlive;

		CColorBrushPalettet* m_pColorBrushPalettet;

		CImage* m_pGround;
		CGameObject* m_pGroundObject;
		CBoxCollider* m_pGroundBoxCollider;
		CRectCollider* m_pGroundRectCollider;

		CImage* m_pPlayer;
		CGameObject* m_pPlayerObject;
		CBoxCollider* m_pPlayerBoxCollider;

		CImage* m_pBulletImage;
		CGameObject* m_pBulletObject;
		CRectCollider* m_pBulletCollider;

		CImage* m_pNumberImage;
		CGameObject* m_pNumberObject0;
		CGameObject* m_pNumberObject1;

		CImage* m_pScoreImage;
		CGameObject* m_pScoreObject;

		CImage* m_pStartImage;
		CGameObject* m_pStartObject;

		CImage* m_pGameOverImage;
		CGameObject* m_pGameOverObject;

		SVector2 m_vPos;

		std::vector<CGameObject*> m_pGameObjects;
		std::vector<CImage*> m_pImages;
		std::vector<CRectCollider*> m_pRectColliders;
	public:
		float m_fPlayerSpeed;
		StudyScene();
		~StudyScene();

		void Initialize(HWND hWnd, CDriect2DFramwork* pDX2DFramework) override; //초기화
		void Release() override; //동적객체 제거

		void Update() override; //정보변경
		void Draw() override; //객체 그리기
		void ColliderDraw();
	};
}