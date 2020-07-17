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
int cmd_button(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    double size = 1.0;
    uint32_t icon = 0;
    uint32_t icon_pos = ESCRIPT_ICON_POS_LEFT;
    el::color body_color = el::get_theme().default_button_color;
    const char *text = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-size", nullptr, &size, "Size", nullptr},
        {ESCRIPT_ARGV_ICON, "-icon", nullptr, &icon, "Icon", nullptr},
        {ESCRIPT_ARGV_ICON_POS, "-icon_pos", nullptr, &icon_pos, "Icon position", nullptr},
        {ESCRIPT_ARGV_COLOR, "-body_color", nullptr, &body_color, "Body color", nullptr},
        {TCL_ARGV_STRING, nullptr, nullptr, &text, "Text", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"button: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto button = icon ? ((icon_pos == ESCRIPT_ICON_POS_LEFT) ?
                          el::share(el::button(icon, text, size, body_color)) :
                          el::share(el::button(text, icon, size, body_color))) :
        el::share(el::button(text, size, body_color));

    Tcl_Obj *result = create_element_result(interp, id, *button);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_basic_thumb(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    el::color color = el::colors::black;
    int size = 0;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_COLOR, "-color", nullptr, &color, "Color", nullptr},
        {TCL_ARGV_INT, nullptr, nullptr, &size, "Size", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"basic_thumb: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto thumb = el::share(dy::basic_thumb(size, color));

    Tcl_Obj *result = create_element_result(interp, id, *thumb);
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
        {TCL_ARGV_INT, nullptr, nullptr, &size, "Size", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
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

    Tcl_Obj *result = create_element_result(interp, id, *track);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_slider(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    double init_value = 0.0;
    element_obj *thumb = nullptr;
    element_obj *track = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-init_value", nullptr, &init_value, "Initial value", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &thumb, "Thumb", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &track, "Track", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"slider: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto slider = el::share(el::slider(el::hold(thumb->element), el::hold(track->element), init_value));

    Tcl_Obj *result = create_element_result(interp, id, *slider);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_slider_marks(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    element_obj *subject = nullptr;
    int size = 0;
    int num_divs = 50;
    int major_divs = 10;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_INT, "-num_divs", nullptr, &num_divs, "Number of divisions", nullptr},
        {TCL_ARGV_INT, "-major_divs", nullptr, &major_divs, "Number of major divisions", nullptr},
        {TCL_ARGV_INT, nullptr, nullptr, &size, "Size", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"slider_marks: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto slider_marks = el::share(dy::slider_marks(el::hold(subject->element), size, num_divs, major_divs));

    Tcl_Obj *result = create_element_result(interp, id, *slider_marks);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_slider_labels(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    element_obj *subject = nullptr;
    int size = 0;
    double font_size = 0;
    std::vector<std::string> labels;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_INT, nullptr, nullptr, &size, "Size", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &size, "Font size", nullptr},
        {ESCRIPT_ARGV_STRING_LIST, nullptr, nullptr, &labels, "Labels", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"slider_labels: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto slider_labels = el::share(dy::slider_labels(el::hold(subject->element), size, font_size));
    slider_labels->_labels = labels;

    Tcl_Obj *result = create_element_result(interp, id, *slider_labels);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

template <class T>
int cmd_generic_composite(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    std::vector<element_obj *> items;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT_REST, nullptr, nullptr, &items, "Items", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"composite: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto composite = el::share(T());
    for (element_obj *item : items)
        composite->push_back(item->element);

    Tcl_Obj *result = create_element_result(interp, id, *composite);
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

template <class F>
int cmd_generic_proxy_0m(F &&make, ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    element_obj *subject = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"proxy: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto proxy = el::share(make(el::hold(subject->element)));

    Tcl_Obj *result = create_element_result(interp, id, *proxy);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}

int cmd_align_left(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    auto make = [](el::indirect<el::shared_element<el::element>> subject) { return el::align_left(std::move(subject)); };
    return cmd_generic_proxy_0m(make, client_data, interp, objc, objv);
}

int cmd_align_center(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    auto make = [](el::indirect<el::shared_element<el::element>> subject) { return el::align_center(std::move(subject)); };
    return cmd_generic_proxy_0m(make, client_data, interp, objc, objv);
}

int cmd_align_right(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    auto make = [](el::indirect<el::shared_element<el::element>> subject) { return el::align_right(std::move(subject)); };
    return cmd_generic_proxy_0m(make, client_data, interp, objc, objv);
}

int cmd_align_top(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    auto make = [](el::indirect<el::shared_element<el::element>> subject) { return el::align_top(std::move(subject)); };
    return cmd_generic_proxy_0m(make, client_data, interp, objc, objv);
}

int cmd_align_middle(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    auto make = [](el::indirect<el::shared_element<el::element>> subject) { return el::align_middle(std::move(subject)); };
    return cmd_generic_proxy_0m(make, client_data, interp, objc, objv);
}

int cmd_align_bottom(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    auto make = [](el::indirect<el::shared_element<el::element>> subject) { return el::align_bottom(std::move(subject)); };
    return cmd_generic_proxy_0m(make, client_data, interp, objc, objv);
}

template <class T, class Coord = float>
int cmd_generic_proxy_1f(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    double coord = 0.0;
    element_obj *subject = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &coord, "Coord", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"proxy: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto proxy = el::share(T(Coord(coord), el::hold(subject->element)));

    Tcl_Obj *result = create_element_result(interp, id, *proxy);
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

int cmd_halign(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::halign_element<el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

int cmd_valign(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return cmd_generic_proxy_1f<el::valign_element<el::indirect<el::shared_element<el::element>>>>(client_data, interp, objc, objv);
}

template <class T, class Coord = float>
int cmd_generic_proxy_2f(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id = nullptr;
    double coord1 = 0.0;
    double coord2 = 0.0;
    element_obj *subject = nullptr;

    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &coord1, "Coord 1", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &coord2, "Coord 2", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"proxy: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto proxy = el::share(T({Coord(coord1), Coord(coord2)}, el::hold(subject->element)));

    Tcl_Obj *result = create_element_result(interp, id, *proxy);
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
        {TCL_ARGV_FLOAT, "-scale", nullptr, &scale, "Scale factor", nullptr},
        {TCL_ARGV_STRING, nullptr, nullptr, &filename, "File name", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"proxy: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto image = el::share(T(filename, scale));

    Tcl_Obj *result = create_element_result(interp, id, *image);
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
        {TCL_ARGV_STRING, nullptr, nullptr, &filename, "File name", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &height, "Height", nullptr},
        TCL_ARGV_TABLE_END
    };

    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"proxy: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }

    auto sprite = el::share(el::sprite(filename, height, scale));

    Tcl_Obj *result = create_element_result(interp, id, *sprite);
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

} // namespace escript
