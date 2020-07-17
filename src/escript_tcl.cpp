#include "escript_tcl.hpp"
#include "escript_bindings.hpp"
#include <elements/support/icon_ids.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include <limits>
#include <cstdlib>

namespace escript {

struct ArgvFuncContext {
    Tcl_Interp *interp;
    int *err;
};

static Tcl_ArgvFuncProc *argv_func_proc_ex(int type)
{
    switch (type) {
        case ESCRIPT_ARGV_OBJ:
            return [](ClientData cd, Tcl_Obj *obj, void *dst) -> int
            {
                ArgvFuncContext *ctx = (ArgvFuncContext *)cd;
                if (!obj)
                    *ctx->err = TCL_ERROR;
                else
                    *(Tcl_Obj **)dst = obj;
                return 1;
            };
        case ESCRIPT_ARGV_ELEMENT:
            return [](ClientData cd, Tcl_Obj *obj, void *dst) -> int
            {
                ArgvFuncContext *ctx = (ArgvFuncContext *)cd;
                if (!obj || obj->typePtr != &element_obj_type)
                    *ctx->err = TCL_ERROR;
                else
                    *(element_obj **)dst = (element_obj *)obj->internalRep.twoPtrValue.ptr1;
                return 1;
            };
        case ESCRIPT_ARGV_COLOR:
            return [](ClientData cd, Tcl_Obj *obj, void *dst) -> int
            {
                ArgvFuncContext *ctx = (ArgvFuncContext *)cd;
                const char *str = obj ? Tcl_GetString(obj) : nullptr;
                cycfi::elements::color c;
                if (!str || !parse_color(str, c))
                    *ctx->err = TCL_ERROR;
                else
                    *(cycfi::elements::color *)dst = c;
                return 1;
            };
        case ESCRIPT_ARGV_ICON:
            return [](ClientData cd, Tcl_Obj *obj, void *dst) -> int
            {
                ArgvFuncContext *ctx = (ArgvFuncContext *)cd;
                const char *str = obj ? Tcl_GetString(obj) : nullptr;
                uint32_t i;
                if (!str || !parse_icon(str, i))
                    *ctx->err = TCL_ERROR;
                else
                    *(uint32_t *)dst = i;
                return 1;
            };
        case ESCRIPT_ARGV_ICON_POS:
            return [](ClientData cd, Tcl_Obj *obj, void *dst) -> int
            {
                ArgvFuncContext *ctx = (ArgvFuncContext *)cd;
                const char *str = obj ? Tcl_GetString(obj) : nullptr;
                uint32_t i;
                if (!str || !parse_icon_pos(str, i))
                    *ctx->err = TCL_ERROR;
                else
                    *(uint32_t *)dst = i;
                return 1;
            };
        case ESCRIPT_ARGV_STRING_LIST:
            return [](ClientData cd, Tcl_Obj *obj, void *dst) -> int
            {
                ArgvFuncContext *ctx = (ArgvFuncContext *)cd;
                if (!obj || to_string_list(ctx->interp, obj, *(std::vector<std::string> *)dst) != TCL_OK)
                    *ctx->err = TCL_ERROR;
                return 1;
            };
    }

    return nullptr;
}

static Tcl_ArgvGenFuncProc *argv_genfunc_proc_ex(int type)
{
    switch (type) {
        case ESCRIPT_ARGV_ELEMENT_REST:
            return [](ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const *objv, void *dst) -> int
            {
                if (objc < 1)
                    return -1;
                std::vector<element_obj *> &list = *(std::vector<element_obj *> *)dst;
                list.resize(objc);
                for (unsigned i = 0; i < objc; ++i) {
                    if (objv[i]->typePtr != &element_obj_type)
                        return -1;
                    list[i] = (element_obj *)objv[i]->internalRep.twoPtrValue.ptr1;
                }
                return 0;
            };
    }

    return nullptr;
}

int parse_objv_ex(Tcl_Interp *interp, const Tcl_ArgvInfo *info, int objc, Tcl_Obj *const *objv)
{
    size_t info_size = 0;
    while (info[info_size].type != TCL_ARGV_END)
        ++info_size;

    int argv_func_err = TCL_OK;
    ArgvFuncContext argv_func_ctx;
    argv_func_ctx.interp = interp;
    argv_func_ctx.err = &argv_func_err;

    // create temporary keyword info list for Tcl_ParseArgsObjv
    // keyword arguments have non-null keyStr
    std::vector<Tcl_ArgvInfo> keyword_info;
    keyword_info.reserve(info_size + 3);
    for (size_t i = 0; i < info_size; ++i) {
        if (!info[i].keyStr)
            continue;
        keyword_info.push_back(info[i]);
        Tcl_ArgvInfo &current = keyword_info.back();

        // genfunc would be good here, but rem is broken after using it...
        // func cannot return an error code, we'll store error status in the context

        if (info[i].type >= ESCRIPT_ARGV_FIRST && info[i].type <= ESCRIPT_ARGV_LAST) {
            Tcl_ArgvFuncProc *proc = argv_func_proc_ex(current.type);
            if (!proc)
                return TCL_ERROR;
            current.type = TCL_ARGV_FUNC;
            current.srcPtr = (void *)proc;
            current.clientData = (ClientData)&argv_func_ctx;
        }
    }
    keyword_info.push_back(TCL_ARGV_AUTO_REST);
    keyword_info.push_back(TCL_ARGV_AUTO_HELP);
    keyword_info.push_back(TCL_ARGV_TABLE_END);

    // do keyword args
    Tcl_Obj **rem = nullptr;
    if (Tcl_ParseArgsObjv(interp, keyword_info.data(), &objc, objv, &rem) != TCL_OK)
        return TCL_ERROR;
    ck_u rem_cleanup(rem);
    // check if one of the func conversions has failed
    if (argv_func_err != TCL_OK)
        return TCL_ERROR;

    // do the rest as positional args
    objv = rem + 1;
    objc = objc - 1;
    for (size_t i = 0; i < info_size; ++i) {
        const Tcl_ArgvInfo &current = info[i];

        if (info[i].keyStr)
            continue;

        switch (current.type) {
        case TCL_ARGV_STRING:
            {
                const char *str = nullptr;
                if (objc < 1 || !(str = Tcl_GetString(objv[0])))
                    return TCL_ERROR;
                *(const char **)current.dstPtr = str;
                ++objv;
                --objc;
            }
            break;
        case TCL_ARGV_INT:
            {
                int val = 0;
                if (objc < 1 || Tcl_GetIntFromObj(interp, objv[0], &val) != TCL_OK)
                    return TCL_ERROR;
                *(int *)current.dstPtr = val;
                ++objv;
                --objc;
            }
            break;
        case TCL_ARGV_FLOAT:
            {
                double val = 0;
                if (objc < 1 || Tcl_GetDoubleFromObj(interp, objv[0], &val) != TCL_OK)
                    return TCL_ERROR;
                *(double *)current.dstPtr = val;
                ++objv;
                --objc;
            }
            break;
        default:
            {
                ArgvFuncContext argv_func_ctx;
                int argv_func_err = TCL_OK;
                argv_func_ctx.interp = interp;
                argv_func_ctx.err = &argv_func_err;

                if (Tcl_ArgvFuncProc *fproc = argv_func_proc_ex(current.type)) {
                    int n = fproc((ClientData)&argv_func_ctx, (objc > 0) ? objv[0] : nullptr, current.dstPtr);
                    if (argv_func_err != TCL_OK)
                        return TCL_ERROR;
                    if (n) {
                        ++objv;
                        --objc;
                    }
                }
                else if (Tcl_ArgvGenFuncProc *gproc = argv_genfunc_proc_ex(current.type)) {
                    int new_objc = gproc((ClientData)&argv_func_ctx, interp, objc, objv, current.dstPtr);
                    if (new_objc < 0)
                        return TCL_ERROR;
                    int n = objc - new_objc;
                    objv += n;
                    objc -= n;
                }
                else {
                    return TCL_ERROR;
                }
            }
            break;
        }
    }

    if (objc > 0)
        return TCL_ERROR;

    return TCL_OK;
}

static int hex_digit_from_char(char c)
{
    return (c >= '0' && c <= '9') ? (c - '0') :
        (c >= 'a' && c <= 'z') ? (c - 'a' + 10) :
        (c >= 'A' && c <= 'Z') ? (c - 'A' + 10) : -1;
}

bool parse_color(cycfi::string_view name, cycfi::elements::color &color)
{
    if (name.empty() || name[0] != '#')
        return false;

    name = name.substr(1);
    size_t length = name.size();

    uint32_t rgba = 0;
    if (length == 6 || length == 8) {
        for (size_t i = 0; i < length; ++i) {
            int d = hex_digit_from_char(name[i]);
            if (d == -1)
                return false;
            rgba = (rgba << 4) | d;
        }
    }
    if (length == 6)
        rgba = (rgba << 8) | 0xff;

    color.red = (rgba >> 24) / 255.0;
    color.green = ((rgba >> 16) & 0xff) / 255.0;
    color.blue = ((rgba >> 8) & 0xff) / 255.0;
    color.alpha = (rgba & 0xff) / 255.0;

    return true;
}

bool parse_icon(const char *name, uint32_t &icon)
{
    static const std::unordered_map<cycfi::string_view, uint32_t> icons = {
        #define ICON(x) {#x, cycfi::elements::icons::x}
        ICON(left),
        ICON(right),
        ICON(up),
        ICON(down),
        ICON(left_dir),
        ICON(right_dir),
        ICON(up_dir),
        ICON(down_dir),
        ICON(left_circled),
        ICON(right_circled),
        ICON(up_circled),
        ICON(down_circled),
        ICON(left_open),
        ICON(right_open),
        ICON(up_open),
        ICON(down_open),
        ICON(angle_left),
        ICON(angle_right),
        ICON(angle_up),
        ICON(angle_down),
        ICON(angle_double_left),
        ICON(angle_double_right),
        ICON(angle_double_up),
        ICON(angle_double_down),
        ICON(angle_circled_left),
        ICON(angle_circled_right),
        ICON(angle_circled_up),
        ICON(angle_circled_down),
        ICON(exclamation),
        ICON(block),
        ICON(pencil),
        ICON(pin),
        ICON(resize_vertical),
        ICON(resize_horizontal),
        ICON(move),
        ICON(resize_full_alt),
        ICON(resize_full),
        ICON(resize_small),
        ICON(magnifying_glass),
        ICON(zoom_in),
        ICON(zoom_out),
        ICON(volume_off),
        ICON(volume_down),
        ICON(volume_up),
        ICON(cw),
        ICON(ccw),
        ICON(cycle),
        ICON(level_up),
        ICON(level_down),
        ICON(shuffle),
        ICON(exchange),
        ICON(power),
        ICON(play),
        ICON(stop),
        ICON(pause),
        ICON(record),
        ICON(to_end),
        ICON(to_start),
        ICON(fast_forward),
        ICON(fast_backward),
        ICON(wrench),
        ICON(trash),
        ICON(trash_empty),
        ICON(ok),
        ICON(cancel),
        ICON(plus),
        ICON(minus),
        ICON(cog),
        ICON(doc),
        ICON(docs),
        ICON(lock_open),
        ICON(lock),
        ICON(sliders),
        ICON(floppy),
        ICON(attention),
        ICON(info),
        ICON(error),
        ICON(lightbulb),
        ICON(mixer),
        ICON(hand),
        ICON(question),
        ICON(menu),
        #undef ICON
    };

    auto it = icons.find(name);
    if (it != icons.end()) {
        icon = it->second;
        return true;
    }

    errno = 0;
    char *endptr = nullptr;
    unsigned long value = strtoul(name, &endptr, 0);
    unsigned long max_value = std::numeric_limits<uint32_t>::max();
    if (errno == 0 && *endptr == '\0' && value <= max_value) {
        icon = (uint32_t)value;
        return true;
    }

    return false;
}

bool parse_icon_pos(cycfi::string_view name, uint32_t &icon_pos)
{
    if (name == "left") {
        icon_pos = ESCRIPT_ICON_POS_LEFT;
        return true;
    }
    if (name == "right") {
        icon_pos = ESCRIPT_ICON_POS_RIGHT;
        return true;
    }
    return false;
}

///
int to_string_list(Tcl_Interp *interp, Tcl_Obj *obj, std::vector<std::string> &dst)
{
    int err;
    unsigned objc = 0;
    Tcl_Obj **objv = nullptr;

    err = Tcl_ListObjGetElements(interp, obj, (int *)&objc, &objv);
    if (err != TCL_OK)
        return err;

    dst.resize(objc);

    for (unsigned i = 0; i < objc; ++i) {
        const char *str = Tcl_GetString(objv[i]);
        if (!str)
            return TCL_ERROR;
        dst[i].assign(str);
    }

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
