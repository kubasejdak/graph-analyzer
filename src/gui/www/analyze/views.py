# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from subprocess import Popen

from options.models import PendingFile
from tools.SystemStatus import SystemStatus

def render_analyzeTask(request):
	# get system status
	systemStatus = SystemStatus()
	systemStatus.get()
	
	c = RequestContext(request, {"version": systemStatus.version, "tasks": True, "analyze": True})
	c.update(csrf(request))

	# ===================================== GET ======================================
	
	# ===================================== POST =====================================
	
	# run system if "Analyze" clicked
	if request.method == "POST":
		# run group re-make
		if "regroup" in request.POST:
			Popen(["graph-analyzer", "--regroup"])
		
		# add filename to database
		if "add" in request.POST:
			filename = PendingFile()
			filename.name = request.POST["file"]
			filename.save()
		
		# run analysis
		if "analyze" in request.POST and PendingFile.objects.count() > 0:
			Popen(["graph-analyzer", "-t"])

		# run analysis
		if "clear" in request.POST:
			print "clear clicked"
	
	pending_files = PendingFile.objects.all()
	c.update({"error": systemStatus.last_error,
			  "status": systemStatus.current_task,
			  "pending_files": pending_files,
			  "progress": systemStatus.progress,
			  "exploits": 0,
			  "samples": 0,
			  "files": 0,
			  "errors": systemStatus.errors_num})

	return render_to_response("analyze.html", c)