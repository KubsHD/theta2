import clang.cindex

clang.cindex.Config.set_library_file("./libclang.dll")

import clang.cindex

def generate_save_load_functions(header_file, class_name):
    index = clang.cindex.Index.create()
    tu = index.parse(header_file, args=['-x', 'c++'])

    class_variables = []

    for cursor in tu.cursor.get_children():
        if cursor.kind == clang.cindex.CursorKind.CLASS_DECL and cursor.spelling == class_name:
            for field in cursor.get_children():
                if field.kind == clang.cindex.CursorKind.FIELD_DECL:
                    variable_name = field.spelling
                    variable_type = field.type.spelling
                    class_variables.append((variable_name, variable_type))

    # Generate C++ save and load functions
    save_function = f"void save{class_name}(const {class_name}& obj, const std::string& filename) {{\n"
    load_function = f"{class_name} load{class_name}(const std::string& filename) {{\n"
    
    save_function += "    std::ofstream outfile(filename);\n"
    load_function += f"    {class_name} obj;\n"
    load_function += "    std::ifstream infile(filename);\n"

    for variable_name, variable_type in class_variables:
        save_function += f"    outfile << obj.{variable_name} << '\\n';\n"
        load_function += f"    infile >> obj.{variable_name};\n"

    save_function += "}\n"
    load_function += "    return obj;\n}\n"

    return save_function, load_function

if __name__ == "__main__":
    header_file = "C:/dev/git/theta2/src/components/animator.h"
    class_name = "Animator"
    save_function, load_function = generate_save_load_functions(header_file, class_name)

    print(save_function)
    print(load_function)
