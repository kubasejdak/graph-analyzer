from XMLParser import XMLParser

class ExportTask(object):
    # tasks file
    TASKS_FILE = "/var/www/jsejdak/GraphAnalyzerTasks.xml"
    
    def __init__(self):
        self.__name = "noname"
        self.__files = []
        self.__from = "dd.mm.rrrr"
        self.__until = "dd.mm.rrrr"
        self.__input = ""
        self.__output = []
        self.__onlyIndexFile = False
        self.__export_dir = "/var/www/jsejdak/export"

    def setName(self, name):
        self.__name = name

    def setFile(self, filename):
        self.__files.append(filename)

    def setFrom(self, fromDate):
        self.__from = fromDate

    def setUntil(self, untilDate):
        self.__until = untilDate

    def setInput(self, inputMod):
        self.__input = inputMod

    def setOutput(self, outputMod):
        self.__output.append(outputMod)
    
    def setOnlyIndexFile(self, onlyIndexFile):
        self.__onlyIndexFile = onlyIndexFile
        
    def setExportDir(self, exportDir):
        self.__export_dir = exportDir

    def save(self):
        xmlParser = XMLParser()
        opened = xmlParser.open(self.TASKS_FILE)
        
        if(opened == True):
            rootNode = xmlParser.createRoot("Task")
            rootNode.setAttribute("type", "export")
            
            # name
            xmlParser.createChild(rootNode, "Name").setAttribute("val", self.__name)
            
            # files
            for f in self.__files:
                e = xmlParser.createChild(rootNode, "File")
                e.setAttribute("source", "local")
                e.setAttribute("path", f)
            
            # from
            xmlParser.createChild(rootNode, "From").setAttribute("date", self.__from)

            # until
            xmlParser.createChild(rootNode, "Until").setAttribute("date", self.__until)

            # input
            xmlParser.createChild(rootNode, "Input").setAttribute("val", self.__input)
            
            # output
            for o in self.__output:
                xmlParser.createChild(rootNode, "Output").setAttribute("val", o)
            
            # only index file
            if(self.__onlyIndexFile == True):
                xmlParser.createChild(rootNode, "OnlyIndexFile").setAttribute("val", "true")
            else:
                xmlParser.createChild(rootNode, "OnlyIndexFile").setAttribute("val", "false")

            # export directory
            xmlParser.createChild(rootNode, "ExportDir").setAttribute("path", self.__export_dir)
            
            xmlParser.close()