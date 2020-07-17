#include "escript.hpp"
#include "escript_bindings.hpp"

namespace escript {

script_engine::script_engine(Tcl_Interp *interp, bool owns_interp)
    : interp_(interp), owner_(owns_interp ? interp : nullptr)
{
}

script_engine::~script_engine()
{
}

void script_engine::init()
{
    Tcl_Interp *interp = interp_;

    Tcl_SetVar2Ex(interp, "*escript*", nullptr, engine_obj_new(this), TCL_GLOBAL_ONLY);

    Tcl_RegisterObjType(&engine_obj_type);
    Tcl_RegisterObjType(&element_obj_type);

    Tcl_CreateObjCommand(interp, "button", &cmd_button, this, nullptr);
    Tcl_CreateObjCommand(interp, "htile", &cmd_htile, this, nullptr);
    Tcl_CreateObjCommand(interp, "vtile", &cmd_vtile, this, nullptr);
    Tcl_CreateObjCommand(interp, "layer", &cmd_layer, this, nullptr);
    Tcl_CreateObjCommand(interp, "deck", &cmd_deck, this, nullptr);
    Tcl_CreateObjCommand(interp, "hsize", &cmd_hsize, this, nullptr);
    Tcl_CreateObjCommand(interp, "vsize", &cmd_vsize, this, nullptr);
    Tcl_CreateObjCommand(interp, "hmin_size", &cmd_hmin_size, this, nullptr);
    Tcl_CreateObjCommand(interp, "vmin_size", &cmd_vmin_size, this, nullptr);
    Tcl_CreateObjCommand(interp, "hmax_size", &cmd_hmax_size, this, nullptr);
    //Tcl_CreateObjCommand(interp, "vmax_size", &cmd_vmax_size, this, nullptr);
    Tcl_CreateObjCommand(interp, "left_margin", &cmd_left_margin, this, nullptr);
    Tcl_CreateObjCommand(interp, "top_margin", &cmd_top_margin, this, nullptr);
    Tcl_CreateObjCommand(interp, "right_margin", &cmd_right_margin, this, nullptr);
    Tcl_CreateObjCommand(interp, "bottom_margin", &cmd_bottom_margin, this, nullptr);
    Tcl_CreateObjCommand(interp, "min_size", &cmd_min_size, this, nullptr);
    Tcl_CreateObjCommand(interp, "max_size", &cmd_max_size, this, nullptr);
    Tcl_CreateObjCommand(interp, "xside_margin", &cmd_xside_margin, this, nullptr);
    Tcl_CreateObjCommand(interp, "yside_margin", &cmd_yside_margin, this, nullptr);
    Tcl_CreateObjCommand(interp, "image", &cmd_image, this, nullptr);
    Tcl_CreateObjCommand(interp, "gizmo", &cmd_gizmo, this, nullptr);
    Tcl_CreateObjCommand(interp, "hgizmo", &cmd_hgizmo, this, nullptr);
    Tcl_CreateObjCommand(interp, "vgizmo", &cmd_vgizmo, this, nullptr);
    Tcl_CreateObjCommand(interp, "sprite", &cmd_sprite, this, nullptr);
    Tcl_CreateObjCommand(interp, "basic_thumb", &cmd_basic_thumb, this, nullptr);
    Tcl_CreateObjCommand(interp, "basic_track", &cmd_basic_track, this, nullptr);
}

void script_engine::register_element(const char *id, const element_obj &elt)
{
    elements_[id].reset(new element_obj(elt));
}

cycfi::elements::element_ptr script_engine::get_element(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->element;
}

template <> cycfi::elements::receiver<bool> *script_engine::get_receiver(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->receiver_bool;
}

template <> cycfi::elements::receiver<int> *script_engine::get_receiver(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->receiver_int;
}

template <> cycfi::elements::receiver<double> *script_engine::get_receiver(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->receiver_double;
}

template <> cycfi::elements::receiver<std::string> *script_engine::get_receiver(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->receiver_string;
}

template <> cycfi::elements::sender<bool> *script_engine::get_sender(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->sender_bool;
}

template <> cycfi::elements::sender<int> *script_engine::get_sender(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->sender_int;
}

template <> cycfi::elements::sender<double> *script_engine::get_sender(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->sender_double;
}

template <> cycfi::elements::sender<std::string> *script_engine::get_sender(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->sender_string;
}

cycfi::elements::text_reader *script_engine::get_text_reader(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->text_reader;
}

cycfi::elements::text_writer *script_engine::get_text_writer(const char *id)
{
    auto it = elements_.find(id);
    if (it == elements_.end())
        return {};
    return it->second->text_writer;
}

} // namespace escript
