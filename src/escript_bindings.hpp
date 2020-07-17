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

int cmd_button(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_htile(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_vtile(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_layer(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_deck(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_align_left(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_align_center(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_align_right(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_align_top(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_align_middle(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_align_bottom(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
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
int cmd_halign(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_valign(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_min_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_max_size(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_xside_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_yside_margin(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_image(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_gizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_hgizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_vgizmo(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_sprite(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_basic_thumb(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_basic_track(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_slider(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_slider_marks(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
int cmd_slider_labels(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

void register_element(Tcl_Interp *interp, const char *id, const element_obj &elt);

} // namespace escript
