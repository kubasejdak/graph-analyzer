from XMLParser import XMLParser

class LoggingOptions(object):
    # options file
    OPTIONS_FILE = "/var/www/jsejdak/GraphAnalyzer.conf"
    
    level = 2
    
    def get(self):
        xmlParser = XMLParser()
        opened = xmlParser.open(self.OPTIONS_FILE)
        
        if(opened == True):
            rootNode = None
            if(xmlParser.hasRoot("Logging") == False):
                rootNode = xmlParser.createRoot("Logging")
            else:
                rootNode = xmlParser.root("Logging")
            
            self.level = xmlParser.child(rootNode, "Level").attribute("val")
        
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

            xmlParser.child(rootNode, "Level").setAttribute("val", self.level)
        
            xmlParser.close()