#pragma once
#include <tcl.h>
#include <memory>

namespace escript {

struct Tcl_Interp_deleter {
    void operator()(Tcl_Interp *x) const noexcept
    {
        Tcl_DeleteInterp(x);
    }
};

typedef std::unique_ptr<Tcl_Interp, Tcl_Interp_deleter> Tcl_Interp_u;

///
int parse_positional_objv(Tcl_Interp *interp, Tcl_Obj **objv, ...);

///
#define ESCRIPT_ARGV_OBJ -1

} // namespace escript
