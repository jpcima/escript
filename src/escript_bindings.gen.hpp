namespace detail {
    static const int k_true = 1;
}
static int cmd_button(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double size {1.0};
    uint32_t icon {};
    uint32_t icon_pos {ESCRIPT_ICON_POS_LEFT};
    el::color body_color {el::get_theme().default_button_color};
    const char * text {};
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
    auto element =
    (icon && icon_pos == ESCRIPT_ICON_POS_LEFT) ? el::share(el::button(icon, text, size, body_color)) :
    icon ? el::share(el::button(text, icon, size, body_color)) :
    el::share(el::button(text, size, body_color));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_basic_thumb(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    el::color color {el::colors::black};
    int size {};
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
    auto element = el::share(dy::basic_thumb(size, color));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_basic_track(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    el::color color {el::colors::black};
    int vertical {};
    int size {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_COLOR, "-color", nullptr, &color, "Color", nullptr},
        {TCL_ARGV_CONSTANT, "-vertical", ((void *)&detail::k_true), &vertical, "Vertical", nullptr},
        {TCL_ARGV_INT, nullptr, nullptr, &size, "Size", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"basic_track: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(dy::basic_track(size, vertical, color));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_slider(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double init_value {};
    element_obj * thumb {};
    element_obj * track {};
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
    auto element = el::share(el::slider(el::hold(thumb->element), el::hold(track->element), init_value));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_slider_marks(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    int num_divs {50};
    int major_divs {10};
    int size {};
    element_obj * subject {};
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
    auto element = el::share(dy::slider_marks(el::hold(subject->element), size, num_divs, major_divs));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_slider_labels(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    int size {};
    double font_size {};
    std::vector<std::string> labels {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_INT, nullptr, nullptr, &size, "Size", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &font_size, "Font size", nullptr},
        {ESCRIPT_ARGV_STRING_LIST, nullptr, nullptr, &labels, "Labels", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"slider_labels: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(dy::slider_labels(el::hold(subject->element), size, font_size));
element->_labels = labels;
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_htile(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    std::vector<element_obj *> items {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT_REST, nullptr, nullptr, &items, "Items", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"htile: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::htile_composite());
for (element_obj *item : items)
    element->push_back(item->element);
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_vtile(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    std::vector<element_obj *> items {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT_REST, nullptr, nullptr, &items, "Items", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"vtile: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::vtile_composite());
for (element_obj *item : items)
    element->push_back(item->element);
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_layer(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    std::vector<element_obj *> items {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT_REST, nullptr, nullptr, &items, "Items", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"layer: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::layer_composite());
for (element_obj *item : items)
    element->push_back(item->element);
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_deck(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    std::vector<element_obj *> items {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT_REST, nullptr, nullptr, &items, "Items", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"deck: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::deck_composite());
for (element_obj *item : items)
    element->push_back(item->element);
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_align_left(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"align_left: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::align_left(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_align_center(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"align_center: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::align_center(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_align_right(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"align_right: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::align_right(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_align_top(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"align_top: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::align_top(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_align_middle(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"align_middle: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::align_middle(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_align_bottom(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"align_bottom: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::align_bottom(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_hsize(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double width {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &width, "Width", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"hsize: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::hsize(width, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_vsize(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double height {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &height, "Height", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"vsize: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::vsize(height, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_hmin_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double width {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &width, "Width", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"hmin_size: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::hmin_size(width, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_vmin_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double height {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &height, "Height", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"vmin_size: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::vmin_size(height, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_hmax_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double width {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &width, "Width", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"hmax_size: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::hmax_size(width, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_left_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double size {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &size, "Size", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"left_margin: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::left_margin(size, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_top_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double size {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &size, "Size", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"top_margin: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::top_margin(size, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_right_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double size {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &size, "Size", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"right_margin: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::right_margin(size, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_bottom_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double size {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &size, "Size", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"bottom_margin: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::bottom_margin(size, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_halign(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double align {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &align, "Alignment", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"halign: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::halign(align, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_valign(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double align {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &align, "Alignment", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"valign: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::valign(align, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_min_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double width {};
    double height {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &width, "Width", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &height, "Height", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"min_size: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::min_size(el::point(width, height), el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_max_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double width {};
    double height {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &width, "Width", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &height, "Height", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"max_size: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::max_size(el::point(width, height), el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_xside_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double left {};
    double right {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &left, "Left", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &right, "Right", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"xside_margin: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::xside_margin(el::xside_margin_rect(left, right), el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_yside_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double top {};
    double bottom {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &top, "Top", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &bottom, "Bottom", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"yside_margin: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::yside_margin(el::yside_margin_rect(top, bottom), el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_image(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double scale {1.0};
    const char * filename {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-scale", nullptr, &scale, "Scale factor", nullptr},
        {TCL_ARGV_STRING, nullptr, nullptr, &filename, "File name", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"image: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::image(filename, scale));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_gizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double scale {1.0};
    const char * filename {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-scale", nullptr, &scale, "Scale factor", nullptr},
        {TCL_ARGV_STRING, nullptr, nullptr, &filename, "File name", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"gizmo: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::gizmo(filename, scale));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_hgizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double scale {1.0};
    const char * filename {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-scale", nullptr, &scale, "Scale factor", nullptr},
        {TCL_ARGV_STRING, nullptr, nullptr, &filename, "File name", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"hgizmo: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::hgizmo(filename, scale));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_vgizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double scale {1.0};
    const char * filename {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-scale", nullptr, &scale, "Scale factor", nullptr},
        {TCL_ARGV_STRING, nullptr, nullptr, &filename, "File name", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"vgizmo: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::vgizmo(filename, scale));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_sprite(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double scale {1.0};
    const char * filename {};
    double height {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, "-scale", nullptr, &scale, "Scale factor", nullptr},
        {TCL_ARGV_STRING, nullptr, nullptr, &filename, "File name", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &height, "Height", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        Tcl_SetResult(interp, (char *)"sprite: invalid command arguments", TCL_STATIC);
        return TCL_ERROR;
    }
    auto element = el::share(el::sprite(filename, height, scale));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
void register_element_commands(Tcl_Interp *interp, ClientData client_data)
{
    Tcl_CreateObjCommand(interp, "button", &cmd_button, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "basic_thumb", &cmd_basic_thumb, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "basic_track", &cmd_basic_track, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "slider", &cmd_slider, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "slider_marks", &cmd_slider_marks, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "slider_labels", &cmd_slider_labels, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "htile", &cmd_htile, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "vtile", &cmd_vtile, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "layer", &cmd_layer, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "deck", &cmd_deck, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_left", &cmd_align_left, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_center", &cmd_align_center, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_right", &cmd_align_right, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_top", &cmd_align_top, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_middle", &cmd_align_middle, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_bottom", &cmd_align_bottom, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "hsize", &cmd_hsize, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "vsize", &cmd_vsize, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "hmin_size", &cmd_hmin_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "vmin_size", &cmd_vmin_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "hmax_size", &cmd_hmax_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "left_margin", &cmd_left_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "top_margin", &cmd_top_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "right_margin", &cmd_right_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "bottom_margin", &cmd_bottom_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "halign", &cmd_halign, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "valign", &cmd_valign, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "min_size", &cmd_min_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "max_size", &cmd_max_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "xside_margin", &cmd_xside_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "yside_margin", &cmd_yside_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "image", &cmd_image, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "gizmo", &cmd_gizmo, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "hgizmo", &cmd_hgizmo, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "vgizmo", &cmd_vgizmo, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "sprite", &cmd_sprite, client_data, nullptr);
}
