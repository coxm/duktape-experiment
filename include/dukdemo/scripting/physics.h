struct b2Vec2;
struct b2BodyDef;


namespace dukdemo {
namespace scripting {


/**
 * Load coordinates from an object on the value stack.
 *
 * @param pContext the duktape context.
 * @param pVec2 a pointer to the b2Vec2 to store values in.
 * @param vecIdx the value stack index of the JS property.
 */
void setVec2(duk_context* pContext, duk_idx_t vecIdx, b2Vec2* pVec);


/**
 * Load values from an object on the value stack into a b2BodyDef.
 *
 * @param pContext the duktape context.
 * @param pDef a pointer to the body definition to overwrite values in.
 * @param defIdx the value stack index of the JS body definition.
 */
void setBodyDef(duk_context* pContext, duk_idx_t defIdx, b2BodyDef* pDef);


} // namespace scripting
} // namespace dukdemo
