#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>
#include <iclient.h>
#include <iserver.h>

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
	LuaPrint(LUA, "ReconnectPlayer called");

	LUA->CheckType( 1, GarrysMod::Lua::Type::Number );
	int entIndex = (LUA->GetNumber( 1 ));

	IClient* player = server->GetClient( entIndex );

	if (player == nullptr) {
		LUA->ThrowError("Invalid client given");
		return 0;
	}

	if (!player->IsFakeClient()) {
		player->Reconnect();
		LUA->PushBool(true);
	} else {
		LUA->PushBool(false);
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

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}
