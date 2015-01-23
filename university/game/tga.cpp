#include "tga.h"
// All russian comments are in KOI8-U encoding
// i will certainly convert them later


// úÁÇÒÕÖÁÅÔ ÎÅÓÖÁÔÙÊ ÆÁÊÌ
bool LoadTGA(Texture * texture, char * filename)
//bool LoadUncompressedTGA(Texture *, char *, FILE *);
{
	FILE * fTGA; // ïÂßÑ×ÌÑÅÍ ÕËÁÚÁÔÅÌØ ÎÁ ÆÁÊÌ
	fTGA = fopen(filename, "rb"); // ïÔËÒÙ×ÁÅÍ ÆÁÊÌ ÎÁ ÞÔÅÎÉÅ
	if(fTGA == NULL)        // åÓÌÉ ÂÙÌÁ ÏÛÉÂËÁ
	{
		printf("Can't open file!\n");//...Error code...
		return false;         // ÷ÏÚ×ÒÁÝÁÅÍ False
	}

	// ðÏÐÙÔËÁ ÐÒÏÞÉÔÁÔØ ÚÁÇÏÌÏ×ÏË ÆÁÊÌÁ
	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)
	{
		printf("Can't read file header!\n");//...úÄÅÓØ ËÏÄ ÏÛÉÂËÉ...
		return false;        // ðÒÉ ÏÛÉÂËÉ ×ÏÚ×ÒÁÝÁÅÍ false
	}  

	// åÓÌÉ ÚÁÇÏÌÏ×ÏË ÆÁÊÌÁ ÓÏÏÔ×ÅÔÓÔ×ÕÅÔ ÚÁÇÏÌÏ×ËÕ ÎÅÓÖÁÔÏÇÏ ÆÁÊÌÁ
	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{
		// úÁÇÒÕÖÁÅÍ ÎÅÓÖÁÔÙÊ TGA
		LoadUncompressedTGA(texture, filename, fTGA);
	}
	// åÓÌÉ ÚÁÇÏÌÏ×ÏË ÆÁÊÌÁ ÓÏÏÔ×ÅÔÓÔ×ÕÅÔ ÚÁÇÏÌÏ×ÏË ÓÖÁÔÏÇÏ ÆÁÊÌÁ
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{                      
		// úÁÇÒÕÖÁÅÍ ÓÖÁÔÙÊ TGA
		printf("Image is compressed TGA, is is not suported!\n");//LoadCompressedTGA(texture, filename, fTGA);
		return false;
	}
	else            // åÓÌÉ ÎÅ ÓÏÏÔ×ÅÔÓÔ×ÕÅÔ ÎÉËÁËÏÍÕ
	{
		printf("Unkown header format!\n");//... úÄÅÓØ ËÏÄ ÏÛÉÂËÉ...            
		return false;        // ÷ÏÚ×ÒÁÝÁÅÍ false False
	}  
}

