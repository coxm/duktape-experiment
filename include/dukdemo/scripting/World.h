#include <duk_config.h>


class b2World;


namespace dukdemo {
namespace scripting {
namespace world {


/**
 * Initialise the World constructor and prototype.
 *
 * Exposes the `World` constructor and hides the prototype on the global
 * object.
 */
void
init(duk_context* pContext);


/** Get the @ref b2World pointer owned by `this`. */
b2World*
getOwnWorldPtr(duk_context* pContext);


/**
 * Push a `World` object onto the stack, wrapping an existing @ref b2World.
 *
 * @note The Box2D world is **not** destroyed after the JS object goes out of
 * scope. If a finalizer is required, use @ref pushWorldWithFinalizer.
 *
 * @param pContext the duktape context.
 * @param pWorld a pointer to the Box2D world.
 */
duk_idx_t
pushWorldWithoutFinalizer(duk_context* pContext, b2World* pWorld);


/**
 * Push a `World` object onto the stack, wrapping an existing @ref b2World.
 *
 * @param pContext the duktape context.
 * @param pWorld a pointer to the Box2D world. The pointer will be released and
 * the @ref b2World instance destroyed when the JS object is finalized.
 */
duk_idx_t
pushWorldWithFinalizer(duk_context* pContext, std::unique_ptr<b2World> pWorld);


/**
 * Construct a `World` object.
 *
 * Requires a `gravity` array argument.
 */
duk_ret_t
constructor(duk_context* pContext);


/**
 * Finalize a `World` object.
 *
 * Destructs the owned @ref b2World.
 */
duk_ret_t
finalizer(duk_context* pContext);


namespace methods {


/**
 * Set the gravity of a `World` object.
 *
 * Requires `x`-, `y`-coordinates as two separate arguments.
 */
duk_ret_t
setGravity(duk_context* pContext);


/**
 * Get the gravity of a `World` object.
 *
 * Requires an array argument capable of storing two floating point values.
 * Returns the same argument.
 */
duk_ret_t
getGravity(duk_context* pContext);


} // namespace methods
} // namespace world
} // namespace scripting
} // namespace dukdemo
