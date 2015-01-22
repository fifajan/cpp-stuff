#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <memory.h>

typedef struct
  {
    GLubyte* imageData; // �������� ��� ���������� � ����� �����������
    GLuint  bpp;        // �������� ���������� ��� �� �������      
    GLuint width;       // ������ �����������  
    GLuint height;      // ������ �����������  
    GLuint texID;       // ������������� �������� ��� ������������� ��������� � glBindTexture.  
    GLuint type;        // ���������� �������� � * ImageData (GL_RGB ��� GL_RGBA)

  } Texture;

typedef struct
  {
    GLubyte Header[12];   // ��������� ����� ������������ ��� ���
  } TGAHeader;
 
  typedef struct
  {
    GLubyte header[6];    // �������� ������ �������� 6 ���� �����
    GLuint bytesPerPixel; // ���������� ���� �� ������� (3 ��� 4)
    GLuint imageSize;     // ������ ������ ����������� ��� �������� �����������
    GLuint type;          // ��� �����������, GL_RGB ��� GL_RGBA
    GLuint Height;        // ������ �����������          
    GLuint Width;         // ������ �����������        
    GLuint Bpp;           // ���������� ��� �� ������� (24 ��� 32)
  } TGA;

TGAHeader tgaheader; // ������������ ��� �������� ��������� �����
TGA tga;             // ������������ ��� �������� ���������� � �����

// ��������� ��������� TGA
GLubyte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
// ��������� ������� TGA
GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};
// ��������� �������� ����
bool LoadUncompressedTGA(Texture *, char *, FILE *);
