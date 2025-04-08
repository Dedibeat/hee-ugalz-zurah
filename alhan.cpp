#include <bits/stdc++.h>
#include<GL/freeglut.h>
using namespace std;

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


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(3.0f);
    glColor3f(0.0f, 0.0f, 0.0);

    for(int j = -27; j < 30; j += 6) {
        int y = static_cast<float>(j);
        glBegin(GL_LINES);
        for(int i = -30; i < 31; i += 6) {
            float x = static_cast<float>(i);
            for(int k = -1; k < 2; k += 2) {
                glVertex2f(0.5f * k + x, 0.0f + y); glVertex2f(0.5f * k + x, 1.0f + y);
                glVertex2f(0.5f * k + x, 1.0f + y); glVertex2f(2.5f * k + x, 1.0f + y);
                glVertex2f(2.5f * k + x, 1.0f + y); glVertex2f(2.5f * k + x, 2.0f + y);
            }
            // alhan hee ni bosoo tenhelegiin daguu tegsh hemtei uchir neg taliig ni zuraad
            // x - iin coordinatuudiin temdgiig solij dahin nogoo taliin zurj baina.
            glVertex2f(-2.5f + x, 2.0f + y); glVertex2f(2.5f + x, 2.0f + y);
            glVertex2f(x + 0.5, 0.0f + y); glVertex2f(x + 5.5, 0.0f + y);
        }
        // ug uildiin 11 udaa davtaj baina, uuruur helvel 1 shuluun ni 11 - n alhan heetei baina
        for(int i = -1; i < 4; i += 4) {
            float x = static_cast<float>(i);
            glVertex2f(-30.0f, x + y); glVertex2f(30.0f, x + y);
        }
        // alhan heenii deetliin bolon dootliin urt shuluuniig zurj baigaa heseg 
        glEnd();
    }
    // ug uildiin 10 udaa hiine
    glEnd();


    static bool screenshotSaved = false;
    if (!screenshotSaved)
    {
        glFinish();
        saveScreenshotBMP("alhan.bmp", 200, 200);
        screenshotSaved = true;
    }
    
    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-30.0, 30.0, -30.0, 30.0); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(200, 200);
    glutCreateWindow("Алхан хээ");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}


// Алгоритм :
// Алхан хээ нь нийт 8 цэгээс тогтох ба эдгээр нь 4 ба 4-өөрөө Y тэнхлэгийн дагуу тэгш хэмтэй байна.
// Иймд 4 цэгийг зураад, үлдсэн 4 цэгийг өмнө зурсан 4 цэгийн X тэнхлэгийн координатын тэмдгийг солих аргаар шийдэж болно.
// Эхний 4 цэгийн координатууд нь: (0.5, 0), (0.5, 1), (2.5, 1), (2.5, 2)
// Үүний дараа бид 1 ширхэг алхан хээтэй болох ба уг алхан хээг X тэнхлэгийн дагуу 6 нүдээр зөөж түүнтэй холбогдсон дахин 1 шинэ алхан хээг зурна.
// Үүнийг олон удаа 1 мөр болон баганаар давтан хийх замаар нийт хээг гарган авсан болно.
