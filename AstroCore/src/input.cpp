#include "../header/input.h"
#include <iostream>


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
        InputEvent newEvent = InputEvent(controllerId, positiveIndex); 
        action->AddInputEvent(newEvent);
    }
    
    // Action has not yet been added, create a new action and add event
    else
    {
        shared_ptr<InputAction> newAction = std::make_shared<InputAction>(actionName);
        InputEvent newEvent = InputEvent(controllerId, positiveIndex);
        AddInputAction(newAction);
        newAction->AddInputEvent(newEvent);
    }

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