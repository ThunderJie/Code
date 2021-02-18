#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

DWORD RVAOffset(PIMAGE_NT_HEADERS pNtHeader, DWORD Rva)
{
	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)IMAGE_FIRST_SECTION(pNtHeader);
	for (int i = 0; i < pNtHeader->FileHeader.NumberOfSections; i++)
	{
		DWORD SectionBeginRva = pSectionHeader[i].VirtualAddress;

		DWORD SectionEndRva = pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;
		if (Rva >= SectionBeginRva && Rva <= SectionEndRva)
		{
			DWORD Temp = Rva - SectionBeginRva;
			DWORD Rwa = Temp + pSectionHeader[i].PointerToRawData;
			return Rwa;
		}
	}
}

int main(int argc, char* argv[])
{
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;
	char szFilePath[MAX_PATH];
	OPENFILENAME ofn;//定义结构，调用打开对话框选择要分析的文件及其保存路径
	PIMAGE_DOS_HEADER  pDH = NULL;//指向IMAGE_DOS结构的指针
	PIMAGE_NT_HEADERS  pNtH = NULL;//指向IMAGE_NT结构的指针
	PIMAGE_FILE_HEADER pFH = NULL;//指向IMAGE_FILE结构的指针
	PIMAGE_OPTIONAL_HEADER pOH = NULL;//指向IMAGE_OPTIONALE结构的指针

	memset(szFilePath, 0, MAX_PATH);
	memset(&ofn, 0, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = ".";
	ofn.lpstrFile = szFilePath;
	ofn.lpstrTitle = "choose a PE file --by Thunder_J";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrFilter = "*.*\0*.*\0";

	if (!GetOpenFileName(&ofn))
	{
		printf("打开文件错误:%d\n", GetLastError());
		return 0;
	}

	hFile = CreateFile(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (!hFile)
	{
		MessageBox(NULL, "打开文件错误", NULL, MB_OK);
		return 0;
	}

	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (!hMapping)
	{
		printf("创建映射错误:%d", GetLastError());
		CloseHandle(hFile);
		return 0;
	}

	ImageBase = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	if (!ImageBase)
	{
		printf("文件映射错误:%d", GetLastError());
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return 0;
	}
	/************************************************************************/
	/*							  PE头的判断                                 */
	/************************************************************************/
	if (!ImageBase) //判断映像地址
	{
		printf("Not a valid PE file 1!\n");
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return 0;
	}
	printf("--------------------PEheader------------------------\n");
	pDH = (PIMAGE_DOS_HEADER)ImageBase;
	if (pDH->e_magic!=IMAGE_DOS_SIGNATURE) //判断是否为MZ
	{
		printf("Not a valid PE file 2!\n");
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return 0;
	}
	pNtH = (PIMAGE_NT_HEADERS)((DWORD)pDH + pDH->e_lfanew); //判断是否为PE格式
	if (pNtH->Signature!=IMAGE_NT_SIGNATURE)
	{
		printf("Not a valid PE file 3!\n");
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return 0;
	}
	printf("PE e_lfanew is: 0x%x\n", pNtH);

	/************************************************************************/
	/*							  FileHeader                                */
	/************************************************************************/
	pFH = &pNtH->FileHeader;
	printf("-----------------FileHeader------------------------\n");
	printf("NumberOfSections: %d\n", pFH->NumberOfSections);
	printf("SizeOfOptionalHeader: %d\n", pFH->SizeOfOptionalHeader);

	/************************************************************************/
	/*							  OptionalHeader                            */
	/************************************************************************/
	pOH = &pNtH->OptionalHeader;
	printf("-----------------OptionalHeader---------------------\n");
	printf("SizeOfCode:0x%08x\n", pOH->SizeOfCode);
	printf("AddressOfEntryPoint: 0x%08X\n", pOH->AddressOfEntryPoint);
	printf("ImageBase is 0x%x\n", ImageBase);
	printf("SectionAlignment: 0x%08x\n", pOH->SectionAlignment);
	printf("FileAlignment: 0x%08x\n", pOH->FileAlignment);
	printf("SizeOfImage: 0x%08x\n", pOH->SizeOfImage);
	printf("SizeOfHeaders: 0x%08x\n", pOH->SizeOfHeaders);
	printf("NumberOfRvaAndSizes: 0x%08x\n", pOH->NumberOfRvaAndSizes);

	/************************************************************************/
	/*							  SectionTable                              */
	/************************************************************************/
	int SectionNumber = 0;
	DWORD SectionHeaderOffset = (DWORD)pNtH + 24 + (DWORD)pFH->SizeOfOptionalHeader; //节表位置的计算
	printf("--------------------SectionTable---------------------\n");
	for (SectionNumber; SectionNumber < pFH->NumberOfSections;SectionNumber++)
	{
		PIMAGE_SECTION_HEADER pSh = (PIMAGE_SECTION_HEADER)(SectionHeaderOffset + 40 * SectionNumber);
		printf("%d 's Name is %s\n", SectionNumber + 1, pSh->Name);
		printf("VirtualAddress: 0x%08X\n", (DWORD)pSh->VirtualAddress);
		printf("SizeOfRawData: 0x%08X\n", (DWORD)pSh->SizeOfRawData);
		printf("PointerToRawData: 0x%08X\n", (DWORD)pSh->PointerToRawData);
	}
	/************************************************************************/
	/*							  ExportTable                               */
	/************************************************************************/
	printf("--------------------ExportTable----------------------\n");
	DWORD Export_table_offset = RVAOffset(pNtH, (DWORD)pNtH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	PIMAGE_EXPORT_DIRECTORY pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((DWORD)ImageBase + Export_table_offset);
	DWORD EXport_table_offset_Name = (DWORD)ImageBase + RVAOffset(pNtH, pExportDirectory->Name);
	DWORD * pNameOfAddress = (DWORD *)((DWORD)ImageBase + RVAOffset(pNtH, pExportDirectory->AddressOfNames));
	DWORD * pFunctionOfAdress = (DWORD *)((DWORD)ImageBase + RVAOffset(pNtH, pExportDirectory->AddressOfFunctions));
	WORD * pNameOrdinalOfAddress = (WORD *)((DWORD)ImageBase + RVAOffset(pNtH, pExportDirectory->AddressOfNameOrdinals));

	printf("Name:%s\n", EXport_table_offset_Name);
	printf("NameOfAddress:%08X\n", RVAOffset(pNtH, pExportDirectory->AddressOfNames));
	printf("FunctionOfAdress:%08X\n", RVAOffset(pNtH, pExportDirectory->AddressOfFunctions));
	printf("NameOrdinalOfAddress:%08X\n", RVAOffset(pNtH, pExportDirectory->AddressOfNameOrdinals));

	if (pExportDirectory->NumberOfFunctions == 0)
	{
		puts("!!!!!!!!!!!!!!!!!NO EXPORT!!!!!!!!!!!!!!!!!!!!!");
		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
		}
		if (hMapping != NULL)
		{
			CloseHandle(hMapping);
		}
		if (ImageBase != NULL)
		{
			UnmapViewOfFile(ImageBase);
		}

	}

	printf("NumberOfNames:%d\n", pExportDirectory->NumberOfNames);
	printf("NumberOfFunctions:%d\n", pExportDirectory->NumberOfFunctions);

	/************************************************************************/
	/*							  ImportTable                               */
	/************************************************************************/
	printf("--------------------ImportTable----------------------\n");

	int cont = 0;
	do
	{
		DWORD dwImportOffset = RVAOffset(pNtH, pNtH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		dwImportOffset = dwImportOffset + cont;
		PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)ImageBase + dwImportOffset);
		if (pImport->OriginalFirstThunk == 0 && pImport->TimeDateStamp == 0 && pImport->ForwarderChain == 0 && pImport->Name == 0 && pImport->FirstThunk == 0)
			break;
		DWORD dwOriginalFirstThunk = (DWORD)ImageBase + RVAOffset(pNtH, pImport->OriginalFirstThunk);
		DWORD dwFirstThunk = (DWORD)ImageBase + RVAOffset(pNtH, pImport->FirstThunk);
		DWORD dwName = (DWORD)ImageBase + RVAOffset(pNtH, pImport->Name);
		printf("---------Import File Name: %s\n", dwName);
		if (dwOriginalFirstThunk == 0x00000000)
		{
			dwOriginalFirstThunk = dwFirstThunk;
		}
		DWORD* pdwTrunkData = (DWORD*)dwOriginalFirstThunk;
		int n = 0, x = 0;
		while (pdwTrunkData[n] != 0)
		{
			DWORD TrunkData = pdwTrunkData[n];
			if (TrunkData < IMAGE_ORDINAL_FLAG32)//名字导入
			{
				PIMAGE_IMPORT_BY_NAME pInportByName = (PIMAGE_IMPORT_BY_NAME)((DWORD)ImageBase + RVAOffset(pNtH, TrunkData));
				printf("ImportByName: %s\n", pInportByName->Name);
			}
			else
			{
				DWORD FunNumber = (DWORD)(TrunkData - IMAGE_ORDINAL_FLAG32);
				printf("ImportByNumber: %-4d \n", FunNumber);
			}
			if (x != 0 && x % 3 == 0) printf("\n");
			n++;
			x++;
		}
		cont = cont + 40;
	} while (1);
	{
		if (ImageBase)
		{
			UnmapViewOfFile(ImageBase);
		}
		if (hMapping)
		{
			CloseHandle(hMapping);
		}
		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
		}
		return 0;
	}
}