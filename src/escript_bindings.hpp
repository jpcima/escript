#pragma once
#include "escript_tcl.hpp"
#include "escript_elements_fwd.hpp"
#include <infra/string_view.hpp>
#include <string>
#include <memory>

namespace escript {

class script_engine;

extern Tcl_ObjType engine_obj_type;
extern Tcl_ObjType element_obj_type;

struct element_obj {
    cycfi::elements::element_ptr element;
    cycfi::elements::receiver<bool> *receiver_bool = nullptr;
    cycfi::elements::receiver<double> *receiver_double = nullptr;
    cycfi::elements::receiver<std::string> *receiver_string = nullptr;
    cycfi::elements::sender<bool> *sender_bool = nullptr;
    cycfi::elements::sender<double> *sender_double = nullptr;
    cycfi::elements::sender<std::string> *sender_string = nullptr;
};

Tcl_Obj *engine_obj_new(escript::script_engine *engine);

int cmd_button(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

uint32_t lookup_icon(cycfi::string_view name);
uint32_t lookup_icon_pos(cycfi::string_view name);
bool parse_color(cycfi::string_view name, cycfi::elements::color &color);

} // namespace escript
