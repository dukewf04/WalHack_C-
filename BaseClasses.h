#pragma once
#include "Maths.h"

class SSystemGlobalEnvironment;
class IGameFramework;
class IGame;
class IActorSystem;
class ICVar;
class IRenderer;
class IEntitySystem;
class IEntity;
class IEntityClassRegistry;
class IPhysicalWorld;
class ISystem;
class IWeapon;

class i_actor;
class i_entity_proxy;
class i_entity;
class i_actor_system;
class i_entity_it;
class i_entity_system;

/*
#define sge 0x141CBBFB0
#define igFramework 0x141FC5818
#define iCvar 0x141FC4B70  // https://www.unknowncheats.me/forum/warface/336692-reversing-cvars-warface.html
#define gdDevice 0xA6D0 // https://www.unknowncheats.me/forum/warface/346016-vmt-hook-64-bit-warface.html

#define EU_sge 0x141C1A150
#define EU_igFramework 0x141F238F8
#define EU_iCvar 0x141F22C50
#define EU_gdDevice 0xA6D0
*/
#define sge 0x141D132D0
#define igFramework 0x14201CD18
#define iCvar 0x14201C070  // https://www.unknowncheats.me/forum/warface/336692-reversing-cvars-warface.html
#define gdDevice 0xA6D0 // https://www.unknowncheats.me/forum/warface/346016-vmt-hook-64-bit-warface.html


#define sge_IRenderer 0x48 
#define sge_IGame 0xC8 
#define sge_ICVar 0xC0
#define sge_IEntitySystem 0xD8
#define SSystemGlobalEnvironment__IGameRoom 0x224
#define SSystemGlobalEnvironment__pIPhysicalWorld 0x44
#define V_SetPostEffectParamI 0x23C / 4
#define IEntityRenderProxy__SetHUDSilhouettesParams 0xB0 / 4
#define ISkeletonPose__GetJointNameByID 0xC / 4
#define V_GetISkeletonPose 0x14 / 4
#define ISkeletonPose__GetAbsJointByID 0x1C / 4
#define IEntity___GetCharacter 0x148 / 4
#define V_GetProxy 0xEC / 4
#define V_GetCurrentGameRules 0x780
#define IGameRules__GetTeam 0x360 / 8
#define SSystemGlobalEnvironment__pIRenderer 0x30
#define SSystemGlobalEnvironment__pEntitySystem 0x78

typedef unsigned int EntityId;

template< typename cData >
cData vFun_Call(PVOID BaseClass, DWORD64 vOFFSET)
{
	PDWORD64* vPointer = (PDWORD64*)BaseClass;
	PDWORD64 vFunction = *vPointer;
	DWORD64 dwAddress = vFunction[vOFFSET];
	return (cData)(dwAddress);
};

template <typename t>
t f_virtual(PVOID f_base, __int64 f_index)
{
	return (*(t**)f_base)[f_index / 8];
};

enum game_data : __int64
{
	a_system_global = 0x141C140F0,
	a_gameframework = 0x141F221F0,
	o_entity_system = 0xD8,
	o_system = 0xE8
};

enum virtual_data : __int32
{
	f_get_entity_iterator = 168, // new-168 old-160 искать GetEntitiesByClass
	f_entity_next = 48,
	f_get_actor = 120,
	f_set_hudsilhouettes_params = 352,
	f_get_proxy = 472, 
	f_get_client_actor = 1136
};

enum e_entity_proxy
{
	entity_proxy_render,
	entity_proxy_physics,
	entity_proxy_script,
	entity_proxy_sound,
	entity_proxy_ai,
	entity_proxy_area,
	entity_proxy_boids,
	entity_proxy_boid_object,
	entity_proxy_camera,
	entity_proxy_flow_graph,
	entity_proxy_substitution,
	entity_proxy_trigger,
	entity_proxy_rope,
	entity_proxy_entity_node,
	entity_proxy_user,
	entity_proxy_last
};

enum entity_data : __int32
{
	o_set_render_flags = 0x24
};

enum actor_data : __int32
{
	o_get_entity = 0x10,
	o_get_team_id = 0x3E0
};

enum game_frame_work_data
{
	o_get_actor_system = 0x4E0
};

//-----------------------------------------------------------------------------------------------------------

class SSystemGlobalEnvironment
{
public:

