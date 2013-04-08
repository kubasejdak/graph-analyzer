from PyQt4.QtXml import QDomDocument, QDomElement
from PyQt4.QtCore import QFile, QIODevice

class MyClass(object):

    def __init__(self):
        return
    
    def open(self, filename):
        self.__file = QFile(filename)
        if(self.__file.open(QIODevice.ReadWrite) != True):
            return False
        
        self.__doc = QDomDocument("conf")
        if(self.__doc.setContent(self.__file) != True):
            self.close()
            return False
        
        self.__docRoot = self.__doc.documentElement()
        return True

    def close(self):
        self.save()
        self.__file.close()

    def clear(self):
        self.__docRoot.clear()

    def hasRoot(self, rootName):
        roots = self.__docRoot.childNodes();
        for r in roots:
            if(r.toElement().tagName() == rootName):
                return True

        return False

    def hasChild(self, rootNode, childNode):
        childList = rootNode.childNodes();
        for n in childList:
            if(n.toElement().tagName() == childNode):
                return True

        return False

    def root(self, rootName):
        roots = self.__docRoot.childNodes();
        for r in roots:
            if(r.toElement().tagName() == rootName):
                return r.toElement();

        return QDomElement()
    
    def child(self, rootNode, childNode):
        childList = rootNode.childNodes()
        for c in childList:
            if(c.toElement().tagName() == childNode):
                return c.toElement()

        return QDomElement()

    def removeRoot(self, rootNode):
        self.__docRoot.removeChild(rootNode)

    def removeChild(self, rootNode, childNode):
        rootNode.removeChild(childNode)

    def createRoot(self, rootName):
        newRoot = self.__doc.createElement(rootName);
        self.__docRoot.appendChild(newRoot)
        return newRoot

    def createChild(self, rootNode, childName):
        newChild = self.__doc.createElement(childName)
        rootNode.appendChild(newChild)
        return newChild

    def save(self):
        self.__file.resize(0);
        self.__file.write(self.__doc.toByteArray(4));