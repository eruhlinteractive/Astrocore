#include "input.h"
#include <iostream>

using namespace Astrolib;

bool InputManager::IsActionPressed(string actionName)
{
    // Do we have an action at this name currently?
    if (HasAction(actionName))
    {
        std::shared_ptr<InputAction> action = GetAction(actionName);
        vector<InputEvent> actionEvents = action->GetEvents();

        // Loop through each event, checking if it is currently being pressed
        for(int i= 0; i < actionEvents.size(); i++)
        {
            // Check keyboard inputs
            if(actionEvents[i].controllerId == -1)
            {
                if (IsKeyPressed(actionEvents[i].positiveInput))
                {
                    return true;
                }
                // No controller input defined, continue the loop
                continue;
            }

            // Check for controller input
            else
            {
                if(IsGamepadAvailable(actionEvents[i].controllerId))
                {
                    if(IsGamepadButtonPressed(actionEvents[i].controllerId, actionEvents[i].positiveInput))
                    {
                        return true;
                    }
                }
            }
        }
    }
   
    return false;

}


/// @brief Is an action currently pressed
/// @param actionName The name of the action to check
/// @return True if an action with the name actionName is pressed
bool InputManager::IsActionDown(string actionName)
{
    // Do we have an action at this name currently?
    if (HasAction(actionName))
    {
        std::shared_ptr<InputAction> action = GetAction(actionName);
        vector<InputEvent> actionEvents = action->GetEvents();

        // Loop through each event, checking if it is currently being pressed
        for(int i= 0; i < actionEvents.size(); i++)
        {
            // Check keyboard inputs
            if(actionEvents[i].controllerId == -1)
            {
                if (IsKeyDown(actionEvents[i].positiveInput))
                {
                    return true;
                }
                // No controller input defined, continue the loop
                continue;
            }

            // Check for controller input
            else
            {
                if(IsGamepadAvailable(actionEvents[i].controllerId))
                {
                    if(IsGamepadButtonDown(actionEvents[i].controllerId, actionEvents[i].positiveInput))
                    {
                        return true;
                    }
                }
            }
        }
    }
   
    return false;
}

/// @brief Add a trigger for a specific action, creating the action if it doesn't exist
/// @param actionName The name of the action to trigger
/// @param positiveIndex The int index of the button/key press
/// @param isController Is the action coming from a controller
void InputManager::AddActionEvent(string actionName, int positiveIndex, int controllerId)
{
    // Update the current action
    if (inputMap->count(actionName) > 0)
    {
        shared_ptr<InputAction> action = (*inputMap)[actionName];

        // Create new input action
        InputEvent newEvent = InputEvent(positiveIndex, controllerId); 
        action->AddInputEvent(newEvent);
    }
    
    // Action has not yet been added, create a new action and add event
    else
    {
        shared_ptr<InputAction> newAction = std::make_shared<InputAction>(actionName);
        InputEvent newEvent = InputEvent(positiveIndex, controllerId);
        AddInputAction(newAction);
        newAction->AddInputEvent(newEvent);
    }

}

float InputManager::GetActionStrength(string actionName)
{
    // TODO: Get this to work with gamepad joysticks and such
    return 0;
}

Vector2 InputManager::GetInputAxis(string positiveActionName, string negativeActionName)
{
    assert(HasAction(positiveActionName) && "Positive action name does not have a defined action in InputManager::GetInputAxis()");
    assert(HasAction(negativeActionName) && "Negative action name does not have a defined action in InputManager::GetInputAxis()");

}

/// @brief Does the input map have an action?
/// @param actionName The name of the action
/// @return True if the input manager has a registered action with this name
bool InputManager::HasAction(string actionName)
{
    return (inputMap->count(actionName) > 0);
}

/// @brief Remove an input action and any associated triggers
/// @param actionName The name of the input action
void InputManager::RemoveInputAction(string actionName)
{
    // Action already exists, throw error
    if (!HasAction(actionName))
    {
        throw std::runtime_error("Action " + actionName + " does not exist!");
    }

    // Remove
    inputMap->erase(actionName);
}

/// @brief Add a new input action to the input map
/// @param newAction The new action to add
void InputManager::AddInputAction(shared_ptr<InputAction> newAction)
{
    // Action already exists, throw error
    if (HasAction(newAction->GetName()))
    {
        throw std::runtime_error("Action " + newAction->GetName() + " already exists!");
    }

    // Add action
    inputMap->emplace(newAction->GetName(), newAction);
}

/// @brief Get an input action
/// @param actionName The name of the input action
/// @return The input action object
shared_ptr<InputAction> InputManager::GetAction(string actionName)
{
    if (!HasAction(actionName))
    {
        throw std::runtime_error("Action " + actionName + " does not exist! Cannot retrieve..");
        //return InputAction();
    }
    return (*inputMap)[actionName];
}