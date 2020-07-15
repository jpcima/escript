#include "escript_tcl.hpp"

namespace escript {

int parse_positional_objv(Tcl_Interp *interp, Tcl_Obj **objv, ...)
{
    va_list ap;
    va_start(ap, objv);

    size_t index = 1;
    if (objv[0] == nullptr) {
        va_end(ap);
        return TCL_ERROR;
    }

    int argtype;
    while ((argtype = va_arg(ap, int))) {
        switch (argtype) {
        case TCL_ARGV_STRING:
            {
                const char *str = nullptr;
                if (!objv[index] || !(str = Tcl_GetString(objv[index]))) {
                    va_end(ap);
                    return TCL_ERROR;
                }
                *va_arg(ap, const char **) = str;
                ++index;
            }
            break;
        case TCL_ARGV_INT:
            {
                int val = 0;
                if (!objv[index] || Tcl_GetIntFromObj(interp, objv[index], &val) != TCL_OK) {
                    va_end(ap);
                    return TCL_ERROR;
                }
                *va_arg(ap, int *) = val;
                ++index;
            }
            break;
        case TCL_ARGV_FLOAT:
            {
                double val = 0;
                if (!objv[index] || Tcl_GetDoubleFromObj(interp, objv[index], &val) != TCL_OK) {
                    va_end(ap);
                    return TCL_ERROR;
                }
                *va_arg(ap, double *) = val;
                ++index;
            }
            break;
        default:
            va_end(ap);
            return TCL_ERROR;
        }
    }

    if (objv[index] != nullptr) {
        va_end(ap);
        return TCL_ERROR;
    }

    va_end(ap);
    return TCL_OK;
}

} // namespace escript
