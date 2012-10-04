# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from subprocess import call

from analyze.forms import AnalyzeForm
from options.models import Option, PendingFile
from web_gui.settings import ANALYZE_SCRIPT, UPDATE_SCRIPT

def show_analyze(request):
	# get options (only one object should exists)
	options_list = Option.objects.all()
	if options_list.count() == 0:
		options = Option()
		options.output_dest = "DatabaseOutput"
		options.log_level = 1
		options.log_file = "/home/kuba/nalyzer_log"
		options.save()
	else:
		options = options_list[0]
	
	# get system info
	call([UPDATE_SCRIPT])

	# create form and update html content	
	form = AnalyzeForm()
	c = RequestContext(request, {"version": "unknown", "form": form})
	c.update(csrf(request))
	
	# run system if "Analyze" clicked
	if request.method == "POST":
		
		# add filename to database
		if "add" in request.POST:
			form = AnalyzeForm(request.POST)
			if form.is_valid():
				cd = form.cleaned_data
				filename = PendingFile()
				filename.name = cd["file"]
				filename.save()
		
		# run analysis
		if "analyze" in request.POST:
			call([ANALYZE_SCRIPT])
	
	pending_files = PendingFile.objects.all()
	c.update({"error": "unknown", "status": "unknown", "pending_files": pending_files})
	return render_to_response("analyze.html", c)