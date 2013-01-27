

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


void draw_bg_column(VideoBuffer &vid, int x)
{
    // Draw a vertical column of tiles.

    int dstx = umod(x, 18);
    int srcx = umod(x, Map.tileWidth());
    
    //LOG("dstx: %d \n srcx: %d", dstx, srcx);

    vid.bg0.image(vec(dstx, 0), vec(1, 18), Map, vec(srcx, 0));
}

void draw_bg_row(VideoBuffer &vid, int y)
{
    // Draw a horizontal column of the tiles.
    
    int dsty = umod(y, 18);
    int srcy = umod(y, Map.tileHeight());
    
    //LOG("dsty: %d \n srcy: %d", dsty, srcy);
    
    vid.bg0.image(vec(0,dsty), vec(18, 1), Map, vec(0, srcy));
}

void main()
{
    VideoBuffer vid;
    vid.initMode(BG0);
    vid.attach(cube);
    
    for (int x = -1; x < 17; x++)
    {
        draw_bg_column(vid, x);
    }
    
    for (int y = -1; y < 17; y++)
    {
        draw_bg_row(vid, y);
    }
    
    float x = 0;
    float y = 0;
    int prev_xt = 0;
    int prev_yt = 0;
    
    while (1)
    {
        // Scroll based on accelerometer tilt
        Int2 accel = vid.physicalAccel().xy();
       
        
        // Floating point pixels
        if ( accel.x > 1 || accel.x < -1)
        {
            x += accel.x * (40.0f / 128.0f);
        }
        if ( accel.y > 1 || accel.y < -1)
        {
            y += accel.y * (40.0f / 128.0f);
        }
        
        // Integer pixels
        int xi = x + 0.5f;
        int yi = y + 0.5f;
        
        // Integer tiles
        int xt = x / 8;
        int yt = y / 8;
        
        while (prev_xt < xt) {
            // Fill in new tiles, just past the right edge of the screen
            draw_bg_column(vid, prev_xt + 17);
            prev_xt++;
        }
        
        while (prev_xt > xt) {
            // Fill in new tiles, just past the left edge
            draw_bg_column(vid, prev_xt - 2);
            prev_xt--;
        }
        
        while (prev_yt < yt) {
            // Fill in new tiles, just past the bottom edge of the screen
            draw_bg_row(vid, prev_yt + 17);
            prev_yt++;
        }
        
        while (prev_yt > yt) {
            // Fill in new tiles, just past the bottom edge
            draw_bg_row(vid, prev_yt - 2);
            prev_yt--;
        }
        
        // Firmware handles all pixel-level scrolling
        vid.bg0.setPanning(vec(xi, 0));
        vid.bg0.setPanning(vec(0, yi));
        
        System::paint();
    }
}