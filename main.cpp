

#include <sifteo.h>
#include "assets.gen.h"

using namespace Sifteo;

static const unsigned numCubes = 3;
static unsigned posX = 0;
static unsigned posY = 0;
static VideoBuffer vBuffer[numCubes];

AssetSlot MainSlot = AssetSlot::allocate()
.bootstrap(GameAssets);

CubeSet allCubes(0, numCubes);

static Metadata M = Metadata()
.title("Telltale")
.package("com.GGJ.Telltale", "1.0")
.cubeRange(1);

void main()
{
    for (CubeID cube : allCubes)
    {
        vBuffer[cube].initMode(BG0);
        vBuffer[cube].attach(cube);
        vBuffer[cube].erase();
    }
    
    LOG("Game Loop Starting...\n");
    CubeID cube(0);
    while(1)
    {
        if (cube.accel().x > 1.5)
            posX += cube.accel().x;
        
        for (CubeID cube : allCubes)
        {
            vBuffer[cube].bg0.image(vec(posX,posY), Map);
        }
        
        System::paint();
    }
    
}