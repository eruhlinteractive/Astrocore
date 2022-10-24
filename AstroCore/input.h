#include <raylib.h>
#include <vector>
#include <map>
#include <string>
#include <memory>

using namespace std;

/// @brief A struct representing a single input event (i.e an 'E' key press or 'A' button press on a controller)
struct InputEvent
{
    // Constructor
    InputEvent(int inputIndex, int actionControllerId =-1) 
    {
        controllerId = actionControllerId;
        positiveInput = inputIndex;
    };
    /// Is this a controller action?
    int controllerId = -1;

    // What index is the positive(on) input?
    int positiveInput;
};

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

    // Singleton
    static InputManager& instance()
    {
       static InputManager instance;
       return instance;
    };

    bool IsActionPressed(string actionName);
    bool RemoveActionTrigger(string actionName, int positiveIndex, bool isController=false);
    bool HasAction(string actionName);
    void AddInputAction(shared_ptr<InputAction> newAction);
    void RemoveInputAction(string actionName);
    void AddActionEvent(string actionName, int positiveIndex, int controllerId);
};
