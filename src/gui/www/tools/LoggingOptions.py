from XMLParser import XMLParser

class LoggingOptions(object):
    # options file
    OPTIONS_FILE = "/var/www/jsejdak/GraphAnalyzer.conf"
    
    level = 2
    exportStatusDB = True
    exportDescriptionXML = True
    
    def get(self):
        xmlParser = XMLParser()
        opened = xmlParser.open(self.OPTIONS_FILE)
        
        if(opened == True):
            rootNode = None
            if(xmlParser.hasRoot("Logging") == False):
                rootNode = xmlParser.createRoot("Logging")
            else:
                rootNode = xmlParser.root("Logging")
            
            # logging level
            self.level = xmlParser.child(rootNode, "Level").attribute("val")
            
            # status strategy
            if(xmlParser.child(rootNode, "StatusStrategy").attribute("type") == "database"):
                self.exportStatusDB = True
            else:
                self.exportStatusDB = False
            
            # description strategy
            if(xmlParser.child(rootNode, "DescriptionStrategy").attribute("type") == "xml"):
                self.exportDescriptionXML = True
            else:
                self.exportDescriptionXML = False
        
            xmlParser.close()
    
    def save(self):
        xmlParser = XMLParser()
        opened = xmlParser.open(self.OPTIONS_FILE)
        
        if(opened == True):
            rootNode = None
            if(xmlParser.hasRoot("Logging") == False):
                rootNode = xmlParser.createRoot("Logging")
            else:
                rootNode = xmlParser.root("Logging")

            # logging level
            xmlParser.child(rootNode, "Level").setAttribute("val", self.level)
            
            # status strategy
            s = xmlParser.child(rootNode, "StatusStrategy")
            xmlParser.removeChild(rootNode, s)
            if(self.exportStatusDB == True):
                xmlParser.createChild(rootNode, "StatusStrategy").setAttribute("type", "database")
        
        
            # description strategy
            d = xmlParser.child(rootNode, "DescriptionStrategy")
            xmlParser.removeChild(rootNode, d)
            if(self.exportDescriptionXML == True):
                xmlParser.createChild(rootNode, "DescriptionStrategy").setAttribute("type", "xml")
            
            xmlParser.close()