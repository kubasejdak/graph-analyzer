from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf

from options.forms import OptionsForm
from options.models import Option, SystemInfo, Feedback

def show_home(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    info = systemInfo_list[0]
    
    c = RequestContext(request, {"version": info.version, "home": True})
    return render_to_response("home.html", c)

def show_options(request):
    # get options (only one object should exists)
    options_list = Option.objects.all()
    if options_list.count() == 0:
        options = Option()
        options.log_level = 2
        options.log_file = "/var/www/jsejdak/analyzer_log"
        options.resemblence_level = 50
        options.emulation_steps = 1000000
        options.graphs_dir = "/var/www/jsejdak/graphs"
        options.big_files_protection = True
        options.max_input_file_size = 20971520
        options.skip_empty_samples = True
        options.skip_broken_samples = True
        options.broken_samples_size = 30
        options.save()
    else:
        options = options_list[0]
        
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    info = systemInfo_list[0]
    
    # create objects
    form = OptionsForm(initial = {"log_level": options.log_level,
								  "log_file": options.log_file,
								  "resemblence_level": options.resemblence_level,
								  "emulation_steps": options.emulation_steps,
        						  "graphs_dir": options.graphs_dir,
        			     		  "big_files_protection": options.big_files_protection,
        					 	  "max_input_file_size": options.max_input_file_size,
        				     	  "skip_empty_samples": options.skip_empty_samples,
        						  "skip_broken_samples": options.skip_broken_samples,
        			     		  "broken_samples_size": options.broken_samples_size})
    
    c = RequestContext(request, {"version": info.version, "is_message": False, "options": True})
    c.update(csrf(request))
    
    # save option in session if "Save" clicked
    if request.method == "POST":
        form = OptionsForm(request.POST)
        if form.is_valid():
            cd = form.cleaned_data
            options.log_level = cd["log_level"]
            options.log_file = cd["log_file"]
            options.resemblence_level = cd["resemblence_level"]
            options.emulation_steps = cd["emulation_steps"]
            options.graphs_dir = cd["graphs_dir"]
            options.big_files_protection = cd["big_files_protection"]
            options.max_input_file_size = cd["max_input_file_size"]
            options.skip_empty_samples = cd["skip_empty_samples"]
            options.skip_broken_samples = cd["skip_broken_samples"]
            options.broken_samples_size = cd["broken_samples_size"]
            options.save(force_update = True)
   
        c.update({"is_message": True})
    
    c.update({"form": form})
    return render_to_response("options.html", c)

def show_feedback(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
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