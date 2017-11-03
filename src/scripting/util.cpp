#include <Box2D/Common/b2Math.h>

#include <duktape.h>

#include "dukdemo/scripting/util.h"


namespace dukdemo {
namespace scripting {


void*
getPointerFromThis(duk_context* pContext, char const* const pPropName)
{
	duk_push_this(pContext);
	duk_get_prop_string(pContext, -1, pPropName);
	void* const ptr = duk_get_pointer(pContext, -1);
	duk_pop_2(pContext);
	return ptr;
}


void
writeVec2ToArray(duk_context* pContext, duk_idx_t arrayIdx, b2Vec2 const& vec)
{
	duk_push_number(pContext, vec.x);
	duk_put_prop_index(pContext, arrayIdx, 0);
	duk_push_number(pContext, vec.y);
	duk_put_prop_index(pContext, arrayIdx, 1);
}


} // namespace scripting
} // namespace dukdemo
