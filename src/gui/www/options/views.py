from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf

from tools.CoreOptions import CoreOptions
from tools.SystemStatus import SystemStatus
from tools.LoggingOptions import LoggingOptions
from tools.DatabaseOptions import DatabaseOptions

def render_options(request):
    # get system status
    systemStatus = SystemStatus()
    systemStatus.get()
    
    c = RequestContext(request, {"version": systemStatus.version, "options": True})
    c.update(csrf(request))
    
    coreOptions = CoreOptions()
    loggingOptions = LoggingOptions()
    databaseOptions = DatabaseOptions()
    
    # ===================================== GET =====================================

    if(request.method == "GET"):
        # get core options
        coreOptions.get()
    
        # get logging options
        loggingOptions.get()
        print "level =", loggingOptions.level
        
        # get database options
        databaseOptions.get()
        
    # ===================================== POST =====================================
    
    # save options if "Save" clicked
    if(request.method == "POST"):
        # core options
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
        
        # logging options
        levelDictStr = {"OFF": 0, "Low": 1, "Normal": 2, "High": 3}
        levelStr = request.POST["logLevel"]
        loggingOptions.level = levelDictStr[levelStr]
        if("exportStatusXML" in request.POST):
            loggingOptions.exportStatusXML = True
        else:
            loggingOptions.exportStatusXML = False
        
        # database options
        databaseOptions.driver = request.POST["dbDriver"]
        databaseOptions.host = request.POST["dbHost"]
        databaseOptions.name = request.POST["dbName"]
        databaseOptions.user = request.POST["dbUser"]
        databaseOptions.password = request.POST["dbPassword"]
        
        coreOptions.save()
        loggingOptions.save()
        databaseOptions.save()
        c.update({"dataSaved": True})
    
    # update core options
    c.update({"graphsDirectory": coreOptions.graphs_dir,
			  "emulationSteps": coreOptions.emulation_steps,
              "skipBigFiles": coreOptions.skip_big_files,
              "skipBigFilesSize": coreOptions.skip_big_files_size,
              "skipBrokenSamples": coreOptions.skip_broken_samples,
              "skipBrokenSamplesSize": coreOptions.skip_broken_samples_size,
              "skipEmptySamples": coreOptions.skip_empty_samples,})

    # update logging options
    c.update({"logLevel": int(loggingOptions.level),
			  "exportStatusXML": loggingOptions.exportStatusXML})
    
    # update database options
    c.update({"dbDriver": databaseOptions.driver,
			  "dbHost": databaseOptions.host,
              "dbName": databaseOptions.name,
              "dbUser": databaseOptions.user,
              "dbPassword": databaseOptions.password})
    
    return render_to_response("options.html", c)