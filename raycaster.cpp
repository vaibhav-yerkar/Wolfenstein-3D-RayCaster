#include <iostream>
#include <math.h>

#include <SDL.h>

#define mapWidht 10 
#define mapHeight 10 
#define mapUnitSize 64
#define windowWidth 1280
#define windowHeight 640

int map[] = {
  1,1,1,1,1,1,1,1,1,1,
  1,0,1,0,0,0,0,0,0,1,
  1,0,1,0,0,0,0,0,0,1,
  1,0,1,0,0,0,0,0,0,1,
  1,0,0,0,0,0,1,0,0,1,
  1,0,0,0,0,0,1,0,0,1,
  1,0,0,0,0,0,1,0,0,1,
  1,0,0,0,0,0,1,0,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1	
};

float degToRad(int a) { return a*M_PI/180.0; }
int FixAng(int a) 
{ 
  if (a > 359){ a-=360; }
  if (a < 0){ a+=360; }
  return a;
}

float posX, posY, posDX, posDY, posAng;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void drawMap2D()
{
  int x, y;
  for(y = 0; y < mapHeight ; y++)
  {
    for(x = 0; x < mapWidht; x++)
    {
      if (map[y * mapWidht+x] == 1)
      {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }
      SDL_Rect rect = {x * mapUnitSize+1, y * mapUnitSize+1, mapUnitSize-1, mapUnitSize-1};
      SDL_RenderFillRect(renderer, &rect);
    }
  }
}

void drawPlayer2D()
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  SDL_Rect playerRect = { (int)posX-4, (int)posY-4 , 8, 8};
  SDL_RenderFillRect(renderer, &playerRect);

  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  SDL_RenderDrawLine(renderer, (int)posX, (int)posY, (int)(posX+posDX*20), (int)(posY+posDY*20));
}

void drawRay2D()
{
  int ray, mx, my, mp, dof, side;
  float vx, vy, rayX, rayY, rayAng, xOff, yOff, distV, distH;

  rayAng = FixAng(posAng+30);

  for(ray=0; ray < 60; ray++)
  {
    dof = 0, side =0, distV = 100000;
    float Tan = tan(degToRad(rayAng));

    if (cos(degToRad(rayAng))> 0.001)
    {
      rayX = (((int)posX >> 6) << 6) + mapUnitSize;
      rayY = (posX - rayX) * Tan + posY;
      xOff = mapUnitSize;
      yOff = -xOff * Tan;
    }
    else if (cos(degToRad(rayAng)) < -0.001)
    {
      rayX = (((int)posX >>6) << 6) - 0.0001 ;
      rayY = (posX - rayX) * Tan + posY;
      xOff = -mapUnitSize;
      yOff = -xOff * Tan;
    }
    else
    {
      rayX = posX;
      rayY = posY;
      dof = mapWidht;
    }

    while (dof < mapWidht)
    {
      mx = (int)(rayX) >> 6;
      my = (int)(rayY) >> 6;
      mp = my * mapWidht + mx;
      if (mp > 0 && mp < mapWidht * mapHeight && map[mp] == 1)
      {
        dof = mapWidht;
        distV = cos(degToRad(rayAng))*(rayX-posX) - sin(degToRad(rayAng))*(rayY-posY);
      }
      else
      {
        rayX += xOff;
        rayY += yOff;
        dof+=1;
      }
    }

    vx = rayX;
    vy = rayY;

    dof = 0;
    distH = 100000;
    Tan = 1.0/Tan;
    
    if(sin(degToRad(rayAng))> 0.001)
    {
      rayY = (((int)posY >> 6) << 6) - 0.0001;
      rayX = (posY - rayY) * Tan + posX;
      yOff = -mapUnitSize;
      xOff = -yOff * Tan;
    }
    else if (sin(degToRad(rayAng)) < -0.001)
    {
      rayY = (((int) posY >> 6) << 6) + mapUnitSize;
      rayX = (posY - rayY) * Tan + posX;
      yOff = mapUnitSize;
      xOff = -yOff * Tan;
    }
    else
    {
      rayY = posY;
      rayX = posX;
      dof = mapWidht;
    }

    while (dof < mapWidht)
    {
      mx = (int)(rayX) >> 6;
      my = (int)(rayY) >> 6;
      mp = my * mapWidht + mx;
      if (mp > 0 && mp < mapWidht * mapHeight && map[mp] == 1)
      {
        dof = mapWidht;
        distH = cos(degToRad(rayAng))*(rayX-posX) - sin(degToRad(rayAng))*(rayY-posY);
      }
      else
      {
        rayX += xOff;
        rayY += yOff;
        dof+=1;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
    if (distV < distH)
    {
      rayX = vx;
      rayY = vy;
      distH = distV;
      SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
    }

    SDL_RenderDrawLine(renderer, (int)posX, (int)posY, (int)rayX, (int)rayY);

    int ca = FixAng(posAng - rayAng);
    distH = distH * cos(degToRad(ca));
    int render_width = (int) windowWidth/3;
    int render_height = (int) windowHeight/3;
    int lineH = (mapUnitSize * render_width )/ (int) distH;
    if(lineH > render_width) { lineH = render_width; }
    int lineOff = render_height -(lineH >> 1);

    SDL_Rect wallRect = {ray * mapWidht + render_width+render_height, lineOff, mapWidht, lineH};
    SDL_RenderFillRect(renderer, &wallRect);

    rayAng = FixAng(rayAng - 1);

  }
}

int main(int argc, char* argv[])
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "Failed to initialise SDL! SDL Error : " << SDL_GetError() << std::endl;
    return -1;
  }
  
  window = SDL_CreateWindow("Raycasting",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      windowWidth, windowHeight,
      SDL_WINDOW_SHOWN);
  if (!window)
  {
    std::cerr << "Failed to create winodw! SDL Error : "<<SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }
  
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(!renderer)
  {
    std::cerr << "Failed to create renderer! SDL Error : "<< SDL_GetError() <<std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  posX = 150, posY = 400, posAng = 90;
  posDX = cos(degToRad(posAng));
  posDY = -sin(degToRad(posAng));

  bool running = true;
  while(running)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = false;
      }
      const Uint8* keyState = SDL_GetKeyboardState(NULL);

      if(keyState[SDL_SCANCODE_W])
      {
        posX += posDX * 5;
        posY += posDY * 5;
      }
      if(keyState[SDL_SCANCODE_S])
      {
        posX -= posDX * 5;
        posY -= posDY * 5;
      }
      if(keyState[SDL_SCANCODE_A])
      {
        posAng += 5;
        posAng = FixAng(posAng);
        posDX = cos(degToRad(posAng));
        posDY = -sin(degToRad(posAng));
      }
      if(keyState[SDL_SCANCODE_D])
      {
        posAng -= 5;
        posAng = FixAng(posAng);
        posDX = cos(degToRad(posAng));
        posDY = -sin(degToRad(posAng));
      }
    }
    SDL_SetRenderDrawColor(renderer, 77, 77, 77, 255);
    SDL_RenderClear(renderer);

    drawMap2D();
    drawPlayer2D();
    drawRay2D();

    SDL_RenderPresent(renderer);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
