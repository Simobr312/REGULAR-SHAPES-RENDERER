#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#define  CLEAR "cls"
#endif

const double pi = 355./113.;

const int dimX = 100;
const int dimY = dimX / 2;
const char* glyph = " *";

clock_t initTime, oldTime;
double dt, elapsedTime;

const int default_pos_x = dimX / 2, default_pos_y = dimY;
float initial_radius;

void clear(bool grid[dimX][dimY]);
void render(bool grid[dimX][dimY]);

void line(bool grid[dimX][dimY], float ox, float oy, float x1, float y1, float x2, float y2 );
void circle(bool grid[dimX][dimY], int vert, float r,  float posX, float posY, double alpha);
void regularPoligon(bool grid[dimX][dimY], int vert, float r,  float posX, float posY, double alpha);

void armonicZoom(float& r)  { r += (cos(elapsedTime) * 10.)/initial_radius;  }
void linearZoom(float& r)   { r += (2 * dt); }
void popZoom(float& r)      { r = r < (initial_radius + 15) ? r += dt * (r * r / 100.) : initial_radius; }

void armonicMove(float& pos){ pos += cos(elapsedTime);   }

void linearRotation(double& alpha)  { alpha += (2 * dt); }
void armonicRotation(double& alpha) { alpha += cos(elapsedTime) * 0.3; }

void calculateTime();

int main() {
    printf("Deloped by Simone Riccio\n");
    printf("This little program will draw you in the console every regular poligon possible to that resolution\n");
    printf("and you can also choose some effects to apply on that. \n");
    printf("The great thing is that everything you see is only made by printing letter.\n \n");

    bool isRunning = true;
    oldTime = clock();
    char buffer[dimX * dimY + 10];
    setbuf(stdout, buffer);
    
    do {
        bool grid[dimX][dimY]; 
        int c, vert; 

        void (*shape)(bool [][dimY], int, float, float, float, double);

        printf("Choose the radius: \n");
        fflush(stdout);
        scanf("%f", &initial_radius);

        printf("Choose the number of vertices of the poligon: ");
        fflush(stdout);
        scanf("%d", &vert);
        shape = ( vert< 2  || vert > 10) ? circle : regularPoligon ;

        void (*zoom)(float&);
        printf("Choose the way the figure changes size: \n0. None\n1. Linear\n2. Armonic\n3. Pop\n");
        fflush(stdout);
        scanf("%d", &c);
        switch(c) {
            default:    zoom = nullptr;             break;
            case 1:     zoom = linearZoom;          break;
            case 2:     zoom = armonicZoom;         break;
            case 3:     zoom = popZoom;             break;
        }

        void (*moveX)(float&);
        printf("Choose the way the figure moves on the X axis: \n0. None\n1. Armonic\n");
        fflush(stdout);
        scanf("%d", &c);
        switch(c) {
            default:    moveX = nullptr;            break;
            case 1:     moveX = armonicMove;        break;
        }

        void (*moveY)(float&);
        printf("Choose the way the figure moves on the Y axis: \n0. None\n1. Armonic\n");
        fflush(stdout);
        scanf("%d", &c);
        switch(c) {
            default:    moveY = nullptr;            break;
            case 1:     moveY = armonicMove;        break;
        }

        void (*rotation)(double&);
        printf("Choose the way the figure rotates on his z axis: \n0. None\n1. Linear\n2. Armonic\n");
        fflush(stdout);
        scanf("%d", &c);
        switch(c) {
            default:    rotation = nullptr;         break;
            case 1:     rotation = linearRotation;  break;
            case 2:     rotation = armonicRotation; break;
        }

        float r = initial_radius;
        float posX = default_pos_x, posY = default_pos_y;
        double alpha = 0;    
        bool a = true;
        initTime = clock();
        while(a) {
            calculateTime();
            clear(grid);
            
            if(zoom)        zoom(r) ;
            if(moveX)       moveX(posX);
            if(moveY)       moveY(posY);
            if(rotation)    rotation(alpha);

            shape(grid, vert, r, posX, posY, alpha);
            
            render(grid);

            #ifdef _WIN32
            if (GetKeyState(VK_ESCAPE) & 0x8000) a = false; 
            #endif
        }
    } while(isRunning);

    return 0;
}

void clear(bool grid[dimX][dimY]) {
    for(int y = 0 ; y < dimY ; ++y) {
            for(int x = 0 ; x < dimX ; ++x) {
                grid[x][y] = false;
        }
    }   
}

void render(bool output[dimX][dimY]) {
    system(CLEAR);
    for(int y = 0 ; y < dimY ; ++y) {
        for(int x = 0 ; x < dimX ; ++x) {
            putchar(glyph[output[x][y]]);
        }
        putchar('\n');
    }
    fflush(stdout);
}

bool isInRange(int x, int y) { return x >= 0 && x < dimX && y >= 0 && y < dimY;}

void circle(bool grid[dimX][dimY], int vert, float r, float posX, float posY, double alpha ) {
    for(alpha = 0 ; alpha <= (2 * pi) ; alpha += 0.01) {
        int x = posX + r * cos(alpha);
        int y = (posY + r * sin(alpha)) / 2;

        if(isInRange(x, y)) grid[x][y] = true;
    }
}

float max(float a, float b) { return a > b ? a : b; }
float min(float a, float b) { return a < b ? a : b; }

void line(bool grid[dimX][dimY], float ox, float oy, float x1, float y1, float x2, float y2 ) {

    float dX = x2 - x1;
    float dY = y2 - y1;
    
    if(abs(dX) > abs(dY)) {
        float m = dY / dX;
        float xi = min(x1, x2); float xf = max(x1, x2);

        for(float x = xi ; x <= xf ; x += 0.1f) {
            float y = m * (x - x1) + y1;

            int xp = ox + round(x), yp = (oy + round(y)) / 2;
            
            if(isInRange(xp, yp)) grid[xp][yp] = true;
        }
    } else {      
        float m = dX / dY;
        float yi = min(y1, y2); float yf = max(y1, y2);
        
        for(float y = yi; y <= yf ; y += 0.1) {
            float x = m * (y - y1) + x1;

            int xp = ox + round(x), yp = (oy + round(y)) / 2;

            if(isInRange(xp, yp))
                grid[xp][yp] = true;
        }
    }
}

void regularPoligon(bool grid[dimX][dimY], int vert, float r, float posX, float posY, double alpha ) {
    for(int i = 0 ; i < vert ; ++i) {
        float x1 = r * cos(alpha), y1 = r * sin(alpha);
        alpha += (2. * pi / vert);
        float x2 = r * cos(alpha), y2 = r * sin(alpha);
        
        line(grid, posX, posY, x1, y1, x2, y2);
    }
} 

void calculateTime() {
    elapsedTime = (clock() - initTime) / (double)CLOCKS_PER_SEC;
    dt = (clock() - oldTime) / (double)CLOCKS_PER_SEC;
    oldTime = clock();
}