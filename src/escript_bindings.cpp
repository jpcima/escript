#include "escript_bindings.hpp"
#include "escript.hpp"
#include "dynamic/slider.hpp"
#include <elements.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
namespace el = cycfi::elements;
namespace dy = dynamic::elements;

namespace escript {

Tcl_Obj *engine_obj_new(escript::script_engine *engine)
{
    Tcl_Obj *obj = Tcl_NewObj();
    if (!obj)
        throw std::bad_alloc();

    obj->internalRep.twoPtrValue.ptr1 = engine;
    obj->typePtr = &engine_obj_type;

    Tcl_InvalidateStringRep(obj);
    return obj;
}

static void engine_obj_free(Tcl_Obj *obj)
{
    obj->internalRep.twoPtrValue.ptr1 = nullptr;
    obj->typePtr = nullptr;
}

static void engine_obj_dup(Tcl_Obj *src_obj, Tcl_Obj *dst_obj)
{
    free_internal_rep(dst_obj);
    dst_obj->internalRep.twoPtrValue.ptr1 = src_obj->internalRep.twoPtrValue.ptr1;
    dst_obj->typePtr = &engine_obj_type;
}

static void engine_obj_update(Tcl_Obj *obj)
{
    escript::script_engine *rep = reinterpret_cast<escript::script_engine *>(
        obj->internalRep.twoPtrValue.ptr1);

    std::ostringstream os;
    os << "<The ENGINE " << std::hex << rep << ">";
    std::string str = os.str();

    obj->bytes = Tcl_Alloc(str.size() + 1);
    obj->length = str.size();
    memcpy(obj->bytes, str.c_str(), str.size() + 1);
}

static int engine_obj_set(Tcl_Interp *interp, Tcl_Obj *obj)
{
    (void)interp;
    return TCL_ERROR;
}

Tcl_ObjType engine_obj_type = {
    "engine",
    &engine_obj_free,
    &engine_obj_dup,
    &engine_obj_update,
    &engine_obj_set,
};

///
static Tcl_Obj *element_obj_new()
{
    std::unique_ptr<element_obj> elt{new element_obj};

    Tcl_Obj *obj = Tcl_NewObj();
    if (!obj)
        throw std::bad_alloc();

    obj->internalRep.twoPtrValue.ptr1 = elt.release();
    obj->typePtr = &element_obj_type;

    Tcl_InvalidateStringRep(obj);
    return obj;
}

static void element_obj_free(Tcl_Obj *obj)
{
    element_obj *rep = reinterpret_cast<element_obj *>(obj->internalRep.twoPtrValue.ptr1);
    delete rep;
    obj->internalRep.twoPtrValue.ptr1 = nullptr;
    obj->typePtr = nullptr;
}

static void element_obj_dup(Tcl_Obj *src_obj, Tcl_Obj *dst_obj)
{
    free_internal_rep(dst_obj);
    dst_obj->internalRep.twoPtrValue.ptr1 = new element_obj(
        *reinterpret_cast<element_obj *>(src_obj->internalRep.twoPtrValue.ptr1));
    dst_obj->typePtr = &element_obj_type;
}

static void element_obj_update(Tcl_Obj *obj)
{
    element_obj *rep = reinterpret_cast<element_obj *>(obj->internalRep.twoPtrValue.ptr1);

    std::ostringstream os;
    os << "<The ELEMENT " << std::hex << rep->element.get() << ">";
    std::string str = os.str();

    obj->bytes = Tcl_Alloc(str.size() + 1);
    obj->length = str.size();
    memcpy(obj->bytes, str.c_str(), str.size() + 1);
}

static int element_obj_set(Tcl_Interp *interp, Tcl_Obj *obj)
{
    (void)interp;
    return TCL_ERROR;
}

Tcl_ObjType element_obj_type = {
    "element",
    &element_obj_free,
    &element_obj_dup,
    &element_obj_update,
    &element_obj_set,
};

///
template <class I, class E>
typename std::enable_if<std::is_base_of<I, E>::value, bool>::type
get_element_interface(I &interface, E &element)
{
    interface = static_cast<I &>(element);
    return true;
}

template <class I, class E>
typename std::enable_if<!std::is_base_of<I, E>::value, bool>::type
get_element_interface(I &interface, E &element)
{
    return false;
}

///
static void register_element(Tcl_Interp *interp, const char *id, const element_obj &elt)
{
    if (!id)
        return;

    Tcl_Obj *escript = Tcl_GetVar2Ex(interp, "*escript*", nullptr, TCL_GLOBAL_ONLY);
    if (!escript || escript->typePtr != &engine_obj_type)
        return;

    escript::script_engine *eng = reinterpret_cast<escript::script_engine *>(
        escript->internalRep.twoPtrValue.ptr1);
    eng->register_element(id, elt);
}

///
template <class Element>
static Tcl_Obj *create_element_result(Tcl_Interp *interp, const char *id, Element &element)
{
    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);
    elt->element = el::share(element);
    get_element_interface(*elt->receiver_bool, element);
    get_element_interface(*elt->receiver_int, element);
    get_element_interface(*elt->receiver_double, element);
    get_element_interface(*elt->receiver_string, element);
    get_element_interface(*elt->sender_bool, element);
    get_element_interface(*elt->sender_int, element);
    get_element_interface(*elt->sender_double, element);
    get_element_interface(*elt->sender_string, element);
    get_element_interface(*elt->text_reader, element);
    get_element_interface(*elt->text_writer, element);
    Tcl_InvalidateStringRep(result);
    register_element(interp, id, *elt);
    return result;
}

///
#include "escript_bindings.gen.hpp"

} // namespace escript
