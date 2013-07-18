# Create your views here.

from django.shortcuts import render_to_response
from django.views.generic.simple import redirect_to
from django.template import RequestContext
from django.core.context_processors import csrf

from tools.SystemStatus import SystemStatus
from tasks.models import Task, TaskTrait
from group.models import Group, GroupAssignment

def render_showGroup(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()

    c = RequestContext(request, {"version": systemStatus.version, "browse_groups": True})
    c.update(csrf(request))

    # ===================================== GET =====================================

    if request.method == "GET":
        if "dataSaved" in request.GET:
            c.update({"dataSaved": True})

        if "groupId" in request.GET:
            show_group = Group.objects.get(id = request.GET["groupId"])
            show_groupassignment = GroupAssignment.objects.filter(group_id = request.GET["groupId"]).order_by("-resemblence")
            c.update({"show_group": show_group,
                      "show_groupassignment": show_groupassignment})
            return render_to_response("show_group.html", c)

    # ===================================== POST =====================================

    if request.method == "POST":
        if "saveGroup" in request.POST:
            group = Group.objects.get(id = request.POST["saveGroup"])
            group.comment = request.POST["comment"]
            group.save()
            site = "%s&dataSaved" % request.get_full_path()
            return redirect_to(request, url=site)

def render_showGroupTask(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "show_tasks_group": True, "list_tasks_group": True})
    c.update(csrf(request))

    # ===================================== GET =====================================

    if request.method == "GET":
        if "taskId" in request.GET:
            # get task info
            task = Task.objects.get(id = request.GET["taskId"])
            c.update({"task": task})
            
            # get task traits info
            traitsList = TaskTrait.objects.all().filter(task = request.GET["taskId"])
            c.update({"traitsList": traitsList})
            
            # get task groups info
            groupsList = Group.objects.all().filter(task = request.GET["taskId"]).order_by("-members_num")
            c.update({"groupsList": groupsList})
    
    return render_to_response("show_task_group.html", c)