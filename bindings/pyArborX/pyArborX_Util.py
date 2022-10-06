from typing import List


class cppClassTemplateInfo:
    def __init__(className: str,classTemplateArgs: List[str]):
        self.className=className
        self.classTemplateArgs=classTemplateArgs
        self.typename=className+"<"+",".join(classTemplateArgs)+">"
