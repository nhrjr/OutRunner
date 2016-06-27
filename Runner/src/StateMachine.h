#pragma once
#include <memory>
#include <unordered_map>
#include <typeinfo>

class StateMachine;
class BaseState
{
public:

	virtual void enterState(BaseState* previousState) = 0;

	virtual void updateState(float delta) = 0;

	virtual bool isValidNextState(BaseState* state) = 0;

	virtual void exitState(BaseState* nextState) = 0;

	StateMachine* getStateMachine()
	{
		return _stateMachine;
	}

	void setStateMachine(StateMachine* stateMachine)
	{
		_stateMachine = stateMachine;
	}

	virtual ~BaseState() {};
protected:
	StateMachine* _stateMachine;
};


class StateMachine
{
	std::unordered_map<std::string, BaseState*> states;
	BaseState* state;
	
	
public:
	StateMachine() : state(nullptr) {}
	~StateMachine()
	{
		state = nullptr;
	}

	//static StateMachine* create()
	//{
	//	auto ref = new (std::nothrow) StateMachine();
	//	return ref;
	//}

	template<class T>
	T* findState()
	{
		const std::string name = typeid(T).name();
		if (states.count(name) != 0)
		{
			return static_cast<T*>(states[name]);
		}
		return nullptr;
	}

	template<class T, class... Args>
	void addState(Args&&... args)
	{
		auto typeID = typeid(T).name();
		auto newState = new T(std::forward<Args>(args)...);
		newState->setStateMachine(this);
		states.insert({ typeID, newState });
	}

	template<class T>
	bool canEnterState()
	{
		if (state == nullptr) return true;
		else {
			auto state = findState<T>();
			if (state)
			{
				return state->isValidNextState(state);
			}
		}
		return false;
	}

	template<class T>
	bool enterState()
	{
		auto newState = findState<T>();
		if (newState)
		{
			if (state == nullptr)
			{
				state = newState;
				state->enterState(nullptr);
				return true;
			}
			else
			{
				if (state->isValidNextState(newState))
				{
					state->exitState(newState);
					newState->enterState(state);
					state = newState;
					return true;
				}
			}
		}
		return false;
	}

	template<class T>
	bool setState()
	{
		auto newState = findState<T>();
		if (newState)
		{
			if (state == nullptr)
			{
				state = newState;
				state->enterState(nullptr);
			}
			else
			{
				state->exitState(newState);
				newState->enterState(state);
				state = newState;
			}
			return true;
		}
		return false;
	}

	void updateWithDeltaTime(float dt)
	{
		if (state != nullptr)
		{
			state->updateState(dt);
		}
	}

	BaseState* getState()
	{
		return state;
	}

};

