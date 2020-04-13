// L2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdlib.h>
#include <locale.h>  
#include <stdio.h>
#include <windows.h>
#include <tchar.h>


void ErrorMes(LPTSTR lpszFunction)
{

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);


	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	wprintf(L"%s failed with error %d: %s",
		lpszFunction, dw, lpMsgBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);

}

BOOL FindFileByClaster(TCHAR* volume) {

	HANDLE hDevice = CreateFile(volume,  
		GENERIC_READ,                       
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,           
		OPEN_EXISTING, 
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		ErrorMes((LPTSTR)L"CreateFile");
		return FALSE;
	}

	STARTING_VCN_INPUT_BUFFER input = { 0 };
	input.StartingVcn.QuadPart = 0L;
	// input.Cluster[0].QuadPart = cluster;

	DWORD output[5000] = {};
	DWORD dwRes = 0;
	RETRIEVAL_POINTERS_BUFFER  result = { 0 };

	BOOL bRes = DeviceIoControl((HANDLE)hDevice,	// handle to file, directory, or volume 
		FSCTL_GET_RETRIEVAL_POINTERS,				// dwIoControlCode
		(LPVOID)&input,								// input buffer 
		(DWORD)sizeof(input),						// size of input buffer 
		(LPVOID)output,							// output buffer 
		(DWORD)5000,								// size of output buffer 
		(LPDWORD)&dwRes,							// number of bytes returned 
		NULL);										// OVERLAPPED structure

	if (bRes == FALSE) {
		ErrorMes((LPTSTR)L"DeviceIoControl");
		return FALSE;
	}

	memcpy(&result, &output, sizeof(RETRIEVAL_POINTERS_BUFFER));

	if (result.ExtentCount == 0) {
		wprintf(L"File not found\n");
		return FALSE;
	}
	wprintf(L"Extent count: %d\n", result.ExtentCount);
	wprintf(L"File information:\n");
	wprintf(L"\tStart cluster: %llu ", result.Extents[0].Lcn.QuadPart);
	wprintf(L"\r\n\tLength: %llu \n\n", result.Extents[0].NextVcn.QuadPart - result.StartingVcn.QuadPart);
	for (int i = 1; i < result.ExtentCount; i++)
	{
		wprintf(L"File information:\n");
		wprintf(L"\tStart cluster: %llu ", result.Extents[i].Lcn.QuadPart);
		wprintf(L"\r\n\tLength: %llu \n\n", result.Extents[i].NextVcn.QuadPart - result.Extents[i-1].NextVcn.QuadPart);
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	TCHAR inp[30];
	/*wprintf(L"Введите путь к каталогу: \n");
	scanf_s("%s", inp, 30);
	printf("%s", inp);
	FindFileByClaster(inp);*/
	FindFileByClaster((TCHAR*)L"E:\\Games\\Spore\\SporeBin\\SporeApp.exe");

	system("PAUSE");
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
