#include "tga.h"

// ��������� �������� ����
bool LoadTGA(Texture * texture, char * filename)
//bool LoadUncompressedTGA(Texture *, char *, FILE *);
{
	FILE * fTGA; // ��������� ��������� �� ����
	fTGA = fopen(filename, "rb"); // ��������� ���� �� ������
	if(fTGA == NULL)        // ���� ���� ������
	{
		printf("Can't open file!\n");//...Error code...
		return false;         // ���������� False
	}

	// ������� ��������� ��������� �����
	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)
	{
		printf("Can't read file header!\n");//...����� ��� ������...
		return false;        // ��� ������ ���������� false
	}  

	// ���� ��������� ����� ������������� ��������� ��������� �����
	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{
		// ��������� �������� TGA
		LoadUncompressedTGA(texture, filename, fTGA);
	}
	// ���� ��������� ����� ������������� ��������� ������� �����
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{                      
		// ��������� ������ TGA
		printf("Image is compressed TGA, is is not suported!\n");//LoadCompressedTGA(texture, filename, fTGA);
		return false;
	}
	else            // ���� �� ������������� ��������
	{
		printf("Unkown header format!\n");//... ����� ��� ������...            
		return false;        // ���������� false False
	}  
}

// ��������� �������� TGA!
bool LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA)
{
	// �������� ��������� ��������� 6 ����
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{                    
		printf("Can't read file header!\n");//...����� ��� ������...
		return false;        // ���������� False
	}
	texture->width  = tga.header[1] * 256 + tga.header[0];  // ��������� ������
	texture->height = tga.header[3] * 256 + tga.header[2];  // ��������� ������
	texture->bpp = tga.header[4]; // ��������� ���������� ��� �� �������
	tga.Width = texture->width;   // ��������� ������ � ��������� ���������
	tga.Height = texture->height; // ��������� ������ � ��������� ���������
	tga.Bpp = texture->bpp;       // ��������� ���������� ��� �� ������� � ��������� ���������

	// ���������� ��� ��� ���������� ���������
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))
	{
		printf("Unknown or corrupted TGA header!\n");//...����� ��� ������...
		return false;        // ���������� False
	}

	if(texture->bpp == 24)        // ��� ����������� 24bpp?
		texture->type  = GL_RGB;    // ���� ��, ������������� GL_RGB
	else                          // ���� �� 24, ����� 32
		texture->type  = GL_RGBA;   // ������������� GL_RGBA


	tga.bytesPerPixel = (tga.Bpp / 8); // ����������� ���������� ���� �� �������
	// ������� ������ ������ ����������� ��� �������� �����������
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);


	// �������� ������
	texture->imageData = (GLubyte *) new char[tga.imageSize];
	if(texture->imageData == NULL)      // ���������� ��� ��� ���� ��������
	{
		printf("Can't allocate memory for image!\n");//...����� ��� ������...
		return false;        // ���� ���, ���������� False
	}
	// �������� ������� ��� �����������
	if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)
	{
		printf("Can't read image data!\n");//...����� ��� ������...                
		return false;        // ���� �� ����������, ���������� False
	}
	// �������� ����
	GLuint temp;
	for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		// ������ ���� XOR ������ ���� XOR ������ ���� XOR ������ ����
		// texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
		// texture->imageData[cswap] ^= texture->imageData[cswap+2];
		temp=texture->imageData[cswap];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[cswap] = texture->imageData[cswap + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[cswap + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose(fTGA);          // ��������� ����
	return true;          // ���������� true
}
/*
int main(void) 
{
	Texture img;
	img.imageData = NULL;
	if(LoadTGA(&img, "img.tga"))
	{
		printf("Loaded sucessful!\n");
		delete img.imageData;
	}
	else
	{
		printf("Failed!\n");
		delete img.imageData;
	}

}*/
