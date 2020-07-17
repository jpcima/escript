#pragma once
#include <tcl.h>
#include <memory>
#include <cstdarg>

namespace escript {

struct Tcl_Interp_deleter {
    void operator()(Tcl_Interp *x) const noexcept
    {
        Tcl_DeleteInterp(x);
    }
};

typedef std::unique_ptr<Tcl_Interp, Tcl_Interp_deleter> Tcl_Interp_u;

///
struct ck_deleter {
    void operator()(void *x) const noexcept
    {
        ckfree(x);
    }
};

typedef std::unique_ptr<void, ck_deleter> ck_u;

///
int parse_positional_objv(Tcl_Interp *interp, int objc, Tcl_Obj *const *objv, ...);
int vparse_positional_objv(Tcl_Interp *interp, int objc, Tcl_Obj *const *objv, va_list ap);
int parse_objv(Tcl_Interp *interp, const Tcl_ArgvInfo *info, int objc, Tcl_Obj *const *objv, ...);
int vparse_objv(Tcl_Interp *interp, const Tcl_ArgvInfo *info, int objc, Tcl_Obj *const *objv, va_list ap);

///
#define ESCRIPT_ARGV_OBJ -1

///
void free_internal_rep(Tcl_Obj *obj);

} // namespace escript
