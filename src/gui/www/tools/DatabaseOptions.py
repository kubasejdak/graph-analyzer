from XMLParser import XMLParser

class DatabaseOptions(object):
    # options file
    OPTIONS_FILE = "/var/www/jsejdak/GraphAnalyzer.conf"
    
    driver = "PSQL"
    host = "localhost"
    name = "graph_analyzer_db"
    user = "jsejdak"
    password = "poliritiper"
    
    def get(self):
        xmlParser = XMLParser()
        opened = xmlParser.open(self.OPTIONS_FILE)
        
        if(opened == True):
            rootNode = None
            if(xmlParser.hasRoot("Database") == False):
                rootNode = xmlParser.createRoot("Database")
            else:
                rootNode = xmlParser.root("Database")
            
            self.driver = xmlParser.child(rootNode, "DBDriver").attribute("val")
            self.host = xmlParser.child(rootNode, "DBHost").attribute("val")
            self.name = xmlParser.child(rootNode, "DBName").attribute("val")
            self.user = xmlParser.child(rootNode, "DBUser").attribute("val")
            self.password = xmlParser.child(rootNode, "DBPass").attribute("val")
        
            xmlParser.close()
    
    def save(self):
        xmlParser = XMLParser()
        opened = xmlParser.open(self.OPTIONS_FILE)
        
        if(opened == True):
            rootNode = None
            if(xmlParser.hasRoot("Database") == False):
                rootNode = xmlParser.createRoot("Database")
            else:
                rootNode = xmlParser.root("Database")

            xmlParser.child(rootNode, "DBDriver").setAttribute("val", self.driver)
            xmlParser.child(rootNode, "DBHost").setAttribute("val", self.host)
            xmlParser.child(rootNode, "DBName").setAttribute("val", self.name)
            xmlParser.child(rootNode, "DBUser").setAttribute("val", self.user)
            xmlParser.child(rootNode, "DBPass").setAttribute("val", self.password)
        
            xmlParser.close()