	char pad_0x0034[0x10];
	IGame * pIGame() { return *(IGame**)((DWORD64)this + sge_IGame); }
	IRenderer* pRenderer() { return *(IRenderer**)((DWORD64)this + sge_IRenderer); }
	IEntitySystem* pEntitySystem() { return *(IEntitySystem**)((DWORD64)this + sge_IEntitySystem); }
	static SSystemGlobalEnvironment* GetInstance() { return *(SSystemGlobalEnvironment**)sge; } //  EU_sge 0x141BC5370 sge 0x141BC3310

	i_entity_system* get_entity_system()
	{
		return (i_entity_system*)*(__int64*)((__int64)this + game_data::o_entity_system);
	}
};




class IActor {
public:
	EntityId getEntityId() { return *(EntityId*)((DWORD64)this + 0x20); }
	INT IsDead() { return vFun_Call<int(__thiscall*)(PVOID)>(this, 24)(this); }
	IEntity* GetEntity() { return *(IEntity**)((DWORD64)this + 0x10); }
};


class IGame
{
public:
	//IGameFramework* GetIGameFramework() { return *(IGameFramework**)((DWORD)this + 0x28); }
	IGameFramework* GetIGameFramework() { return *(IGameFramework**)igFramework; } // igFramework 0x141ECC628 EU_igFramework 0x141ECE698
	ICVar * GetICvar()
	{
		//return (ICVar*)*(DWORD64*)((DWORD64)this + sge_ICVar);  
		return *(ICVar**)iCvar; // https://www.unknowncheats.me/forum/warface/336692-reversing-cvars-warface.html
	}
};




class IGameRules
{
public:
	INT GetTeam(EntityId entityId)
	{
		return vFun_Call<int(__thiscall*)(PVOID, EntityId)>(this, IGameRules__GetTeam)(this, entityId);
	}
	BOOL EliminationTeam(IActor* pPlayer, IActor* mPlayer)
	{
		int mTeam = GetTeam(mPlayer->getEntityId());
		int pTeam = GetTeam(pPlayer->getEntityId());
		//if (pPlayer->GetDead())return FALSE;
		if ((mTeam != pTeam || pTeam == 0) && (pPlayer != mPlayer)) return TRUE; else return FALSE;
	}
};


class IGameFramework {
public:
	ISystem * GetISystem() { return *(ISystem**)((DWORD64)this + 0x8); }
	IActorSystem* GetIActorSystem() { return *(IActorSystem**)((DWORD64)this + 0x4E0); }
	BOOLEAN GetClientActor(i_actor** pActor)
	{
		using GetClientActorP = bool(__thiscall*)(PVOID, i_actor**);
		return vFun_Call<GetClientActorP >(this, 142)(this, pActor);
	}
	IGameRules* GetCurrentGameRules() { return *(IGameRules**)((DWORD64)this + V_GetCurrentGameRules); }
	static IGameFramework*GetGameFramework() { return*(IGameFramework**)igFramework; } // 

	i_actor_system* get_actor_system()
	{
		return *(i_actor_system**)((__int64)this + game_frame_work_data::o_get_actor_system);
	}

	bool get_client_actor(i_actor** p_actor)
	{
		return f_virtual<bool(__thiscall*)(PVOID, i_actor**)>(this, virtual_data::f_get_client_actor)(this, p_actor);
	}
};

class IActorIterator
{
public:
	IActor * Next() { return vFun_Call<IActor*(__thiscall*)(PVOID)>(this, 1)(this); }
	INT Сount() { return vFun_Call<INT(__thiscall*)(PVOID)>(this, 3)(this); }
};

class IActorSystem
{
public:
	int GetCount() { return vFun_Call<int(__thiscall*)(PVOID)>(this, 13)(this); }
	void CreateActorIterator(IActorIterator** ActorIterator)
	{
		using CreateActorIteratorFn = void(__thiscall*)(PVOID, IActorIterator**);
		return vFun_Call<CreateActorIteratorFn>(this, 144 / 8)(this, ActorIterator);
	}
	IActorIterator*GetActorIterator()
	{
		IActorIterator*pActorIterator = nullptr;
		CreateActorIterator(&pActorIterator);
		if (pActorIterator)return pActorIterator;
		return NULL;
	}
};


