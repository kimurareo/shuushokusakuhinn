#pragma once

#include "Effect.h"
#include "GoalItem.h"
#include "KamataEngine.h"
#include "Player.h"
#include "Stage.h"
#include <list>
#include <memory>

using namespace KamataEngine;

enum class Scene { kTitle, kGame, kClear };

class GameScene {
public:
	~GameScene();
	void Initialize();
	void Update();
	void Draw();

private:
	Scene scene_ = Scene::kTitle;
	Camera camera_;

	// 3Dモデル
	Model* modelPlayer_ = nullptr;
	Model* modelItem_ = nullptr;
	Model* modelEffect_ = nullptr;
	Model* modelBlock_ = nullptr;

	// 各オブジェクトのインスタンス（std::unique_ptrを使うと自動解放できてより安全です）
	std::unique_ptr<Player> player_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<GoalItem> goalItem_;

	// エフェクトリスト
	std::list<Effect*> effects_;

	// 内部ヘルパー
	void Reset();
	void EffectBorn(Vector3 position);
	void EffectBornTrail(Vector3 position);
	bool CheckCollision(const WorldTransform& a, const Vector3& sizeA, const WorldTransform& b, const Vector3& sizeB);
};