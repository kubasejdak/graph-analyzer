from django.shortcuts import render_to_response
from django.template import RequestContext

from tools.SystemStatus import SystemStatus

def render_home(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "home": True})
    return render_to_response("home.html", c)