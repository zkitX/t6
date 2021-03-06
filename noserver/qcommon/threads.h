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

#ifndef THREADS_H
#define THREADS_H

extern "C" {
	void SetThreadName(unsigned int, char const*);
	void Sys_Sleep(int);
	void NET_Sleep(unsigned int);
	void Sys_SetEvent(void**);
	void Sys_ResetEvent(void**);
	void Sys_CreateEvent(int, int, void**);
	int Sys_WaitForSingleObjectTimeout(void**, unsigned int);
	void Sys_WaitForSingleObject(void**);
	unsigned int Sys_GetCpuCount(void);
	void Win_InitThreads(void);
	void Sys_InitMainThread(void);
	void Sys_InitThread(int);
	unsigned long Sys_ThreadMain(void*);
	void Sys_CreateThread(unsigned int, void(__cdecl*)(unsigned int));
	void Sys_TitleServerThreadData(void);
	void Sys_InitDemoStreamingEvent(void);
	void Sys_WaitForDemoStreamingEvent(void);
	int Sys_WaitForDemoStreamingEventTimeout(unsigned int);
	void Sys_SetDemoStreamingEvent(void);
	void Sys_InitWebMStreamingEvent(void);
	void Sys_InitServerEvents(void);
	void Sys_NotifyRenderer(void);
	int Sys_WaitServer(int);
	bool Sys_IsDBPrintingSuppressed(void);
	void Sys_StartGumpLoading(void);
	int Sys_IsLoadingGump(void);
	int Sys_WaitForGumpLoad(int);
	int Sys_WaitForGumpFlush(int);
	void Sys_WakeServer(void);
	void Sys_ServerCompleted(void);
	int Sys_WaitStartServer(int);
	bool Sys_IsServerThread(void);
	void Sys_DatabaseCompleted(void);
	void Sys_WaitStartDatabase(void);
	int Sys_IsDatabaseReady(void);
	void Sys_WakeDatabase(void);
	void Sys_NotifyDatabase(void);
	void Sys_DatabaseCompleted2(void);
	int Sys_IsDatabaseReady2(void);
	void Sys_WakeDatabase2(void);
	bool Sys_IsRenderThread(void);
	bool Sys_IsDatabaseThread();;
	bool Sys_IsMainThread(void);
	int Sys_GetThreadContext(void);
	void Sys_SetValue(int, void*);
	void* Sys_GetValue(int);
	void Sys_SetWin32QuitEvent(void);;
	int Sys_QueryWin32QuitEvent(void);
	void Sys_SetRGRegisteredEvent(void);
	int Sys_QueryRGRegisteredEvent(void);
	void Sys_SetRenderEvent(void);
	void Sys_SetD3DShutdownEvent(void);
	int Sys_QueryD3DShutdownEvent(void);
	bool Sys_SpawnStreamThread(void (*)(unsigned int));
	void Sys_StreamSleep(void);
	void Sys_ResetSndInitializedEvent(void);
	int Sys_QueryStreamPaused(void);
	void Sys_WakeStream(void);
	bool Sys_IsStreamThread(void);
	void Sys_SetServerAllowNetworkEvent(void);
	void Sys_ResetServerAllowNetworkEvent(void);
	void Sys_SetServerNetworkCompletedEvent(void);
	void Sys_ResetServerNetworkCompletedEvent(void);
	void Sys_WaitServerNetworkCompleted(void);
	unsigned int Sys_GetDefaultWorkerThreadsCount(void);
	bool Sys_SpawnServerThread(void (*)(unsigned int));
	bool Sys_SpawnDatabaseThread(void (*)(unsigned int));
	void Sys_InitWorkerThreadContext(void);
	void Sys_RenderCompleted(void);
	void Sys_FrontEndSleep(void);
	void Sys_WakeRenderer(void*);
	void Sys_SleepServer(void);
	void Sys_SyncDatabase(void);
	char const* Sys_GetCurrentThreadName(void);
	void Sys_WaitAllowServerNetworkLoop(void);
	void Sys_GumpPrint(char const*, ...);
	void Sys_GumpLoaded(void);
	void Sys_GumpFlushed(void);
}

#endif // THREADS_H