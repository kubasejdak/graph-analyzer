from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from web_gui.settings import STATICFILES_DIRS
import shutil
import os.path
from stat import S_IRWXG, S_IRWXO, S_IRWXU

from options.models import SystemInfo
from analyze.models import Sample, Loop, API, Hash

def show_browse(request):
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
    
    c = RequestContext(request, {"version": info.version, "browse": True})
    c.update(csrf(request))
    if request.method == "GET":
        # get all available samples
        sample_list = Sample.objects.all()
        c.update({"sample_list": sample_list})
        return render_to_response("browse.html", c)
    if request.method == "POST":
        print request.POST
        if "comments" in request.POST:
            is_message = True
            c.update({"is_message": is_message})
            new_sample = Sample.objects.get(id = request.POST["show"])
            new_sample.comment = request.POST["comments"]
            new_sample.save()

        if "show" in request.POST:
            # get sample info
            show_sample = Sample.objects.get(id = request.POST["show"])
            show_loops = Loop.objects.filter(sample = request.POST["show"])
            show_api = API.objects.filter(sample = request.POST["show"])
            show_hash = Hash.objects.get(sample = request.POST["show"])
            c.update({"show_sample": show_sample, "show_loops": show_loops, "show_api": show_api, "show_hash": show_hash})
            
            # get graph picture
            graph_source = show_sample.graph_name
            graph_file = graph_source.rpartition("/")[2]
            graph_dir = STATICFILES_DIRS[0] + "tmp_graphs/"
            if not os.path.exists(graph_dir + graph_file):
                shutil.copy(graph_source, STATICFILES_DIRS[0] + "tmp_graphs")
                os.chmod(graph_dir + graph_file, S_IRWXU | S_IRWXG | S_IRWXO)

            c.update({"graph_file": graph_file})
            c.update(csrf(request))
            return render_to_response("show.html", c)