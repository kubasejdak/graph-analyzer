# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from datetime import date, datetime

from tools.SystemStatus import SystemStatus

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
	
	return render_to_response("group.html", c)