from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf

from tools.SystemStatus import SystemStatus
from models import Feedback

def render_feedback(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "is_message": False, "feedback": True})
    c.update(csrf(request))
    
    # save bug/feature in session if "Save" clicked
    if request.method == "POST":
        if "add" in request.POST:
            issue = Feedback()
            issue.type = request.POST["type"]
            issue.description = request.POST["description"]
            issue.status = "pending"
            issue.save()
            c.update({"is_message": True})
    
    # get all bugs and features
    bug_list = Feedback.objects.filter(type = "bug").order_by("-status")
    feature_list = Feedback.objects.filter(type = "feature").order_by("-status")
    
    c.update({"bug_list": bug_list, "feature_list": feature_list})
    return render_to_response("feedback.html", c)