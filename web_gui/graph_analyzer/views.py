# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext

def show_home(request):
	c = RequestContext(request, {"version": "0.1"})
	return render_to_response("home.html", c)

def show_analyze(request):
	c = RequestContext(request, {"version": "0.1"})
	return render_to_response("analyze.html", c)

def show_browse(request):
	c = RequestContext(request, {"version": "0.1"})
	return render_to_response("browse.html", c)

def show_options(request):
	c = RequestContext(request, {"version": "0.1"})
	return render_to_response("options.html", c)