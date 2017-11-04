#ifndef DUKDEMO_INCLUDE__DUKDEMO__SCRIPTING__BODY__H
#define DUKDEMO_INCLUDE__DUKDEMO__SCRIPTING__BODY__H
#include <duk_config.h>


class b2Body;


namespace dukdemo {
namespace scripting {
namespace body {


/** Initialise the Body prototype. */
void
init(duk_context* pContext);


duk_idx_t
pushBodyWithoutFinalizer(duk_context* pContext, b2Body* pBody);


duk_idx_t
pushBodyWithFinalizer(duk_context* pContext, b2Body* pBody);


/**
 * Remove and destroy a body at a given index.
 *
 * @param pContext the duktape context.
 * @param bodyIdx the value stack index of the body object;
 */
void
destroyBodyAt(duk_context* pContext, duk_idx_t bodyIdx);


/** Get the @ref b2Body pointer from the body object represented by `this`. */
b2Body*
getOwnBodyPtr(duk_context* pContext);


/** Finalize a body object. */
duk_ret_t
finalizer(duk_context* pContext);


namespace methods {


duk_ret_t
setLinearVelocity(duk_context* pContext);


duk_ret_t
getLinearVelocity(duk_context* pContext);


} // namespace methods
} // namespace body
} // namespace scripting
} // namespace dukdemo
#endif // #ifndef DUKDEMO_INCLUDE__DUKDEMO__SCRIPTING__BODY__H
