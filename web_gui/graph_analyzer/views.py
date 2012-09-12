# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from GraphAnalyzerPython import CoreSystem

def show_home(request):
	system = CoreSystem()
	c = RequestContext(request, {"version": system.getVersion()})
	return render_to_response("home.html", c)

def show_analyze(request):
	system = CoreSystem()
	c = RequestContext(request, {"version": system.getVersion()})
	return render_to_response("analyze.html", c)

def show_browse(request):
	system = CoreSystem()
	c = RequestContext(request, {"version": system.getVersion()})
	return render_to_response("browse.html", c)

def show_options(request):
	system = CoreSystem()
	c = RequestContext(request, {"version": system.getVersion()})
	return render_to_response("options.html", c)