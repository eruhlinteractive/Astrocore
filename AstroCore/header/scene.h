#include "entity.h"
#include <vector>

#ifndef __SCENE__

#define __SCENE__

namespace Astrolib
{
class Scene
{

public:

    Entity2D* FindEntityByName(std::string name);


private:

    std::vector<Entity2D> entities;

};
}
#endif // !__SCENE__