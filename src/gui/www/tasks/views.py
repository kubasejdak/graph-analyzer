# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from subprocess import Popen

from tools.SystemStatus import SystemStatus

def render_tasksManage(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "tasks": True, "manage_tasks": True})
    c.update(csrf(request))

    # ===================================== GET =====================================
    
    # ===================================== POST =====================================
    
    # run system if "Analyze" clicked
    if request.method == "POST":
        # run analysis
        if "analyze" in request.POST:
            Popen(["graph-analyzer", "-t"])

    return render_to_response("manage_tasks.html", c)