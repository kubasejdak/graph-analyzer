# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from subprocess import Popen

from tools.SystemStatus import SystemStatus
from models import Task

def render_tasksManage(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "tasks": True, "manage_tasks": True})
    c.update(csrf(request))
    
    # ===================================== POST =====================================
    
    if request.method == "POST":
        # start tasks
        if("start" in request.POST):
            Popen(["graph-analyzer", "-t"])
        
        # remove task
        if("accept" in request.POST):
            acceptedTask = Task.objects.get(id = request.POST["accept"])
            acceptedTask.finished = True
            acceptedTask.save()
            c.update({"taskRemoved": True})

    # ===================================== GET =====================================
    
    tasksList = Task.objects.filter(finished = False).order_by("-progress", "name")
    c.update({"tasksList": tasksList})
            

    return render_to_response("manage_tasks.html", c)