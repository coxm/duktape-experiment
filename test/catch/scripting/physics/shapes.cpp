#include <cstring>

#include <catch.hpp>

#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2EdgeShape.h>
#include <Box2D/Collision/Shapes/b2ChainShape.h>

#include <duktape.h>

#include "dukdemo/scripting/physics.h"

#include "./test_utils.h"


namespace ds = dukdemo::scripting;


template <typename Result>
using ObjectInitialiser = void(Result*);


template <typename Result>
using ObjectLoader = bool(duk_context*, duk_idx_t, Result*);


template <typename Result>
using ObjectChecker = void(Result const&);


template <typename Result>
void
testObjectLoader(
	ObjectInitialiser<Result> initObject,
	ObjectLoader<Result> loadObject,
	char const* const pInvalidJSON,
	char const* const pValidFullJSON,
	char const* const pValidPartialJSON,
	ObjectChecker<Result> checkFullyLoadedObject,
	ObjectChecker<Result> checkPartiallyLoadedObject,
	ObjectChecker<Result> checkUnmodified
)
{
	testutils::duk_context_ptr pContext{duk_create_heap_default()};
	duk_push_number(pContext.get(), 123);
	duk_push_string(pContext.get(), "dummy test data");

	Result result;
	initObject(&result);

	WHEN("loading from a full object")
	{
		THEN("true is returned on success")
		{
			testutils::pushJSONObject(pContext.get(), pValidFullJSON);
			bool const valid = loadObject(pContext.get(), -1, &result);
			REQUIRE(valid);
		}

		THEN("all values are loaded")
		{
			testutils::pushJSONObject(pContext.get(), pValidFullJSON);
			bool const valid = loadObject(pContext.get(), -1, &result);
			REQUIRE(valid);
			checkFullyLoadedObject(result);
		}
	}

	WHEN("given an invalid JS value")
	{
		THEN("false is returned if given a non-object")
		{
			duk_push_number(pContext.get(), 123);
			bool const valid = loadObject(pContext.get(), -1, &result);
			CHECK(!valid);
		}

		THEN("the result is not modified")
		{
			testutils::pushJSONObject(pContext.get(), pInvalidJSON);
			bool const valid = loadObject(pContext.get(), -1, &result);
			REQUIRE(!valid);
			checkUnmodified(result);
		}
	}

	// If a valid partial JSON string was provided, check it results in a valid
	// object. Not all objects support this though.
	if (pValidPartialJSON)
	{
		THEN("partial definitions are accepted")
		{
			testutils::pushJSONObject(pContext.get(), pValidPartialJSON);
			bool const valid = loadObject(pContext.get(), -1, &result);
			REQUIRE(valid);
			checkPartiallyLoadedObject(result);
		}
	}
}


inline void initCircleShape(b2CircleShape* pCircle)
{
	pCircle->m_radius = 0.0f;
	pCircle->m_p.Set(0.0f, 0.0f);
}


inline void checkCircleShapeFullyLoaded(b2CircleShape const& shape)
{
	CHECK(shape.m_radius == Approx(5.4321f));
	CHECK(shape.m_p.x == Approx(1.2345f));
	CHECK(shape.m_p.y == Approx(6.7890f));
}


void checkCircleUnmodified(b2CircleShape const& circle)
{
	CHECK(circle.m_p.x == Approx(0.0f));
	CHECK(circle.m_p.y == Approx(0.0f));
	CHECK(circle.m_radius == Approx(0.0f));
}


SCENARIO("Loading a b2CircleShape from JS", "[loadCircle]")
{
	GIVEN("a duktape context")
	{
		constexpr char const* const pFullValidJSON = R"JSON({
			"radius": 5.4321,
			"position": [1.2345, 6.7890]
		})JSON";
		constexpr char const* const pInvalidJSON = R"JSON({
			"position": [1]
		})JSON";

		testObjectLoader<b2CircleShape>(
			initCircleShape,
			ds::loadCircle,
			pInvalidJSON,
			pFullValidJSON,
			nullptr, // Partial valid JSON.
			checkCircleShapeFullyLoaded,
			nullptr,
			checkCircleUnmodified
		);
	}

	GIVEN("incomplete data")
	{
		testutils::duk_context_ptr pContext{duk_create_heap_default()};

		THEN("a missing radius is invalid")
		{
			testutils::pushJSONObject(
				pContext.get(), "{\"position\": [1, 2]}");
			b2CircleShape circle;
			REQUIRE(!ds::loadCircle(pContext.get(), -1, &circle));
		}
	}
}


constexpr std::size_t numVertices = 4ul;
constexpr std::pair<float, float> vertices[numVertices] = {
	{-1.0f, -1.0f},
	{ 1.0f, -1.0f},
	{ 1.0f,  1.0f},
	{-1.0f,  1.0f},
};


inline void initPolygonShape(b2PolygonShape* pShape)
{
	pShape->m_count = 0;
}


