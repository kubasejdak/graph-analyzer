from XMLParser import XMLParser

class AnalyzeTask(object):
	# tasks file
	TASKS_FILE = "/var/www/jsejdak/GraphAnalyzerTasks.xml"

	def __init__(self):
		self.__name = "noname"
		self.__files = []
		self.__output = []
		self.__duplicate = False

	def setName(self, name):
		self.__name = name

	def setFile(self, filename):
		self.__files.append(filename)

	def setOutput(self, output):
		self.__output.append(output)

	def setDuplicate(self, value):
		self.__duplicate = value

	def save(self):
		xmlParser = XMLParser()
		opened = xmlParser.open(self.TASKS_FILE)
		
		if(opened == True):
			rootNode = xmlParser.createRoot("Task")
			rootNode.setAttribute("type", "analyze")
			
			# name
			xmlParser.createChild(rootNode, "Name").setAttribute("val", self.__name)
			
			# files
			for f in self.__files:
				e = xmlParser.createChild(rootNode, "File")
				e.setAttribute("source", "local")
				e.setAttribute("path", f)
			
			# output
			for o in self.__output:
				xmlParser.createChild(rootNode, "Output").setAttribute("val", o)
			
			# override
			if(self.__duplicate == True):
				xmlParser.createChild(rootNode, "Duplicate").setAttribute("val", "true")
			else:
				xmlParser.createChild(rootNode, "Duplicate").setAttribute("val", "false")

			xmlParser.close()