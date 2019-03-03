// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <detours.h>
#include "rpc/server.h"

// Vars
HMODULE CachedHModule;
rpc::server* srv;

// Func defs
void LibraryThread();
void CreateRpcServer();
void HookGameThread();
void UnhookGameThread();
int __fastcall GameThreadHook(void* This, void* Unused);
#include "funcs.h"

// Function that gets executed in a thread once our library has been loaded into the game
void LibraryThread()
{
	CreateRpcServer();
	HookGameThread();
}

// Set up a server that listens for RPCs
void CreateRpcServer()
{
	srv = new rpc::server("127.0.0.1", 64720);
	srv->bind("SetPlayerHuman", &RpcFuncs::SetPlayerHuman);
	srv->bind("SetPlayerComputer", &RpcFuncs::SetPlayerComputer);
	srv->bind("SetPlayerTeam", &RpcFuncs::SetPlayerTeam);
	srv->bind("LockTeams", &RpcFuncs::LockTeams);
	
	srv->bind("SetGameType", &RpcFuncs::SetGameType);
	srv->bind("SetMultiplayerGame", &RpcFuncs::SetMultiplayerGame);
	srv->bind("SetSinglePlayerGame", &RpcFuncs::SetSinglePlayerGame);
	srv->bind("SetMapType", &RpcFuncs::SetMapType);
	srv->bind("SetMapSize", &RpcFuncs::SetMapSize);

	srv->bind("SetScenarioName", &RpcFuncs::SetScenarioName);

	srv->bind("IsScenarioGame", &RpcFuncs::IsScenarioGame);
	srv->bind("LaunchGame", &RpcFuncs::LaunchGame);
}

// Hook into the game thread
void HookGameThread()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)GameFuncs::TribeGame::HandleIdle, GameThreadHook);
	DetourTransactionCommit();
}

// Remove the game thread hook
void UnhookGameThread()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)GameFuncs::TribeGame::HandleIdle, GameThreadHook);
	DetourTransactionCommit();
}

int __fastcall GameThreadHook(void* This, void* Unused)
{
	int ReturnValue = GameFuncs::TribeGame::HandleIdle(This);

	// Poll the RPC server for work, from the game thread
	if (srv)
	{
		srv->poll();
	}
	
	return ReturnValue;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);
			CachedHModule = hModule;

			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LibraryThread, NULL, NULL, NULL);
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			UnhookGameThread();
			if (srv) srv->stop();
			break;
		}
	}

	return TRUE;
}