#include "Player.h"
#include <cmath>

void Player::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = scale_;
	worldTransform_.translation_ = {-8.0f, -2.5f, 0.0f};
	worldTransform_.UpdateMatrix();
	velocity_ = {0.0f, 0.0f, 0.0f};
	isGrounded_ = false;
}

void Player::SetPosition(const Vector3& position) {
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
}

void Player::Update(Input* input) {
	float moveSpeed = 0.25f;
	velocity_.x = 0.0f;

	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_A)) {
		velocity_.x -= moveSpeed;
	}
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_D)) {
		velocity_.x += moveSpeed;
	}

	// 重力加算
	velocity_.y += kGravity_;

	// ジャンプ処理
	if (isGrounded_) {
		if (input->TriggerKey(DIK_SPACE) || input->TriggerKey(DIK_UP) || input->PushKey(DIK_W)) {
			velocity_.y = kJumpVelocity;
			isGrounded_ = false;
		}
	}

	// 各フレームの最初に判定前リセット
	isGrounded_ = false;
}

void Player::ResolveCollisionX(const WorldTransform& blockTransform, const Vector3& blockSize, float moveX) {
	constexpr float kEpsilon = 0.1f;

	float playerHalfX = size_.x * 0.5f;
	float playerHalfY = size_.y * 0.5f;
	float blockHalfX = blockSize.x * 0.5f;
	float blockHalfY = blockSize.y * 0.5f;

	float playerMinX = worldTransform_.translation_.x - playerHalfX;
	float playerMaxX = worldTransform_.translation_.x + playerHalfX;
	float playerMinY = worldTransform_.translation_.y - playerHalfY;
	float playerMaxY = worldTransform_.translation_.y + playerHalfY;

	float blockMinX = blockTransform.translation_.x - blockHalfX;
	float blockMaxX = blockTransform.translation_.x + blockHalfX;
	float blockMinY = blockTransform.translation_.y - blockHalfY;
	float blockMaxY = blockTransform.translation_.y + blockHalfY;

	if (playerMaxY <= blockMinY || playerMinY >= blockMaxY)
		return;
	if (playerMaxX <= blockMinX || playerMinX >= blockMaxX)
		return;

	if (moveX > 0.0f) {
		worldTransform_.translation_.x = blockMinX - playerHalfX - kEpsilon;
	} else if (moveX < 0.0f) {
		worldTransform_.translation_.x = blockMaxX + playerHalfX + kEpsilon;
	}
	worldTransform_.UpdateMatrix();
}

void Player::ResolveCollisionY(const WorldTransform& blockTransform, const Vector3& blockSize, float moveY) {
	constexpr float kEpsilon = 0.001f;

	float playerHalfX = size_.x * 0.5f;
	float playerHalfY = size_.y * 0.5f;
	float blockHalfX = blockSize.x * 0.5f;
	float blockHalfY = blockSize.y * 0.5f;

	float playerMinX = worldTransform_.translation_.x - playerHalfX;
	float playerMaxX = worldTransform_.translation_.x + playerHalfX;
	float playerMinY = worldTransform_.translation_.y - playerHalfY;
	float playerMaxY = worldTransform_.translation_.y + playerHalfY;

	float blockMinX = blockTransform.translation_.x - blockHalfX;
	float blockMaxX = blockTransform.translation_.x + blockHalfX;
	float blockMinY = blockTransform.translation_.y - blockHalfY;
	float blockMaxY = blockTransform.translation_.y + blockHalfY;

	if (playerMaxX <= blockMinX || playerMinX >= blockMaxX)
		return;
	if (playerMaxY <= blockMinY || playerMinY >= blockMaxY)
		return;

	if (moveY > 0.0f) {
		worldTransform_.translation_.y = blockMinY - playerHalfY - kEpsilon;
		velocity_.y = 0.0f;
	} else if (moveY < 0.0f) {
		worldTransform_.translation_.y = blockMaxY + playerHalfY + kEpsilon;
		velocity_.y = 0.0f;
		isGrounded_ = true;
	}
	worldTransform_.UpdateMatrix();
}

void Player::Draw(Camera& camera) {
	if (model_) {
		model_->Draw(worldTransform_, camera);
	}
}