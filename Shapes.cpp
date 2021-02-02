#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#define  CLEAR "cls"
#endif

const double pi = 355./113.;

const int dimX = 50;
const int dimY = dimX / 2;

clock_t initTime, oldTime;
double dt, elapsedTime;

const int default_pos_x = dimX / 2;
const int default_pos_y = dimY;

const char* glyph = " *";

float initial_radius;

void Clear(bool grid[dimX][dimY]);
void Render(bool grid[dimX][dimY]);

void Line(bool grid[dimX][dimY], float ox, float oy, float x1, float y1, float x2, float y2 );
void Circle(bool grid[dimX][dimY], int vert, float r,  float posX, float posY, double alpha);
void RegularPoligon(bool grid[dimX][dimY], int vert, float r,  float posX, float posY, double alpha);

void ArmonicZoom(float& r)  { r += (cos(elapsedTime) * 10.)/initial_radius;  }
void LinearZoom(float& r)   { r += (2 * dt); }
void PopZoom(float& r)      { r = r < (initial_radius + 15) ? r += dt * (r * r / 100.) : initial_radius; }

void ArmonicMove(float& pos){ pos += cos(elapsedTime);   }

void LinearRotation(double& alpha)  { alpha += (2 * dt); }
void ArmonicRotation(double& alpha) { alpha += cos(elapsedTime) * 0.3; }

void CalculateTime();

int main() {
    printf("Deloped by Simone Riccio\n");
    printf("This little program will draw you in the console every regular poligon possible to that resolution\n");
    printf("and you can also choose some effects to apply on that. \n");
    printf("The great thing is that everything you see is only made by printing letter.\n \n");

    bool isRunning = true;
    oldTime = clock();
    
    do {
        bool grid[dimX][dimY]; 
        int c, vert; 

        void (*Shape)(bool [][dimY], int, float, float, float, double);

        printf("Choose the radius: \n");
        scanf("%f", &initial_radius);

        printf("Choose the number of vertices of the poligon: ");
        scanf("%d", &vert);
        Shape = ( vert< 2  || vert > 10) ? Circle : RegularPoligon ;

        void (*Zoom)(float&);
        printf("Choose the way the figure changes size: \n0. None\n1. Linear\n2.Armonic\n3.Pop\n");
        scanf("%d", &c);
        switch(c) {
            default:    Zoom = nullptr;             break;
            case 1:     Zoom = LinearZoom;          break;
            case 2:     Zoom = ArmonicZoom;         break;
            case 3:     Zoom = PopZoom;             break;
        }

        void (*MoveX)(float&);
        printf("Choose the way the figure moves on the X axis: \n0. None\n1. Armonic\n");
        scanf("%d", &c);
        switch(c) {
            default:    MoveX = nullptr;            break;
            case 1:     MoveX = ArmonicMove;        break;
        }

        void (*MoveY)(float&);
        printf("Choose the way the figure moves on the Y axis: \n0. None\n1. Armonic\n");
        scanf("%d", &c);
        switch(c) {
            default:    MoveY = nullptr;            break;
            case 1:     MoveY = ArmonicMove;        break;
        }

        void (*Rotation)(double&);
        printf("Choose the way the figure rotates on his z axis: \n0. None\n1. Linear\n2.Armonic\n");
        scanf("%d", &c);
        switch(c) {
            default:    Rotation = nullptr;         break;
            case 1:     Rotation = LinearRotation;  break;
            case 2:     Rotation = ArmonicRotation; break;
        }

        float r = initial_radius;
        float posX = default_pos_x, posY = default_pos_y;
        double alpha = 0;    
        bool a = true;
        initTime = clock();
        while(a) {
            CalculateTime();
            Clear(grid);
            
            if(Zoom)        Zoom(r) ;
            if(MoveX)       MoveX(posX);
            if(MoveY)       MoveY(posY);
            if(Rotation)    Rotation(alpha);

            Shape(grid, vert, r, posX, posY, alpha);
            system(CLEAR);
            Render(grid);

            #ifdef _WIN32
            if (GetKeyState(VK_ESCAPE) & 0x8000) a = false; 
            #endif
        }
    } while(isRunning);

    return 0;
}

void Clear(bool grid[dimX][dimY]) {
    for(int y = 0 ; y < dimY ; ++y) {
            for(int x = 0 ; x < dimX ; ++x) {
                grid[x][y] = false;
        }
    }   
}

void Render(bool output[dimX][dimY]) {
    for(int y = 0 ; y < dimY ; ++y) {
        for(int x = 0 ; x < dimX ; ++x) {
            putchar(glyph[output[x][y]]);
        }
        putchar('\n');
    }
}

bool isInRange(int x, int y) { return x >= 0 && x < dimX && y >= 0 && y < dimY;}

void Circle(bool grid[dimX][dimY], int vert, float r, float posX, float posY, double alpha ) {
    for(alpha = 0 ; alpha <= (2 * pi) ; alpha += 0.01) {
        int x = posX + r * cos(alpha);
        int y = (posY + r * sin(alpha)) / 2;

        if(isInRange(x, y)) grid[x][y] = true;
    }
}

float Max(float a, float b) { return a > b ? a : b; }
float Min(float a, float b) { return a < b ? a : b; }

void Line(bool grid[dimX][dimY], float ox, float oy, float x1, float y1, float x2, float y2 ) {

    float dX = x2 - x1;
    float dY = y2 - y1;
    
    if(abs(dX) > abs(dY)) {
        float m = dY / dX;
        float xi = Min(x1, x2); float xf = Max(x1, x2);

        for(float x = xi ; x <= xf ; x += 0.1f) {
            float y = m * (x - x1) + y1;

            int xp = ox + round(x), yp = (oy + round(y)) / 2;
            
            if(isInRange(xp, yp)) grid[xp][yp] = true;
        }
    } else {      
        float m = dX / dY;
        float yi = Min(y1, y2); float yf = Max(y1, y2);
        
        for(float y = yi; y <= yf ; y += 0.1) {
            float x = m * (y - y1) + x1;

            int xp = ox + round(x), yp = (oy + round(y)) / 2;

            if(isInRange(xp, yp))
                grid[xp][yp] = true;
        }
    }
}

void RegularPoligon(bool grid[dimX][dimY], int vert, float r, float posX, float posY, double alpha ) {
    for(int i = 0 ; i < vert ; ++i) {
        float x1 = r * cos(alpha), y1 = r * sin(alpha);
        alpha += (2. * pi / vert);
        float x2 = r * cos(alpha), y2 = r * sin(alpha);
        
        Line(grid, posX, posY, x1, y1, x2, y2);
    }
} 

void CalculateTime() {
    elapsedTime = (clock() - initTime) / (double)CLOCKS_PER_SEC;
    dt = (clock() - oldTime) / (double)CLOCKS_PER_SEC;
    oldTime = clock();
}