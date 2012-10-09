from django.shortcuts import render_to_response
from django.template import RequestContext

from options.models import SystemInfo

def show_browse(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    if systemInfo_list.count() == 0:
        info = SystemInfo()
        info.version = "0.00"
        info.status = "idle"
        info.error = "no_error"
        info.save()
    else:
        info = systemInfo_list[0]
    
    c = RequestContext(request, {"version": info.version})
    return render_to_response("browse.html", c)