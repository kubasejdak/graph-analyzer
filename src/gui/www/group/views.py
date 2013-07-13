# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from datetime import date, datetime

from tools.SystemStatus import SystemStatus
from tools.GroupTask import GroupTask

def render_groupTask(request):
	# get system status
	systemStatus = SystemStatus()
	systemStatus.get()

	# get date
	today = date.today()
	taskName = "group_" + str(today) + "_" + str(datetime.time(datetime.now()))

	c = RequestContext(request, {"version": systemStatus.version, "tasks": True, "group": True, "taskName": taskName})
	c.update(csrf(request))

	# ===================================== GET ======================================
	
	# ===================================== POST =====================================
	
	# save group task
	if "save" in request.POST:
		c.update({"is_message": True})
		groupTask = GroupTask()
		
		# name
		groupTask.setName(request.POST["taskName"])

		# group files
		files = request.POST.getlist("groupFiles")
		for f in files:
			groupTask.setFile(f)

		# from
		groupTask.setFrom(request.POST["fromDate"])

		# until
		groupTask.setUntil(request.POST["untilDate"])
		
		# algorithm
		groupTask.setAlgorithm(request.POST["algorithm"])

		# algorithm context
		if(request.POST["algorithm"] == "SymetricProbability"):
			groupTask.setContext("threshold", request.POST["threshold_SymetricProbability"])

		# input
		if("databaseInput" in request.POST):
			groupTask.setInput("database")

		# output
		if("databaseOutput" in request.POST):
			groupTask.setOutput("database")
		if("consoleOutput" in request.POST):
			groupTask.setOutput("console")
			
		# override
		if("override" in request.POST):
			groupTask.setOverride(True)

		groupTask.save()
	
	return render_to_response("group.html", c)