#include "Box2D/Common/b2Math.h"
#include <Box2D/Collision/Shapes/b2Shape.h>


struct b2Vec2;
struct b2BodyDef;
struct b2FixtureDef;
struct b2Filter;

class b2Shape;
class b2CircleShape;
class b2PolygonShape;
class b2EdgeShape;
class b2ChainShape;


namespace dukdemo {
namespace scripting {


/**
 * Load coordinates from an object on the value stack.
 *
 * @param pContext the duktape context.
 * @param pResult a pointer to the b2Vec2 to store values in.
 * @param vecIdx the value stack index of the JS property.
 * @returns false iff the JS object is invalid.
 */
bool
loadVec2(duk_context* pContext, duk_idx_t vecIdx, b2Vec2* pResult) noexcept;


/**
 * Load a b2Vec2 property if provided.
 *
 * @param pContext the duktape context.
 * @param ownerIdx the value stack index of the owning object.
 * @param pPropName the name of the property to load.
 * @param pResult a pointer to the b2Vec2 to store values in.
 * @returns false if the property exists but is invalid. In particular note
 * that `true` is returned if the value is missing.
 */
bool
loadOptionalVec2Prop(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	b2Vec2* pResult
);


/**
 * Load a required b2Vec2 property.
 *
 * @param pContext the duktape context.
 * @param ownerIdx the value stack index of the owning object.
 * @param pPropName the name of the property to load.
 * @param pResult a pointer to the b2Vec2 to store values in.
 * @returns true iff the property was provided and successfully loaded.
 */
bool
loadRequiredVec2Prop(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	b2Vec2* pResult
);


/**
 * Load a float property if provided.
 *
 * @param pContext the duktape context.
 * @param ownerIdx the value stack index of the owning object.
 * @param pPropName the name of the property to load.
 * @param pResult a pointer to the float to store values in.
 * @returns false if the property exists but is invalid. In particular note
 * that `true` is returned if the value is missing.
 */
bool
loadOptionalFloatProp(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	float* pResult
);


/**
 * Load a required b2Vec2 property.
 *
 * @param pContext the duktape context.
 * @param ownerIdx the value stack index of the owning object.
 * @param pPropName the name of the property to load.
 * @param pResult a pointer to the float to store values in.
 * @returns true iff the property was provided and successfully loaded.
 */
bool
loadRequiredFloatProp(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	float* pResult
);


/**
 * Load a 16-bit integer property if provided.
 *
 * @param pContext the duktape context.
 * @param ownerIdx the value stack index of the owning object.
 * @param pPropName the name of the property to load.
 * @param pResult a pointer to the integer to store values in.
 * @returns false if the property exists but is invalid. In particular note
 * that `true` is returned if the value is missing.
 */
bool
loadOptionalInt16Prop(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	int16* pResult
);


/**
 * Load a 16-bit unsigned integer property if provided.
 *
 * @param pContext the duktape context.
 * @param ownerIdx the value stack index of the owning object.
 * @param pPropName the name of the property to load.
 * @param pResult a pointer to the integer to store values in.
 * @returns false if the property exists but is invalid. In particular note
 * that `true` is returned if the value is missing.
 */
bool
loadOptionalUint16Prop(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	uint16* pResult
);


/**
 * Load a boolean property if provided.
 *
 * @param pContext the duktape context.
 * @param ownerIdx the value stack index of the owning object.
 * @param pPropName the name of the property to load.
 * @param pResult a pointer to the bool to store values in.
 * @returns false if the property exists but is invalid. In particular note
 * that `true` is returned if the value is missing.
 */
bool
loadOptionalBoolProp(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	bool* pResult
);


/**
 * Lookup a name in a table.
 *
 * Used for translating strings into enumerated values.
 *
 * @param pCandidate the name to look up.
 * @param pNameTable the name table.
 * @param tableSize the number of entries in the table.
 * @returns the index of the name if found; otherwise `tableSize`.
 */
duk_uint_t
lookupName(
	char const* const pCandidate,
	char const* const* const pNameTable,
	duk_uint_t const tableSize
) noexcept;


/**
 * Get an enumerated value.
 *
 * @param pContext the duktape context.
 * @param idx the value stack index of the owning object.
 * @param pNameTable the name table, organised in the enumeration order.
 * @param tableSize the number of entries in the table.
 */
duk_uint_t
getEnum(
	duk_context* pContext,
	duk_idx_t idx,
	char const* const* const pNameTable,
	duk_uint_t tableSize
) noexcept;


/**
 * Load values from an object on the value stack into a b2BodyDef.
 *
 * @note Does not attempt to load any user data.
 *
 * @param pContext the duktape context.
 * @param pDef a pointer to the body definition to overwrite values in.
 * @param defIdx the value stack index of the JS body definition.
 * @returns false iff the JS object is invalid.
 */
bool
loadBodyDef(
	duk_context* pContext,
	duk_idx_t defIdx,
	b2BodyDef* pDef
) noexcept;


/**
 * Load values from an object on the value stack into a b2AABB.
 *
 * @param pContext the duktape context.
 * @param pDef a pointer to the filter definition to overwrite values in.
 * @param index the value stack index of the JS filter definition.
 * @returns false iff the JS object is invalid.
 */
bool
loadFilter(duk_context* pContext, duk_idx_t defIdx, b2AABB* pAABB) noexcept;


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
 * @returns false iff the JS object is invalid.
 */
bool
loadFixtureDefWithoutShape(
	duk_context* pContext,
	duk_idx_t defIdx,
	b2FixtureDef* pDef
) noexcept;


/**
 * Get the type of a b2Shape from an object on the value stack.
 *
 * @param pContext the duktape context.
 * @param shapeIdx the value stack index of the shape JS object.
 * @returns the shape type if valid; otherwise @ref b2Shape::e_typeCount.
 */
b2Shape::Type
getShapeType(duk_context* pContext, duk_idx_t shapeIdx) noexcept;


/**
 * Load a circle shape.
 *
 * @param pContext the duktape context.
 * @param idx the value stack index of the shape JS object.
 * @returns false iff the JS object is invalid.
 */
bool
loadCircle(
	duk_context* pContext,
	duk_idx_t idx,
	b2CircleShape* pShape
) noexcept;


/**
 * Load a polygon shape.
 *
 * @param pContext the duktape context.
 * @param idx the value stack index of the shape JS object.
 * @returns false iff the JS object is invalid.
 */
bool
loadPolygon(
	duk_context* pContext,
	duk_idx_t idx,
	b2PolygonShape* pShape
) noexcept;


/**
 * Load an edge shape.
 *
 * @param pContext the duktape context.
 * @param idx the value stack index of the shape JS object.
 * @returns false iff the JS object is invalid.
 */
bool
loadEdge(
	duk_context* pContext,
	duk_idx_t idx,
	b2EdgeShape* pShape
) noexcept;


/**
 * Load a chain shape.
 *
 * @param pContext the duktape context.
 * @param idx the value stack index of the shape JS object.
 * @returns false iff the JS object is invalid.
 */
bool
loadChain(
	duk_context* pContext,
	duk_idx_t idx,
	b2ChainShape* pShape
);


} // namespace scripting
} // namespace dukdemo
