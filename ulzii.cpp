#include <GL/freeglut.h>
#include <bits/stdc++.h>
using namespace std;
using cd = complex<double>;
const cd i(0, 1); // i - хийсвэр тоо
const double PI = acos(-1);

// Хээг зурах цэгүүдийн дараалал буюу зам
vector<pair<double, double>> path;

// Үзэг болон түүний чиглэл
cd pen(0, 0), dir(-1, 0);

// Үзэгний одоогийн байрлалыг хадгалах
void save(cd pt)
{
    path.emplace_back(pt.real(), pt.imag());
}

// Урагш unit нэгжээр алхах
void move_forw(double unit)
{
    save(pen);
    pen += dir * unit;
}

// Тодорхой төвийн(center) эргэн тойронд degree өнцгөөр эргэх
void rotate(cd center, double degree)
{
    save(pen);
    cd vec = pen - center;
    for(int i = 1; i < 1000; i++)
    {
        double angle = degree * i / 1000;
        cd pt = center + vec * exp(cd(0, angle));
        save(pt);
    }
    
    pen = center + vec * exp(cd(0, degree));
    dir *= exp(cd(0, degree));
}

// Хээг зурах замыг олдог функц
void find_path()
{
    pen = cd(-100, 100);
    move_forw(50);

    // үзгээс у буюу хийсвэр тэнхлэгээр 50 аар дээр цэг дээр эргүүлэх
    rotate(pen + cd(0, 50), -PI * 1.5);
    
    move_forw(300);
    // үзгээс x буюу бодит тэнхлэгээр 50 аар дээр цэг дээр эргүүлэх
    rotate(pen + cd(50, 0), PI);
    
    move_forw(300);
    rotate(pen + cd(50, 0), -PI);
    
    move_forw(300);
    rotate(pen + cd(50, 0), PI * 1.5);
    
    move_forw(300);
    rotate(pen + cd(0, 50), -PI);
    
    move_forw(300);
    rotate(pen + cd(0, 50), PI);
    
    move_forw(250);

    save(pen);
}

// Зургийг зурах буюу дэлгэцэнд харуулах функц
void display() 
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluOrtho2D(-400, 400, -400, 400);
    
    // 135 градусаар нар буруу эргүүлнэ
    glRotatef(135, 0, 0, 1);  

    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    for (auto [x, y] : path) 
        glVertex2d(x, y);
    glEnd();


    glutSwapBuffers();
}

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("УЛЗИЙ ХЭЭ");

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(10.0f);

    find_path();

    glutDisplayFunc(display);
    glClearColor(0, 0, 0, 1);

    
    glutMainLoop();
    return 8 >> 13;
}
