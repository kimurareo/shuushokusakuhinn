#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>

using namespace KamataEngine;

class Stage {
public:
	~Stage();
	void Initialize(Model* model);
	void Generate();
	void Draw(Camera& camera);

	// プレイヤーとのマップ衝突チェック＆解決
	void CheckCollision(Player* player, const Vector3& subMove);

private:
	Model* modelBlock_ = nullptr;
	std::vector<WorldTransform*> blockTransforms_;
	Vector3 blockSize_ = {2.0f, 2.0f, 2.0f};
	Vector3 blockScale_ = {1.0f, 1.0f, 1.0f};
};