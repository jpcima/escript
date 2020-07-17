#include "escript_bindings.hpp"
#include "escript.hpp"
#include "dynamic/slider.hpp"
#include <elements.hpp>
#include <unordered_map>
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
    const char *body_color = nullptr;
    const char *text = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-size", nullptr, &size, "Size", nullptr},
        {TCL_ARGV_STRING, "-icon", nullptr, &icon, "Icon", nullptr},
        {TCL_ARGV_STRING, "-icon_pos", nullptr, &icon_pos, "Icon position", nullptr},
        {TCL_ARGV_STRING, "-body_color", nullptr, &body_color, "Body color", nullptr},
        TCL_ARGV_AUTO_REST, TCL_ARGV_AUTO_HELP, TCL_ARGV_TABLE_END
    };

    if (parse_objv(interp, info, objc, objv, TCL_ARGV_STRING, &text, 0) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"button: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

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

    el::color body_color_code;
    if (body_color) {
        if (!parse_color(body_color, body_color_code)) {
            Tcl_SetResult(interp, (char *)"button: invalid body color code", TCL_STATIC);
            return TCL_ERROR;
        }
    } else
        body_color_code = el::get_theme().default_button_color;

    auto button = icon ? ((icon_pos_code == 1) ?
                          el::share(el::button(icon_code, text, size, body_color_code)) :
                          el::share(el::button(text, icon_code, size, body_color_code))) :
        el::share(el::button(text, size, body_color_code));

    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);
    elt->element = button;
    elt->receiver_bool = static_cast<el::receiver<bool> *>(button.get());
    elt->sender_bool = static_cast<el::sender<bool> *>(button.get());
    Tcl_InvalidateStringRep(result);
    register_element(interp, id, *elt);

    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_basic_thumb(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    const char *color = nullptr;
    int size = 0;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_STRING, "-color", nullptr, &color, "Color", nullptr},
        TCL_ARGV_AUTO_REST, TCL_ARGV_AUTO_HELP, TCL_ARGV_TABLE_END
    };

    if (parse_objv(interp, info, objc, objv, TCL_ARGV_INT, &size, 0) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"basic_thumb: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    el::color color_code = el::colors::black;
    if (color) {
        if (!parse_color(color, color_code)) {
            Tcl_SetResult(interp, (char *)"basic_thumb: invalid color code", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    auto thumb = el::share(dy::basic_thumb(size, color_code));

    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);
    elt->element = thumb;
    Tcl_InvalidateStringRep(result);
    register_element(interp, id, *elt);

    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_basic_track(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    const char *color = nullptr;
    int vertical = 0;
    int size = 0;

    int k_true = 1;
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_STRING, "-color", nullptr, &color, "Color", nullptr},
        {TCL_ARGV_CONSTANT, "-vertical", &k_true, &vertical, "Vertical", nullptr},
        TCL_ARGV_AUTO_REST, TCL_ARGV_AUTO_HELP, TCL_ARGV_TABLE_END
    };

    if (parse_objv(interp, info, objc, objv, TCL_ARGV_INT, &size, 0) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"basic_track: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    el::color color_code = el::colors::black;
    if (color) {
        if (!parse_color(color, color_code)) {
            Tcl_SetResult(interp, (char *)"basic_track: invalid color code", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    auto track = el::share(dy::basic_track(size, vertical, color_code));

    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);
    elt->element = track;
    Tcl_InvalidateStringRep(result);
    register_element(interp, id, *elt);

    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_slider(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    double init_value = 0.0;
    Tcl_Obj *thumb = nullptr;
    Tcl_Obj *track = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-init_value", nullptr, &init_value, "Initial value", nullptr},
        TCL_ARGV_AUTO_REST, TCL_ARGV_AUTO_HELP, TCL_ARGV_TABLE_END
    };

    if (parse_objv(interp, info, objc, objv, ESCRIPT_ARGV_OBJ, &thumb, ESCRIPT_ARGV_OBJ, &track, 0) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"slider: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    if (thumb->typePtr != &element_obj_type) {
        Tcl_SetResult(interp, (char *)"slider: the thumb is not of type ELEMENT", TCL_STATIC);
        return TCL_ERROR;
    }
    if (track->typePtr != &element_obj_type) {
        Tcl_SetResult(interp, (char *)"slider: the track is not of type ELEMENT", TCL_STATIC);
        return TCL_ERROR;
    }

    element_obj *elt_thumb = reinterpret_cast<element_obj *>(thumb->internalRep.twoPtrValue.ptr1);
    element_obj *elt_track = reinterpret_cast<element_obj *>(track->internalRep.twoPtrValue.ptr1);

    auto slider = el::share(el::slider(el::hold(elt_thumb->element), el::hold(elt_track->element), init_value));

    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);
    elt->element = slider;
    Tcl_InvalidateStringRep(result);
    register_element(interp, id, *elt);

    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

template <class T>
int cmd_generic_composite(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        TCL_ARGV_AUTO_REST, TCL_ARGV_AUTO_HELP, TCL_ARGV_TABLE_END
    };

    Tcl_Obj **rem = nullptr;
    if (Tcl_ParseArgsObjv(interp, info, &objc, objv, &rem) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"composite: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    ck_u rem_cleanup(rem);

    auto composite = el::share(T());
    for (int i = 1; i < objc; ++i) {
        Tcl_Obj *item = rem[i];
        element_obj *elt = reinterpret_cast<element_obj *>(item->internalRep.twoPtrValue.ptr1);
        if (item->typePtr != &element_obj_type) {
            Tcl_SetResult(interp, (char *)"composite: the item is not of type ELEMENT", TCL_STATIC);
            return TCL_ERROR;
        }
        composite->push_back(elt->element);
    }

    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);
    elt->element = composite;
    Tcl_InvalidateStringRep(result);
    register_element(interp, id, *elt);

    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_htile(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_composite<el::htile_composite>(client_data, interp, objc, objv);
}

int cmd_vtile(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_composite<el::vtile_composite>(client_data, interp, objc, objv);
}

int cmd_layer(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_composite<el::layer_composite>(client_data, interp, objc, objv);
}

int cmd_deck(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_composite<el::deck_composite>(client_data, interp, objc, objv);
}

template <class T, class Coord = float>
int cmd_generic_proxy_1f(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    double coord = 0.0;
    Tcl_Obj *subject = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        TCL_ARGV_AUTO_REST, TCL_ARGV_AUTO_HELP, TCL_ARGV_TABLE_END
    };

    if (parse_objv(interp, info, objc, objv, TCL_ARGV_FLOAT, &coord, ESCRIPT_ARGV_OBJ, &subject, 0) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"proxy: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    if (subject->typePtr != &element_obj_type) {
        Tcl_SetResult(interp, (char *)"proxy: the item is not of type ELEMENT", TCL_STATIC);
        return TCL_ERROR;
    }

    element_obj *subject_elt = reinterpret_cast<element_obj *>(subject->internalRep.twoPtrValue.ptr1);
    auto proxy = el::share(T(Coord(coord), el::hold(subject_elt->element)));

    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);
    elt->element = proxy;
    Tcl_InvalidateStringRep(result);
    register_element(interp, id, *elt);

    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_hsize(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::hsize_element<el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_vsize(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::vsize_element<el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_hmin_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::hmin_size_element<el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_vmin_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::vmin_size_element<el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_hmax_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::hmax_size_element<el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

//int cmd_vmax_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
//{
//    return cmd_generic_proxy_1f<el::vmax_size_element<el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
//}

int cmd_left_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::margin_element<el::left_margin_rect, el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_top_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::margin_element<el::top_margin_rect, el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_right_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::margin_element<el::right_margin_rect, el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_bottom_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::margin_element<el::bottom_margin_rect, el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

template <class T, class Coord = float>
int cmd_generic_proxy_2f(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    double coord1 = 0.0;
    double coord2 = 0.0;
    Tcl_Obj *subject = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        TCL_ARGV_AUTO_REST, TCL_ARGV_AUTO_HELP, TCL_ARGV_TABLE_END
    };

    if (parse_objv(interp, info, objc, objv, TCL_ARGV_FLOAT, &coord1, TCL_ARGV_FLOAT, &coord2, ESCRIPT_ARGV_OBJ, &subject, 0) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"proxy: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    if (subject->typePtr != &element_obj_type) {
        Tcl_SetResult(interp, (char *)"proxy: the item is not of type ELEMENT", TCL_STATIC);
        return TCL_ERROR;
    }

    element_obj *subject_elt = reinterpret_cast<element_obj *>(subject->internalRep.twoPtrValue.ptr1);
    auto proxy = el::share(T({Coord(coord1), Coord(coord2)}, el::hold(subject_elt->element)));

    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);
    elt->element = proxy;
    Tcl_InvalidateStringRep(result);
    register_element(interp, id, *elt);

    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_min_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_2f<el::min_size_element<el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_max_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_2f<el::max_size_element<el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_xside_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_2f<el::margin_element<el::xside_margin_rect, el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_yside_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_2f<el::margin_element<el::yside_margin_rect, el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

template <class T>
int cmd_generic_image(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    double scale = 1.0;
    const char *filename = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-scale", nullptr, &id, "Scale factor", nullptr},
        TCL_ARGV_AUTO_REST, TCL_ARGV_AUTO_HELP, TCL_ARGV_TABLE_END
    };

    if (parse_objv(interp, info, objc, objv, TCL_ARGV_STRING, &filename, 0) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"proxy: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto image = el::share(T(filename, scale));

    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);
    elt->element = image;
    Tcl_InvalidateStringRep(result);
    register_element(interp, id, *elt);

    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_image(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_image<el::image>(client_data, interp, objc, objv);
}

int cmd_gizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_image<el::gizmo>(client_data, interp, objc, objv);
}

int cmd_hgizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_image<el::hgizmo>(client_data, interp, objc, objv);
}

int cmd_vgizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_image<el::vgizmo>(client_data, interp, objc, objv);
}

int cmd_sprite(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    double scale = 1.0;
    const char *filename = nullptr;
    double height = 0.0;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-scale", nullptr, &id, "Scale factor", nullptr},
        TCL_ARGV_AUTO_REST, TCL_ARGV_AUTO_HELP, TCL_ARGV_TABLE_END
    };

    if (parse_objv(interp, info, objc, objv, TCL_ARGV_STRING, &filename, TCL_ARGV_FLOAT, &height, 0) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"proxy: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto sprite = el::share(el::sprite(filename, height, scale));

    Tcl_Obj *result = element_obj_new();
    element_obj *elt = reinterpret_cast<element_obj *>(result->internalRep.twoPtrValue.ptr1);
    elt->element = sprite;
    elt->receiver_int = static_cast<el::receiver<int> *>(sprite.get());
    elt->receiver_double = static_cast<el::receiver<double> *>(sprite.get());
    Tcl_InvalidateStringRep(result);
    register_element(interp, id, *elt);

    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

///
void register_element(Tcl_Interp *interp, const char *id, const element_obj &elt)
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

static int hex_digit_from_char(char c)
{
    return (c >= '0' && c <= '9') ? (c - '0') :
        (c >= 'a' && c <= 'z') ? (c - 'a' + 10) :
        (c >= 'A' && c <= 'Z') ? (c - 'A' + 10) : -1;
}

bool parse_color(cycfi::string_view name, cycfi::elements::color &color)
{
    if (name.empty() || name[0] != '#')
        return false;

    name = name.substr(1);
    size_t length = name.size();

    uint32_t rgba = 0;
    if (length == 6 || length == 8) {
        for (size_t i = 0; i < length; ++i) {
            int d = hex_digit_from_char(name[i]);
            if (d == -1)
                return false;
            rgba = (rgba << 4) | d;
        }
    }
    if (length == 6)
        rgba = (rgba << 8) | 0xff;

    color.red = (rgba >> 24) / 255.0;
    color.green = ((rgba >> 16) & 0xff) / 255.0;
    color.blue = ((rgba >> 8) & 0xff) / 255.0;
    color.alpha = (rgba & 0xff) / 255.0;

    return true;
}

} // namespace escript
