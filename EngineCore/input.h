#include <raylib.h>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct InputAction
{
    /// Is this a controller action?
    bool isControllerInput;

    // What index is the positive(on) input?
    int positiveInput;
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

    bool IsActionPressed(string actionName);
    void AddActionTrigger(string actionName, int positiveIndex, bool isController=false);
    bool RemoveActionTrigger(string actionName, int positiveIndex, bool isController=false);
    bool HasAction(string actionName);

private:
    InputManager() {}
    //~InputManager();

    /// @brief The input map containing all input actions
    map<string, vector<InputAction>> inputMap = map<string, vector<InputAction>>();
    
};