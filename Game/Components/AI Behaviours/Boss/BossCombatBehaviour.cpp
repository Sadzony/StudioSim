#include "BossCombatBehaviour.h"
#include "../../TurnManager.h"
#include "BossAttackAction.h"

BossCombatBehaviour::BossCombatBehaviour(StateMachine* parentFSMArg, std::vector<glm::vec2> spawnerPositionsArg)
{
	isInFSM = true;
	parentFSM = parentFSMArg;
	spawnerPositions = spawnerPositionsArg;
}

void BossCombatBehaviour::WorldAnalysis()
{
	//find whether spawner ability is off cooldown
	//otherwise attack
}

void BossCombatBehaviour::ActionAnalysis()
{
	PlannedBehaviour::ActionAnalysis();
}

void BossCombatBehaviour::GenerateBehaviourList()
{
	availableActions["Attack"] = std::make_pair(0, new BossAttackAction(parentFSM->getOwner()));
	for (auto action : availableActions)
	{
		if (!action.second.second->GetInitValue())
		{
			action.second.second->start();
		}
	}
}

void BossCombatBehaviour::GenerateEffects()
{
}

void BossCombatBehaviour::endTurn()
{
	TurnManager::Instance()->endTurn();
	LOG_INFO("BossCombatBehaviour::endTurn() - Enemy turn ended");
}
