#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#endif

const double pi = 355./113.;

const int dimX = 50;
const int dimY = dimX / 2;

const int default_pos_x = dimX / 2;
const int default_pos_y = dimY;

const char* glyph = " *";

float initial_radius;

void Clear(bool grid[dimX][dimY]);
void Render(bool grid[dimX][dimY]);

void Line(bool grid[dimX][dimY], float ox, float oy, float x1, float y1, float x2, float y2 );
void Circle(bool grid[dimX][dimY], int vert, float r,  float posX, float posY, double alpha);
void RegularPoligon(bool grid[dimX][dimY], int vert, float r,  float posX, float posY, double alpha);

float Null(float& a) {}
double Null(double& a) {}   

float ArmonicZoom(float& r) { r += cos(clock() * 0.002);    }
float LinearZoom(float& r)  {r += 1; }
float PopZoom(float& r) { r = r < (initial_radius + 15) ? r += r * 0.1 : initial_radius; }

float ArmonicMove(float& pos) { pos += sin(clock() * 0.002);   }

double LinearRotation(double& alpha) { alpha += 0.1; }
double ArmonicRotation(double& alpha) { alpha += cos(clock() * 0.002) * 0.3; }

int main() {
    printf("Deloped by Simone Riccio\n");
    printf("This little program will draw you in the console every regular poligon possible to that resolution\n");
    printf("and you can also choose some effects to apply on that. \n");
    printf("The great thing is that everything you see is only made by printing letter.\n \n");

    while(true) {
        bool grid[dimX][dimY]; 
        int c, vert; 

        void (*Shape)(bool [][dimY], int, float, float, float, double);

        printf("Choose the radius: \n");
        scanf("%f", &initial_radius);

        printf("Choose the number of vertices of the poligon: ");
        scanf("%d", &vert);
        Shape = ( vert< 2  || vert > 10) ? Circle : RegularPoligon ;

        float (*Zoom)(float&);
        printf("Choose the way the figure changes size: \n0. None\n1. Linear\n2.Armonic\n3.Pop");
        scanf("%d", &c);
        switch(c) {
            default:    Zoom = Null;                break;
            case 1:     Zoom = LinearZoom;          break;
            case 2:     Zoom = ArmonicZoom;         break;
            case 3:     Zoom = PopZoom;             break;
        }

        float (*MoveX)(float&);
        printf("Choose the way the figure moves on the X axis: \n0. None\n1. Armonic\n");
        scanf("%d", &c);
        switch(c) {
            default:    MoveX = Null;                break;
            case 1:     MoveX = ArmonicMove;         break;
        }

        float (*MoveY)(float&);
        printf("Choose the way the figure moves on the Y axis: \n0. None\n1. Armonic\n");
        scanf("%d", &c);
        switch(c) {
            default:    MoveY = Null;                break;
            case 1:     MoveY = ArmonicMove;         break;
        }

        double (*Rotation)(double&);
        printf("Choose the way the figure rotates on his z axis: \n0. None\n1. Linear\n2.Armonic\n");
        scanf("%d", &c);
        switch(c) {
            default:    Rotation = Null;            break;
            case 1:     Rotation = LinearRotation;  break;
            case 2:     Rotation = ArmonicRotation; break;
        }

        float r = initial_radius;
        float posX = default_pos_x, posY = default_pos_y;
        double alpha = 0;    

        bool a = true;
        while(a) {
            Clear(grid);
            
            Zoom(r) ;
            MoveX(posX);
            MoveY(posY);
            Rotation(alpha);

            Shape(grid, vert, r, posX, posY, alpha);
            system("cls");
            Render(grid);

            #ifdef _WIN32
            if (GetKeyState(VK_ESCAPE) & 0x8000) a = false; 
            #endif
        }
    }
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

void Circle(bool grid[dimX][dimY], int vert, float r, float posX, float posY, double alpha ) {
    for(double alpha = 0 ; alpha <= (2 * pi) ; alpha += 0.01) {
        int x = posX + r * cos(alpha);
        int y = (posY + r * sin(alpha)) / 2;

        if(x >= 0 && x < dimX && y >= 0 && y < dimY)
            grid[x][y] = true;
    }
}

float Max(float a, float b) { return a > b ? a : b; }
float Min(float a, float b) { return a < b ? a : b; }

void Line(bool grid[dimX][dimY], float ox, float oy, float x1, float y1, float x2, float y2 ) {
    if(round(x1) != round(x2) ) {
        float m = (y2 - y1) / (x2 - x1);
        for(float x = Min(x1, x2) ; x <= Max(x1, x2) ; x += 0.1 ) {
            float y = (m * (x - x1)) + y1; 
            grid[(int)(x + ox)][(int)(y + oy) / 2] = true;
        }
    } else {
        for(float y = Min(y1, y2) ; y <= Max(y1, y2) ; y += 0.1) {
            grid[(int)(x1 + ox)][(int)(y + oy) / 2] = true;
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