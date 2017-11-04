#ifndef DUKDEMO_INCLUDE__DUKDEMO__SCRIPTING__UTIL__H
#define DUKDEMO_INCLUDE__DUKDEMO__SCRIPTING__UTIL__H
#include <duk_config.h>


class b2Vec2;


namespace dukdemo {
namespace scripting {


#define GLOBAL_HIDDEN_SYMBOL(str) ("!G" str)
#define LOCAL_HIDDEN_SYMBOL(str)  ("!L" str)


constexpr char const* const g_ownBodyPtrSym = LOCAL_HIDDEN_SYMBOL("mpBody");
constexpr char const* const g_ownWorldPtrSym = LOCAL_HIDDEN_SYMBOL("mpWrld");

constexpr char const* const g_ownWorldPropSym = "world";


constexpr char const* const g_bodyProtoSym = GLOBAL_HIDDEN_SYMBOL("BProto");
constexpr char const* const g_worldProtoSym = GLOBAL_HIDDEN_SYMBOL("WProto");

constexpr char const* const g_worldCtorSym = "World";


void*
getPointerFromThis(duk_context* pContext, char const* const pPropName);


void
writeVec2ToArray(duk_context* pContext, duk_idx_t arrayIdx, b2Vec2 const& vec);


} // namespace scripting
} // namespace dukdemo
#endif // #ifndef DUKDEMO_INCLUDE__DUKDEMO__SCRIPTING__UTIL__H
