#include <duk_config.h>

namespace dukdemo {
namespace scripting {


#define GLOBAL_HIDDEN_SYMBOL(str) ("!G" str)
#define LOCAL_HIDDEN_SYMBOL(str)  ("!L" str)


constexpr char const* const g_ownBodyPtrSym = LOCAL_HIDDEN_SYMBOL("mpBody");
constexpr char const* const g_ownWorldPtrSym = LOCAL_HIDDEN_SYMBOL("mpWrld");

constexpr char const* const g_ownWorldPropSym = "world";


constexpr char const* const g_bodyProtoSym = GLOBAL_HIDDEN_SYMBOL("BodyPr");
constexpr char const* const g_worldProtoSym = GLOBAL_HIDDEN_SYMBOL("WrldPr");

constexpr char const* const g_worldCtorSym = "World";


void*
getPointerFromThis(duk_context* pContext, char const* const pPropName);


} // namespace scripting
} // namespace dukdemo
