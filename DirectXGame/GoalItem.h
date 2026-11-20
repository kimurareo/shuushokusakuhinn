#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class GoalItem {
public:
	void Initialize(Model* model);
	void Reset();
	void Draw(Camera& camera);

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetSize() const { return size_; }
	bool IsActive() const { return isActive_; }
	void SetActive(bool active) { isActive_ = active; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Vector3 size_ = {1.0f, 1.0f, 1.0f};
	Vector3 scale_ = {1.0f, 1.0f, 1.0f};
	bool isActive_ = true;
};