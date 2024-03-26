#include <iostream>
#include <SDL.h>

const int delay = 20;

struct Coords
{
    float x;
    float y;
    float z;
};

Coords rotation_x_matrix_product(Coords coords, float angle)
{
    /*
    [1 0   0   ] * [x] = [     x     ]
    [0 cos -sin]   [y]   [y*cos-z*sin]
    [0 sin cos ]   [z]   [y*sin+z*cos]
    */

    return {
        coords.x,
        coords.y * cos(angle) - coords.z * sin(angle),
        coords.y * sin(angle) + coords.z * cos(angle)};
}
Coords rotation_y_matrix_product(Coords coords, float angle)
{
    /*
    [cos  0 sin] * [x] = [x*cos+sin*z]
    [0    1 0  ]   [y]   [     y     ]
    [-sin 0 cos]   [z]   [-x*sin+cos*z]
    */

    return {
        coords.x * cos(angle) + sin(angle) * coords.z,
        coords.y,
        -coords.x * sin(angle) + coords.z * cos(angle)};
}
Coords rotation_z_matrix_product(Coords coords, float angle)
{
    /*
    [cos -sin 0] * [x] = [x*cos-sin*y]
    [sin cos  0]   [y]   [x*sin+cos*y]
    [0   0    1]   [z]   [     z     ]
    */

    return {
        coords.x * cos(angle) - sin(angle) * coords.y,
        coords.x * sin(angle) + cos(angle) * coords.y,
        coords.z};
}

Coords project_on_screen(Coords coords, int screen_width, int screen_height, float scaling)
{
    return {
        ((float)(screen_width) / 2.f - scaling / 2.f) + coords.x * scaling,
        ((float)(screen_height) / 2.f - scaling / 2.f) + coords.y * scaling,
        0};
}

Coords points[16] = {
    {-1, -1, -1},
    {1, -1, -1},
    {1, -1, 1},
    {-1, -1, 1},
    {-1, -1, -1},

    {-1, 1, -1},

    {1, 1, -1},
    {1, -1, -1},
    {1, 1, -1},

    {1, 1, 1},
    {1, -1, 1},
    {1, 1, 1},

    {-1, 1, 1},
    {-1, -1, 1},
    {-1, 1, 1},

    {-1, 1, -1},
};

int main(int argc, char **argv)
{
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("3d tests",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    int count = 0;

    bool isPressing = false;

    int last_x, last_y = 0;

    int velocity_x, velocity_y = 0;

    while (!quit)
    {
        SDL_Delay(10);
        SDL_PollEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                isPressing = true;
                velocity_x = 0;
                velocity_y = 0;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                isPressing = false;
            }
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        float cubeSize = 0.f;
        if (width > height)
        {
            cubeSize = (float)(height) / 5.f;
        }
        else
        {
            cubeSize = (float)(width) / 5.f;
        }

        int x, y;
        SDL_GetGlobalMouseState(&x, &y);

        if (isPressing)
        {

            int dx = x - last_x;
            int dy = y - last_y;

            velocity_x += dx/delay;
            velocity_y += dy/delay;

            for (int i = 0; i < sizeof(points) / sizeof(Coords); i++)
            {
                points[i] = rotation_y_matrix_product(points[i], (dx * 2 * M_PI) / width);
                points[i] = rotation_x_matrix_product(points[i], (-dy * 2 * M_PI) / height);
            }

            /*                 points[i] = rotation_x_matrix_product(points[i], PI / 48);
                            points[i] = rotation_y_matrix_product(points[i], PI / 48);
                            points[i] = rotation_z_matrix_product(points[i], PI / 48); */
        }
        else
        {
            velocity_x *= 0.9;
            velocity_y *= 0.9;
            for (int i = 0; i < sizeof(points) / sizeof(Coords); i++)
            {
                points[i] = rotation_y_matrix_product(points[i], (velocity_x * 2 * M_PI) / width);
                points[i] = rotation_x_matrix_product(points[i], (velocity_y * 2 * M_PI) / height);
            }
        }

        if (count > delay)
        {
            last_x = x;
            last_y = y;
        }

        for (int i = 0; i < sizeof(points) / sizeof(Coords); i++)
        {

            Coords projected_now = project_on_screen(points[i], width, height, cubeSize);

            Coords projected_last = projected_now;
            float z_average = 0.f;
            if (i >= 1)
            {
                projected_last = project_on_screen(points[i - 1], width, height, cubeSize);
                z_average = (points[i].z + points[i - 1].z) / 4.f;
            }
            Uint8 color = 125 + (125 * z_average);
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawLine(renderer, projected_last.x, projected_last.y, projected_now.x, projected_now.y);
        }

        SDL_RenderPresent(renderer);

        if (count > delay)
        {
            count = 0;
        }
        count++;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}