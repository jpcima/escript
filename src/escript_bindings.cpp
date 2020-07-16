#include "escript_bindings.hpp"
#include "escript.hpp"
#include <elements.hpp>
#include <sstream>
namespace el = cycfi::elements;

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
    engine_obj_free(dst_obj);
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
    element_obj_free(dst_obj);
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
int cmd_button(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    double size = 1.0;
    const char *icon = nullptr;
    const char *icon_pos = nullptr;
    const char *text = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-size", nullptr, &size, "Size", nullptr},
        {TCL_ARGV_STRING, "-icon", nullptr, &icon, "Icon", nullptr},
        {TCL_ARGV_STRING, "-icon_pos", nullptr, &icon_pos, "Icon position", nullptr},
        TCL_ARGV_AUTO_REST, TCL_ARGV_AUTO_HELP, TCL_ARGV_TABLE_END
    };

    Tcl_Obj **rem = nullptr;
    if (Tcl_ParseArgsObjv(interp, info, &objc, objv, &rem) != TCL_OK ||
        parse_positional_objv(interp, rem, TCL_ARGV_STRING, &text, 0) != TCL_OK)
    {
        Tcl_SetResult(interp, (char *)"button: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);

    uint32_t icon_code = 0;
    if (icon && !(icon_code = lookup_icon(icon))) {
        Tcl_SetResult(interp, (char *)"button: invalid icon code", TCL_STATIC);
        return TCL_ERROR;
    }

    uint32_t icon_pos_code = 1;
    if (icon_pos && !(icon_pos_code = lookup_icon_pos(icon_pos))) {
        Tcl_SetResult(interp, (char *)"button: invalid icon position code", TCL_STATIC);
        return TCL_ERROR;
    }

    auto button = icon ? ((icon_pos_code == 1) ?
                          el::share(el::button(icon_code, text, size)) :
                          el::share(el::button(text, icon_code, size))) :
        el::share(el::button(text, size));
    elt->element = button;

    elt->receiver_bool = static_cast<el::receiver<bool> *>(button.get());
    elt->sender_bool = static_cast<el::sender<bool> *>(button.get());

    Tcl_InvalidateStringRep(result);

    if (id) {
        Tcl_Obj *escript = Tcl_GetVar2Ex(interp, "*escript*", nullptr, TCL_GLOBAL_ONLY);
        if (escript && escript->typePtr == &engine_obj_type) {
            escript::script_engine *eng = reinterpret_cast<escript::script_engine *>(
                escript->internalRep.twoPtrValue.ptr1);
            eng->register_element(id, *elt);
        }
    }

    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

uint32_t lookup_icon(cycfi::string_view name)
{
    static const std::unordered_map<cycfi::string_view, uint32_t> icons = {
        #define ICON(x) {#x, el::icons::x}
        ICON(left),
        ICON(right),
        ICON(up),
        ICON(down),
        ICON(left_dir),
        ICON(right_dir),
        ICON(up_dir),
        ICON(down_dir),
        ICON(left_circled),
        ICON(right_circled),
        ICON(up_circled),
        ICON(down_circled),
        ICON(left_open),
        ICON(right_open),
        ICON(up_open),
        ICON(down_open),
        ICON(angle_left),
        ICON(angle_right),
        ICON(angle_up),
        ICON(angle_down),
        ICON(angle_double_left),
        ICON(angle_double_right),
        ICON(angle_double_up),
        ICON(angle_double_down),
        ICON(angle_circled_left),
        ICON(angle_circled_right),
        ICON(angle_circled_up),
        ICON(angle_circled_down),
        ICON(exclamation),
        ICON(block),
        ICON(pencil),
        ICON(pin),
        ICON(resize_vertical),
        ICON(resize_horizontal),
        ICON(move),
        ICON(resize_full_alt),
        ICON(resize_full),
        ICON(resize_small),
        ICON(magnifying_glass),
        ICON(zoom_in),
        ICON(zoom_out),
        ICON(volume_off),
        ICON(volume_down),
        ICON(volume_up),
        ICON(cw),
        ICON(ccw),
        ICON(cycle),
        ICON(level_up),
        ICON(level_down),
        ICON(shuffle),
        ICON(exchange),
        ICON(power),
        ICON(play),
        ICON(stop),
        ICON(pause),
        ICON(record),
        ICON(to_end),
        ICON(to_start),
        ICON(fast_forward),
        ICON(fast_backward),
        ICON(wrench),
        ICON(trash),
        ICON(trash_empty),
        ICON(ok),
        ICON(cancel),
        ICON(plus),
        ICON(minus),
        ICON(cog),
        ICON(doc),
        ICON(docs),
        ICON(lock_open),
        ICON(lock),
        ICON(sliders),
        ICON(floppy),
        ICON(attention),
        ICON(info),
        ICON(error),
        ICON(lightbulb),
        ICON(mixer),
        ICON(hand),
        ICON(question),
        ICON(menu),
        #undef ICON
    };

    auto it = icons.find(name);
    return (it != icons.end()) ? it->second : 0;
}

uint32_t lookup_icon_pos(cycfi::string_view name)
{
    return (name == "left") ? 1 : (name == "right") ? 2 : 0;
}

} // namespace escript
