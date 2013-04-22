from XMLParser import XMLParser

class TaskCollector(object):
	# tasks file
	TASKS_FILE = "/var/www/jsejdak/GraphAnalyzerTasks.xml"
	
	def collectTasks(self):
		xmlParser = XMLParser()
		opened = xmlParser.open(self.TASKS_FILE)
		taskNames = []
		
		if(opened == True):
			taskNode = xmlParser.root("Task")
			
			while(taskNode.isNull() == False):
				name = taskNode.firstChildElement("Name").attribute("val")
				taskNames.append(name)
				taskNode = taskNode.nextSiblingElement("Task")

			xmlParser.close()
		
		return taskNames
	
	def removeTask(self, removeId):
		xmlParser = XMLParser()
		opened = xmlParser.open(self.TASKS_FILE)
		
		if(opened == True):
			taskNode = xmlParser.root("Task")
			taskId = 1
			
			while(str(taskId) != removeId):
				taskNode = taskNode.nextSiblingElement("Task")
				taskId += 1

			xmlParser.removeRoot(taskNode)
			xmlParser.close()