#!/usr/bin/env python

import yaml
import argparse
import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
SOURCE_FILE = os.path.join(SCRIPT_DIR, '..', 'src', 'data', 'elements.yml')

ctype_of_type = {
    'string': 'const char *',
    'float': 'double',
    'int': 'int',
    'icon': 'uint32_t',
    'icon_pos': 'uint32_t',
    'color': 'el::color',
    'constant_true': 'int',
    'element': 'element_obj *',
    'string_list': 'std::vector<std::string>',
    'point': 'el::point',
    'rect': 'el::rect',
    'side_margin': 'std::array<float, 2>',
    'element_rest': 'std::vector<element_obj *>',
}

argvtype_of_type = {
    'string': 'TCL_ARGV_STRING',
    'float': 'TCL_ARGV_FLOAT',
    'int': 'TCL_ARGV_INT',
    'icon': 'ESCRIPT_ARGV_ICON',
    'icon_pos': 'ESCRIPT_ARGV_ICON_POS',
    'color': 'ESCRIPT_ARGV_COLOR',
    'constant_true': 'TCL_ARGV_CONSTANT',
    'element': 'ESCRIPT_ARGV_ELEMENT',
    'string_list': 'ESCRIPT_ARGV_STRING_LIST',
    'point': 'ESCRIPT_ARGV_POINT',
    'rect': 'ESCRIPT_ARGV_RECT',
    'side_margin': 'ESCRIPT_ARGV_SIDE_MARGIN',
    'element_rest': 'ESCRIPT_ARGV_ELEMENT_REST',
}

srcptr_of_type = {
    'constant_true': '((void *)&detail::k_true)',
}

def generate_start(element, stream=sys.stdout):
    stream.write('''namespace detail {
    static const int k_true = 1;
}
''')

def generate_command(element, stream=sys.stdout):
    stream.write('''static int cmd_%s(ClientData client_data, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    const char *id {};
''' % (element['name']))

    for arg in element['kwargs'] + element['args']:
        stream.write('    %s %s %s;\n' % \
                     (ctype_of_type[arg['type']],
                      arg['name'],
                      arg.get('init', '{}')))

    stream.write('''    const Tcl_ArgvInfo info[] = {
        {TCL_ARGV_STRING, "-id", nullptr, &id, "Identifier", nullptr},
''')

    for arg in element['kwargs']:
        stream.write('        {%s, "-%s", %s, &%s, "%s", nullptr},\n' % \
                     (argvtype_of_type[arg['type']],
                      arg['name'],
                      srcptr_of_type.get(arg['type'], 'nullptr'),
                      arg['name'],
                      arg.get('description', '')))
    for arg in element['args']:
        stream.write('        {%s, nullptr, %s, &%s, "%s", nullptr},\n' % \
                     (argvtype_of_type[arg['type']],
                      srcptr_of_type.get(arg['type'], 'nullptr'),
                      arg['name'],
                      arg.get('description', '')))

    stream.write('''        TCL_ARGV_TABLE_END
    };
    if (parse_objv_ex(interp, info, objc, objv) != TCL_OK) {
        const char *msgp = Tcl_GetStringResult(interp);
        const std::string msg = msgp ? msgp : "unknown error";
        Tcl_ResetResult(interp);
        Tcl_AppendResult(interp, "%s: ", msg.c_str(), nullptr);
        return TCL_ERROR;
    }
    ''' % (element['name']))

    stream.write(element['make'])

    stream.write('''    Tcl_Obj *result = create_element_result(interp, id, *element);
    Tcl_SetObjResult(interp, result);
    return TCL_OK;
}
''')

def generate_command_registration(doc, stream=sys.stdout):
    stream.write('''void register_element_commands(Tcl_Interp *interp, ClientData client_data)
{
''')
    for element in doc['elements']:
        stream.write('    Tcl_CreateObjCommand(interp, "%s", &cmd_%s, client_data, nullptr);\n' % \
                     (element['name'],
                      element['name']))
    stream.write('''}
''')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Generate binding source code.')
    parser.add_argument('-o', metavar='output-file', help='output file')
    args = parser.parse_args(sys.argv[1:])

    stream = sys.stdout
    if args.o is not None:
        stream = open(args.o, 'w')

    doc = yaml.safe_load(open(SOURCE_FILE))
    generate_start(doc, stream)
    for element in doc['elements']:
        generate_command(element, stream)
    generate_command_registration(doc, stream)
