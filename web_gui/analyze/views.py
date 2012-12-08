# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from subprocess import Popen

from analyze.forms import AnalyzeForm
from options.models import PendingFile, SystemInfo

def show_analyze(request):
	# get system info (only one object should exists)
	systemInfo_list = SystemInfo.objects.all()
	info = systemInfo_list[0]

	# create form and update html content	
	form = AnalyzeForm()
	c = RequestContext(request, {"version": info.version, "form": form, "analyze": True})
	c.update(csrf(request))
	
	# run system if "Analyze" clicked
	if request.method == "POST":
		# run group re-make
		if "regroup" in request.POST:
			Popen(["graph-analyzer", "--regroup"])
		
		# add filename to database
		if "add" in request.POST:
			form = AnalyzeForm(request.POST)
			if form.is_valid():
				cd = form.cleaned_data
				filename = PendingFile()
				filename.name = cd["file"]
				filename.save()
		
		# run analysis
		if "analyze" in request.POST and PendingFile.objects.count() > 0:
			Popen(["graph-analyzer"])

		# run analysis
		if "clear" in request.POST:
			PendingFile.objects.all().delete()
			systemInfo_list = SystemInfo.objects.all()
			info = systemInfo_list[0]
			info.exploits_num = 0
			info.samples_num = 0;
			info.status = "idle"
			info.error = "no error"
			info.progress = 0
			info.save()
	
	pending_files = PendingFile.objects.all()
	systemInfo_list = SystemInfo.objects.all()
	info = systemInfo_list[0]
	c.update({"error": info.error,
			  "status": info.status,
			  "pending_files": pending_files,
			  "progress": info.progress,
			  "exploits": info.exploits_num,
			  "samples": info.samples_num,
			  "files": info.files_num})
	return render_to_response("analyze.html", c)