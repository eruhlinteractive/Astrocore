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

    // Returns true if an action currently pressed
    bool IsActionPressed(string actionName);

    // Adds an action trigger, creating a new action if needed
    void AddActionTrigger(string actionName, int positiveIndex, bool isController=false);

    // Removes an input action, returning true if the action was removed successfully
    bool RemoveActionTrigger(string actionName, int positiveIndex, bool isController=false);

private:
    InputManager() {}

    /// @brief The input map containing all input actions
    map<string, vector<InputAction>>* inputMap;
    
     // Constructor
    //Input();
    // Destructor 
    //~Input();

};