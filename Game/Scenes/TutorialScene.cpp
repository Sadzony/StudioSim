﻿#include "TutorialScene.h"

#include "Core/SceneManager.h"
#include "Core/Components/Camera.h"
#include "Core/Components/TextRenderer.h"
#include "Core/Grid/GridSystem.h"
#include "Core/Renderer/Renderer.h"
#include "Util/Events/Events.h"
#include "../Components/TurnManager.h"
#include "../Tiles/TestTile.h"
#include "../Tiles/LightTile.h"
#include "../Components/Player/PlayerController.h"
#include "../Components/EnemyComponent.h"
#include "../Components/UI/HUD.h"
#include "Core/Components/Transform.h"
#include "../Tiles/SpikeTile.h"
#include "../LootTable.h"
#include "../Components/UI/InventoryHUD.h"
#include "../Tiles/ChestTile.h"
#include "Core/AudioEngine.h"
#include "Core/Components/AnimatedSpriteRenderer.h"
#include "../Tiles/ShopTile.h"
#include "../Components/Items/Armour/LegendaryArmour.h"
#include "../Components/Items/Armour/CommonArmour.h"
#include "../Components/Items/Weapons/LegendaryHammer.h"

void TutorialScene::createSlime(const glm::vec2 pos)
{
	const glm::vec2 tileWorldSpace = GridSystem::Instance()->getWorldPosition(pos);
	int random = rand() % 10000000;
		
	auto* enemy = SceneManager::Instance()->createGameObject("TestEnemy-" + std::to_string(random), tileWorldSpace);
	enemy->getTransform()->setSize(glm::vec2(48, 24));

	const std::vector textureList = ResourceManager::GetTexturesContaining("Blue-Slime-Idle");
	auto sprite = enemy->addComponent<AnimatedSpriteRenderer>(textureList, 0.05f);
	sprite->setColor(glm::vec3(1, 1, 1));
	sprite->setLit(true);
	sprite->setPivot(Pivot::Center);
	
	StateMachine* fsm = enemy->addComponent<NormalEnemyFSM>();
	EnemyStats slimeStats = EnemyStats();
	slimeStats.attack = 0;
	slimeStats.critChance = 0.0f;
	slimeStats.maxHealth = 5;
	slimeStats.currentHealth = slimeStats.maxHealth;
	slimeStats.defence = 0;
	slimeStats.deathEnemyList = ResourceManager::GetTexturesContaining("Blue-Slime-Death");
	
	EnemyComponent component = EnemyComponent(fsm, slimeStats);
	enemy->addComponent<EnemyComponent>(component);

	GridSystem::Instance()->setSatOnTile(0, pos, enemy);
}

