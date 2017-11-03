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


} // namespace scripting
} // namespace dukdemo
