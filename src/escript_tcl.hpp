#pragma once
#include "escript_elements_fwd.hpp"
#include <tcl.h>
#include <elements/support/color.hpp>
#include <infra/string_view.hpp>
#include <vector>
#include <array>
#include <string>
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
int parse_objv_ex(Tcl_Interp *interp, const Tcl_ArgvInfo *info, int objc, Tcl_Obj *const *objv);
bool parse_color(cycfi::string_view name, cycfi::elements::color &color);
bool parse_icon(const char *name, uint32_t &icon);
bool parse_icon_pos(cycfi::string_view name, uint32_t &icon_pos);

///
enum : uint32_t {
    ESCRIPT_ICON_POS_LEFT,
    ESCRIPT_ICON_POS_RIGHT,
};

///
enum {
    ESCRIPT_ARGV_FIRST = -1000,

    ESCRIPT_ARGV_OBJ = ESCRIPT_ARGV_FIRST,
    ESCRIPT_ARGV_ELEMENT,
    ESCRIPT_ARGV_COLOR,
    ESCRIPT_ARGV_ICON,
    ESCRIPT_ARGV_ICON_POS,
    ESCRIPT_ARGV_STRING_LIST,
    ESCRIPT_ARGV_POINT,
    ESCRIPT_ARGV_SIDE_MARGIN,

    // n-args, positional only
    ESCRIPT_ARGV_ELEMENT_REST,

    ESCRIPT_ARGV_AFTER_LAST,
    ESCRIPT_ARGV_LAST = ESCRIPT_ARGV_AFTER_LAST - 1,
};

///
int to_string_list(Tcl_Interp *interp, Tcl_Obj *obj, std::vector<std::string> &dst);
int to_point(Tcl_Interp *interp, Tcl_Obj *obj, cycfi::elements::point &dst);
int to_side_margin(Tcl_Interp *interp, Tcl_Obj *obj, std::array<float, 2> &dst);

///
void free_internal_rep(Tcl_Obj *obj);

} // namespace escript
