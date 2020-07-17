#include "escript_tcl.hpp"

namespace escript {

int parse_positional_objv(Tcl_Interp *interp, int objc, Tcl_Obj *const *objv, ...)
{
    va_list ap;
    va_start(ap, objv);
    int err = vparse_positional_objv(interp, objc, objv, ap);
    va_end(ap);
    return err;
}

int vparse_positional_objv(Tcl_Interp *interp, int objc, Tcl_Obj *const *objv, va_list ap)
{
    int index = 1;
    if (objc < 1)
        return TCL_ERROR;

    int argtype;
    while ((argtype = va_arg(ap, int))) {
        switch (argtype) {
        case ESCRIPT_ARGV_OBJ:
            if (index == objc)
                return TCL_ERROR;
            *va_arg(ap, Tcl_Obj **) = objv[index];
            ++index;
            break;
        case TCL_ARGV_STRING:
            {
                const char *str = nullptr;
                if (index == objc || !(str = Tcl_GetString(objv[index])))
                    return TCL_ERROR;
                *va_arg(ap, const char **) = str;
                ++index;
            }
            break;
        case TCL_ARGV_INT:
            {
                int val = 0;
                if (index == objc || Tcl_GetIntFromObj(interp, objv[index], &val) != TCL_OK)
                    return TCL_ERROR;
                *va_arg(ap, int *) = val;
                ++index;
            }
            break;
        case TCL_ARGV_FLOAT:
            {
                double val = 0;
                if (index == objc || Tcl_GetDoubleFromObj(interp, objv[index], &val) != TCL_OK)
                    return TCL_ERROR;
                *va_arg(ap, double *) = val;
                ++index;
            }
            break;
        default:
            return TCL_ERROR;
        }
    }

    if (index != objc)
        return TCL_ERROR;

    return TCL_OK;
}

int parse_objv(Tcl_Interp *interp, const Tcl_ArgvInfo *info, int objc, Tcl_Obj *const *objv, ...)
{
    va_list ap;
    va_start(ap, objv);
    int err = vparse_objv(interp, info, objc, objv, ap);
    va_end(ap);
    return err;
}

int vparse_objv(Tcl_Interp *interp, const Tcl_ArgvInfo *info, int objc, Tcl_Obj *const *objv, va_list ap)
{
    Tcl_Obj **rem = nullptr;
    int err = TCL_OK;

    err = Tcl_ParseArgsObjv(interp, info, &objc, objv, &rem);
    if (err != TCL_OK)
        return err;
    ck_u rem_cleanup{rem};

    err = vparse_positional_objv(interp, objc, rem, ap);
    if (err != TCL_OK)
        return err;

    return TCL_OK;
}

///
void free_internal_rep(Tcl_Obj *obj)
{
    const Tcl_ObjType *type = obj->typePtr;
    if (!type)
        return;

    if (type->freeIntRepProc) {
        type->freeIntRepProc(obj);
        obj->typePtr = nullptr;
    }
}

} // namespace escript
