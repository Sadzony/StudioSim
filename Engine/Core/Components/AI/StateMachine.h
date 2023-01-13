#pragma once


#include "Core/Components/AI/Behaviour.h"

//Class interface used for Behaviour Selection based on FSMTransition Events. Derive this class into unique AI state machines for the games.
class StateMachine : public Behaviour
{
// Properties
protected:

	//The base state that the state machine can return into.
	Behaviour* baseState = nullptr;
	Behaviour* currentState = nullptr;
	int8_t eventResponseID = -1;
	int8_t onTransitionReceivedID = -1;

//Public Methods
public:

	void destroy() override;
	void start() override;
	void update() override;
	void lateUpdate() override;

	//performs Act() on the current state.
	virtual void Act() override;

	//Exits current state, and tells update to wait till its ready for the change.
	virtual void ChangeState(Behaviour* behaviourParam);

	//Sets the behaviour to the state machine's base state.
	virtual void ResetToBase();

	void EventResponse(const BehaviourEvent* event) override;

	//Checks the transition against the current state.
	virtual void OnTransitionReceived(const StateTransition* event);

	void CleanUp();
};
