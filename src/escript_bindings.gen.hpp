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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "button: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "basic_thumb: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "basic_track: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "slider: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "slider_marks: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "slider_labels: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "htile: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "vtile: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "layer: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "deck: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::deck_composite());
for (element_obj *item : items)
    element->push_back(item->element);
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_floating(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    el::rect bounds {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_RECT, nullptr, nullptr, &bounds, "Bounds", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "floating: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::floating(bounds, el::hold(subject->element)));
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "align_left: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "align_center: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "align_right: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "align_top: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "align_middle: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "align_bottom: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::align_bottom(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_fixed_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    el::point size {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_POINT, nullptr, nullptr, &size, "Size", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "fixed_size: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::fixed_size(size, el::hold(subject->element)));
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "hsize: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "vsize: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "hmin_size: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "vmin_size: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "hmax_size: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::hmax_size(width, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    el::rect rect {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_RECT, nullptr, nullptr, &rect, "Margin", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "margin: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::margin(rect, el::hold(subject->element)));
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "left_margin: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "top_margin: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "right_margin: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "bottom_margin: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "halign: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "valign: ", msg.c_str(), nullptr);
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
    el::point size {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_POINT, nullptr, nullptr, &size, "Size", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "min_size: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::min_size(size, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_max_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    el::point size {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_POINT, nullptr, nullptr, &size, "Size", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "max_size: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::max_size(size, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_hstretch(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double stretch {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &stretch, "Stretch", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "hstretch: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::hstretch(stretch, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_vstretch(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double stretch {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &stretch, "Stretch", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "vstretch: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::vstretch(stretch, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_no_hstretch(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "no_hstretch: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::no_hstretch(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_no_vstretch(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "no_vstretch: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::no_vstretch(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_hcollapsible(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "hcollapsible: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::hcollapsible(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_vcollapsible(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "vcollapsible: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::vcollapsible(el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_scale(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    double factor {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {TCL_ARGV_FLOAT, nullptr, nullptr, &factor, "Scale factor", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "scale: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::scale(factor, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_limits(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    el::point min {};
    el::point max {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_POINT, nullptr, nullptr, &min, "Minimum", nullptr},
        {ESCRIPT_ARGV_POINT, nullptr, nullptr, &max, "Maximum", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "limits: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::limit(el::view_limits{min, max}, el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_xside_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    std::array<float, 2> left_right {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_SIDE_MARGIN, nullptr, nullptr, &left_right, "Left/Right", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "xside_margin: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::xside_margin(el::xside_margin_rect(left_right[0], left_right[1]), el::hold(subject->element)));
    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
static int cmd_yside_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
    std::array<float, 2> top_bottom {};
    element_obj * subject {};
    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
        {ESCRIPT_ARGV_SIDE_MARGIN, nullptr, nullptr, &top_bottom, "Top/Bottom", nullptr},
        {ESCRIPT_ARGV_ELEMENT, nullptr, nullptr, &subject, "Subject", nullptr},
        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "yside_margin: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    auto element = el::share(el::yside_margin(el::yside_margin_rect(top_bottom[0], top_bottom[1]), el::hold(subject->element)));
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "image: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "gizmo: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "hgizmo: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "vgizmo: ", msg.c_str(), nullptr);
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
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "sprite: ", msg.c_str(), nullptr);
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
    Tcl_CreateObjCommand(interp, "floating", &cmd_floating, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_left", &cmd_align_left, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_center", &cmd_align_center, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_right", &cmd_align_right, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_top", &cmd_align_top, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_middle", &cmd_align_middle, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "align_bottom", &cmd_align_bottom, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "fixed_size", &cmd_fixed_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "hsize", &cmd_hsize, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "vsize", &cmd_vsize, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "hmin_size", &cmd_hmin_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "vmin_size", &cmd_vmin_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "hmax_size", &cmd_hmax_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "margin", &cmd_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "left_margin", &cmd_left_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "top_margin", &cmd_top_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "right_margin", &cmd_right_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "bottom_margin", &cmd_bottom_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "halign", &cmd_halign, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "valign", &cmd_valign, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "min_size", &cmd_min_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "max_size", &cmd_max_size, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "hstretch", &cmd_hstretch, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "vstretch", &cmd_vstretch, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "no_hstretch", &cmd_no_hstretch, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "no_vstretch", &cmd_no_vstretch, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "hcollapsible", &cmd_hcollapsible, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "vcollapsible", &cmd_vcollapsible, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "scale", &cmd_scale, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "limits", &cmd_limits, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "xside_margin", &cmd_xside_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "yside_margin", &cmd_yside_margin, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "image", &cmd_image, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "gizmo", &cmd_gizmo, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "hgizmo", &cmd_hgizmo, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "vgizmo", &cmd_vgizmo, client_data, nullptr);
    Tcl_CreateObjCommand(interp, "sprite", &cmd_sprite, client_data, nullptr);
}
