#ifndef INPUT_H
#define INPUT_H


#include <raylib.h>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "structs.h"


using namespace std;

/// @brief A struct representing a single input action (i.e jump)
class InputAction
{
public:
    InputAction() {};

    // Parameterized constructors
    InputAction(string name)
    {
        eventName = name;
        eventActions = vector<InputEvent>();
    };

    // Create input with default event
    InputAction(string name, int inputTrigger, int controllerId = -1): InputAction(name)
    {
        InputEvent defaultEvent = InputEvent(inputTrigger, controllerId);
        this->AddInputEvent(defaultEvent);
    }


    InputAction(string name, vector<InputEvent> newEventActions) : InputAction(name)
    {
        eventActions = newEventActions;
    };


    /// @brief Add an input event to this action
    /// @param newEvent The new event to add
    void AddInputEvent(InputEvent newEvent)
    {
        eventActions.push_back(newEvent);
    };

    /// @brief Get the name of this input action
    /// @return The name of the input action
    string GetName()
    {
        return eventName;
    }


    vector<InputEvent> GetEvents()
    {
        return eventActions;
    }


private:
    string eventName;
    vector<InputEvent> eventActions = vector<InputEvent>();
    float eventStrength = 0.0f;

};

class InputManager
{
private:
    InputManager(){};

    std::shared_ptr<InputAction> GetAction(string actionName);

    /// @brief The input map containing all input actions keyed to the action name
    std::shared_ptr< map<string, std::shared_ptr<InputAction> > > inputMap = std::make_shared<map<string, std::shared_ptr<InputAction>>>();

    
    
public:
    bool IsActionDown(string actionName);
    bool IsActionPressed(string actionName);
    bool RemoveActionTrigger(string actionName, int positiveIndex, bool isController=false);
    bool HasAction(string actionName);
    void AddInputAction(shared_ptr<InputAction> newAction);
    void RemoveInputAction(string actionName);
    void AddActionEvent(string actionName, int positiveIndex, int controllerId);

    static InputManager& instance()
    {
        static InputManager INSTANCE;
        return INSTANCE;
    }
};
#endif