struct b2Vec2;
struct b2BodyDef;
struct b2Filter;


namespace dukdemo {
namespace scripting {


/**
 * Load coordinates from an object on the value stack.
 *
 * @param pContext the duktape context.
 * @param pVec2 a pointer to the b2Vec2 to store values in.
 * @param vecIdx the value stack index of the JS property.
 * @returns false if unable to process the vector.
 */
bool loadVec2(duk_context* pContext, duk_idx_t vecIdx, b2Vec2* pVec);


/**
 * Load values from an object on the value stack into a b2BodyDef.
 *
 * @note Does not attempt to load any user data.
 *
 * @param pContext the duktape context.
 * @param pDef a pointer to the body definition to overwrite values in.
 * @param defIdx the value stack index of the JS body definition.
 */
bool loadBodyDef(duk_context* pContext, duk_idx_t defIdx, b2BodyDef* pDef);


/**
 * Load values from an object on the value stack into a b2AABB.
 *
 * @param pContext the duktape context.
 * @param pDef a pointer to the filter definition to overwrite values in.
 * @param index the value stack index of the JS filter definition.
 */
bool loadFilter(duk_context* pContext, duk_idx_t defIdx, b2AABB* pAABB);


/**
 * Load values from an object on the value stack into a b2FixtureDef.
 *
 * @note Does not attempt to load any user data.
 * @note Does not load the shape as this would complicate memory management.
 * Use in conjunction with the shape loading methods.
 * @note Expects the b2Filter properties to be present on the fixture
 * definition, rather than in a property named `filter`.
 *
 * @param pContext the duktape context.
 * @param pDef a pointer to the fixture definition to overwrite values in.
 * @param defIdx the value stack index of the JS fixture definition.
 */
bool loadFixtureDefWithoutShape(
	duk_context* pContext,
	duk_idx_t defIdx,
	b2FixtureDef* pDef
);


} // namespace scripting
} // namespace dukdemo
