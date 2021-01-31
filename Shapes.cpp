#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <windows.h>

const double pi = 355./113.;

const int dimX = 50;
const int dimY = dimX / 2;

const char* glyph = " *";

void Clear(bool grid[dimX][dimY]);
void Render(bool grid[dimX][dimY]);

void Line(bool grid[dimX][dimY], float o, float x1, float y1, float x2, float y2 );
void Circle(bool grid[dimX][dimY], int vert, float r, float pos, double alpha);
void RegularPoligon(bool grid[dimX][dimY], int vert, float r, float pos, double alpha);

float Null(float& a) {}
double Null(double& a) {}   

float ArmonicZoom(float& r) { r += cos(clock() * 0.002);    }
float LinearZoom(float& r)  {r += 1; }

float ArmonicMove(float& pos) { pos += sin(clock() * 0.002);   }

double LinearRotation(double& alpha) { alpha += 0.1; }

int main() {
    while(true) {
        bool grid[dimX][dimY];
        float r = 0; 
        int c, vert; 

        void (*Shape)(bool [][dimY], int, float, float, double);

        printf("Scegli il raggio: \n");
        scanf("%f", &r);

        printf("Scegli il numero di vertici della figura: ");
        scanf("%d", &vert);
        Shape = ( vert< 2  || vert > 10) ? Circle : RegularPoligon ;

        float (*Zoom)(float&);
        printf("Scegli lo zoom:\n0. None\n1. Linear\n2.Armonic\n");
        scanf("%d", &c);
        switch(c) {
            default:    Zoom = Null;                break;
            case 1:     Zoom = LinearZoom;          break;
            case 2:     Zoom = ArmonicZoom;     break;
        }

        float (*Move)(float&);
        printf("Scegli il movimento:\n0. None\n1. Armonic\n");
        scanf("%d", &c);
        switch(c) {
            default:    Move = Null;                break;
            case 1:     Move = ArmonicMove;         break;
        }

        double (*Rotation)(double&);
        printf("Scegli la rotazione:\n0. None\n1. Linear\n");
        scanf("%d", &c);
        switch(c) {
            default:    Rotation = Null;            break;
            case 1:     Rotation = LinearRotation;  break;
        }

        float pos = dimX/2;
        double alpha = 0;    

        bool a = true;
        while(a) {
            Clear(grid);
            
            Zoom(r) ;
            Move(pos);
            Rotation(alpha);

            Shape(grid, vert, r, pos, alpha);
            system("cls");
            Render(grid);

            if (GetKeyState(VK_ESCAPE) & 0x8000) a = false; 
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

void Circle(bool grid[dimX][dimY], int vert, float r, float pos, double alpha ) {
    for(double alpha = 0 ; alpha <= (2 * pi) ; alpha += 0.01) {
        int x = pos + r * cos(alpha);
        int y = (pos + r * sin(alpha)) / 2;

        if(x >= 0 && x < dimX && y >= 0 && y < dimY)
            grid[x][y] = true;
    }
}

float Max(float a, float b) { return a > b ? a : b; }
float Min(float a, float b) { return a < b ? a : b; }

void Line(bool grid[dimX][dimY], float o, float x1, float y1, float x2, float y2 ) {
    if(round(x1) != round(x2) ) {
        float m = (y2 - y1) / (x2 - x1);
        for(float x = Min(x1, x2) ; x <= Max(x1, x2) ; x += 0.1 ) {
            float y = (m * (x - x1)) + y1; 
            grid[(int)(x + o)][(int)(y + o) / 2] = true;
        }
    } else {
        for(float y = Min(y1, y2) ; y <= Max(y1, y2) ; y += 0.1) {
            grid[(int)(x1 + o)][(int)(y + o) / 2] = true;
        }
    }
}

void RegularPoligon(bool grid[dimX][dimY], int vert, float r, float pos, double alpha ) {
    float origin0 = dimX / 2;

    for(int i = 0 ; i < vert ; ++i) {
        float x1 = r * cos(alpha), y1 = r * sin(alpha);
        alpha += (2. * pi / vert);
        float x2 = r * cos(alpha), y2 = r * sin(alpha);
        
        Line(grid, pos, x1, y1, x2, y2);
    }
} 