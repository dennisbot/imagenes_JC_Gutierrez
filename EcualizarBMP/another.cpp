//#include <iostream>
//#include <windows.h>
//
//using namespace std;
//
//void SaveBitmapToFile(BYTE* pBitmapBits, LONG lWidth, LONG lHeight, WORD wBitsPerPixel, LPCTSTR lpszFileName)
//{
//	RGBQUAD palette[256];
//	for (int i = 0; i < 256; ++i)
//	{
//		palette[i].rgbBlue = (byte)i;
//		palette[i].rgbGreen = (byte)i;
//		palette[i].rgbRed = (byte)i;
//	}
//
//	BITMAPINFOHEADER bmpInfoHeader = { 0 };
//	// Set the size
//	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
//	// Bit count
//	bmpInfoHeader.biBitCount = wBitsPerPixel;
//	// Use all colors
//	bmpInfoHeader.biClrImportant = 0;
//	// Use as many colors according to bits per pixel
//	bmpInfoHeader.biClrUsed = 0;
//	// Store as un Compressed
//	bmpInfoHeader.biCompression = BI_RGB;
//	// Set the height in pixels
//	bmpInfoHeader.biHeight = lHeight;
//	// Width of the Image in pixels
//	bmpInfoHeader.biWidth = lWidth;
//	// Default number of planes
//	bmpInfoHeader.biPlanes = 1;
//	// Calculate the image size in bytes
//	bmpInfoHeader.biSizeImage = lWidth* lHeight * (wBitsPerPixel / 8);
//
//	BITMAPFILEHEADER bfh = { 0 };
//	// This value should be values of BM letters i.e 0x4D42
//	// 0x4D = M 0×42 = B storing in reverse order to match with endian
//
//	bfh.bfType = 'B' + ('M' << 8);
//	// <<8 used to shift ‘M’ to end
//
//	// Offset to the RGBQUAD
//	bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + sizeof(RGBQUAD) * 256;
//	// Total size of image including size of headers
//	bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;
//	// Create the file in disk to write
//	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL,
//		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if (!hFile) // return if error opening file
//	{
//		return;
//	}
//
//	DWORD dwWritten = 0;
//	// Write the File header
//	WriteFile(hFile, &bfh, sizeof(bfh), &dwWritten, NULL);
//	// Write the bitmap info header
//	WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL);
//	// Write the palette
//	WriteFile(hFile, &palette[0], sizeof(RGBQUAD) * 256, &dwWritten, NULL);
//	// Write the RGB Data
//	if (lWidth % 4 == 0)
//	{
//		WriteFile(hFile, pBitmapBits, bmpInfoHeader.biSizeImage, &dwWritten, NULL);
//	}
//	else
//	{
//		char* empty = new char[4 - lWidth % 4];
//		for (int i = 0; i < lHeight; ++i)
//		{
//			WriteFile(hFile, &pBitmapBits[i * lWidth], lWidth, &dwWritten, NULL);
//			WriteFile(hFile, empty, 4 - lWidth % 4, &dwWritten, NULL);
//		}
//	}
//	// Close the file handle
//	CloseHandle(hFile);
//}
//
//void SaveBitmapToFileColor(BYTE* pBitmapBits, LONG lWidth, LONG lHeight, WORD wBitsPerPixel, LPCTSTR lpszFileName)
//{
//
//	BITMAPINFOHEADER bmpInfoHeader = { 0 };
//	// Set the size
//	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
//	// Bit count
//	bmpInfoHeader.biBitCount = wBitsPerPixel;
//	// Use all colors
//	bmpInfoHeader.biClrImportant = 0;
//	// Use as many colors according to bits per pixel
//	bmpInfoHeader.biClrUsed = 0;
//	// Store as un Compressed
//	bmpInfoHeader.biCompression = BI_RGB;
//	// Set the height in pixels
//	bmpInfoHeader.biHeight = lHeight;
//	// Width of the Image in pixels
//	bmpInfoHeader.biWidth = lWidth;
//	// Default number of planes
//	bmpInfoHeader.biPlanes = 1;
//	// Calculate the image size in bytes
//	bmpInfoHeader.biSizeImage = lWidth* lHeight * (wBitsPerPixel / 8);
//
//	BITMAPFILEHEADER bfh = { 0 };
//	// This value should be values of BM letters i.e 0x4D42
//	// 0x4D = M 0×42 = B storing in reverse order to match with endian
//
//	bfh.bfType = 'B' + ('M' << 8);
//	// <<8 used to shift ‘M’ to end
//
//	// Offset to the RGBQUAD
//	bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
//	// Total size of image including size of headers
//	bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;
//	// Create the file in disk to write
//	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL,
//		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if (!hFile) // return if error opening file
//	{
//		return;
//	}
//
//	DWORD dwWritten = 0;
//	// Write the File header
//	WriteFile(hFile, &bfh, sizeof(bfh), &dwWritten, NULL);
//	// Write the bitmap info header
//	WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL);
//	// Write the palette
//	//WriteFile( hFile, &palette[0], sizeof(RGBQUAD) * 256, &dwWritten, NULL );
//	// Write the RGB Data
//	if (lWidth % 4 == 0)
//	{
//		WriteFile(hFile, pBitmapBits, bmpInfoHeader.biSizeImage, &dwWritten, NULL);
//	}
//	else
//	{
//		char* empty = new char[4 - lWidth % 4];
//		for (int i = 0; i < lHeight; ++i)
//		{
//			WriteFile(hFile, &pBitmapBits[i * lWidth], lWidth, &dwWritten, NULL);
//			WriteFile(hFile, empty, 4 - lWidth % 4, &dwWritten, NULL);
//		}
//	}
//	// Close the file handle
//	CloseHandle(hFile);
//}
//
//
//int main() {
//
//}