# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from GraphAnalyzerPython import CoreSystem
from graph_analyzer.forms import AnalyzeForm
from django.core.context_processors import csrf

def show_home(request):
	system = CoreSystem()
	c = RequestContext(request, {"version": system.getVersion()})
	return render_to_response("home.html", c)

def show_analyze(request):
	system = CoreSystem()
	system.setLogLevel(1)
	form = AnalyzeForm()
	c = RequestContext(request, {"version": system.getVersion(), "form": form})
	c.update(csrf(request))
	
	if request.method == "POST":
		form = AnalyzeForm(request.POST)
		if form.is_valid():
			cd = form.cleaned_data
			system.setOutput(cd["output"].encode("utf-8"))
			system.addFile(cd["path"].encode("utf-8"))
			system.run()
			
	c.update({"error": system.getError(), "status": system.getStatus()})
	return render_to_response("analyze.html", c)

def show_browse(request):
	system = CoreSystem()
	c = RequestContext(request, {"version": system.getVersion()})
	return render_to_response("browse.html", c)

def show_options(request):
	system = CoreSystem()
	c = RequestContext(request, {"version": system.getVersion()})
	return render_to_response("options.html", c)