#include <cmath>
#include <cstring>
#include <limits>

#include <Box2D/Common/b2Math.h>
#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2EdgeShape.h>
#include <Box2D/Collision/Shapes/b2ChainShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <duktape.h>

#include "dukdemo/scripting/physics.h"


namespace dukdemo {
namespace scripting {


constexpr char const* const shapeTypeNames[] = {
	"circle",
	"edge",
	"polygon",
	"chain",
};
constexpr duk_uint_t shapeTypeCount = 4u;


constexpr char const* const bodyTypeNames[] = {
	"static",
	"kinematic",
	"dynamic",
};
constexpr duk_uint_t bodyTypeCount = 3u;


bool
loadVec2(duk_context* pContext, duk_idx_t vecIdx, b2Vec2* pVec) noexcept
{
	if (!duk_is_array(pContext, vecIdx))
	{
		return false;
	}

	duk_get_prop_index(pContext, vecIdx, 0);
	auto const x = duk_get_number(pContext, -1);
	duk_pop(pContext);
	if (std::isnan(x))
	{
		return false;
	}

	duk_get_prop_index(pContext, vecIdx, 1);
	auto const y = duk_get_number(pContext, -1);
	duk_pop(pContext);
	if (std::isnan(y))
	{
		return false;
	}

	pVec->Set(static_cast<float>(x), static_cast<float>(y));
	return true;
}


bool
loadOptionalVec2Prop(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	b2Vec2* pResult
)
{
	bool valid = true;
	if (duk_get_prop_string(pContext, ownerIdx, pPropName))
	{
		valid = loadVec2(pContext, -1, pResult);
	}
	duk_pop(pContext);
	return valid;
}


bool
loadRequiredVec2Prop(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	b2Vec2* pVec
)
{
	bool valid =
		duk_get_prop_string(pContext, ownerIdx, pPropName) &&
		loadVec2(pContext, -1, pVec);
	duk_pop(pContext);
	return valid;
}


bool
loadOptionalFloatProp(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	float* pResult
)
{
	bool valid = true;
	if (duk_get_prop_string(pContext, ownerIdx, pPropName))
	{
		auto const value = static_cast<float>(duk_get_number(pContext, -1));
		valid = !std::isnan(value);
		if (valid)
		{
			*pResult = static_cast<float>(value);
		}
	}
	duk_pop(pContext);
	return valid;
}


bool
loadRequiredFloatProp(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	float* pResult
)
{
	bool valid = duk_get_prop_string(pContext, ownerIdx, pPropName);
	float value = static_cast<float>(duk_get_number(pContext, -1));
	duk_pop(pContext);
	if (valid && !std::isnan(value))
	{
		*pResult = value;
		return true;
	}
	return false;
}


bool
loadOptionalInt16Prop(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	int16* pResult
)
{
	bool valid = true;
	if (duk_get_prop_string(pContext, ownerIdx, pPropName))
	{
		valid = duk_is_number(pContext, -1);
		if (valid)
		{
			*pResult = static_cast<int16>(duk_get_int(pContext, -1));
		}
	}
	duk_pop(pContext);
	return valid;
}


bool
loadOptionalUint16Prop(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	uint16* pResult
)
{
	bool valid = true;
	if (duk_get_prop_string(pContext, ownerIdx, pPropName))
	{
		valid = duk_is_number(pContext, -1);
		if (valid)
		{
			auto const value = duk_get_uint(pContext, -1);
			valid = value < std::numeric_limits<uint16>::max();
			if (valid)
			{
				*pResult = static_cast<uint16>(value);
			}
		}
	}
	duk_pop(pContext);
	return valid;
}


bool
loadOptionalBoolProp(
	duk_context* pContext,
	duk_idx_t ownerIdx,
	char const* const pPropName,
	bool* pResult
)
{
	bool valid = true;
	if (duk_get_prop_string(pContext, ownerIdx, pPropName))
	{
		valid = duk_is_boolean(pContext, -1);
		if (valid)
		{
			*pResult = duk_get_boolean(pContext, -1);
		}
	}
	duk_pop(pContext);
	return valid;
}


duk_uint_t
lookupName(
	char const* const pCandidate,
	char const* const* const pNameTable,
	duk_uint_t const tableSize
)
	noexcept
{
	for (duk_uint_t i = 0u; i < tableSize; ++i)
	{
		if (std::strcmp(pCandidate, pNameTable[i]) == 0)
		{
			return i;
		}
	}
	return tableSize;
}


duk_uint_t
getEnum(
	duk_context* pContext,
	duk_idx_t idx,
	char const* const* const pNameTable,
	duk_uint_t tableSize
)
	noexcept
{
	if (duk_is_number(pContext, idx))
	{
		auto const type = duk_get_uint_default(pContext, idx, tableSize);
		return (0 <= type && type < tableSize) ? type : tableSize;
	}

	if (duk_is_string(pContext, idx))
	{
		char const* const pType = duk_get_string(pContext, idx);
		return std::min(lookupName(pType, pNameTable, tableSize), tableSize);
	}

	return bodyTypeCount;
}


bool
loadBodyDef(duk_context* pContext, duk_idx_t defIdx, b2BodyDef* pDef)
	noexcept
{
	if (!duk_is_object(pContext, defIdx))
	{
		return false;
	}

	b2BodyDef tmp;
	std::memcpy(&tmp, pDef, sizeof(b2BodyDef));

	if (duk_get_prop_string(pContext, defIdx, "type"))
	{
		auto const type = getEnum(pContext, -1, bodyTypeNames, bodyTypeCount);
		if (type == bodyTypeCount)
		{
			duk_pop(pContext);
			return false;
		}
		tmp.type = static_cast<b2BodyType>(type);
	}
	duk_pop(pContext);

	if (
		loadOptionalVec2Prop(pContext, defIdx, "position", &tmp.position) &&
		loadOptionalVec2Prop(
			pContext, defIdx, "linearVelocity", &tmp.linearVelocity) &&
		loadOptionalFloatProp(pContext, defIdx, "angle", &tmp.angle) &&
		loadOptionalFloatProp(
			pContext, defIdx, "angularVelocity", &tmp.angularVelocity) &&
		loadOptionalFloatProp(
			pContext, defIdx, "linearDamping", &tmp.linearDamping) &&
		loadOptionalFloatProp(
			pContext, defIdx, "angularDamping", &tmp.angularDamping) &&
		loadOptionalBoolProp(
			pContext, defIdx, "allowSleep", &tmp.allowSleep) &&
		loadOptionalBoolProp(pContext, defIdx, "awake", &tmp.awake) &&
		loadOptionalBoolProp(
			pContext, defIdx, "fixedRotation", &tmp.fixedRotation) &&
		loadOptionalBoolProp(pContext, defIdx, "bullet", &tmp.bullet) &&
		loadOptionalBoolProp(pContext, defIdx, "active", &tmp.active) &&
		loadOptionalFloatProp(
			pContext, defIdx, "gravityScale", &tmp.gravityScale)
	)
	{
		std::memcpy(pDef, &tmp, sizeof(b2BodyDef));
		return true;
	}
	return false;
}


bool
loadFilter(duk_context* pContext, duk_idx_t index, b2Filter* pFilter)
	noexcept
{
	if (!duk_is_object(pContext, index))
	{
		return false;
	}
	b2Filter tmp;
	std::memcpy(&tmp, pFilter, sizeof(b2Filter));

	if (
		loadOptionalUint16Prop(
			pContext, index, "categoryBits", &tmp.categoryBits) &&
		loadOptionalUint16Prop(pContext, index, "maskBits", &tmp.maskBits) &&
		loadOptionalInt16Prop(pContext, index, "groupIndex", &tmp.groupIndex)
	)
	{
		std::memcpy(pFilter, &tmp, sizeof(b2Filter));
		return true;
	}
	return false;
}


bool
loadFixtureDefWithoutShape(
	duk_context* pContext,
	duk_idx_t index,
	b2FixtureDef* pDef
)
	noexcept
{
	if (!duk_is_object(pContext, index))
	{
		return false;
	}

	b2FixtureDef tmp;
	std::memcpy(&tmp, pDef, sizeof(b2FixtureDef));
	if (
		loadFilter(pContext, index, &tmp.filter) &&
		loadOptionalFloatProp(pContext, index, "friction", &tmp.friction) &&
		loadOptionalFloatProp(
			pContext, index, "restitution", &tmp.restitution) &&
		loadOptionalFloatProp(pContext, index, "density", &tmp.density) &&
		loadOptionalBoolProp(pContext, index, "isSensor", &tmp.isSensor)
	)
	{
		std::memcpy(pDef, &tmp, sizeof(b2FixtureDef));
		return true;
	}
	return false;
}


bool
loadCircle(
	duk_context* pContext,
	duk_idx_t idx,
	b2CircleShape* pShape
)
	noexcept
{
	if (!duk_is_object(pContext, idx))
	{
		return false;
	}

	float radius = 0.0f;
	if (
		loadRequiredFloatProp(pContext, idx, "radius", &radius) &&
		loadRequiredVec2Prop(pContext, idx, "position", &(pShape->m_p))
	)
	{
		pShape->m_radius = radius;
		return true;
	}
	return false;
}


bool
loadVertexArray(
	duk_context* pContext,
	duk_idx_t arrayIdx,
	b2Vec2* pVertices,
	duk_idx_t count
)
{
	for (duk_idx_t i = 0ul; i < count; ++i)
	{
		duk_get_prop_index(pContext, arrayIdx, i);
		bool valid = loadVec2(pContext, arrayIdx, pVertices + i);
		duk_pop(pContext);
		if (!valid)
		{
			return false;
		}
	}
	return true;
}


bool
loadPolygon(
	duk_context* pContext,
	duk_idx_t polygonIdx,
	b2PolygonShape* pShape
)
	noexcept
{
	if (!duk_is_object(pContext, polygonIdx))
	{
		return false;
	}

	bool valid = duk_get_prop_string(pContext, polygonIdx, "vertices");
	if (valid)
	{
		valid = duk_is_array(pContext, -1);

		if (valid)
		{
			b2Vec2 vertices[b2_maxPolygonVertices];
			duk_size_t const len = duk_get_length(pContext, -1);
			valid = (0 < len && len < b2_maxPolygonVertices);

			valid &= loadVertexArray(pContext, polygonIdx, &vertices[0], len);
			if (valid)
			{
				pShape->Set(vertices, len);
			}
		}
	}
	duk_pop(pContext);  // Pop `vertices`.
	return valid;
}


bool
loadEdge(
	duk_context* pContext,
	duk_idx_t idx,
	b2EdgeShape* pShape
)
	noexcept
{
	if (!duk_is_object(pContext, idx))
	{
		return false;
	}

	b2EdgeShape tmp{*pShape};

	// Load the edge vectors, returning false on failure.
	if (
		loadRequiredVec2Prop(pContext, idx, "v1", &tmp.m_vertex1) &&
		loadRequiredVec2Prop(pContext, idx, "v2", &tmp.m_vertex2) &&
		loadOptionalVec2Prop(pContext, idx, "prev", &tmp.m_vertex0) &&
		loadOptionalVec2Prop(pContext, idx, "next", &tmp.m_vertex3)
	)
	{
		*pShape = tmp;
		return true;
	}
	return false;
}


bool
loadChain(duk_context* pContext, duk_idx_t idx, b2ChainShape* pShape)
{
	// Get vertices property.
	bool valid =
		duk_get_prop_string(pContext, idx, "vertices") &&
		duk_is_array(pContext, -1);

	std::unique_ptr<b2Vec2[]> pVertices;
	duk_size_t len = 0ul;
	if (valid)
	{
		len = duk_get_length(pContext, -1);
		valid = (len > 0);
		if (valid)
		{
			pVertices = std::make_unique<b2Vec2[]>(len);
			valid = loadVertexArray(pContext, -1, &pVertices[0], len);
		}
	}
	duk_pop(pContext); // Pop `vertices`.
	if (!valid)
	{
		return false;
	}

	// Check if a loop.
	bool const isLoop =
		duk_get_prop_string(pContext, idx, "loop") &&
		duk_get_boolean(pContext, -1);
	duk_pop(pContext); // Pop `loop`.
	if (isLoop)
	{
		pShape->CreateLoop(pVertices.get(), len);
		return true;
	}

	// Load previous vertex.
	b2Vec2 prev;
	auto const hasPrev = duk_get_prop_string(pContext, -1, "prev");
	if (hasPrev)
	{
		valid = loadVec2(pContext, -1, &prev);
	}
	duk_pop(pContext);

	// Load next vertex.
	auto const hasNext = duk_get_prop_string(pContext, -1, "next");
	b2Vec2 next;
	if (hasNext)
	{
		valid &= loadVec2(pContext, -1, &next);
	}
	duk_pop(pContext);

	if (valid)
	{
		pShape->CreateChain(pVertices.get(), len);
		if (hasPrev)
		{
			pShape->SetPrevVertex(prev);
		}
		if (hasNext)
		{
			pShape->SetNextVertex(next);
		}
	}

	return valid;
}


} // namespace scripting
} // namespace dukdemo
