#include "GameScene.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace KamataEngine;

GameScene::~GameScene() {
	delete modelPlayer_;
	delete modelItem_;
	delete modelEffect_;
	delete modelBlock_;

	for (Effect* effect : effects_) {
		delete effect;
	}
	effects_.clear();
}

void GameScene::Initialize() {
	srand(static_cast<unsigned int>(time(NULL)));

	// モデル生成
	modelPlayer_ = Model::CreateFromOBJ("cube");
	modelItem_ = Model::CreateFromOBJ("cube");
	modelEffect_ = Model::CreateFromOBJ("plane");
	modelBlock_ = Model::CreateFromOBJ("cube");

	camera_.Initialize();

	// 各クラスの初期化
	player_ = std::make_unique<Player>();
	stage_ = std::make_unique<Stage>();
	goalItem_ = std::make_unique<GoalItem>();

	stage_->Initialize(modelBlock_);
	player_->Initialize(modelPlayer_);
	goalItem_->Initialize(modelItem_);

	Reset();
}

void GameScene::Reset() {
	player_->Initialize(modelPlayer_);
	goalItem_->Reset();

	for (Effect* effect : effects_) {
		delete effect;
	}
	effects_.clear();
}

void GameScene::Update() {
	Input* input = Input::GetInstance();

	switch (scene_) {
	case Scene::kTitle:
		if (input->TriggerKey(DIK_SPACE)) {
			Reset();
			scene_ = Scene::kGame;
		}
		break;

	case Scene::kGame: {
		// プレイヤー入力・速度計算
		player_->Update(input);

		// サブステップ処理と衝突判定
		const int kSubSteps = 4;
		Vector3 vel = player_->GetVelocity();
		Vector3 subMove = {vel.x / static_cast<float>(kSubSteps), vel.y / static_cast<float>(kSubSteps), 0.0f};

		for (int i = 0; i < kSubSteps; ++i) {
			stage_->CheckCollision(player_.get(), subMove);
		}

		// ----------------------------------------------------
		// 落下による死亡判定
		// ----------------------------------------------------
		if (player_->IsDead()) {
			// 落下時の演出を入れたい場合はここにエフェクト処理等を追加
			// EffectBorn(player_->GetWorldTransform().translation_);

			// ゲーム状態を初期位置にリセット
			Reset();
			break;
		}



		// 軌跡エフェクト
		if (std::abs(vel.x) > 0.0f) {
			EffectBornTrail(player_->GetWorldTransform().translation_);
		}

		// ゴール判定
		if (goalItem_->IsActive()) {
			if (CheckCollision(player_->GetWorldTransform(), player_->GetSize(), goalItem_->GetWorldTransform(), goalItem_->GetSize())) {
				EffectBorn(goalItem_->GetWorldTransform().translation_);
				goalItem_->SetActive(false);
				scene_ = Scene::kClear;
			}
		}
		break;
	}

	case Scene::kClear:
		if (input->TriggerKey(DIK_SPACE)) {
			scene_ = Scene::kTitle;
		}
		break;
	}

	// カメラの追従
	Vector3 playerPos = player_->GetWorldTransform().translation_;
	camera_.translation_ = {playerPos.x, playerPos.y + 3.0f, playerPos.z - 25.0f};
	camera_.UpdateMatrix();

	// エフェクトの更新と削除
	for (Effect* effect : effects_) {
		effect->Update();
	}

	effects_.remove_if([](Effect* effect) {
		if (effect->IsFinished()) {
			delete effect;
			return true;
		}
		return false;
	});
}

void GameScene::Draw() {
	Model::PreDraw();

	if (scene_ == Scene::kGame || scene_ == Scene::kClear) {
		player_->Draw(camera_);
		goalItem_->Draw(camera_);
		stage_->Draw(camera_);
	}

	for (Effect* effect : effects_) {
		effect->Draw(camera_);
	}

	Model::PostDraw();
}

void GameScene::EffectBorn(Vector3 position) {
	if (!modelEffect_)
		return;
	const int kParticleCount = 30;
	for (int i = 0; i < kParticleCount; ++i) {
		Effect* newEffect = new Effect();
		Vector3 color = {static_cast<float>(rand() % 100) / 100.0f, static_cast<float>(rand() % 100) / 100.0f, static_cast<float>(rand() % 100) / 100.0f};
		newEffect->Initialize(modelEffect_, 0.0f, 0.5f, position, color);
		effects_.push_back(newEffect);
	}
}

void GameScene::EffectBornTrail(Vector3 position) {
	if (!modelEffect_)
		return;
	Effect* newEffect = new Effect();
	newEffect->Initialize(modelEffect_, 0.0f, 0.5f, position, {0.8f, 0.8f, 1.0f});
	effects_.push_back(newEffect);
}

bool GameScene::CheckCollision(const WorldTransform& a, const Vector3& sizeA, const WorldTransform& b, const Vector3& sizeB) {
	Vector3 posA = a.translation_;
	Vector3 posB = b.translation_;

	return (std::abs(posA.x - posB.x) < (sizeA.x + sizeB.x) * 0.5f && std::abs(posA.y - posB.y) < (sizeA.y + sizeB.y) * 0.5f && std::abs(posA.z - posB.z) < (sizeA.z + sizeB.z) * 0.5f);
}