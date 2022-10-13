#include "input.h"
#include <iostream>

/// @brief Is an action pressed
/// @param actionName The name of the action to check
/// @return True if an action with the name actionName is pressed
bool InputManager::IsActionPressed(string actionName)
{
    // Do we have an action at this name currently?
    if (inputMap.count(actionName) > 0)
    {
        vector<InputAction> actions = inputMap[actionName];
        std::cout << actions.size() << std::endl;
        return true;
    }
   
    return false;
}

/// @brief Add a trigger for a specific action
/// @param actionName The name of the action to trigger
/// @param positiveIndex The int index of the button/key press
/// @param isController Is the action coming from a controller
void InputManager::AddActionTrigger(string actionName, int positiveIndex, bool isController=false)
{
    if (inputMap.count(actionName) >0)
    {

    }
    
    // Action has not yet been added, create a new action
    else
    {

    }

}