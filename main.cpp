

#include <sifteo.h>
#include "assets.gen.h"

using namespace Sifteo;

static CubeID cube(0);

AssetSlot MainSlot = AssetSlot::allocate()
    .bootstrap(GameAssets);

static Metadata M = Metadata()
    .title("Telltale")
    .package("com.GGJ.Telltale", "1.0")
    .cubeRange(1);

void draw_bg_column(VideoBuffer &vid, int x, int y)
{
    // Draw a vertical column of tiles.

    int dstx = umod(x, 18);
    int srcx = umod(x, Map.tileWidth());
    
    int dsty = umod(y, 18);
    int srcy = umod(y, Map.tileHeight());
    
    LOG("dstx: %d \n srcx: %d", dstx, srcx);

    vid.bg0.image(vec(dstx, 0), vec(1, 18), Map, vec(srcx, srcy));
}

void draw_bg_row(VideoBuffer &vid, int x, int y)
{
    // Draw a horizontal column of the tiles.
    
    int dsty = umod(y, 18);
    int srcy = umod(y, Map.tileHeight());
    
    int dstx = umod(x, 18);
    int srcx = umod(x, Map.tileWidth());
    
    //LOG("y: %d dsty: %d srcy: %d \n", y, dsty, srcy);
    
    vid.bg0.image(vec(0,dsty), vec(18, 1), Map, vec(srcx, srcy));
}

void main()
{
    VideoBuffer vid;
    vid.initMode(BG0);
    vid.attach(cube);
    
    bool moveHorizontal = false;
    bool moveVertical = false;
    
    for (int x = -1; x < 17; x++)
    {
        draw_bg_column(vid, x, 0);
    }
    
    for (int y = -1; y < 17; y++)
    {
        draw_bg_row(vid, 0, y);
    }
    
    float x = 0;
    float y = 0;
    unsigned prev_xt = 0;
    unsigned prev_yt = 0;
    
    while (1)
    {
        // Scroll based on accelerometer tilt
        Int2 accel = vid.physicalAccel().xy();
       
        // Floating point pixels
        if ( accel.x > 1 || accel.x < -1)
        {
            x += accel.x * (40.0f / 128.0f);
            moveHorizontal = true;
        }
        else if ( accel.y > 1 || accel.y < -1)
        {
            y += accel.y * (40.0f / 128.0f);
            moveVertical = true;
        }
        
        // Integer pixels
        int xi = x + 0.5f;
        int yi = y + 0.5f;
        
        // Integer tiles
        int xt = x / 8;
        int yt = y / 8;
        
        if ( moveHorizontal )
        {
            while (prev_xt < xt) {
                // Fill in new tiles, just past the right edge of the screen
                draw_bg_column(vid, prev_xt + 17, prev_yt);
                prev_xt++;
            }
            
            while (prev_xt > xt) {
                // Fill in new tiles, just past the left edge
                draw_bg_column(vid, prev_xt - 2, prev_yt);
                prev_xt--;
            }
            
            //LOG("End of Horizontal\n x: %d, y%d \n", prev_xt, prev_yt);
        }
        
        if ( moveVertical )
        {
            while (prev_yt < yt) {
                // Fill in new tiles, just past the bottom edge of the screen
                draw_bg_row(vid, prev_xt, prev_yt + 17);
                prev_yt++;
            }
            
            while (prev_yt > yt) {
                // Fill in new tiles, just past the bottom edge
                draw_bg_row(vid, prev_xt, prev_yt - 2);
                prev_yt--;
            }
            
             //LOG("End of Horizontal\n x: %d, y%d \n", prev_xt, prev_yt);
        }
        vid.bg0.setPanning(vec(xi, yi));
        
        System::paint();
        moveVertical = moveHorizontal = false;
    }
}