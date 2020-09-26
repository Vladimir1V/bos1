#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <locale.h>
using namespace std;

void ejectDrive(const char* drive_LETTER)
{
	wchar_t drive_name[10] = L"\\\\.\\?:";
	drive_name[4] = drive_LETTER[0];
	DWORD dwRet = 0;
	HANDLE hVol = CreateFile(drive_name, GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hVol == INVALID_HANDLE_VALUE)
		return;

	if (!DeviceIoControl(hVol, FSCTL_LOCK_VOLUME, 0, 0, 0, 0, &dwRet, 0))
		return;

	if (!DeviceIoControl(hVol, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0, &dwRet, 0))
		return;

	DeviceIoControl(hVol, IOCTL_STORAGE_EJECT_MEDIA, 0, 0, 0, 0, &dwRet, 0);

	CloseHandle(hVol);
}

const char *DRF[] = { "Unknown", "Invalid path", "Removable", "Fixed" , "Network drive","CD-ROM", "RAM disk" };

const char* get_drive_type(char *drive_letter)
{
	return DRF[GetDriveTypeA(drive_letter)];
}

bool isRemovableDrive(const char *drive_letter)
{
	if (strcmp(drive_letter, DRF[2]) == 0 || strcmp(drive_letter, DRF[5]) == 0)
	{
		return true;
	}
	return false;
}

void checkEject(char* buffer)
{
	char str[80];
	const char* dir = "//chectejectthisusb";
	sprintf_s(str, "%s%s", buffer, dir); //записывает в str
	ofstream ofs(str); //создать
	
	if (ofs.is_open())
	{
		printf("Ошибка извлечения устройства %s!\n", buffer);
		ofs.close();
		remove(str);
	}
	else
	{
		printf("Устройство %s извлечено!\n", buffer);
	}
}

void ejectDrive_Dialog()
{
	printf("Введите букву извлекаемого устройства: ");
	char buffer[128];
	scanf_s("%c", &buffer[0]);
	buffer[1] = ':';
	buffer[2] = '\\';
	buffer[2] = '\0';
	const char *drive_type = get_drive_type(buffer);
	system("cls");
	if (isRemovableDrive(drive_type))
	{
		ejectDrive(buffer);
		checkEject(buffer);
		return;
	}
	printf("Устройство %s типа %s не может быть извлечено\n\n", buffer, drive_type);
}

int main(void)
{
	setlocale(LC_ALL, "russian");
	char c[9] = "";
	char d[6] = "eject";
	system("cls");
	int ii = 1;
	system("wmic logicaldisk get description");
	system("wmic logicaldisk get FileSystem,FreeSpace,Size,VolumeSerialNumber,name,DriveType");
	printf("Что бы извлечь съемный накопитель введите eject\n");
	for (int i = 0; i < 3; i++)
	{
		gets_s(c);
		if (strcmp(c, d) != 0)
		{
			printf("Неизвестная команда\n");
		}
		else
		{
			ejectDrive_Dialog();
			return 0;
		}
	}
	system("cls");
	system("wmic logicaldisk get description");
	system("wmic logicaldisk get FileSystem,FreeSpace,Size,VolumeSerialNumber,name,DriveType");
	//system("pause");
	return 0;
}