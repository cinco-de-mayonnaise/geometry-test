#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

namespace abdullah_SDL2 {

#define USEREVENT_FRAMETIME (2)
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAX_FPS 60

SDL_Window *window;
SDL_Renderer *renderer;
SDL_TimerID frametimer; // only the timerid.
atomic_uint64_t frameno; // counts number of frames

TTF_Font *systemfont;
Uint64 start_time; // used for measuring fps

Uint32 callback_frametimer(Uint32 interval, void *param);

bool prog_init()
{
    if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_TIMER) < 0)
    {
        cerr << "SDL could not be initialized!\n" << "SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    if (TTF_Init() == -1)
    {
        cerr << "SDL could not be initialized!\n" << "SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    else
    {
        systemfont = TTF_OpenFont("LcdSolid-VPzB.ttf", 20);
        if (systemfont == NULL)
        {
            cerr << "Font could not be loaded!\n" << "SDL_Error: " << SDL_GetError() << endl;
            return false;
        }
    }

    if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) != 0)
    {
        cerr << "SDL could not be initialized!\n" << "SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    
    SDL_SetWindowTitle(window, "Geometry Test - SDL2");

    frametimer = SDL_AddTimer(1000/MAX_FPS, callback_frametimer, nullptr);

    return true;
}

void prog_close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_RemoveTimer(frametimer);
}

// this callback gets called every 16.67ms (1frame of 60fps)
// this posts an event to let the program loop know its time to render and draw a frame.
Uint32 callback_frametimer(Uint32 interval, void *param)
{
    SDL_Event user_event;
    SDL_zero(user_event);  /* SDL will copy this entire struct! Initialize to keep memory checkers happy. */
    
    user_event.type = SDL_USEREVENT;
    user_event.user.code = USEREVENT_FRAMETIME;
    SDL_PushEvent(&user_event);
    
    frameno++;

    return 1000/MAX_FPS;
}

void render_fps()
{
    double time_taken = (double)(SDL_GetPerformanceCounter() - start_time) / SDL_GetPerformanceFrequency();
    int size_s = snprintf(nullptr, 0, "FPS: %lf", time_taken);
    //cout << size_s << endl;
    char buf[size_s+2];
    
    snprintf(buf, size_s, "FPS: %lf", 1/time_taken);

    SDL_Surface* surf = TTF_RenderText_Solid_Wrapped(systemfont, buf, (SDL_Color){255, 255, 255, 100}, 0);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_Rect dst = surf->clip_rect; 
    dst.x = 40;
    dst.y = 40;
    SDL_RenderCopy(renderer, tex, NULL, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}


struct Triangle
{
    SDL_FPoint points[4];
} the_dorito;

void render_triangle()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLinesF(renderer, the_dorito.points, 4);
    
    // fix color back after rendering or else RenderClear will use the last set color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

// all rendering work should be done here.
void render()
{
    SDL_RenderClear(renderer);
    // render
    render_fps();
    render_triangle();
    
    SDL_RenderPresent(renderer);
    
}


void update_points()
{
    // updates the position of the dorito wrt time(frameno), 1s = 60 frames

    // speed: we want the dorito to make 1 rev in 3 seconds: therefore, 360 degrees in 180 frames
    //          therefore, 2 degrees per frame.

    // 1 rev in 10 seconds: therefore 360 degrees in 70*10 = 700 frames
    // therefore 1 degree = 700/360 = 1.944 frames

    double th = 2;
    th *= M_PI/180;
    double cos_th = cos(th);
    double sin_th = sin(th);

    for (int i = 0; i < 4; i++)
    {
        double x, y;
        x = the_dorito.points[i].x - SCREEN_WIDTH/2;   // remove offset so it is centered around (0,0) before we apply rotation
        y = the_dorito.points[i].y - SCREEN_HEIGHT/2; 

        the_dorito.points[i].x = x*cos_th + (-sin_th)*y + SCREEN_WIDTH/2;   // rotate
        the_dorito.points[i].y = x*sin_th + (cos_th)*y + SCREEN_HEIGHT/2;
        // add offset back in while we're doing the computation cause thats faster
    }
}

Triangle generate_dorito(int sidelength_px)
{
    Triangle t;
    
    t.points[0].x = 0;
    t.points[0].y = sidelength_px;
    for (int i = 1; i < 4; i++)
    {
        t.points[i].x = t.points[i-1].x*-0.5 + (-sqrtf(3)/2)*t.points[i-1].y;  // rotate 120 degrees
        t.points[i].y = t.points[i-1].x*(sqrtf(3)/2) + -0.5*t.points[i-1].y;
    }
    // im curious
    cout << t.points[0].x << " " << t.points[0].y << endl;
    cout << t.points[1].x << " " << t.points[1].y<< endl;
    cout << t.points[2].x << " " << t.points[2].y<< endl;
    cout << t.points[3].x << " " << t.points[3].y<< endl;

    // add offsets
    for (int i = 0; i < 4; i++)
    {
        t.points[i].x += SCREEN_WIDTH/2;
        t.points[i].y += SCREEN_HEIGHT/2;
    }

    return t;
}

int main(void)
{
    if (!prog_init())
        return 1;

    bool running = true;
    SDL_Event ev;
    start_time = SDL_GetPerformanceCounter();
    frameno = 0;
    the_dorito = generate_dorito(100);

    while (running)
    {
        if (SDL_WaitEvent(&ev) == 0)
        {
            cerr << SDL_GetError();
            return 1;
        } 

        switch(ev.type)
        {
            case SDL_EventType::SDL_USEREVENT: // time to publish frame
                if (ev.user.code == USEREVENT_FRAMETIME)
                {   
                    //cout << "New frame!" << endl;
                    update_points();
                    render();
                    start_time = SDL_GetPerformanceCounter();
                }
                break;
            case SDL_EventType::SDL_QUIT:
                running = false;
                break;
            case SDL_EventType::SDL_KEYDOWN:
                switch (ev.key.keysym.sym)
                {
                    case SDL_KeyCode::SDLK_q:
                        running = false;
                        break;
                }
        }

        // program/game logic
        // ???
    }

    prog_close();

    return 0;
}

}