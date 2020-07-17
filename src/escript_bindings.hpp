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
    cycfi::elements::receiver<int> *receiver_int = nullptr;
    cycfi::elements::receiver<double> *receiver_double = nullptr;
    cycfi::elements::receiver<std::string> *receiver_string = nullptr;
    cycfi::elements::sender<bool> *sender_bool = nullptr;
    cycfi::elements::sender<int> *sender_int = nullptr;
    cycfi::elements::sender<double> *sender_double = nullptr;
    cycfi::elements::sender<std::string> *sender_string = nullptr;
    cycfi::elements::text_reader *text_reader = nullptr;
    cycfi::elements::text_writer *text_writer = nullptr;
};

Tcl_Obj *engine_obj_new(escript::script_engine *engine);

void register_element_commands(Tcl_Interp *interp, ClientData client_data);

} // namespace escript
