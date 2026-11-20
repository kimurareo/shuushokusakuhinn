#include "Stage.h"

Stage::~Stage() {
	for (WorldTransform* block : blockTransforms_) {
		delete block;
	}
	blockTransforms_.clear();
}

void Stage::Initialize(Model* model) {
	modelBlock_ = model;
	Generate();
}

void Stage::Generate() {
	for (WorldTransform* block : blockTransforms_) {
		delete block;
	}
	blockTransforms_.clear();

	const int map[6][40] = {
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	float startX = -10.0f;
	float startY = 4.0f;

	for (int y = 0; y < 6; ++y) {
		for (int x = 0; x < 40; ++x) {
			if (map[y][x] == 1) {
				WorldTransform* block = new WorldTransform();
				block->Initialize();
				block->translation_ = {startX + x * 2.0f, startY - y * 2.0f, 0.0f};
				block->scale_ = blockScale_;
				block->UpdateMatrix();
				blockTransforms_.push_back(block);
			}
		}
	}
}

void Stage::CheckCollision(Player* player, const Vector3& subMove) {
	if (subMove.x != 0.0f) {
		Vector3 pos = player->GetWorldTransform().translation_;
		pos.x += subMove.x;
		player->SetPosition(pos);

		for (WorldTransform* block : blockTransforms_) {
			player->ResolveCollisionX(*block, blockSize_, subMove.x);
		}
	}

	if (subMove.y != 0.0f) {
		Vector3 pos = player->GetWorldTransform().translation_;
		pos.y += subMove.y;
		player->SetPosition(pos);

		for (WorldTransform* block : blockTransforms_) {
			player->ResolveCollisionY(*block, blockSize_, subMove.y);
		}
	}
}

void Stage::Draw(Camera& camera) {
	if (modelBlock_) {
		for (WorldTransform* block : blockTransforms_) {
			modelBlock_->Draw(*block, camera);
		}
	}
}
