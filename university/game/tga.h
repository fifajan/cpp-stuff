#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
  {
    GLubyte* imageData; // Содержит всю информацию о цвете изображения 
    GLuint  bpp;        // Содержит количество бит на пиксель
    GLuint width;       // Ширина изображения
    GLuint height;      // Высота изображения
    GLuint texID;       // идентификатор текстуры для использования совместно с glBindTexture
    GLuint type;        // Информация хранимая в * ImageData (GL_RGB или GL_RGBA) 

  } Texture;

typedef struct
  {
    GLubyte Header[12];   // Заголовок файла определяющий его тип
  } TGAHeader;
 
  typedef struct
  {
    GLubyte header[6];    // Содержит первые полезные 6 байт файла 
    GLuint bytesPerPixel; // Количество байт на пиксель (3 или 4)
    GLuint imageSize;     // Размер памяти необходимый для хранения изображения
    GLuint type;          // Тип изображения, GL_RGB или GL_RGBA
    GLuint Height;        // Высота изображения
    GLuint Width;         // Ширина изображения
    GLuint Bpp;           // Количество бит на пиксель (24 или 32
  } TGA;

TGAHeader tgaheader; // Используется для хранения заголовка файла
TGA tga;             // Используется для хранения информации о файле

// Заголовок несжатого TGA
GLubyte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
// Заголовок сжатого TGA
GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};
// Загружает несжатый файл
bool LoadUncompressedTGA(Texture *, char *, FILE *);
