from XMLParser import XMLParser

class GroupTask(object):
	# tasks file
	TASKS_FILE = "/var/www/jsejdak/GraphAnalyzerTasks.xml"
	
	def __init__(self):
		self.__name = "noname"
		self.__files = []
		self.__from = "dd.mm.rrrr"
		self.__until = "dd.mm.rrrr"
		self.__algorithm = "SymetricSimilarity"
		self.__context = {}
		self.__input = ""
		self.__output = []
		self.__override = False

	def setName(self, name):
		self.__name = name

	def setFile(self, filename):
		self.__files.append(filename)

	def setFrom(self, fromDate):
		self.__from = fromDate

	def setUntil(self, untilDate):
		self.__until = untilDate

	def setAlgorithm(self, algorithm):
		self.__algorithm = algorithm
	
	def setContext(self, key, value):
		self.__context[key] = value

	def setInput(self, inputMod):
		self.__input = inputMod

	def setOutput(self, outputMod):
		self.__output.append(outputMod)

	def setOverride(self, value):
		self.__override = value

	def save(self):
		xmlParser = XMLParser()
		opened = xmlParser.open(self.TASKS_FILE)
		
		if(opened == True):
			rootNode = xmlParser.createRoot("Task")
			rootNode.setAttribute("type", "group")
			
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

			# algorithm
			xmlParser.createChild(rootNode, "Algorithm").setAttribute("name", self.__algorithm)
			
			# context
			for c in self.__context.keys():
				n = xmlParser.createChild(rootNode, "Context")
				n.setAttribute("name", c)
				n.setAttribute("val", self.__context[c])
			
			# input
			xmlParser.createChild(rootNode, "Input").setAttribute("val", self.__input)
			
			# output
			for o in self.__output:
				xmlParser.createChild(rootNode, "Output").setAttribute("val", o)
			
			# override
			if(self.__override == True):
				xmlParser.createChild(rootNode, "Override").setAttribute("val", "true")
			else:
				xmlParser.createChild(rootNode, "Override").setAttribute("val", "false")

			xmlParser.close()