#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>
#include <iserver.h>
#include <iclient.h>

static IServer* server = nullptr;

void LuaPrint(GarrysMod::Lua::ILuaBase* LUA, char* msg)
{
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "print");
		LUA->PushString(msg);
		LUA->Call(1, 0);
	LUA->Pop();
}

LUA_FUNCTION( ReconnectPlayer )
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	IClient* player = server->GetClient(LUA->GetNumber(1));

	if (player == nullptr) {
		LUA->ThrowError("Failed to dereference player");
		return 0;
	}

	if (server == nullptr) {
		LUA->ThrowError("Failed to dereference IServer");
		return 0;
	}

	if (player->IsFakeClient()) {
		LUA->PushBool(false);
	} else {
		player->Reconnect();
		LUA->PushBool(true);
	}

	return 1;
}

GMOD_MODULE_OPEN()
{
	server = InterfacePointers::Server();
	if (server == nullptr) {
		LUA->ThrowError("Failed to dereference IServer");
		return 0;
	}

	LuaPrint(LUA, "ReconnectPlayer module loaded");

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->PushCFunction(ReconnectPlayer);
		LUA->SetField(-2, "ReconnectPlayer");
	LUA->Pop();

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "FindMetaTable");
		LUA->PushString("Player");
		LUA->Call(1, 1);
		LUA->PushString("Reconnect");
		LUA->PushCFunction(ReconnectPlayer);
		LUA->SetTable(-3);
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}
