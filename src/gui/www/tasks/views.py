# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from subprocess import Popen
from time import sleep

from tools.SystemStatus import SystemStatus
from tools.TaskCollector import TaskCollector
from models import Task

def render_tasksManage(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "tasks": True, "manage_tasks": True})
    c.update(csrf(request))
    
    taskCollector = TaskCollector()
    
    # ===================================== POST =====================================
    
    if request.method == "POST":
        # start tasks
        if("start" in request.POST):
            Popen(["graph-analyzer", "-t"])
            sleep(1)
        
        # mark task as finished
        if("accept" in request.POST):
            acceptedTask = Task.objects.get(id = request.POST["accept"])
            acceptedTask.finished = True
            acceptedTask.save()
            c.update({"taskAccepted": True})
         
        # remove task
        if("remove" in request.POST):
            taskCollector.removeTask(request.POST["remove"])
            c.update({"taskRemoved": True})

    # ===================================== GET =====================================
    
    workingTasksList = Task.objects.filter(finished = False).order_by("-progress", "name")
    allTasksList = Task.objects.order_by("-progress", "name")
    scheduledTasksList = taskCollector.collectTasks()
    
    c.update({"workingTasksList": workingTasksList, "scheduledTasksList": scheduledTasksList, "allTasksList": allTasksList})
    return render_to_response("manage_tasks.html", c)