void TutorialScene::init()
{
	engineRenderID = Griddy::Events::subscribe(this, &TutorialScene::onEngineRender);
	

	LootTable::Instance()->LoadingIntoLootTableArray();
	EnemyDropLootTable::Instance()->EnemyDropLoadingIntoLootTableArray();

	auto backgroundSortingLayer = Renderer::addSortingLayer("Background Grid", -1);
	auto middleSortingLayer = Renderer::addSortingLayer("Middle Grid", 0);
	auto enemySortingLayer = Renderer::addSortingLayer("Top Grid", 1);

	GridSystem* grid_system = GridSystem::Instance();
	grid_system->init(glm::vec2(48, 48), glm::vec2(70, 70));
	grid_system->setOrderMap(
	{
		{0, backgroundSortingLayer},
		{1, middleSortingLayer},
		{2, enemySortingLayer},
	});
	
	grid_system->setEmptyTileIDs(0, std::vector<int>{0});
	grid_system->setWallIDs(0, std::vector<int>{1,9,3,4,5,6});
	grid_system->setTextureMap(0, std::map<int, Texture>
	{
		{ 1, ResourceManager::GetTexture("tile25")},
		{ 2, ResourceManager::GetTexture("tile218")},
		{ 3, ResourceManager::GetTexture("tile2")},
		{ 4, ResourceManager::GetTexture("tile4") },
		{ 5, ResourceManager::GetTexture("tile50") },
		{ 6, ResourceManager::GetTexture("tile28") },
		{ 7, ResourceManager::GetTexture("tile51") },
		{ 8, ResourceManager::GetTexture("tile204") },
		{ 9, ResourceManager::GetTexture("tile26") },
		{ 10, ResourceManager::GetTexture("tile57") }, //Stairs. 57 is lattice
		{ 11, ResourceManager::GetTexture("tile242") },
		{ 19, ResourceManager::GetTexture("tile218") },
		{ 56, ResourceManager::GetTexture("tile218") } // Spike
	});
	std::vector<glm::vec2> bossEntranceTiles;
	bossEntranceTiles.push_back(glm::vec2(31, 23));
	bossEntranceTiles.push_back(glm::vec2(31, 22));
	bossEntranceTiles.push_back(glm::vec2(31, 21));
	grid_system->setTileFunctionMap(0, std::map<int, std::function<Tile*()>>
	{
		{ 10, [] { return new TestTile(Texture(), "level1", true); } },
		{ 56, [] { return new SpikeTile(Texture()); } }
	});
	
	grid_system->loadFromFile(0, "Grid/TutorialLevelDesign.txt");
	
	grid_system->setEmptyTileIDs(1, std::vector<int>{});
	grid_system->setWallIDs(1, std::vector<int>{35, 36, 41, 42, 43, 44, 31, 32, 33});
	grid_system->setTextureMap(1, std::map<int, Texture>
	{
		{ 21, ResourceManager::GetTexture("tile12")},//tile 12 above tile 36 // tile 11 above 35 // tile 13 above 37
		{ 22, ResourceManager::GetTexture("tile36")},//tile111 skulls
		{ 23, ResourceManager::GetTexture("tile11")},//tile223 rocks
		{ 24, ResourceManager::GetTexture("tile35")}, //Pillars : 20 44 68, 21 45 69, 22 46 70,
		{ 25, ResourceManager::GetTexture("tile13")}, //104 105
		{ 26, ResourceManager::GetTexture("tile37")}, //stair case/window
		{ 27, ResourceManager::GetTexture("tile111")}, //skulls
		{ 28, ResourceManager::GetTexture("tile223")}, //rocks
		{ 29, ResourceManager::GetTexture("tile20")},
		{ 30, ResourceManager::GetTexture("tile44")},
		{ 31, ResourceManager::GetTexture("tile68")},
		{ 32, ResourceManager::GetTexture("tile129")},	//vases
		{ 33, ResourceManager::GetTexture("tile153")},	//vases
		{ 34, ResourceManager::GetTexture("tile57") }, //lattice
		{ 35, ResourceManager::GetTexture("tile127") },//barrels
		{ 36, ResourceManager::GetTexture("tile151") },
		{ 37, ResourceManager::GetTexture("tile131") },
		{ 38, ResourceManager::GetTexture("tile155") },
		{ 39, ResourceManager::GetTexture("tile288") },
		{ 40, ResourceManager::GetTexture("tile289") },
		{ 41, ResourceManager::GetTexture("tile132") },
		{ 42, ResourceManager::GetTexture("tile133") },
		{ 43, ResourceManager::GetTexture("tile156") },
		{ 44, ResourceManager::GetTexture("tile157") },
		{ 45, ResourceManager::GetTexture("tile290") }, //skulls
		{ 46, ResourceManager::GetTexture("tile291") }, //skulls
		{ 47, ResourceManager::GetTexture("tile73") }, //window with bars
		{ 48, ResourceManager::GetTexture("tile130") },//Jar
		{ 49, ResourceManager::GetTexture("tile154") },
		{ 56, ResourceManager::GetTexture("tile60") }, // Spike
		{ 93, ResourceManager::GetTexture("chest_2") }, //Chest
	});
	
	grid_system->setTileFunctionMap(1, std::map<int, std::function<Tile*()>>
	{
		{ 37, [] { return new LightTile(Texture()); } },
		{ 56, [] { return new SpikeTile(Texture()); } },
		{ 93, [] { return new ChestTile(Texture()); } }
	});
	
	grid_system->loadFromFile(1, "Grid/TutorialLevelDesignDetail.txt");

	grid_system->setEmptyTileIDs(2, std::vector<int>{});
	grid_system->setWallIDs(2, std::vector<int>{});
	grid_system->setSpawnFunctionMap(2,
	{
		{ 91, [this](glm::vec2 pos)
		{
			PlayerController::Instance()->createPlayer();
			PlayerController::Instance()->playerPTR->getTransform()->setPosition(GridSystem::Instance()->getWorldPosition(pos));
		} },
		{ 92, [this](glm::vec2 pos)
		{
			createSlime(pos);
		} },
		{ 98, [this](glm::vec2 pos)
		{
			//Crab Anim
			auto* Crab = SceneManager::Instance()->createGameObject("Crab", pos * GridSystem::Instance()->getTileSize());
			Crab->getTransform()->setSize(glm::vec2(256, 256));

			const std::vector textureListCrab = ResourceManager::GetTexturesContaining("crab");
			auto sprite = Crab->addComponent<AnimatedSpriteRenderer>(textureListCrab, 0.075f);
			sprite->setColor(glm::vec3(1, 1, 1));
			sprite->setLit(false);
		} }
	});
	
	grid_system->loadFromFile(2, "Grid/TutorialLevelDesignSP.txt");
	TurnManager::Instance()->startTurnSystem();

	AudioEngine::Instance()->playSound("Sounds\\MainTheme.wav", false, 0.1f, PlayerController::Instance()->playerPTR->getTransform()->getPosition().x,
		PlayerController::Instance()->playerPTR->getTransform()->getPosition().y, AudioType::BackgroundMusic);
}

