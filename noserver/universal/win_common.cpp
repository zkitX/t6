/*
 * Copyright (c) 2020-2021 OpenIW
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "win_common.h"

#include <universal/com_memory.h>
#include <universal/com_shared.h>
#include <universal/mem_userhunk.h>
#include <universal/q_shared.h>
#include <win32/win_main.h>
#include <direct.h>
#include <io.h>

char exePath[256];
char cwd[256];
char homePath[256];

static bool inited_1;

static _RTL_CRITICAL_SECTION s_criticalSection[77];

static unsigned int s_threadAffinityMask;

void TRACK_win_common(void)
{
}

void Sys_Mkdir(char const* path)
{
	_mkdir(path);
}

void Sys_MkdirEx(char const* _path)
{
	char currChar;
	char path[1024];

	strcpy(path, _path);
	for (int i = 0; i < strlen(path); ++i)
	{
		currChar = path[i];
		if (currChar == '/' || currChar == '\\')
		{
			path[i] = 0;
			_mkdir(path);
			path[i] = currChar;
		}
	}
}

char const* Sys_Cwd(void)
{
	_getcwd(cwd, 255);
	cwd[255] = 0;
	return cwd;
}

char const* Sys_DefaultCDPath(void)
{
	return "";
}

char const* Sys_DefaultHomePath(void)
{
	if (!homePath[0])
	{
		_getcwd(cwd, 255);
		cwd[255] = 0;
		_snprintf(homePath, 0x100u, "%s\\identities\\%s", cwd, Sys_GetIdentityParam(IDENTITY_ID));
	}
	return homePath;
}

char const* Sys_DefaultInstallPath(void)
{
	DWORD fileNamePos;
	char currChar;

	if (exePath[0])
		return exePath;
	if (IsDebuggerPresent())
	{
		_getcwd(cwd, 255);
		cwd[255] = 0;
		I_strncpyz(exePath, cwd, 256);
		return exePath;
	}
	fileNamePos = GetModuleFileNameA(0, exePath, 0x100u);
	if (fileNamePos == 256)
	{
		fileNamePos = 255;
		for (; fileNamePos; --fileNamePos)
		{
			currChar = exePath[fileNamePos];
			if (currChar == '\\')
				break;
			if (currChar == '/')
				break;
			if (currChar == ':')
				break;
		}
		exePath[fileNamePos] = 0;
		return exePath;
	}
	else
	{
		exePath[fileNamePos] = 0;
		return exePath;
	}

}

int HasFileExtension(const char* name, const char* extension)
{
	char search[256];

	Com_sprintf(search, 256, "*.%s", extension);
	return I_stricmpwild(search, name) == 0;
}

bool Sys_FileExists(char const* path)
{
	return GetFileAttributes(path) != -1;
}

void Sys_ListFilteredFiles(HunkUser* user, char const* basedir, char const* subdirs, char const* filter, char** list, int* numfiles)
{
	int srchHandle;
	_finddata64i32_t findinfo;
	char search[256];
	char filename[256];

	if (*numfiles < 0xFFFF)
	{
		if (strlen(subdirs))
			Com_sprintf(search, 256, "%s\\%s\\*", basedir, subdirs);
		else
			Com_sprintf(search, 256, "%s\\*", basedir);
		srchHandle = _findfirst64i32(search, &findinfo);
		if (srchHandle != -1)
		{
			for (; _findnext64i32(srchHandle, &findinfo) != -1;)
			{
				if ((findinfo.attrib & 0x10) == 0
					|| I_stricmp(findinfo.name, ".") && I_stricmp(findinfo.name, "..") && I_stricmp(findinfo.name, "CVS"))
				{
					if (*numfiles >= 0xFFFF)
						break;
					if (subdirs)
						Com_sprintf(filename, 256, "%s\\%s", subdirs, findinfo.name);
					else
						Com_sprintf(filename, 256, "%s", findinfo.name);
					if (Com_FilterPath(filter, filename, 0))
						list[(*numfiles)++] = Hunk_CopyString(user, filename);
				}
			}
			_findclose(srchHandle);
		}
	}
}

char** Sys_ListFiles(char const* directory, char const* extension, char const* filter, int* numfiles, int wantsubs)
{
	char** result;
	_finddata64i32_t findinfo;
	int flag;
	char** listCopy;
	int findhandle;
	char* list[16384];
	int nfiles;
	HunkUser* user;
	char search[256];
	int i;

	if (filter)
	{
		user = Hunk_UserCreate(0x20000, HU_SCHEME_DEFAULT, 0, NULL, "Sys_ListFiles", 3);
		nfiles = 0;
		Sys_ListFilteredFiles(user, directory, "", filter, list, &nfiles);
		list[nfiles] = NULL;
		*numfiles = nfiles;
		if (nfiles)
		{
			listCopy = (char**)Hunk_UserAlloc(user, 4 * nfiles + 8, 4, NULL);
			*listCopy = (char*)user;
			++listCopy;
			for (i = 0; i < nfiles; ++i)
			{
				listCopy[i] = list[i];
			}
			listCopy[i] = NULL;
			result = listCopy;
		}
		else
		{
			Hunk_UserDestroy(user);
			result = NULL;
		}
	}
	else
	{
		if (!extension)
		{
			extension = "";
		}

		if (*extension != 47 || extension[1])
		{
			flag = 16;
		}
		else
		{
			extension = "";
			flag = 0;
		}

		if (*extension)
		{
			Com_sprintf(search, 256, "%s\\*.%s", directory, extension);
		}
		else
		{
			Com_sprintf(search, 256, "%s\\*", directory);
		}

		nfiles = 0;
		findhandle = _findfirst64i32(search, &findinfo);
		if (findhandle == -1)
		{
			*numfiles = 0;
			result = NULL;
		}
		else
		{
			user = Hunk_UserCreate(0x20000, HU_SCHEME_DEFAULT, 0, NULL, "Sys_ListFiles", 3);
			do
			{
				if ((!wantsubs && flag != (findinfo.attrib & 0x10) || wantsubs && findinfo.attrib & 0x10)
					&& (!(findinfo.attrib & 0x10)
						|| I_stricmp(findinfo.name, ".") && I_stricmp(findinfo.name, "..") && I_stricmp(findinfo.name, "CVS"))
					&& (!*extension || HasFileExtension(findinfo.name, extension)))
				{
					list[nfiles++] = Hunk_CopyString(user, findinfo.name);
					if (nfiles == 0x3FFF)
					{
						break;
					}
				}
			}             while (_findnext64i32(findhandle, &findinfo) != -1);

			list[nfiles] = NULL;
			_findclose(findhandle);
			*numfiles = nfiles;
			if (nfiles)
			{
				listCopy = (char**)Hunk_UserAlloc(user, 4 * nfiles + 8, 4, NULL);
				*listCopy = (char*)user;
				++listCopy;
				for (i = 0; i < nfiles; ++i)
				{
					listCopy[i] = list[i];
				}
				listCopy[i] = NULL;
				result = listCopy;
			}
			else
			{
				Hunk_UserDestroy(user);
				result = NULL;
			}
		}
	}
	return result;
}

int Sys_DirectoryHasContents(char const* directory)
{
	int srchHandle;
	_finddata64i32_t findinfo;
	char search[256];

	Com_sprintf(search, 256, "%s\\*", directory);
	srchHandle = _findfirst64i32(search, &findinfo);
	if (srchHandle == -1)
		return 0;
	while ((findinfo.attrib & 0x10) != 0 && (!I_stricmp(findinfo.name, ".") || !I_stricmp(findinfo.name, "..") || !I_stricmp(findinfo.name, "CVS")))
	{
		if (_findnext64i32(srchHandle, &findinfo) == -1)
		{
			_findclose(srchHandle);
			return 0;
		}
	}
	_findclose(srchHandle);
	return 1;
}

void Sys_InitializeCriticalSections(void)
{
	_RTL_CRITICAL_SECTION* critSect;
	if (!inited_1)
	{
		inited_1 = 1;
		for (critSect = s_criticalSection; (int)critSect < (int)&s_threadAffinityMask;)
			InitializeCriticalSection(critSect++);
	}
}
