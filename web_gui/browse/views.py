from django.shortcuts import render_to_response
from django.template import RequestContext

def show_browse(request):
    c = RequestContext(request, {"version": "unknown"})
    return render_to_response("browse.html", c)