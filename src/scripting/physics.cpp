#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>

#include <duktape.h>

#include "dukdemo/scripting/physics.h"


#define LOAD_PROPERTY(ctx, pDef, prop, duk_type, real_type) \
	if (duk_get_prop_string(pContext, 0, #prop)) \
	{ \
		pDef->prop = static_cast<real_type>( \
			duk_get_ ## duk_type(ctx, -1) \
		); \
	}


#define LOAD_VEC2_PROPERTY(ctx, pDef, prop) \
	if (duk_get_prop_string(pContext, 0, #prop)) \
	{ \
		setVec2(pContext, -1, &(pDef->prop)); \
	}


namespace dukdemo {
namespace scripting {


void setVec2(duk_context* pContext, duk_idx_t vecIdx, b2Vec2* pVec)
{
	// Stack: [..., <vecIdx> vec, ...].
	duk_get_prop_index(pContext, vecIdx, 0);
	// Stack: [..., <vecIdx> vec, ..., x].
	auto const x = duk_get_number(pContext, -1);
	// If the index is negative, decrement it to reflect the changed stack.
	duk_get_prop_index(pContext, vecIdx - (vecIdx < 0), 1);
	// Stack: [..., <vecIdx> vec, ..., x, y].
	auto const y = duk_get_number(pContext, -1);
	pVec->Set(static_cast<float>(x), static_cast<float>(y));
}


void setBodyDef(duk_context* pContext, duk_idx_t defIdx, b2BodyDef* pDef)
{
	if (!duk_check_type(pContext, defIdx, DUK_TYPE_OBJECT))
	{
		throw std::runtime_error{"Can't load body def: not an object"};
	}
	// Note: userData not supported yet, so we leave it as is.
	LOAD_PROPERTY(pContext, pDef, type, int, b2BodyType)
	LOAD_VEC2_PROPERTY(pContext, pDef, position)
	LOAD_PROPERTY(pContext, pDef, angle, number, float)
	LOAD_VEC2_PROPERTY(pContext, pDef, linearVelocity)
	LOAD_PROPERTY(pContext, pDef, angularVelocity, number, float)
	LOAD_PROPERTY(pContext, pDef, linearDamping, number, float)
	LOAD_PROPERTY(pContext, pDef, angularDamping, number, float)
	LOAD_PROPERTY(pContext, pDef, allowSleep, boolean, bool)
	LOAD_PROPERTY(pContext, pDef, awake, number, float)
	LOAD_PROPERTY(pContext, pDef, fixedRotation, number, float)
	LOAD_PROPERTY(pContext, pDef, bullet, number, float)
	LOAD_PROPERTY(pContext, pDef, active, number, float)
	LOAD_PROPERTY(pContext, pDef, gravityScale, number, float)
}


} // namespace scripting
} // namespace dukdemo


#undef LOAD_PROPERTY
#undef LOAD_VEC2_PROPERTY
