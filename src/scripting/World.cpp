#include <memory>

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>

#include <duktape.h>

#include "dukdemo/scripting/util.h"
#include "dukdemo/scripting/World.h"
#include "dukdemo/scripting/loaders.h"


namespace dukdemo {
namespace scripting {
namespace world {


void
init(duk_context* pContext)
{
	duk_push_c_function(pContext, constructor, 1); // [ctor].

	// Initialise prototype.
	auto const prototypeIdx = duk_push_object(pContext); // [ctor, proto].
#define PUSH_METHOD(method, nargs) \
	duk_push_c_function(pContext, methods::method, nargs); \
	duk_put_prop_string(pContext, prototypeIdx, #method)

	PUSH_METHOD(setGravity, 2);
	PUSH_METHOD(getGravity, 1);
#undef PUSH_METHOD

	duk_dup(pContext, prototypeIdx); // [ctor, proto, proto].

	// Store the prototype globally under a hidden symbol.
	duk_put_global_string(pContext, g_worldProtoSym); // [ctor, proto].

	// Store the prototype on the constructor.
	duk_put_prop_string(pContext, -2, "prototype"); // [ctor].

	// Put the constructor on the global object.
	duk_put_global_string(pContext, g_worldCtorSym); // [].
}


b2World*
getOwnWorldPtr(duk_context* pContext)
{
	return static_cast<b2World*>(
		getPointerFromThis(pContext, g_ownWorldPtrSym));
}


void
initialiseWorldObject(duk_context* pContext, duk_idx_t objIdx, b2World* pWorld)
{
	assert(objIdx >= 0 && "Index must be non-negative");
	duk_get_global_string(pContext, g_worldProtoSym);
	duk_set_prototype(pContext, objIdx);
	duk_push_pointer(pContext, pWorld);
	duk_put_prop_string(pContext, objIdx, g_ownWorldPtrSym);
}


void
pushWorld(duk_context* pContext, b2World* pWorld, bool cleanup)
{
	auto const worldIdx = duk_push_object(pContext);
	initialiseWorldObject(pContext, worldIdx, pWorld);
	if (cleanup)
	{
		duk_push_c_function(pContext, finalizer, 1);
		duk_set_finalizer(pContext, -1);
	}
}


void
pushWorld(duk_context* pContext, std::unique_ptr<b2World> pWorld)
{
	pushWorld(pContext, pWorld.get(), true);
	pWorld.release();
}


duk_ret_t
constructor(duk_context* pContext)
{
	if (!duk_is_constructor_call(pContext))
	{
		return DUK_RET_TYPE_ERROR;
	}

	b2Vec2 gravity{0.0f, 0.0f};
	if (!loadVec2(pContext, 0, &gravity))
	{
		return DUK_RET_TYPE_ERROR;
	}

	auto pWorld = std::make_unique<b2World>(gravity);
	duk_push_this(pContext);
	initialiseWorldObject(pContext, 1, pWorld.get());
	pWorld.release();
	return 0;
}


duk_ret_t
finalizer(duk_context* pContext)
{
	duk_get_prop_string(pContext, 0, g_ownWorldPtrSym);
	auto* const pWorld = static_cast<b2World*>(duk_get_pointer(pContext, 1));
	duk_pop(pContext);

	// If the world pointer is null, we're finalising something which has
	// already been destroyed, or the prototype itself, so can bail out.
	if (pWorld == nullptr)
	{
		return 0;
	}

	// Replace the world's internal pointer with a nullptr.
	duk_push_pointer(pContext, nullptr);
	duk_put_prop_string(pContext, 0, g_ownWorldPtrSym);
	pWorld->~b2World();
	return 0;
}


duk_ret_t
methods::setGravity(duk_context* pContext)
{
	assert(duk_is_number(pContext, 0) && duk_is_number(pContext, 1) &&
		"setGravity requires two arguments");
	b2Vec2 vec{
		float(duk_get_number(pContext, 0)),
		float(duk_get_number(pContext, 1))
	};
	duk_pop_2(pContext);

	auto* const pWorld = getOwnWorldPtr(pContext);
	pWorld->SetGravity(vec);
	return 0;
}


duk_ret_t
methods::getGravity(duk_context* pContext)
{
	assert(duk_is_array(pContext, 0) && "getGravity requires array argument");
	auto const* const pWorld = getOwnWorldPtr(pContext);
	writeVec2ToArray(pContext, 0, pWorld->GetGravity());
	return 1;
}


} // namespace world
} // namespace scripting
} // namespace dukdemo
