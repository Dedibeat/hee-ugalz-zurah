#define GL_SILENCE_DEPRECATION
#include <GL/freeglut.h>
#include <bits/stdc++.h>

using namespace std;

const float PI = 3.14159265358979323846;
const float R = 2.0f; // radius
const float SPACING = 3 * R; // 2 has heenii hoorondoh za radius tai tentsuu baina
                             // harin 1 has heenii urt ni 2 * radius baina
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
void drawSymbol(float cx, float cy, float offsetAngleDeg) {
    float offsetRad = offsetAngleDeg * PI / 180.0;
    float diag = R * sqrt(2);
    
    for (int i = 0; i < 4; ++i) {
        // alhan hee ni 4-n hesgees togtoh ba tedgeeriin hoorondiin ontsgiin zoruu ni 360 / 4 = 90 baina
        float angle = (i * 90.0) * PI / 180.0 + offsetRad;
        float x1 = cx + R * cos(angle);
        float y1 = cy + R * sin(angle);

        glVertex2f(cx, cy);        
        glVertex2f(x1, y1);

        float x2 = cx + diag * cos(angle + PI / 4);
        float y2 = cy + diag * sin(angle + PI / 4);

        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(6.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            float cx = (col - 1.5f) * SPACING;
            float cy = -1.0f * (row - 1.5f) * SPACING;
            float angle = 6.0f * (4 * row + col); 
            drawSymbol(cx, cy, angle);
        }
    }
    // niit 4 * 4 = 16 has hee zursan ba tedgeer ni umnuhuuse 90 / (16 - 1) = 6 gradus-iin zoruutei baina.
    glEnd();

    static bool screenshotSaved = false;
    if (!screenshotSaved)
    {
        glFinish();
        saveScreenshotBMP("has.bmp", 200, 200);
        screenshotSaved = true;
    }
    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-6 * R, 6 * R, -6 * R, 6 * R);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(200, 200);
    glutCreateWindow("Хас хээ");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

// Алгоритм:
// Координатын эхлэл цэгээс `radius` урттай, X тэнхлэгтэй `angle` өнцөг үүсгэсэн цэгийн координат нь:
//     {radius × cos(angle), radius × sin(angle)} байна.
//
// Хас хээ нь 4 хэсгээс бүрдэх ба эдгээр нь хоорондоо 90 градусын зөрүүтэй байрлана.
// Өөрөөр хэлбэл, нэг хэсгийг зурсан тохиолдолд for давталт ашиглан үлдсэн хэсгүүдийг амархан зурж болно.
//
// Одоо хас хээний нэг хэсгийг авч үзье:
// Энэ хэсэг нь хоёр цэгээс бүрдэх бөгөөд эхний цэг нь:
//     - X тэнхлэгтэй `angle` өнцөг үүсгэн байрлаж,
//     - Координатын эхээс `radius` зайд байрлана.
//
// Харин хоёр дахь цэг нь:
//     - Өнцөг нь `angle + 45 градус`,
//     - Зай нь `sqrt(2) × radius` байна.
//
// Үлдсэн 3 хэсгийн цэгүүдийг `angle`-ийг 90 градусаар нэмэгдүүлэн тооцоолж гаргана.
