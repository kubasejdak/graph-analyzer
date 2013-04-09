from XMLParser import XMLParser

class SystemStatus(object):
    # status file
    STATUS_FILE = "/var/www/jsejdak/GraphAnalyzerStatus.xml"
    
    version = "n/a"
    progress = 0
    current_task = "idle"
    last_error = "no error"
    errors_num = 0

    
    def get(self):
        xmlParser = XMLParser()
        opened = xmlParser.open(self.STATUS_FILE)
        
        if(opened == True):
            rootNode = None
            if(xmlParser.hasRoot("SystemStatus") == False):
                rootNode = xmlParser.createRoot("SystemStatus")
            else:
                rootNode = xmlParser.root("SystemStatus")
            
            self.version = xmlParser.child(rootNode, "Version").attribute("val")
            self.progress = xmlParser.child(rootNode, "Progress").attribute("val")
            self.current_task = xmlParser.child(rootNode, "CurrentTask").attribute("name")
            self.last_error = xmlParser.child(rootNode, "LastError").attribute("desc")
            self.errors_num = xmlParser.child(rootNode, "ErrorsNum").attribute("val")
        
            xmlParser.close()