#pragma once
#include "PlayerController.h"
#include "Core/Components/AI/Behaviour.h"
#include "glm/fwd.hpp"

class PlayerMovementBehaviour : public Behaviour
{
public:
	PlayerMovementBehaviour();
	explicit PlayerMovementBehaviour(bool isInFSMParam);
	glm::fvec2 moveDir;
	glm::fvec2 origPos;
	bool canMove = true;
private:
	void Act() override;
	void onKeyDownResponse(Griddy::Event*);
	void onKeyUpResponse(Griddy::Event*);
protected:
	FunctionMap CreateFunctionMap() override;

	
};
