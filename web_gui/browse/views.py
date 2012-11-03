from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf

from options.models import SystemInfo
from analyze.models import Sample

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
        if "show" in request.POST:
            # redirect to show sample page
            show_sample = Sample.objects.get(id = request.POST["show"])
            c.update({"show_sample": show_sample})
            return render_to_response("show.html", c)