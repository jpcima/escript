#pragma once
#include "escript_tcl.hpp"
#include "escript_elements_fwd.hpp"
#include <string>
#include <unordered_map>
#include <memory>

namespace escript {

struct element_obj;

class script_engine {
public:
    script_engine(Tcl_Interp *interp, bool owns_interp);
    ~script_engine();

    script_engine(const script_engine &) = delete;
    script_engine &operator=(const script_engine &) = delete;

    void init();

    void register_element(const char *id, const element_obj &elt);

    cycfi::elements::element_ptr get_element(const char *id);

private:
    Tcl_Interp *interp_ = nullptr;
    Tcl_Interp_u owner_;
    std::unordered_map<std::string, std::unique_ptr<element_obj>> elements_;
};

} // namespace escript
