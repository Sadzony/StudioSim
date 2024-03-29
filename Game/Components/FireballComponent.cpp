﻿#include "FireballComponent.h"

#include <vector>

#include "DestroyAfterAnimation.h"
#include "Core/Component.h"
#include "Core/GameObject.h"
#include "Core/Components/AnimatedSpriteRenderer.h"
#include "Core/Components/Light.h"
#include "Core/Components/Transform.h"
#include "Core/Renderer/ResourceManager.h"
#include "Util/Time.h"
#include "Util/Events/Events.h"

void FireballComponent::start()
{
	Component::start();

	const std::vector textureListFireball = ResourceManager::GetTexturesContaining("Fireball");
	getOwner()->addComponent<AnimatedSpriteRenderer>(textureListFireball, 0.1f);
	getOwner()->getComponent<AnimatedSpriteRenderer>()->setColor(glm::vec3(1, 1, 1));
	getOwner()->getComponent<AnimatedSpriteRenderer>()->setSortingLayer(Renderer::getDefaultSortingLayer());
	getOwner()->getComponent<AnimatedSpriteRenderer>()->setPivot(Pivot::Center);
	getOwner()->addComponent<Light>();
	getOwner()->addComponent<DestroyAfterAnimation>();
}

void FireballComponent::update()
{
	Component::update();

	// move the fireball to the right
	getOwner()->getTransform()->setPosition(getOwner()->getTransform()->getPosition() + glm::vec2{ 200.0f, 0.0f } * static_cast<float>(Time::getDeltaTime()));
}

void FireballComponent::getDebugInfo(std::string* string)
{
	std::stringstream ss;
	ss << "No data" << std::endl;
	string->append(ss.str());
	Component::getDebugInfo(string);
}