inline void checkPolygonShapeFullyLoaded(b2PolygonShape const& shape)
{
	REQUIRE(shape.m_count == numVertices);

	// Box2D's b2PolygonShape::Set is allowed to re-order vertices, so we must
	// calculate an offset to the first of our vertices.
	unsigned offset = 0u;
	for (; offset < numVertices; ++offset)
	{
		if (
			shape.m_vertices[offset].x == vertices[0].first &&
			shape.m_vertices[offset].y == vertices[0].second
		)
		{
			break;
		}
	}

	REQUIRE(offset < numVertices);

	for (unsigned i = 0u; i < numVertices; ++i)
	{
		unsigned j = (i + offset) % numVertices;
		CHECK(shape.m_vertices[j].x == Approx(vertices[i].first));
		CHECK(shape.m_vertices[j].y == Approx(vertices[i].second));
	}
}


inline void checkPolygonShapePartiallyLoaded(b2PolygonShape const& shape)
{
	REQUIRE(shape.m_count == 0);
}


void checkPolygonUnmodified(b2PolygonShape const& shape)
{
	REQUIRE(shape.m_count == 0);
}


SCENARIO("Loading a b2PolygonShape from JS", "[loadPolygon]")
{
	GIVEN("a duktape context")
	{
		constexpr char const* const pFullValidJSON = R"JSON({
			"vertices": [
				[-1, -1],
				[ 1, -1],
				[ 1,  1],
				[-1,  1]
			]
		})JSON";
		constexpr char const* const pInvalidJSON = "{\"vertices\": [[1]]}";

		testObjectLoader<b2PolygonShape>(
			initPolygonShape,
			ds::loadPolygon,
			pInvalidJSON,
			pFullValidJSON,
			nullptr,
			checkPolygonShapeFullyLoaded,
			nullptr, // Partially loaded check.
			checkPolygonUnmodified
		);
	}

	GIVEN("incomplete data")
	{
		WHEN("the vertices property is missing")
		{
			THEN("the operation fails")
			{
				testutils::duk_context_ptr pContext{duk_create_heap_default()};
				b2PolygonShape polygon;
				testutils::pushJSONObject(pContext.get(), "{}");
				bool const success = ds::loadPolygon(
					pContext.get(), -1, &polygon);
				REQUIRE(success == false);
			}
		}
	}
}


void initEdgeShape(b2EdgeShape* pEdge)
{
	pEdge->m_vertex0.Set(0.0f, 0.0f);
	pEdge->m_vertex1.Set(0.0f, 0.0f);
	pEdge->m_vertex2.Set(0.0f, 0.0f);
	pEdge->m_vertex3.Set(0.0f, 0.0f);
}


void checkEdgeShapeFullyLoaded(b2EdgeShape const& edge)
{
	CHECK(edge.m_vertex0.x == Approx(-2.0f));
	CHECK(edge.m_vertex0.y == Approx(-2.0f));
	CHECK(edge.m_vertex1.x == Approx(-1.0f));
	CHECK(edge.m_vertex1.y == Approx(-1.0f));
	CHECK(edge.m_vertex2.x == Approx( 1.0f));
	CHECK(edge.m_vertex2.y == Approx( 1.0f));
	CHECK(edge.m_vertex3.x == Approx( 2.0f));
	CHECK(edge.m_vertex3.y == Approx( 2.0f));
}


void checkEdgeShapePartiallyLoaded(b2EdgeShape const& edge)
{
	CHECK(edge.m_vertex1.x == Approx(-1.0f));
	CHECK(edge.m_vertex1.y == Approx(-1.0f));
	CHECK(edge.m_vertex2.x == Approx( 1.0f));
	CHECK(edge.m_vertex2.y == Approx( 1.0f));
}


void checkEdgeUnmodified(b2EdgeShape const& shape)
{
	CHECK(shape.m_vertex0.x == Approx(0.0f));
	CHECK(shape.m_vertex0.y == Approx(0.0f));
	CHECK(shape.m_vertex1.x == Approx(0.0f));
	CHECK(shape.m_vertex1.y == Approx(0.0f));
	CHECK(shape.m_vertex2.x == Approx(0.0f));
	CHECK(shape.m_vertex2.y == Approx(0.0f));
	CHECK(shape.m_vertex3.x == Approx(0.0f));
	CHECK(shape.m_vertex3.y == Approx(0.0f));
}


SCENARIO("Loading a b2EdgeShape from JS", "[loadEdge]")
{
	GIVEN("a duktape context")
	{
		constexpr char const* const pFullValidJSON = R"JSON({
			"prev": [-2, -2],
			"v1": [-1, -1],
			"v2": [1, 1],
			"next": [2, 2]
		})JSON";
		constexpr char const* const pPartialValidJSON = R"JSON({
			"v1": [-1, -1],
			"v2": [1, 1]
		})JSON";
		constexpr char const* const pInvalidJSON = "{\"v1\": \"no vertex\"}";

		testObjectLoader<b2EdgeShape>(
			initEdgeShape,
			ds::loadEdge,
			pInvalidJSON,
			pFullValidJSON,
			pPartialValidJSON,
			checkEdgeShapeFullyLoaded,
			checkEdgeShapePartiallyLoaded,
			checkEdgeUnmodified
		);
	}
}
