# Create your views here.

from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from subprocess import Popen
from time import sleep
from datetime import date, datetime

from tools.SystemStatus import SystemStatus
from tools.TaskCollector import TaskCollector
from tools.AnalyzeTask import AnalyzeTask
from tools.GroupTask import GroupTask
from tools.ExportTask import ExportTask
from models import Task, TaskTrait

def render_tasksManage(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "create_tasks": True, "manage_tasks": True})
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
    
    workingTasksList = Task.objects.filter(finished = False).order_by("id")
    scheduledTasksList = taskCollector.collectTasks()
    
    c.update({"workingTasksList": workingTasksList, "scheduledTasksList": scheduledTasksList})
    return render_to_response("manage_tasks.html", c)

def render_createAnalyzeTask(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()

    # get date
    today = date.today()
    taskName = "analyze_" + str(today) + "_" + str(datetime.time(datetime.now()))
    
    c = RequestContext(request, {"version": systemStatus.version, "tasks": True, "analyze": True, "taskName": taskName})
    c.update(csrf(request))

    # ===================================== GET ======================================
    
    # ===================================== POST =====================================

    if(request.method == "POST"):
        if "new" in request.POST:
            return render_to_response("create_task_analyze.html", c)
        
        # save analyze task
        if "save" in request.POST:
            c.update({"is_message": True})
            analyzeTask = AnalyzeTask()

            # name
            analyzeTask.setName(request.POST["taskName"])

            # analyze files
            files = request.POST.getlist("analyzeFiles")
            for f in files:
                analyzeTask.setFile(f)

            # output
            if("databaseOutput" in request.POST):
                analyzeTask.setOutput("database")
            if("consoleOutput" in request.POST):
                analyzeTask.setOutput("console")
            
            # override
            if("duplicate" in request.POST):
                analyzeTask.setDuplicate(True)

            analyzeTask.save()
    
    return render_to_response("create_task_analyze.html", c)

def render_listAnalyzeTasks(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "list_tasks_analyze": True})
    c.update(csrf(request))

    # ===================================== GET =====================================

    if request.method == "GET":
        taskList = Task.objects.all().filter(type = "analyze").order_by("start_time")
        c.update({"taskList": taskList})
    
    return render_to_response("list_tasks_analyze.html", c)

def render_createGroupTask(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()

    # get date
    today = date.today()
    taskName = "group_" + str(today) + "_" + str(datetime.time(datetime.now()))

    c = RequestContext(request, {"version": systemStatus.version, "tasks": True, "group": True, "taskName": taskName})
    c.update(csrf(request))

    # ===================================== GET ======================================
    
    # ===================================== POST =====================================
    
    # save group task
    if "save" in request.POST:
        c.update({"is_message": True})
        groupTask = GroupTask()

        # name
        groupTask.setName(request.POST["taskName"])

        # group files
        files = request.POST.getlist("groupFiles")
        for f in files:
            groupTask.setFile(f)

        # from
        groupTask.setFrom(request.POST["fromDate"])

        # until
        groupTask.setUntil(request.POST["untilDate"])
        
        # algorithm
        groupTask.setAlgorithm(request.POST["algorithm"])

        # algorithm context
        if(request.POST["algorithm"] == "SymetricSimilarity"):
            groupTask.setContext("threshold", request.POST["threshold_SymetricSimilarity"])
            groupTask.setContext("trait", request.POST["trait_SymetricSimilarity"])
            traitKey = ""
            if(request.POST["trait_SymetricSimilarity"] == "loop"):
                traitKey = "hash"
            elif(request.POST["trait_SymetricSimilarity"] == "api"):
                traitKey = "syscall"
                
            groupTask.setContext("traitKey", traitKey)

        # input
        if("databaseInput" in request.POST):
            groupTask.setInput("database")

        # output
        if("databaseOutput" in request.POST):
            groupTask.setOutput("database")
        if("consoleOutput" in request.POST):
            groupTask.setOutput("console")
            
        # override
        if("override" in request.POST):
            groupTask.setOverride(True)

        groupTask.save()
    
    return render_to_response("create_task_group.html", c)

def render_listGroupTasks(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "list_tasks_group": True})
    c.update(csrf(request))

    # ===================================== GET =====================================

    if request.method == "GET":
        tasks = Task.objects.all().filter(type = "group").order_by("start_time")
        
        taskList = []
        for t in tasks:
            taskList.append((t, TaskTrait.objects.get(task = t.id, name = "algorithm")))
            
        c.update({"taskList": taskList})
    
    return render_to_response("list_tasks_group.html", c)

def render_createExportTask(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()

    # get date
    today = date.today()
    taskName = "export_" + str(today) + "_" + str(datetime.time(datetime.now()))

    c = RequestContext(request, {"version": systemStatus.version, "tasks": True, "export": True, "taskName": taskName})
    c.update(csrf(request))

    # ===================================== GET ======================================
    
    # ===================================== POST =====================================
    
    # save export task
    if "save" in request.POST:
        c.update({"is_message": True})
        exportTask = ExportTask()
        
        # name
        exportTask.setName(request.POST["taskName"])

        # export files
        files = request.POST.getlist("exportFiles")
        for f in files:
            exportTask.setFile(f)

        # from
        exportTask.setFrom(request.POST["fromDate"])

        # until
        exportTask.setUntil(request.POST["untilDate"])
        
        # input
        if("databaseInput" in request.POST):
            exportTask.setInput("database")

        # output
        if("fileOutput" in request.POST):
            exportTask.setOutput("file")
        
        # export directory
        exportTask.setExportDir(request.POST["exportDirectory"])
        
        # only index file
        if("onlyIndexFile" in request.POST):
            exportTask.setOnlyIndexFile(True)

        exportTask.save()
    
    return render_to_response("create_task_export.html", c)