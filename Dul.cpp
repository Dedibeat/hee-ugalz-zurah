#include <GL/freeglut.h>
#include <bits/stdc++.h>
using namespace std;
using cd = complex<double>;
const cd i(0, 1); // хийсвэр тоо i
const double PI = acos(-1);

const int WINDOW_WIDTH = 200;
const int WINDOW_HEIGHT = 200;

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
    for (int i = 1; i < 1000; i++)
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
    pen = cd(0, -50);
    dir = cd(1, 0);
    rotate(cd(0, 0), -PI);
    rotate(cd(0, -25), -PI);
    rotate(cd(0, 0), -PI * 0.85);
    rotate(cd(-60, 130), PI * 0.6);
    rotate(cd(50, 40) + pen, -PI * 0.45);
    rotate(cd(-40, 30) + pen, PI * 0.4);
    cout << pen.real() << " " << pen.imag() << "\n";
    save(pen);
}

// Функц: OpenGL пикселүүдийг уншиж BMP файлд бичих
// Энэ хувилбараар өгөгдлийг өөрчлөхдөө пикселүүдийг BGR болгож хөрвүүлж хэвлэнэ,
// гэхдээ одоогийн байрлал (байршил) ба хаалт үгүй тул эхийн дарааллыг өөрчлөхгүй.
void saveScreenshotBMP(const char* filename, int width, int height)
{
    // BMP файлд мөр бүр 4-байт-ын үржлийг шаарддаг.
    int rowSize = (3 * width + 3) & (~3);
    int imageSize = rowSize * height;
    int fileSize = 14 + 40 + imageSize; // 14: BMP Header, 40: DIB Header

    // BMP файлын толгой (Header)
    unsigned char bmpFileHeader[14] = {
        'B', 'M',              // Signature "BM"
        0, 0, 0, 0,            // File size (to be filled)
        0, 0,                  // Reserved1
        0, 0,                  // Reserved2
        54, 0, 0, 0            // Offset to pixel data (54 bytes)
    };

    unsigned char bmpInfoHeader[40] = {
        40, 0, 0, 0,           // DIB header size (40 bytes)
        0, 0, 0, 0,            // Width (filled later)
        0, 0, 0, 0,            // Height (filled later)
        1, 0,                  // Planes (must be 1)
        24, 0,                 // Bits per pixel (24 for RGB)
        0, 0, 0, 0,            // Compression (none)
        0, 0, 0, 0,            // Image size (0 for uncompressed)
        0, 0, 0, 0,            // X pixels per meter
        0, 0, 0, 0,            // Y pixels per meter
        0, 0, 0, 0,            // Total colors (0 = default)
        0, 0, 0, 0             // Important colors (0 = all)
    };

    // Fill in file size.
    bmpFileHeader[ 2] = (unsigned char)(fileSize      );
    bmpFileHeader[ 3] = (unsigned char)(fileSize >>  8);
    bmpFileHeader[ 4] = (unsigned char)(fileSize >> 16);
    bmpFileHeader[ 5] = (unsigned char)(fileSize >> 24);

    // Fill in width and height in bmpInfoHeader.
    bmpInfoHeader[ 4] = (unsigned char)(width      );
    bmpInfoHeader[ 5] = (unsigned char)(width >>  8);
    bmpInfoHeader[ 6] = (unsigned char)(width >> 16);
    bmpInfoHeader[ 7] = (unsigned char)(width >> 24);

    bmpInfoHeader[ 8] = (unsigned char)(height      );
    bmpInfoHeader[ 9] = (unsigned char)(height >>  8);
    bmpInfoHeader[10] = (unsigned char)(height >> 16);
    bmpInfoHeader[11] = (unsigned char)(height >> 24);

    // Fill in image size in bmpInfoHeader.
    bmpInfoHeader[20] = (unsigned char)(imageSize      );
    bmpInfoHeader[21] = (unsigned char)(imageSize >>  8);
    bmpInfoHeader[22] = (unsigned char)(imageSize >> 16);
    bmpInfoHeader[23] = (unsigned char)(imageSize >> 24);

    // Allocate memory for pixel data (3 channels: RGB)
    unsigned char* pixels = new unsigned char[width * height * 3];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    // Ensure all drawing commands have completed
    glFinish();
    // Read the pixels from the back buffer (image is already bottom-up as needed)
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Convert pixel data to BMP format (swap RGB to BGR, but do not flip vertically)
    unsigned char* bmpData = new unsigned char[imageSize];
    memset(bmpData, 0, imageSize);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int srcIndex = (y * width + x) * 3;
            int dstIndex = (y * rowSize) + (x * 3);
            bmpData[dstIndex + 0] = pixels[srcIndex + 2]; // Blue channel
            bmpData[dstIndex + 1] = pixels[srcIndex + 1]; // Green channel
            bmpData[dstIndex + 2] = pixels[srcIndex + 0]; // Red channel
        }
    }

    // Write the BMP file using standard C++ libraries
    ofstream out(filename, ios::binary);
    if (out)
    {
        out.write(reinterpret_cast<char*>(bmpFileHeader), sizeof(bmpFileHeader));
        out.write(reinterpret_cast<char*>(bmpInfoHeader), sizeof(bmpInfoHeader));
        out.write(reinterpret_cast<char*>(bmpData), imageSize);
        out.close();
        cout << "Saved screenshot as " << filename << "\n";
    }
    else
    {
        cerr << "Error: could not write " << filename << "\n";
    }

    delete[] pixels;
    delete[] bmpData;
}

// Зургийг зурах буюу дэлгэцэнд харуулах функц
void display() 
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluOrtho2D(-400, 400, -400, 400);
    
    // 135 градусаар нар буруу эргүүлнэ
    glRotatef(-3, 0, 0, 1);
    
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    for (auto [x, y] : path)
        glVertex2d(x, y);
    glEnd();
    
    // Capture the screenshot from the back buffer BEFORE swapping.
    static bool screenshotSaved = false;
    if (!screenshotSaved)
    {
        glFinish();
        saveScreenshotBMP("dul.bmp", WINDOW_WIDTH, WINDOW_HEIGHT);
        screenshotSaved = true;
    }
    
    glutSwapBuffers();
}

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("DUL HEE");

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(10.0f);

    find_path();

    glutDisplayFunc(display);
    glClearColor(0, 0, 0, 1);
    
    glutMainLoop();
    return 0;
}