void TutorialScene::update()
{
	if (hasCompletedTutorialLevel)
	{
		SceneManager::Instance()->changeScene("level1");
	}
	
	if (GridSystem::Instance()->isLoaded() && PlayerController::Instance()->playerPTR != nullptr)
	{
		if (!HUD::Instance()->getHasLoaded())
			HUD::Instance()->createHUD();

		HUD::Instance()->updateHUD();
	}

	if (GridSystem::Instance()->isLoaded() && PlayerController::Instance()->playerPTR != nullptr)
	{
		if (!InventoryHUD::Instance()->getHasLoaded())
			InventoryHUD::Instance()->createHUD();

		InventoryHUD::Instance()->updateHUD();
	}
}

void TutorialScene::renderTextOutlined(const glm::vec2 worldPosition,
	const std::string& text,
	const float scale,
	const float outlineThickness,
	const glm::vec3& color,
	const glm::vec3& colorOutline)
{
	renderText(worldPosition, text, scale - outlineThickness, colorOutline);
	renderText(worldPosition, text, scale, color);
}

void TutorialScene::renderText(const glm::vec2 worldPosition, const std::string& text, const float scale, const glm::vec3& color)
{
	const glm::vec2 sizeOfText = TextRenderer::Instance()->renderTextSize(text, scale);
	TextRenderer::Instance()->renderText(text,
		worldPosition.x - Renderer::Instance()->getCameraPosScreenSpace().x - sizeOfText.x / 2,
		worldPosition.y - Renderer::Instance()->getCameraPosScreenSpace().y - sizeOfText.y / 2,
		scale,
		color,
		glm::vec2{0.5f, 0.5f}
	);
}

void TutorialScene::onEngineRender(const OnEngineRender* event)
{
	// TODO: POC
	renderTextOutlined(
		{240, 2208}, 
		"Press W-A-S-D to move the Character",
		0.4f,
		-0.0075f,
		{1, 1, 1},
		{0,0,0}
	);
	
	renderText({240, 1968}, "Press Q to change to Attack Mode", 0.5f, {1, 1, 1});
	renderText({240, 1920}, "Press E to change to Spell Mode", 0.5f, {1, 1, 1});
	renderText({240, 1872}, "You can see what mode you are in at the bottom right of your HUD", 0.4f, {1, 1, 1});
	renderText({864, 2304}, "These are spikes, when you walk over them you take damage", 0.4f, {1, 1, 1});
	renderText({1248, 2016}, "These are chests", 0.4f, {1, 1, 1});
	renderText({1632, 2160}, "You can open your inventory by clicking the bottom right Icon.", 0.4f, {1, 1, 1});
}

void TutorialScene::destroy()
{
	Griddy::Events::unsubscribe(this, &TutorialScene::onEngineRender, engineRenderID);
}
