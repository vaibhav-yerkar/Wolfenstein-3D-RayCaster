#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#include <SDL.h>

// Constants
#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

// World Map
int worldMap[mapWidth][mapHeight] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

void drawVerticalLine(SDL_Renderer *renderer, int x, int start, int end, SDL_Color color)
{
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
  for (int y = start; y <= end; y++)
  {
    SDL_RenderDrawPoint(renderer, x, y);
  }
}

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
  }

  SDL_Window *window = SDL_CreateWindow("Raycaster",
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        screenWidth, screenHeight,
                                        SDL_WINDOW_SHOWN);

  if (!window)
  {
    std::cerr << "Window could not be created! SDL Error : " << SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
  {
    std::cerr << "Renderer could not be created! SDL Error : " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  bool quit = false;
  SDL_Event event;

  double posX = 22, posY = 12;
  double dirX = -1, dirY = 0;
  double planeX = 0, planeY = 0.66;

  const double moveSpeed = 0.08;
  const double rotSpeed = 0.04;
  // double time = 0;
  // double oldTime = 0;

  while (!quit)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        quit = true;
      }
    }

    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_W])
    {
      if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == 0)
      {
        posX += dirX * moveSpeed;
      }
      if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == 0)
      {
        posY += dirY * moveSpeed;
      }
    }
    if (keyState[SDL_SCANCODE_S])
    {
      if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == 0)
      {
        posX -= dirX * moveSpeed;
      }
      if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == 0)
      {
        posY -= dirY * moveSpeed;
      }
    }
    if (keyState[SDL_SCANCODE_A])
    {
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
    if (keyState[SDL_SCANCODE_D])
    {
      double oldDirX = dirX;
      dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < screenWidth; x++)
    {
      double cameraX = 2 * x / (double)screenWidth - 1;
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;

      int mapX = int(posX);
      int mapY = int(posY);

      double sideDistX;
      double sideDistY;

      double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
      double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
      double perpWallDist;

      int stepX;
      int stepY;
      int hit = 0;
      int side;

      if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }

      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }

      while (hit == 0)
      {
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }

        if (worldMap[mapX][mapY] > 0)
        {
          hit = 1;
        }
      }

      if (side == 0)
      {
        perpWallDist = (sideDistX - deltaDistX);
      }
      else
      {
        perpWallDist = (sideDistY - deltaDistY);
      }

      int lineHeight = (int)(screenHeight / perpWallDist);

      int drawStart = -lineHeight / 2 + screenHeight / 2;
      if (drawStart < 0)
      {
        drawStart = 0;
      }
      int drawEnd = lineHeight / 2 + screenHeight / 2;
      if (drawEnd >= screenHeight)
      {
        drawEnd = screenHeight - 1;
      }

      SDL_Color color;
      switch (worldMap[mapX][mapY])
      {
      case 1:
        color.r = 255;
        color.g = 0;
        color.b = 0;
        color.a = 255;
        break;
      case 2:
        color.r = 0;
        color.g = 255;
        color.b = 0;
        color.a = 255;
        break;
      case 3:
        color.r = 0;
        color.g = 0;
        color.b = 255;
        color.a = 255;
        break;
      case 4:
        color.r = 255;
        color.g = 255;
        color.b = 255;
        color.a = 255;
        break;
      default:
        color.r = 255;
        color.g = 255;
        color.b = 0;
        color.a = 255;
        break;
      }

      if (side == 1)
      {
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
      }
      drawVerticalLine(renderer, x, drawStart, drawEnd, color);
    }
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
