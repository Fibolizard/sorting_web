#define WIN32_LEAN_AND_MEAN // Excluir APIs no esenciales de Windows



#include "raylib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <string.h>
#include <string>
#include <chrono>


using namespace std::chrono;

void swap(int* x_pointer, int* y_pointer)
{
    int temp = *x_pointer;
    *x_pointer = *y_pointer;
    *y_pointer = temp;
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 800;
    int sorting_speed = 120;

    InitWindow(screenWidth, screenHeight, "Visualizador de ordenamiento");
    InitAudioDevice();
    SetTargetFPS(sorting_speed);

    ///////Setup del vector///////
    std::vector<int> v(screenHeight);
    std::iota(std::begin(v), std::end(v), 0); //Me genera todos los números consecutivos iniciando desde 0
    auto rng = std::default_random_engine{}; //Un numero aleatorio
    std::shuffle(std::begin(v), std::end(v), rng); //Desordenar los numeros del vector
    int n = v.size(); //Tamaño del vector
    ////////////////////////////////
    Color col;
    int cur = 0, current = 0;
    bool bubble_start = false, done = false, menu = true, menu2 = false;
    bool insertion_start = false;
    bool pause = false;
    int comparisons = 0, arrAccess = 0;
    auto time_start = high_resolution_clock::now();
    auto time_end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(time_end - time_start);
    float select = 285;
    std::string comp = "Comparaciones= ";
    std::string arrayAccess = "Intercambios= ";
    std::string title = "";
    std::string sort_speed = "";
    std::string time_elapsed = "Tiempo transcurrido: ";
    Sound s = LoadSound("assets/blipSelect.wav");
    Sound s2 = LoadSound("assets/enter.wav");
    Sound s3 = LoadSound("assets/set_sound.wav");


    int j = 0, i = 0;
    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);

        /////////////
        /*MAIN MENU*/
        /////////////
     
        if (menu) {
            DrawText("Visualizador de\n ordenamiento!", screenWidth / 5, screenHeight / 2, 75, WHITE);
            DrawText("PRESIONA ENTER", screenWidth / 5, screenHeight - 100, 50, RED);


        }
        if (menu2) {
            for (i = 0; i < n; i++) {

                DrawRectangle(i, 0, 10, v[i], WHITE);
            }
            DrawRectangleLinesEx(Rectangle{ 60, select, 700, 50 }, 5, RED);
            DrawText("1. REORDENA EL VECTOR!", 70, 300, 25, RED);
            DrawText("2. ORDENAMIENTO POR BURBUJA", 70, 400, 25, RED);
            DrawText("3. ORDENAMIENTO POR INSERCION", 70, 500, 25, RED);
            DrawText("[ESPACIO] para pausar", 70, 650, 20, GREEN);
            DrawText("[DERECHA / IZQ] para cambiar velocidad", 70, 750, 20, GREEN);



        }
        if (menu2) {
            if (IsKeyPressed(KEY_S)) {
                select += 100;
                if (select > 485) {
                    select = 485;
                }
                PlaySound(s);
            }
            if (IsKeyPressed(KEY_W)) {
                select -= 100;
                if (select < 285) {
                    select = 285;

                }
                PlaySound(s);

                
            }
            if (IsKeyDown(KEY_ENTER)) {
                PlaySound(s2);
                switch ((int)select) {
                case 285:
                    rng = std::default_random_engine{};
                    std::shuffle(std::begin(v), std::end(v), rng);
                    bubble_start = false;
                    insertion_start = false;
                    done = false;
                    menu2 = true;
                    comparisons = 0;
                    arrAccess = 0;
                    cur = 0;
                    break;
                case 385:
                    bubble_start = true;
                    menu2 = false;
                    time_start = high_resolution_clock::now();
                    break;
                case 485:
                    time_start = high_resolution_clock::now();

                    break;
                }

            }
        
        }
        if (menu) {
            if (IsKeyPressed(KEY_ENTER)) {
                menu = false;
                menu2 = true;

            }
        }
      
            if (IsKeyPressed(KEY_ENTER)) {
              
                bubble_start = false;
                insertion_start = false;
                done = false;
                menu2 = true;
                comparisons = 0;
                arrAccess = 0;
                cur = 0;
            }
        
            if (IsKeyDown(KEY_ENTER)) {
                switch ((int)select) {
                case 285:
                    rng = std::default_random_engine{};
                    std::shuffle(std::begin(v), std::end(v), rng);
                    bubble_start = false;
                    insertion_start = false;
                    done = false;
                    menu2 = true;
                    comparisons = 0;
                    arrAccess = 0;
                    cur = 0;
                    break;
                case 385:
                    bubble_start = true;
                    menu2 = false;
                    time_start = high_resolution_clock::now();
                    break;
                case 485:
                    insertion_start = true;
                    menu2 = false;
                    time_start = high_resolution_clock::now();
                    

                    break;
                }
            }
            if (IsKeyPressed(KEY_LEFT)) {
                PlaySound(s3);
                sorting_speed -= 10;
                if (sorting_speed == 0) {
                    sorting_speed = 10;

                }
                SetTargetFPS(sorting_speed);
            }
            if (IsKeyPressed(KEY_RIGHT)) {
                PlaySound(s3);
                sorting_speed += 10;
                if (sorting_speed > 300) {
                    sorting_speed = 300;
                }

                SetTargetFPS(sorting_speed);
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                PlaySound(s2);
                rng = std::default_random_engine{};
                std::shuffle(std::begin(v), std::end(v), rng);
                bubble_start = false;
                insertion_start = false;
                done = false;
                menu2 = true;
                comparisons = 0;
                arrAccess = 0;
                cur = 0;
            }
        if (IsKeyPressed(KEY_SPACE)) {
            pause = !pause;
        }
        
        //ORDENAMIENTO POR INSERCION
        if (insertion_start && !done) {
            title = "Ordenamiento por insercion";
            sort_speed = "<- (-) " + std::to_string(sorting_speed) + " (+) ->";
            DrawText(comp.c_str(), 25, screenHeight - 25, 25, RED);
            DrawText(arrayAccess.c_str(), 25, screenHeight - 50, 25, RED);
            DrawText(time_elapsed.c_str(), 25, screenHeight - 75, 25, RED);
            DrawText(title.c_str(), 25, screenHeight - 125, 40, DARKGREEN);
            if (!pause) {
                if (cur < n) {
                    int key, j;
                    key = v[cur];
                    j = cur - 1;
                    while (j >= 0 && v[j] > key) {
                        comparisons++;
                        v[j + 1] = v[j];
                        j = j - 1;
                        arrAccess++;
                    }
                    v[j + 1] = key;
                    arrAccess++;
                    cur++;
                }
                else {
                    done = true;
                }
            }
            //render  
            for (int i = 0; i < n; i++) {
                col = (done) ? GREEN : WHITE;
                if (i == j + 1) col = RED;
                if (i == j) col = GREEN;
                DrawRectangle(i, 0, 1, v[i], col);
            }
            comp = "Comparaciones= " + std::to_string(comparisons);
            arrayAccess = "Accesos al array= " + std::to_string(arrAccess);
            time_end = high_resolution_clock::now();
            duration = duration_cast<milliseconds>(time_end - time_start);
            time_elapsed = "Tiempo transcurrido= " + std::to_string(duration.count()) + "ms";
            DrawText(sort_speed.c_str(), 25, 100, 30, RED);

        }
        //ORDENAMIENTO POR BURBUJA
        if (bubble_start && !done) {
            title = "Ordenamiento por burbuja";
            sort_speed = "<- (-) " + std::to_string(sorting_speed) + " (+) ->";
            DrawText(comp.c_str(), 25, screenHeight - 25, 25, RED);
            DrawText(arrayAccess.c_str(), 25, screenHeight - 50, 25, RED);
            DrawText(time_elapsed.c_str(), 25, screenHeight - 75, 25, RED);
            DrawText(title.c_str(), 25, screenHeight - 125, 40, DARKGREEN);
            if (!pause) {
                if (cur < n) {
                    for (j = 0; j < n - cur - 1; j++) {
                        comparisons++;
                        if (v[j] > v[j + 1]) {
                            arrAccess++;
                            swap(&v[j], &v[j + 1]);
                        }

                    }
                    cur++;
                }
                else {
                    done = true;
                    comp = cur;
                }

               
            }
            for (i = 0; i < n; i++) {
                col = (done) ? GREEN : WHITE;
                if (i == j - i) col = RED;
                if (i == j + 1) col = RED;
                if (i == j) col = RED;
                DrawRectangle(i, 0, 10, v[i], col);
            }
            comp = "Comparaciones= " + std::to_string(comparisons);
            arrayAccess = "Accesos al array= " + std::to_string(arrAccess);
            time_end = high_resolution_clock::now();
            duration = duration_cast<milliseconds>(time_end - time_start);
            time_elapsed = "Tiempo transcurrido= " + std::to_string(duration.count()) + "ms";
            DrawText(sort_speed.c_str(), 25, 100, 30, RED);


        }
        if (done) {
            DrawText(comp.c_str(), 25, screenHeight - 25, 25, RED);
            DrawText(arrayAccess.c_str(), 25, screenHeight - 50, 25, RED);
            DrawText(time_elapsed.c_str(), 25, screenHeight - 75, 25, RED);
            DrawText(title.c_str(), 25, screenHeight - 125, 40, DARKGREEN);
            for (int i = 0; i < n; i++) {

                DrawRectangle(i, 0, 25, v[i], GREEN);
            }
            DrawText("Presionar [RETROCESO] para volver al menu", 50, screenHeight / 2, 30, DARKGREEN);

        }
        
        if (pause) { //JUEGO PAUSADO
            DrawText("PAUSADO", screenWidth / 2, screenHeight / 2, 35, DARKGREEN);
        }
        EndDrawing();
        
    
    }
    CloseWindow();
    CloseAudioDevice();
    UnloadSound(s);
    for (int i = 0; i < n; i++)
    {
        std::cout << v[i] << std::endl;
    }
    return 0;


}
