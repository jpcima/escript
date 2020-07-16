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

} // namespace escript
