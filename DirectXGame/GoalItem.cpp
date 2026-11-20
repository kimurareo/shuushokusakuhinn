#include "GoalItem.h"

void GoalItem::Initialize(Model* model) {
	model_ = model;
	Reset();
}

void GoalItem::Reset() {
	worldTransform_.Initialize();
	worldTransform_.translation_ = {60.0f, -2.0f, 0.0f};
	worldTransform_.scale_ = scale_;
	worldTransform_.UpdateMatrix();
	isActive_ = true;
}

void GoalItem::Draw(Camera& camera) {
	if (isActive_ && model_) {
		model_->Draw(worldTransform_, camera);
	}
}