#include <raylib.h>
#include <vector>
#include <map>
#include <string>

using namespace std;

/// @brief A struct representing a single input event (i.e an 'E' key press or 'A' button press on a controller)
struct InputEvent
{
    // Constructor
    InputEvent(int actionControllerId, int inputIndex) 
    {
        controllerId = actionControllerId;
        positiveInput = inputIndex;
    };
    /// Is this a controller action?
    bool controllerId;

    // What index is the positive(on) input?
    int positiveInput;
};

/// @brief A struct representing a single input action (i.e jump)
struct InputAction
{
    InputAction(string name)
    {
        eventName = name;
        eventActions = vector<InputEvent>();
    }

    string eventName;
    vector<InputEvent> eventActions;
    float eventStrength = 0.0f;

};

class InputManager
{
public:

    // Singleton
    static InputManager& instance()
    {
       static InputManager *instance = new InputManager();
       return *instance;
    };

    void AddActionTrigger(string actionName, int positiveIndex, int controllerId = -1);

    bool IsActionPressed(string actionName);
    bool IsActionHeld(string actionName);
    bool RemoveActionTrigger(string actionName, int positiveIndex, bool isController=false);
    bool HasAction(string actionName);

private:
    InputManager() {}
    //~InputManager();

    /// @brief The input map containing all input actions keyed to the event name
    map<string, InputAction> inputMap = map<string, InputAction>();
    
};