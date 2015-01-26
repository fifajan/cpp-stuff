#include "tga.h"

// Загружает несжатый файл 
bool LoadTGA(Texture * texture, char * filename)
//bool LoadUncompressedTGA(Texture *, char *, FILE *);
{
	FILE * fTGA; // Объявляем указатель на файл
	fTGA = fopen(filename, "rb"); // Открываем файл на чтение
	if(fTGA == NULL)        // Если была ошибка
	{
		printf("Can't open file!\n");//...Error code...
		return false;         // Возвращаем False
	}

	// Попытка прочитать заголовок файла
	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)
	{
		printf("Can't read file header!\n");// ...Здесь код ошибки...
		return false;        // При ошибки возвращаем false
	}  

	// Если заголовок файла соответствует заголовку несжатого файла
	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{
		// Загружаем несжатый TGA
		LoadUncompressedTGA(texture, filename, fTGA);
	}
	// Если заголовок файла соответствует заголовок сжатого файла
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{                      
		// Загружаем сжатый TGA
		printf("Image is compressed TGA, is is not suported!\n");//LoadCompressedTGA(texture, filename, fTGA);
		return false;
	}
	else            // Если не соответствует никакому
	{
		printf("Unkown header format!\n");// ... Здесь код ошибки...
		return false;        // Возвращаем false
	}  
}

// Загружаем несжатый TGA!
bool LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA)
{
	// Пытаемся прочитать следующие 6 байт
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{                    
		printf("Can't read file header!\n");// ...Здесь код ошибки...
		return false;        // Возвращаем false
	}
	texture->width  = tga.header[1] * 256 + tga.header[0];  // Вычисляем высоту
	texture->height = tga.header[3] * 256 + tga.header[2];  // Вычисляем ширину
	texture->bpp = tga.header[4]; // Вычисляем количество бит на пиксель
	tga.Width = texture->width;   // Вычисляем высоту в локальной структуре
	tga.Height = texture->height; // Вычисляем ширину в локальной структуре
	tga.Bpp = texture->bpp;       // Вычисляем количество бит на пиксель в локальной структуре

	// Убеждаемся что вся информация корректна
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))
	{
		printf("Unknown or corrupted TGA header!\n"); // ...Здесь код ошибки...
		return false;        // Возвращаем
	}

	if(texture->bpp == 24)        // Это изображение 24bpp?
		texture->type  = GL_RGB;    // Если да, устанавливаем GL_RGB
	else                          // если не 24, тогда 32
		texture->type  = GL_RGBA;   // устанавливаем GL_RGBA


	tga.bytesPerPixel = (tga.Bpp / 8); // Высчитываем количество байт на пиксель
	// Считаем размер памяти необходимый для хранения изображения
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);


	// Выделяем память
	texture->imageData = (GLubyte *) new char[tga.imageSize];
	if(texture->imageData == NULL)      // Убеждаемся что она была выделена
	{
		printf("Can't allocate memory for image!\n"); // Убеждаемся что она была выделена
		return false;        // Если нет, возвращаем false
	}
	// Пытаемся считать все изображение
	if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)
	{
		printf("Can't read image data!\n"); // ...Здесь код ошибки...
		return false;        // Если не получилось, возвращаем false
	}
	// Начинаем цикл
	GLuint temp;
	for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		// Первый байт XOR третий байт XOR первый байт XOR третий байт
		// texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
		// texture->imageData[cswap] ^= texture->imageData[cswap+2];
		temp=texture->imageData[cswap];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[cswap] = texture->imageData[cswap + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[cswap + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose(fTGA);          // Закрываем файл
	return true;          // Возвращаем true
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
