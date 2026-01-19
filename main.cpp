#include <SDL/SDL.h>
#include "komlex.h"
#include "grafika.h"
#include <stdio.h>
#include <math.h>
#include <pthread.h>
typedef struct
{
    double x,y,zoom;
    int x1,x2,y1,y2;
}

task_p;

void* funkce(void*p){


    task_p * task = (task_p*)p;
    for(int x = task->x1; x <task->x2; x++   )
    {
        for(int y = task->y1;y <task->y2;y++   )
        {
            float xm = ((x-400)*0.01/task->zoom) + task->x;
            float ym = ((y-300)*0.01/task->zoom) + task->y;
            Komplex c (xm,ym);
            Komplex z;

            int s;
            for(s=0 ;s <100 ;s++)
            {
                z= z*z+c;
                if(z.absolutnihodnota() > 4) break;


            }
            Obrazovka* obrazovka = Obrazovka::instance();
            Uint32 pixel;
            if(s==100)
                pixel = SDL_MapRGB(obrazovka->screen->format, 0, 0, 0);


            else{
                pixel = SDL_MapRGB(obrazovka->screen->format,z.absolutnihodnota()*2.5 ,0,s*sqrt(z.absolutnihodnota()));

            }
            obrazovka->putpixel(x, y, pixel);

        }
    }

    pthread_exit(NULL);

}

int main(int argc, char** argv)

{
    Obrazovka* obrazovka = Obrazovka::instance();
    obrazovka->inicializuj(800, 600, 0, 0);
    Pismo cisla;
    cisla.nacti("cisla.png", "+-0123456789");
    Pismo pismena;

    pismena.nacti("pismena.png","qwertzuiopasdfghjklyxcvbnm");


    float zoom = 1;
    float xs = 0;
    float ys =0;

    while(1)
    {
        obrazovka->smaz();

        /* zacatek kresleni */
        pthread_t threads[4];
        task_p tasks[4];

        for(int i = 0;i<4;i++){
            tasks[i].zoom = zoom;
            tasks[i].x = xs;
            tasks[i].y=ys;
            tasks[i].x1=0;
            tasks[i].x2=800;
            tasks[i].y1=i*150;
            tasks[i].y2=(i+1)*150;
            pthread_create(&threads[i],NULL,funkce,(void*)(&tasks[i]));
        }
        for(int i = 0;i<4;i++){
            pthread_join(threads[i],NULL);
        }
        /* konec kresleni */
        obrazovka->aktualizuj();

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_RIGHT:
                    xs+=0.25/zoom;
                    break;
                case SDLK_LEFT:
                    xs-=0.25/zoom;
                    break;
                case SDLK_UP:
                    ys-=0.25/zoom;
                    break;
                case SDLK_DOWN:
                    ys+=0.25/zoom;
                    break;
                case SDLK_KP_PLUS:
                    zoom+=1;

                    break;
                case SDLK_KP_MINUS:
                    zoom-=1;
                    
                    break;
                case SDLK_ESCAPE:
                    SDL_Quit();
                    return 0;
                }
                break;
            }
        }
    }
}
