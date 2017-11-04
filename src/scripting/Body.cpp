#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>

#include <duktape.h>

#include "dukdemo/scripting/util.h"
#include "dukdemo/scripting/Body.h"


namespace dukdemo {
namespace scripting {
namespace body {


void
init(duk_context* pContext)
{
	auto const prototypeIdx = duk_push_object(pContext);

#define PUSH_METHOD(method, nargs) \
	duk_push_c_function(pContext, methods::method, nargs); \
	duk_put_prop_string(pContext, prototypeIdx, #method)

	PUSH_METHOD(setLinearVelocity, 2);
#undef PUSH_METHOD

	duk_put_global_string(pContext, g_bodyProtoSym);
}


duk_idx_t
pushBodyWithoutFinalizer(duk_context* pContext, b2Body* pBody)
{
	auto const bodyIdx = duk_push_object(pContext);
	// Set internal prototype to Body.prototype.
	duk_get_global_string(pContext, g_bodyProtoSym);
	duk_set_prototype(pContext, bodyIdx);

	// Set the body pointer on the JS object.
	duk_push_pointer(pContext, pBody);
	duk_put_prop_string(pContext, bodyIdx, g_ownBodyPtrSym);
	return bodyIdx;
}


duk_idx_t
pushBodyWithFinalizer(duk_context* pContext, b2Body* pBody)
{
	auto const bodyIdx = pushBodyWithoutFinalizer(pContext, pBody);
	duk_push_c_function(pContext, finalizer, 1);
	duk_set_finalizer(pContext, bodyIdx);
	return bodyIdx;
}


void
destroyBodyAt(duk_context* pContext, duk_idx_t bodyIdx)
{
	duk_get_prop_string(pContext, bodyIdx, g_ownBodyPtrSym);
	auto* const pBody = static_cast<b2Body*>(duk_get_pointer(pContext, 1));
	duk_pop_2(pContext);

	if (!pBody)
	{
		// Nothing to do: body was already destroyed.
		return;
	}

	auto* const pWorld = pBody->GetWorld();
	if (pWorld)
	{
		pWorld->DestroyBody(pBody);
	}

	duk_push_pointer(pContext, nullptr);
	duk_put_prop_string(pContext, -1, g_ownBodyPtrSym);
}


b2Body*
getOwnBodyPtr(duk_context* pContext)
{
	return static_cast<b2Body*>(getPointerFromThis(pContext, g_ownBodyPtrSym));
}


duk_ret_t
finalizer(duk_context* pContext)
{
	destroyBodyAt(pContext, 0);
	return 0;
}


duk_ret_t
methods::setLinearVelocity(duk_context* pContext)
{
	b2Vec2 vec{
		float(duk_get_number(pContext, 0)),
		float(duk_get_number(pContext, 1))
	};
	auto* const pBody = getOwnBodyPtr(pContext);
	pBody->SetLinearVelocity(vec);
	return 0;
}


duk_ret_t
methods::getLinearVelocity(duk_context* pContext)
{
	auto const* const pBody = getOwnBodyPtr(pContext);
	writeVec2ToArray(pContext, 0, pBody->GetLinearVelocity());
	return 1;
}


} // namespace body
} // namespace scripting
} // namespace dukdemo
