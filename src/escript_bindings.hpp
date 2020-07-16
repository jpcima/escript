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
int cmd_htile(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_vtile(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_layer(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_deck(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_hsize(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_vsize(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_hmin_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_vmin_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_hmax_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
//int cmd_vmax_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_left_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_top_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_right_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_bottom_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_min_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_max_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_xside_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_yside_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_image(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_gizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_hgizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_vgizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

void register_element(Tcl_Interp *interp, const char *id, const element_obj &elt);
uint32_t lookup_icon(cycfi::string_view name);
uint32_t lookup_icon_pos(cycfi::string_view name);
bool parse_color(cycfi::string_view name, cycfi::elements::color &color);

} // namespace escript
