from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf

from tools.CoreOptions import CoreOptions
from tools.SystemStatus import SystemStatus
from tools.LoggingOptions import LoggingOptions

def render_options(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "options": True})
    c.update(csrf(request))
    
    coreOptions = CoreOptions()
    loggingOptions = LoggingOptions()
    
    # ===================================== GET =====================================

    if(request.method == "GET"):
        # get core options
        coreOptions.get()
    
        # get logging options
        # TODO: read all options from XML
        loggingOptions.get()
        
    # ===================================== POST =====================================
    
    # save options if "Save" clicked
    if(request.method == "POST"):
        print request.POST
        coreOptions.graphs_dir = request.POST["graphsDirectory"]
        coreOptions.emulation_steps = request.POST["emulationSteps"]
        if("skipBigFiles" in request.POST):
            coreOptions.skip_big_files = True
        else:
            coreOptions.skip_big_files = False
        coreOptions.skip_big_files_size = request.POST["skipBigFilesSize"]
        if("skipBrokenSamples" in request.POST):
            coreOptions.skip_broken_samples = True
        else:
            coreOptions.skip_broken_samples = False
        coreOptions.skip_broken_samples_size = request.POST["skipBrokenSamplesSize"]
        if("skipEmptySamples" in request.POST):
            coreOptions.skip_empty_samples = True
        else:
            coreOptions.skip_empty_samples = False
        
        print "POST"
        coreOptions.save()
        loggingOptions.save()
        c.update({"dataSaved": True})
    
    print "GETTING"
    c.update({"graphsDirectory": coreOptions.graphs_dir,
			  "emulationSteps": coreOptions.emulation_steps,
              "skipBigFiles": coreOptions.skip_big_files,
              "skipBigFilesSize": coreOptions.skip_big_files_size,
              "skipBrokenSamples": coreOptions.skip_broken_samples,
              "skipBrokenSamplesSize": coreOptions.skip_broken_samples_size,
              "skipEmptySamples": coreOptions.skip_empty_samples,})

    return render_to_response("options.html", c)