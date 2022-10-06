from typing import List,Optional
from pyArborX_Util import cppClassTemplateInfo

class BVH:

    def __init__(self,classTemplateArgs: List[str],ctorTemplateArgs: Optional[List[str]] ):
        includes = "<ArborX_LinearBVH.hpp>"
        self.cppClassInfo=cppClassTemplateInfo("Arborx::BVH",classTemplateArgs)
        self.bvh_impl = ""

    def __init__(self,classTemplateArgs: List[str],ctorTemplateArgs: List[str], *args):
        self.bvh_impl = 








def BVHCtorTemplate(classInfo: cppClassTemplateInfo, ctorTemplateArgs: List[str], *args) -> BVH:

    
    moduleName = "BVH"+"_classTemplates_"+"_".join(classInfo.classTemplateArgs)+"_ctorTemplates_"+"_".join(ctorTemplateArgs)
    wrapperName = moduleName+"_wrapper"

    includesSource = "#include <pybind11/pybind11.h> \n#include <ArborX_LinearBVH.hpp>\n"
    
    wrapperSource = f"{classInfo.typename} {wrapperName} (py::args args)"
    wrapperSource += "{"
    wrapperSource += f"{classInfo.typename}* bvh = new {classInfo.typename}"
    wrapperSource += "("

    argList = []
    for type,arg in zip(ctorTemplateArgs,args):
        argList.append("arg.cast<{type}>()")
    argString = ",".join(argList)

    wrapperSource += f"{argString}"
    wrapperSource += ");\n"
    wrapperSource += "return *bvh;\n"
    wrapperSource += "}\n"

    bindingSource += f"PYBIND11_MODULE({moduleName},m)\n"
    bindingSource += "{\n"
    bindingSource += f"m.def({wrapperName},&{wrapperName});"
    bindingSource += "}"

    ## write to file

    with open(f"{moduleName}.cpp", "w") as sourceFile:
        sourceFile.write(includesSource)
        sourceFile.write(wrapperSource)
        sourceFile.write(bindingSource)

        sourceFile.close()
