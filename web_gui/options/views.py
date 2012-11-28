from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf

from options.forms import OptionsForm
from options.models import Option, SystemInfo, Feedback

def show_home(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    if systemInfo_list.count() == 0:
        info = SystemInfo()
        info.version = "0.00"
        info.status = "idle"
        info.error = "no error"
        info.save()
    else:
        info = systemInfo_list[0]
    
    c = RequestContext(request, {"version": info.version, "home": True})
    return render_to_response("home.html", c)

def show_options(request):
    # get options (only one object should exists)
    options_list = Option.objects.all()
    if options_list.count() == 0:
        options = Option()
        options.output_dest = "DatabaseOutput"
        options.log_level = 1
        options.log_file = "/var/www/jsejdak/analyzer_log"
        options.resemblence_level = 50
        options.save()
    else:
        options = options_list[0]
        
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    if systemInfo_list.count() == 0:
        info = SystemInfo()
        info.version = "0.00"
        info.status = "idle"
        info.error = "no error"
        info.save()
    else:
        info = systemInfo_list[0]
    
    # create objects
    form = OptionsForm(initial = {"output_dest": options.output_dest, "log_level": options.log_level, "log_file": options.log_file, "resemblence_level": options.resemblence_level})
    
    c = RequestContext(request, {"version": info.version, "is_message": False, "options": True})
    c.update(csrf(request))
    
    # save option in session if "Save" clicked
    if request.method == "POST":
        form = OptionsForm(request.POST)
        if form.is_valid():
            cd = form.cleaned_data
            options.output_dest = cd["output_dest"]
            options.log_level = cd["log_level"]
            options.log_file = cd["log_file"]
            options.resemblence_level = cd["resemblence_level"]
            options.save(force_update = True)
   
        c.update({"is_message": True})
    
    c.update({"form": form})
    return render_to_response("options.html", c)

def show_feedback(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    if systemInfo_list.count() == 0:
        info = SystemInfo()
        info.version = "0.00"
        info.status = "idle"
        info.error = "no error"
        info.save()
    else:
        info = systemInfo_list[0]
    
    c = RequestContext(request, {"version": info.version, "is_message": False, "feedback": True})
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