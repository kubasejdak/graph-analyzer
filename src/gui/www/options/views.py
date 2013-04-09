from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf

from options.forms import OptionsForm
from tools.CoreOptions import CoreOptions
from tools.SystemStatus import SystemStatus
from tools.LoggingOptions import LoggingOptions

def render_options(request):    
    # ===================================== GET =====================================

    # get core options
    coreOptions = CoreOptions()
    coreOptions.get()
    
    # get logging options
    # TODO: read all options from XML
    loggingOptions = LoggingOptions()
    loggingOptions.get()
        
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    # create objects
    form = OptionsForm(initial = {"log_level": loggingOptions.level,
                                  "log_file": "<NOT IMPLEMENTED>",
                                  "resemblence_level": 0,
                                  "emulation_steps": coreOptions.emulation_steps,
                                  "graphs_dir": coreOptions.graphs_dir,
                                   "big_files_protection": coreOptions.skip_big_files,
                                   "max_input_file_size": coreOptions.skip_big_files_size,
                                   "skip_empty_samples": coreOptions.skip_empty_samples,
                                  "skip_broken_samples": coreOptions.skip_broken_samples,
                                   "broken_samples_size": coreOptions.skip_broken_samples_size})
    
    c = RequestContext(request, {"version": systemStatus.version, "is_message": False, "options": True})
    c.update(csrf(request))
    
    # ===================================== POST =====================================
    
    # save options if "Save" clicked
    if request.method == "POST":
        form = OptionsForm(request.POST)

        if form.is_valid():
            cd = form.cleaned_data
            coreOptions.graphs_dir = cd["graphs_dir"]
            coreOptions.emulation_steps = cd["emulation_steps"]
            coreOptions.skip_big_files = cd["big_files_protection"]
            coreOptions.skip_big_files_size = cd["max_input_file_size"]
            coreOptions.skip_broken_samples = cd["skip_broken_samples"]
            coreOptions.skip_broken_samples_size = cd["broken_samples_size"]
            coreOptions.skip_empty_samples = cd["skip_empty_samples"]

            loggingOptions.level = cd["log_level"]
            
            coreOptions.save()
            loggingOptions.save()
            c.update({"is_message": True})
    
    c.update({"form": form})
    return render_to_response("options.html", c)