class IRenderer
{
public:
	struct w2s_info
	{
		float Posx;
		float Posy;
		float Posz;
		float *Scrnx;
		float *Scrny;
		float *Scrnz;
	};
	bool ProjectToScreen(w2s_info *info) { return vFun_Call<bool(__thiscall*)(PVOID, w2s_info*)>(this, 94)(this, info); }
	DWORD64* GetDirectDevice() { return *(DWORD64**)((DWORD64)this + gdDevice); } // https://www.unknowncheats.me/forum/warface/346016-vmt-hook-64-bit-warface.html
};


enum ESilhouettesParams {
	eAllMap = 524312,
	eLimitDistance = 524296
};

class IEntityRenderProxy
{
public:
	virtual void Function0(); //
	virtual void Function1(); //
	virtual void Function2(); //
	virtual void Function3(); //
	virtual void Function4(); //
	virtual void Function5(); //
	virtual void Function6(); //
	virtual void Function7(); //
	virtual void Function8(); //
	virtual void Function9(); //
	virtual void Function10(); //
	virtual void Function11(); //
	virtual void Function12(); //
	virtual void Function13(); //
	virtual void Function14(); //
	virtual void Function15(); //
	virtual void Function16(); //
	virtual void Function17(); //
	virtual void Function18(); //
	virtual void Function19(); //
	virtual void Function20(); //
	virtual void Function21(); //
	virtual void Function22(); //
	virtual void Function23(); //
	virtual void Function24(); //
	virtual void Function25(); //
	virtual void Function26(); //
	virtual void Function27(); //
	virtual void Function28(); //
	virtual void Function29(); //
	virtual void Function30(); //
	virtual void Function31(); //
	virtual void Function32(); //
	virtual void Function33(); //
	virtual void Function34(); //
	virtual void Function35(); //
	virtual void Function36(); //
	virtual void Function37(); //
	virtual void Function38(); //
	virtual void Function39(); //
	virtual void Function40(); //
	virtual void Function41(); //
	virtual void Function42(); //
	virtual void Function43(); //
	virtual void SetHUDSilhouettesParams(float a, float r, float g, float b);

	void SetRndFlags(int Flag) { *(int*)((DWORD64)this + 0x24) = Flag; }
};

class IEntity
{
public:
	IEntityRenderProxy* GetProxy() { return vFun_Call<IEntityRenderProxy*(__thiscall*)(PVOID, int) >(this, V_GetProxy)(this, 0); }
};





//////////////////////////////////////////////////////////////






class i_entity_system
{
public:
	i_entity_it* get_entity_iterator()
	{
		return f_virtual<i_entity_it*(__thiscall*)(PVOID)>(this, virtual_data::f_get_entity_iterator)(this); // new-168 old-160 искать GetEntitiesByClass
	}
};

class i_entity_it
{
public:
	i_entity* entity_next()
	{
		return f_virtual<i_entity*(__thiscall*)(PVOID)>(this, virtual_data::f_entity_next)(this); 
	}
};

class i_actor_system
{
public:
	int aff = 12;
	i_actor* get_actor(__int32 entity_id)
	{
		return f_virtual<i_actor*(__thiscall*)(PVOID, __int32)>(this, virtual_data::f_get_actor)(this, entity_id);
	}
};

class i_entity
{
public:

	__int32 get_entity_id()
	{
		return *(__int32*)((__int64)this + 0x18);
	}

	i_entity_proxy* get_proxy(e_entity_proxy proxy)
	{
		return f_virtual<i_entity_proxy*(__thiscall*)(PVOID, e_entity_proxy)>(this, virtual_data::f_get_proxy)(this, proxy);
	}

};

class i_entity_proxy
{
public:
	int afd = 12;
	void set_render_flags(__int32 flag)	{ *(__int32*)((__int64)this + entity_data::o_set_render_flags) = flag; }

	void set_hudsilhouettes_params(float r, float g, float b, float a)
	{
		return f_virtual<void(__thiscall*)(PVOID, float, float, float, float)>(this, virtual_data::f_set_hudsilhouettes_params)(this, r, g, b, a);
	}
};


class i_actor
{
public:
	i_entity* get_entity()
	{
		return *(i_entity**)((__int64)this + actor_data::o_get_entity);
	}


	__int32 get_team_id()
	{
		return *(__int32*)((__int64)this + actor_data::o_get_team_id);
	}

};