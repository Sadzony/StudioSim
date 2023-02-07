﻿#include "InventoryHUD.h"

#include "ButtonComponent.h"
#include "InventoryButton.h"
#include "InventoryIconButton.h"
#include "UIManager.h"
#include "../Player/PlayerController.h"
#include "Core/Components/AnimatedSpriteRenderer.h"
#include "Core/Components/TextRenderer.h"
#include "Core/Components/Transform.h"
#include "Core/Renderer/ResourceManager.h"
#include "Util/Events/Events.h"

void InventoryHUD::createHUD()
{
	if (hasLoaded)
		return;

	if (sceneChangeID == -1)
		sceneChangeID = Griddy::Events::subscribe(this, &InventoryHUD::onSceneChange);

	ResourceManager::LoadTexture("Sprites/UI/Background.png", "inventoryBackground");
	
	backgroundPanelInventory = UIManager::Instance()->createUIElement<Panel>("backgroundPanelInventory");
	backgroundPanelEquip = UIManager::Instance()->createUIElement<Panel>("backgroundPanelEquip");
	backgroundPanelInventoryText = UIManager::Instance()->createUIElement<TextComponent>("backgroundPanelInventoryText");
	hasLoaded = true;
}

ButtonComponent* InventoryHUD::createButton(const glm::vec2& pos, Item* item)
{
	ButtonComponent* button = UIManager::Instance()->createUIElement<InventoryIconButton>(
		"Inventory_" + std::to_string(pos.x) + " " + std::to_string(pos.y),
		ResourceManager::GetTexture("inventoryBackground"),
		item);
	button->getTransform()->setPosition(pos);
	button->setColor({1, 0, 0});
	
	return button;
}

void InventoryHUD::updateHUD()
{
	if (!hasLoaded)
		return;

	const auto topLeft =
					glm::vec2(0, Renderer::getWindowSize().y) / Renderer::Instance()->getAspectRatio();

	const auto MiddleLeft =
					glm::vec2(0, Renderer::getWindowSize().y / 2) / Renderer::Instance()->getAspectRatio();

	const auto MiddleRight =
				glm::vec2(Renderer::getWindowSize().x, Renderer::getWindowSize().y / 2) / Renderer::Instance()->getAspectRatio();

	const auto topRight =
					glm::vec2(Renderer::getWindowSize().x, Renderer::getWindowSize().y) / Renderer::Instance()->getAspectRatio();

	const auto bottomRight =
					glm::vec2(Renderer::getWindowSize().x, 0) / Renderer::Instance()->getAspectRatio();

	const auto topMiddle =
					glm::vec2((Renderer::getWindowSize().x / 2), Renderer::getWindowSize().y) / Renderer::Instance()->getAspectRatio();
                    
	const auto bottomLeft = 
					glm::vec2(0, 0) / Renderer::Instance()->getAspectRatio();

	// =============================== BACKGROUND PANEL EQUIP ===============================
	backgroundPanelEquip->shouldRender = shouldRender;
	backgroundPanelEquip->getTransform()->setPosition(MiddleRight + glm::vec2(0, 150));
	backgroundPanelEquip->getTransform()->setSize({350, 150});
	backgroundPanelEquip->setTexture(ResourceManager::GetTexture("inventoryBackground"));
	backgroundPanelEquip->setPivot(Pivot::RightCenter);
	backgroundPanelEquip->setColor({1, 0.85f, 0.88f});
	
	// =============================== BACKGROUND PANEL ===============================
	backgroundPanelInventory->shouldRender = shouldRender;
	backgroundPanelInventory->getTransform()->setPosition(MiddleRight + glm::vec2(0, -50));
	backgroundPanelInventory->getTransform()->setSize({350, 250});
	backgroundPanelInventory->setTexture(ResourceManager::GetTexture("inventoryBackground"));
	backgroundPanelInventory->setPivot(Pivot::RightCenter);
	backgroundPanelInventory->setColor({0.81f, 0.85f, 0.88f});
	
	// =============================== BACKGROUND PANEL INVENTORY TEXT ===============================
	const glm::vec2 sizeOfText = TextRenderer::Instance()->renderTextSize("Inventory", 0.4f);
	backgroundPanelInventoryText->shouldRender = shouldRender;
	backgroundPanelInventoryText->getTransform()->setPosition(MiddleRight - glm::vec2{sizeOfText.x / 2, -50} - (glm::vec2{350, 0} / 2.0f));
	backgroundPanelInventoryText->getTransform()->setSize(sizeOfText);
	backgroundPanelInventoryText->setText("Inventory");
	backgroundPanelInventoryText->setScale(0.4f);

	const auto items = PlayerController::Instance()->myInventory->items;

	for (int i = 0; i < items.size(); i++)
	{
		createButton(MiddleRight - glm::vec2{sizeOfText.x / 2, -50} - (glm::vec2{350, 0} / 2.0f), items[i]);
		break;
	}
}

void InventoryHUD::onSceneChange(OnSceneChanged* event)
{
	hasLoaded = false;
}
