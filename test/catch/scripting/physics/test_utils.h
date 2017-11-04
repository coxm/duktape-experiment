#ifndef DUKDEMO_TEST__CATCH__SCRIPTING__PHYSICS__TEST_UTILS__H
#define DUKDEMO_TEST__CATCH__SCRIPTING__PHYSICS__TEST_UTILS__H
#include <memory>

#include <duktape.h>

#include "dukdemo/util/deleters.h"


namespace testutils {


using duk_context_ptr =
	std::unique_ptr<duk_context, dukdemo::util::DukContextDeleter>;


inline void
pushJSONObject(duk_context* pContext, char const* const pJSON)
{
	duk_push_string(pContext, pJSON);
	duk_json_decode(pContext, -1);
}


} // namespace testutils
#endif // #ifndef DUKDEMO_TEST__CATCH__SCRIPTING__PHYSICS__TEST_UTILS__H
