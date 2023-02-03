#include "ChestTile.h"
#include "../LootTable.h"
#include <string>
#include <iostream>
#include "../Engine/Core/Grid/GridSystem.h"
#include <Core/Renderer/ResourceManager.h>
#include "../Engine/Core/SceneManager.h"
#include "Core/Components/Transform.h"



bool ChestTile::canInteractWith()
{
	if (!chestOpened)
		return true;
	else
		return false;
}

void ChestTile::onInteractedWith(TileHolder* curTileHolder)
{
	
	if (chestOpened)
	{
		//Add Chest Opening and dropping item from loot table and changing it to an open chest or removing the chest.
	}
	else if (!chestOpened)
	{
		std::string itemToSpawn = LootTable::Instance()->RollLoot();
		std::cout << itemToSpawn << std::endl;
		chestOpened = true;
		ChangeTexture(curTileHolder, itemToSpawn);
		//Pass in the item(s) name to be dropped
	}
	//else do nothing
}

void ChestTile::ChangeTexture(TileHolder* curTileHolder, std::string itemToSpawn)
{
	Texture m_ItemTexture;
	curTileHolder->tile->setTexture(ResourceManager::GetTexture("chest_open_2"));
	const glm::vec2 tileWorldSpace = GridSystem::Instance()->getWorldPosition(curTileHolder->position);


	auto* Item = SceneManager::Instance()->createGameObject(itemToSpawn, tileWorldSpace);
	Item->getTransform()->setSize(glm::vec2(32, 32));
	if (itemToSpawn.contains("Axe"))
	{
		m_ItemTexture = ResourceManager::LoadTexture("Sprites/Weapons/Axe.png", itemToSpawn);
	}
	else if (itemToSpawn.contains("Sword"))
	{
		m_ItemTexture = ResourceManager::LoadTexture("Sprites/Weapons/Sword.png", itemToSpawn);
	}
	else if (itemToSpawn.contains("Dagger"))
	{
		m_ItemTexture = ResourceManager::LoadTexture("Sprites/Weapons/Dagger.png", itemToSpawn);
	}
	else if (itemToSpawn.contains("Hammer"))
	{
		m_ItemTexture = ResourceManager::LoadTexture("Sprites/Weapons/Hammer.png", itemToSpawn);
	}
	else if (itemToSpawn.contains("common"))
	{
		m_ItemTexture = ResourceManager::LoadTexture("Sprites/Armour/BasicArmourChest.png", itemToSpawn);
	}
	else if (itemToSpawn.contains("rare"))
	{
		m_ItemTexture = ResourceManager::LoadTexture("Sprites/Armour/MidArmourChest.png", itemToSpawn);
	}
	else if (itemToSpawn.contains("legendary"))
	{
		m_ItemTexture = ResourceManager::LoadTexture("Sprites/Armour/TopArmourChest.png", itemToSpawn);
	}

	Item->addComponent<SpriteComponent>();
	Item->getComponent<SpriteComponent>()->setPivot(Pivot::Center);
	Item->getComponent<SpriteComponent>()->setTexture(m_ItemTexture);
	Item->getComponent<SpriteComponent>()->setLit(false);
	Item->getComponent<SpriteComponent>()->setSortingLayer(Renderer::getSortingLayer("Background Grid"));


		//curTileHolder->position
}

/*
void Level1Scene::createEnemy(const glm::vec2 pos)
{
	const glm::vec2 tileWorldSpace = GridSystem::Instance()->getWorldPosition(pos);

	auto* enemy = SceneManager::Instance()->createGameObject("TestEnemy", tileWorldSpace);
	enemy->getTransform()->setSize(glm::vec2(48, 24));
	enemy->addComponent<EnemyTest>();

	GridSystem::Instance()->setSatOnTile(0, pos, enemy);
}

		//auto sprite = testPlayerIdle->addComponent<AnimatedSpriteRenderer>(textureListPlayer, 0.075f);
		//sprite->setColor(glm::vec3(1, 1, 1));
		//sprite->setLit(false);


		pausedSprite = new SpriteComponent();
	const Texture pause = ResourceManager::LoadTexture("Sprites\\pause.png", "pause");
	pausedSprite->setTexture(pause);
	pausedSprite->setLit(false);
*/