// úÁÇÒÕÖÁÅÍ ÎÅÓÖÁÔÙÊ TGA!
bool LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA)
{
	// ðÙÔÁÅÍÓÑ ÐÒÏÞÉÔÁÔØ ÓÌÅÄÕÀÝÉÅ 6 ÂÁÊÔ
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{                    
		printf("Can't read file header!\n");//...úÄÅÓØ ËÏÄ ÏÛÉÂËÉ...
		return false;        // ÷ÏÚ×ÒÁÝÁÅÍ False
	}
	texture->width  = tga.header[1] * 256 + tga.header[0];  // ÷ÙÞÉÓÌÑÅÍ ×ÙÓÏÔÕ
	texture->height = tga.header[3] * 256 + tga.header[2];  // ÷ÙÞÉÓÌÑÅÍ ÛÉÒÉÎÕ
	texture->bpp = tga.header[4]; // ÷ÙÞÉÓÌÑÅÍ ËÏÌÉÞÅÓÔ×Ï ÂÉÔ ÎÁ ÐÉËÓÅÌØ
	tga.Width = texture->width;   // ÷ÙÞÉÓÌÑÅÍ ×ÙÓÏÔÕ × ÌÏËÁÌØÎÏÊ ÓÔÒÕËÔÕÒÅ
	tga.Height = texture->height; // ÷ÙÞÉÓÌÑÅÍ ÛÉÒÉÎÕ × ÌÏËÁÌØÎÏÊ ÓÔÒÕËÔÕÒÅ
	tga.Bpp = texture->bpp;       // ÷ÙÞÉÓÌÑÅÍ ËÏÌÉÞÅÓÔ×Ï ÂÉÔ ÎÁ ÐÉËÓÅÌØ × ÌÏËÁÌØÎÏÊ ÓÔÒÕËÔÕÒÅ

	// õÂÅÖÄÁÅÍÓÑ ÞÔÏ ×ÓÑ ÉÎÆÏÒÍÁÃÉÑ ËÏÒÒÅËÔÎÁ
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))
	{
		printf("Unknown or corrupted TGA header!\n");//...úÄÅÓØ ËÏÄ ÏÛÉÂËÉ...
		return false;        // ÷ÏÚ×ÒÁÝÁÅÍ False
	}

	if(texture->bpp == 24)        // üÔÏ ÉÚÏÂÒÁÖÅÎÉÅ 24bpp?
		texture->type  = GL_RGB;    // åÓÌÉ ÄÁ, ÕÓÔÁÎÁ×ÌÉ×ÁÅÍ GL_RGB
	else                          // ÅÓÌÉ ÎÅ 24, ÔÏÇÄÁ 32
		texture->type  = GL_RGBA;   // ÕÓÔÁÎÁ×ÌÉ×ÁÅÍ GL_RGBA


	tga.bytesPerPixel = (tga.Bpp / 8); // ÷ÙÓÞÉÔÙ×ÁÅÍ ËÏÌÉÞÅÓÔ×Ï ÂÁÊÔ ÎÁ ÐÉËÓÅÌØ
	// óÞÉÔÁÅÍ ÒÁÚÍÅÒ ÐÁÍÑÔÉ ÎÅÏÂÈÏÄÉÍÙÊ ÄÌÑ ÈÒÁÎÅÎÉÑ ÉÚÏÂÒÁÖÅÎÉÑ
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);


	// ÷ÙÄÅÌÑÅÍ ÐÁÍÑÔØ
	texture->imageData = (GLubyte *) new char[tga.imageSize];
	if(texture->imageData == NULL)      // õÂÅÖÄÁÅÍÓÑ ÞÔÏ ÏÎÁ ÂÙÌÁ ×ÙÄÅÌÅÎÁ
	{
		printf("Can't allocate memory for image!\n");//...úÄÅÓØ ËÏÄ ÏÛÉÂËÉ...
		return false;        // åÓÌÉ ÎÅÔ, ×ÏÚ×ÒÁÝÁÅÍ False
	}
	// ðÙÔÁÅÍÓÑ ÓÞÉÔÁÔØ ×ÓÅ ÉÚÏÂÒÁÖÅÎÉÅ
	if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)
	{
		printf("Can't read image data!\n");//...úÄÅÓØ ËÏÄ ÏÛÉÂËÉ...                
		return false;        // åÓÌÉ ÎÅ ÐÏÌÕÞÉÌÏÓØ, ×ÏÚ×ÒÁÝÁÅÍ False
	}
	// îÁÞÉÎÁÅÍ ÃÉËÌ
	GLuint temp;
	for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		// ðÅÒ×ÙÊ ÂÁÊÔ XOR ÔÒÅÔÉÊ ÂÁÊÔ XOR ÐÅÒ×ÙÊ ÂÁÊÔ XOR ÔÒÅÔÉÊ ÂÁÊÔ
		// texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
		// texture->imageData[cswap] ^= texture->imageData[cswap+2];
		temp=texture->imageData[cswap];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[cswap] = texture->imageData[cswap + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[cswap + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose(fTGA);          // úÁËÒÙ×ÁÅÍ ÆÁÊÌ
	return true;          // ÷ÏÚ×ÒÁÝÁÅÍ true
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
