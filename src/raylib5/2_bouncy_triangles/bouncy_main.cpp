#include <bits/stdc++.h>
#include <raylib.h>


using namespace std;

namespace bouncy_triangles_raylib {

#define SCREEN_WIDTH 640   
#define SCREEN_HEIGHT 480

void renderFPSToScreen()
{
    char buf[20];
    snprintf(buf, 20, "FPS: %lf", 1/GetFrameTime());
    DrawText(buf, 50, 50, 20, LIGHTGRAY);

    snprintf(buf, 20, "FPS: %d", GetFPS());
    DrawText(buf, 50, 70, 20, LIGHTGRAY);
}

// the points in this Triangle always respond to actual screen coordinates. To manipulate/rotate the shape, ensure to center origin
// by manually subtracing the offset first.
struct Triangle{
    Vector2 points[3];

    int vel_x = 4, vel_y = 4;
    int theta = 2;  // the angle(in degrees!) with which the triangle will spin.

    // returns true if the triangle is horizontally within the bounds of the screen
    // i.e within x=0 and x=SCREEN_WIDTH
    bool check_x_bounds()
    {
        for (int i = 0; i < 3; i++)
            if (this->points[i].x + this->vel_x < 0 || this->points[i].x + this->vel_x >= SCREEN_WIDTH)
                return false;

        return true;
    }

    // returns true if the triangle is vertically within the bounds of the screen
    // i.e within x=0 and x=SCREEN_HEIGHT
    bool check_y_bounds()
    {
        for (int i = 0; i < 3; i++)
        {
            if (this->points[i].y + this->vel_y < 0 || this->points[i].y + this->vel_y >= SCREEN_HEIGHT)
                return false;
        }

        return true;
    }

    void clamp()
    {
        for (int i = 0; i < 3; i++)
        {
            std::clamp(this->points[i].x, 0.0f, (float)SCREEN_WIDTH); 
            std::clamp(this->points[i].y, 0.0f, (float)SCREEN_HEIGHT);
        }
    }
};

Triangle generate_dorito(int sidelength_px)
{
    Triangle t;
    
    // stupid hecking raylib demands the vertices of the triangle be in anti-clockwise order reeee
    t.points[2].x = 0;
    t.points[2].y = sidelength_px;
    for (int i = 1; i >= 0; i--)
    {
        t.points[i].x = t.points[i+1].x*-0.5 + (-sqrtf(3)/2)*t.points[i+1].y;  // rotate 120 degrees
        t.points[i].y = t.points[i+1].x*(sqrtf(3)/2) + -0.5*t.points[i+1].y;
    }
    // im curious
    cout << t.points[0].x << " " << t.points[0].y << endl;
    cout << t.points[1].x << " " << t.points[1].y<< endl;
    cout << t.points[2].x << " " << t.points[2].y<< endl;

    // add offsets
    for (int i = 0; i < 3; i++)
    {
        t.points[i].x += SCREEN_WIDTH/2;
        t.points[i].y += SCREEN_HEIGHT/2;
    }

    return t;
}

void renderTriangle(Triangle &t)
{
    DrawTriangle(t.points[0], t.points[1], t.points[2], YELLOW);
}

bool rotota = false; 
void update_points(Triangle &t)
{
    // updates the position of the dorito wrt time(frameno), 1s = 60 frames

    // speed: we want the dorito to make 1 rev in 3 seconds: therefore, 360 degrees in 180 frames
    //          therefore, 2 degrees per frame.

    // 1 rev in 10 seconds: therefore 360 degrees in 70*10 = 700 frames
    // therefore 1 degree = 700/360 = 1.944 frames

    double th = t.theta;
    th *= M_PI/180;
    double cos_th = cos(th);
    double sin_th = sin(th);

    auto center_x = (t.points[0].x + t.points[1].x + t.points[2].x)/3;
    auto center_y = (t.points[0].y + t.points[1].y + t.points[2].y)/3;

    // rotate
    for (int i = 0; i < 3; i++)
    {
        double x, y;
        x = t.points[i].x - center_x;   // remove offset so it is centered around (0,0) before we apply rotation
        y = t.points[i].y - center_y; 

        t.points[i].x = x*cos_th + (-sin_th)*y + center_x;   // rotate
        t.points[i].y = x*sin_th + (cos_th)*y + center_y;
        // add offset back in while we're doing the computation cause thats faster    
    }

    // if (!t.check_x_bounds())
    // {
    //     // the rotation caused it to go out of bounds, so change rotation
    //     t.theta = -t.theta;
    //     // clamp (push all points inside the screen region)
    //     //t.clamp();
    // }

    // move: we want bouncy doritos
    if (!t.check_x_bounds())
        t.vel_x = -t.vel_x;
    
    if (!t.check_y_bounds())
        t.vel_y = -t.vel_y;

    for (int i = 0; i < 3; i++)
    {
        t.points[i].x += t.vel_x; 
        t.points[i].y += t.vel_y;

        cout << t.points[i].x << ' ' << t.points[i].y << '|';
        //assert(t.check_x_bounds() && t.check_y_bounds());
    }
    cout << t.vel_x  << ' ' << t.vel_y <<endl;
}






// wow raylib is so gud we magically got rid of global variables! all hail dorit- i mean raylib
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Geometry Test - raylib5");
    SetTargetFPS(60);

    Triangle the_dorito = generate_dorito(100);

    while (!WindowShouldClose())
    {

        if (IsKeyPressed(KEY_R))
            the_dorito.theta = -the_dorito.theta;

        if (IsKeyPressed(KEY_DOWN))
            the_dorito.theta -= 1;
        if (IsKeyPressed(KEY_UP))
            the_dorito.theta += 1;

        update_points(the_dorito);

        BeginDrawing();
            ClearBackground(BLACK);
            
            renderFPSToScreen();
            renderTriangle(the_dorito);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

}