from XMLParser import XMLParser

class CoreOptions(object):
    # options file
    OPTIONS_FILE = "/var/www/jsejdak/GraphAnalyzer.conf"
    
    graphs_dir = "/var/www/jsejdak/graphs/"
    tmp_pcap_dir = "/tmp/tmp_pcap/"
    tcpflow_params = ""
    emulation_steps = 1000000
    skip_big_files = True
    skip_big_files_size = 20971520
    skip_broken_samples = True
    skip_broken_samples_size = 50
    skip_empty_samples = True
    
    def get(self):
        xmlParser = XMLParser()
        opened = xmlParser.open(self.OPTIONS_FILE)
        
        if(opened == True):
            rootNode = None
            if(xmlParser.hasRoot("CoreOptions") == False):
                rootNode = xmlParser.createRoot("CoreOptions")
            else:
                rootNode = xmlParser.root("CoreOptions")
            
            self.graphs_dir = xmlParser.child(rootNode, "GraphsDir").attribute("path")
            self.tmp_pcap_dir = xmlParser.child(rootNode, "TmpPcapDir").attribute("path")
            self.tcpflow_params = xmlParser.child(rootNode, "TcpflowParams").attribute("val")
            self.emulation_steps = xmlParser.child(rootNode, "EmulationSteps").attribute("val")
            if(xmlParser.child(rootNode, "SkipBigFiles").attribute("val") == "false"):
                self.skip_big_files = False
            else:
                self.skip_big_files = True
            self.skip_big_files_size = xmlParser.child(rootNode, "SkipBigFiles").attribute("size")
            if(xmlParser.child(rootNode, "SkipBrokenSamples").attribute("val") == "false"):
                self.skip_broken_samples = False
            else:
                self.skip_broken_samples = True
            self.skip_broken_samples_size = xmlParser.child(rootNode, "SkipBrokenSamples").attribute("size")
            if(xmlParser.child(rootNode, "SkipEmptySamples").attribute("val") == "false"):
                self.skip_empty_samples = False
            else:
                self.skip_empty_samples = True
        
            xmlParser.close()
    
    def save(self):
        xmlParser = XMLParser()
        opened = xmlParser.open(self.OPTIONS_FILE)
        
        if(opened == True):
            rootNode = None
            if(xmlParser.hasRoot("CoreOptions") == False):
                rootNode = xmlParser.createRoot("CoreOptions")
            else:
                rootNode = xmlParser.root("CoreOptions")

            xmlParser.child(rootNode, "GraphsDir").setAttribute("path", self.graphs_dir)
            xmlParser.child(rootNode, "TmpPcapDir").setAttribute("path", self.tmp_pcap_dir)
            xmlParser.child(rootNode, "TcpflowParams").setAttribute("val", self.tcpflow_params)
            xmlParser.child(rootNode, "EmulationSteps").setAttribute("val", self.emulation_steps)
            if(self.skip_big_files == True):
                xmlParser.child(rootNode, "SkipBigFiles").setAttribute("val", "true")
            else:
                xmlParser.child(rootNode, "SkipBigFiles").setAttribute("val", "false")
            xmlParser.child(rootNode, "SkipBigFiles").setAttribute("size", self.skip_big_files_size)
            if(self.skip_broken_samples == True):
                xmlParser.child(rootNode, "SkipBrokenSamples").setAttribute("val", "true")
            else:
                xmlParser.child(rootNode, "SkipBrokenSamples").setAttribute("val", "false")
            xmlParser.child(rootNode, "SkipBrokenSamples").setAttribute("size", self.skip_broken_samples_size)
            if(self.skip_empty_samples == True):
                xmlParser.child(rootNode, "SkipEmptySamples").setAttribute("val", "true")
            else:
                xmlParser.child(rootNode, "SkipEmptySamples").setAttribute("val", "false")
        
            xmlParser.close()