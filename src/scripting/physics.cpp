#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <duktape.h>

#include "dukdemo/scripting/physics.h"


#define LOAD_OPT_PROPERTY(ctx, ownerIdx, duk_type, pDef, prop, real_type) \
	if (duk_get_prop_string(pContext, ownerIdx, #prop)) \
	{ \
		pDef->prop = static_cast<real_type>( \
			duk_get_ ## duk_type(ctx, -1) \
		); \
	}


#define LOAD_OPT_VEC2_PROPERTY(ctx, ownerIdx, pDef, prop) \
	if (duk_get_prop_string(pContext, ownerIdx, #prop)) \
	{ \
		loadVec2(pContext, -1, &(pDef->prop)); \
	}


namespace dukdemo {
namespace scripting {


bool loadVec2(duk_context* pContext, duk_idx_t vecIdx, b2Vec2* pVec)
{
	if (!duk_is_array(pContext, vecIdx))
	{
		return false;
	}

	// Stack: [..., <vecIdx> vec, ...].
	duk_get_prop_index(pContext, vecIdx, 0);
	// Stack: [..., <vecIdx> vec, ..., x].
	auto const x = duk_get_number(pContext, -1);
	if (isnan(x))
	{
		return false;
	}

	// If the index is negative, decrement it to reflect the changed stack.
	duk_get_prop_index(pContext, vecIdx - (vecIdx < 0), 1);
	// Stack: [..., <vecIdx> vec, ..., x, y].
	auto const y = duk_get_number(pContext, -1);
	if (isnan(y))
	{
		return false;
	}

	pVec->Set(static_cast<float>(x), static_cast<float>(y));
	return true;
}


bool loadBodyDef(duk_context* pContext, duk_idx_t defIdx, b2BodyDef* pDef)
{
	if (!duk_check_type(pContext, defIdx, DUK_TYPE_OBJECT))
	{
		return false;
	}
	LOAD_OPT_PROPERTY(pContext, defIdx, int, pDef, type, b2BodyType)
	LOAD_OPT_VEC2_PROPERTY(pContext, defIdx, pDef, position)
	LOAD_OPT_PROPERTY(pContext, defIdx, number, pDef, angle, float)
	LOAD_OPT_VEC2_PROPERTY(pContext, defIdx, pDef, linearVelocity)
	LOAD_OPT_PROPERTY(pContext, defIdx, number, pDef, angularVelocity, float)
	LOAD_OPT_PROPERTY(pContext, defIdx, number, pDef, linearDamping, float)
	LOAD_OPT_PROPERTY(pContext, defIdx, number, pDef, angularDamping, float)
	LOAD_OPT_PROPERTY(pContext, defIdx, boolean, pDef, allowSleep, bool)
	LOAD_OPT_PROPERTY(pContext, defIdx, number, pDef, awake, float)
	LOAD_OPT_PROPERTY(pContext, defIdx, number, pDef, fixedRotation, float)
	LOAD_OPT_PROPERTY(pContext, defIdx, number, pDef, bullet, float)
	LOAD_OPT_PROPERTY(pContext, defIdx, number, pDef, active, float)
	LOAD_OPT_PROPERTY(pContext, defIdx, number, pDef, gravityScale, float)
	return true;
}


bool loadFilter(duk_context* pContext, duk_idx_t index, b2Filter* pFilter)
{
	if (!duk_check_type(pContext, index, DUK_TYPE_OBJECT))
	{
		return false;
	}
	LOAD_OPT_PROPERTY(pContext, index, int, pFilter, categoryBits, uint16)
	LOAD_OPT_PROPERTY(pContext, index, int, pFilter, maskBits, uint16)
	LOAD_OPT_PROPERTY(pContext, index, int, pFilter, groupIndex, uint16)
	return true;
}


bool loadFixtureDefWithoutShape(
	duk_context* pContext,
	duk_idx_t index,
	b2FixtureDef* pDef
)
{
	if (!loadFilter(pContext, index, &(pDef->filter))) // Also checks type.
	{
		return false;
	}
	LOAD_OPT_PROPERTY(pContext, index, number, pDef, friction, float)
	LOAD_OPT_PROPERTY(pContext, index, number, pDef, restitution, float)
	LOAD_OPT_PROPERTY(pContext, index, number, pDef, density, float)
	LOAD_OPT_PROPERTY(pContext, index, number, pDef, isSensor, float)
	return true;
}


} // namespace scripting
} // namespace dukdemo


#undef LOAD_OPT_PROPERTY
#undef LOAD_OPT_VEC2_PROPERTY
