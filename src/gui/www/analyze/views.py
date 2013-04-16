# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from datetime import date, datetime

from tools.SystemStatus import SystemStatus
from tools.AnalyzeTask import AnalyzeTask

def render_analyzeTask(request):
	# get system status
	systemStatus = SystemStatus()
	systemStatus.get()

	# get date
	today = date.today()
	taskName = "analyze_" + str(today) + "_" + str(datetime.time(datetime.now()))
	
	c = RequestContext(request, {"version": systemStatus.version, "tasks": True, "analyze": True, "taskName": taskName})
	c.update(csrf(request))

	# ===================================== GET ======================================
	
	# ===================================== POST =====================================

	if(request.method == "POST"):
		if "new" in request.POST:
			return render_to_response("analyze.html", c)
		
		# run analysis
		if "save" in request.POST:
			c.update({"is_message": True})
			analyzeTask = AnalyzeTask()

			# analyze files
			files = request.POST.getlist("analyzeFiles")
			analyzeTask.setName(request.POST["taskName"])
			for f in files:
				analyzeTask.setFile(f)

			# output
			if("databaseOutput" in request.POST):
				analyzeTask.setOutput("database")
			if("consoleOutput" in request.POST):
				analyzeTask.setOutput("console")
			
			# override
			if("override" in request.POST):
				analyzeTask.setOverride(True)

			analyzeTask.save()
	
	
	return render_to_response("analyze.html", c)