#include <elements.hpp>
#include <elements/support/resource_paths.hpp>
#include <escript.hpp>
#include <iostream>
namespace el = cycfi::elements;

int main(int argc, char *argv[])
{
    el::app app(argc, argv, "Script", "org.sdf1.jpcima.escript.script");
    el::window win(app.name());
    win.on_close = [&app]() { app.stop(); };

    el::view view(win);

    Tcl_Interp *interp = Tcl_CreateInterp();
    if (!interp)
        throw std::bad_alloc();

    escript::script_engine engine(interp, true);
    engine.init();

    ///
    const std::string script_path = el::find_file("script.tcl");
    if (script_path.empty()) {
        std::cerr << "Cannot find the script file.\n";
        return 1;
    }

    ///
    int err = Tcl_EvalFile(interp, script_path.c_str());
    Tcl_Obj *result = Tcl_GetObjResult(interp);
    std::cout << "Result(" << err << "): " << Tcl_GetString(result) << "\n";

    ///
    el::element_ptr elt = engine.get_element("main");
    if (!elt) {
        std::cerr << "Cannot find the main element.\n";
        return 1;
    }

    view.content(elt);

    app.run();

    return 0;
}
