#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

// エフェクト生成通知用のコールバック型関数などの代替として直接参照するか引数渡しにします
class Player {
public:
	void Initialize(Model* model);
	void Update(Input* input);
	void Draw(Camera& camera);

	// 衝突判定・押し戻し処理
	void ResolveCollisionX(const WorldTransform& blockTransform, const Vector3& blockSize, float moveX);
	void ResolveCollisionY(const WorldTransform& blockTransform, const Vector3& blockSize, float moveY);

	// Getters / Setters
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetSize() const { return size_; }
	const Vector3& GetVelocity() const { return velocity_; }
	void SetPosition(const Vector3& position);
	void SetGrounded(bool isGrounded) { isGrounded_ = isGrounded; }

	// 画面外（指定のY座標以下）に落下したか判定
	bool IsDead() const { return worldTransform_.translation_.y < kDeathY_; }


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	Vector3 size_ = {1.0f, 1.0f, 1.0f};
	Vector3 scale_ = {0.5f, 0.5f, 0.5f};
	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};

	bool isGrounded_ = false;

	static inline const float kGravity_ = -0.01f;
	static inline const float kJumpVelocity = 0.3f;

	// 死亡判定となるY座標（ステージの最下層に合わせて調整してください）
	static inline const float kDeathY_ = -10.0f;

};