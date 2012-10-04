from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf

from options.models import Option
from options.forms import OptionsForm

def show_home(request):
    c = RequestContext(request, {"version": "unknown"})
    return render_to_response("home.html", c)

def show_options(request):
    # get options (only one object should exists)
    options_list = Option.objects.all()
    if options_list.count() == 0:
        options = Option()
        options.output_dest = "DatabaseOutput"
        options.log_level = 1
        options.log_file = "/home/kuba/nalyzer_log"
        options.save()
    else:
        options = options_list[0]
    
    # create objects
    form = OptionsForm(initial = {"output_dest": options.output_dest, "log_level": options.log_level, "log_file": options.log_file})
    
    m = "Enter options"
    c = RequestContext(request, {"version": "unknown", "is_message": False})
    c.update(csrf(request))
    
    # save option in session if "Save" clicked
    if request.method == "POST":
        form = OptionsForm(request.POST)
        if form.is_valid():
            cd = form.cleaned_data
            options.output_dest = cd["output_dest"]
            options.log_level = cd["log_level"]
            options.log_file = cd["log_file"]
            options.save(force_update = True)
            m = "Options saved"
        else:
            m = "ERROR: option fields are not valid"    
        c.update({"is_message": True, "message": m})
    
    c.update({"form": form})
            
    return render_to_response("options.html", c)