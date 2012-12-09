from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from web_gui.settings import STATICFILES_DIRS
import shutil
import os.path
from stat import S_IRWXG, S_IRWXO, S_IRWXU

from options.models import SystemInfo
from analyze.models import Sample, SampleGroup, GroupAssignment, LoopAssignment, Loop, APIAssignment, HashAssignment

def show_browse(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    info = systemInfo_list[0]
    
    c = RequestContext(request, {"version": info.version, "browse": True})
    c.update(csrf(request))

    # ===================================== GET =====================================

    if request.method == "GET":
        if "search_sample" in request.GET:
            filter_value = request.GET["filter"]
            if filter_value == "Id":
                search_list = Sample.objects.filter(id = request.GET["value"]).order_by("name")
            elif filter_value == "Name":
                search_list = Sample.objects.filter(name = request.GET["value"]).order_by("name")
            elif filter_value == "Extracted from":
                search_list = Sample.objects.filter(extracted_from = request.GET["value"]).order_by("name")
            elif filter_value == "Graph name":
                search_list = Sample.objects.filter(graph_name = request.GET["value"]).order_by("name")
            elif filter_value == "Size":
                search_list = Sample.objects.filter(size = request.GET["value"]).order_by("name")
            elif filter_value == "File type":
                search_list = Sample.objects.filter(file_type = request.GET["value"]).order_by("name")
            elif filter_value == "File size":
                search_list = Sample.objects.filter(file_size = request.GET["value"]).order_by("name")
            
            c.update({"sample_list": search_list})
            return render_to_response("browse.html", c)
        elif "sort_sample" in request.GET:
            sort_value = request.GET["filter"]
            if sort_value == "Id":
                sort_list = Sample.objects.all().order_by("id")
            elif sort_value == "Name":
                sort_list = Sample.objects.all().order_by("name")
            elif sort_value == "Extracted from":
                sort_list = Sample.objects.all().order_by("extracted_from")
            elif sort_value == "Graph name":
                sort_list = Sample.objects.all().order_by("graph_name")
            elif sort_value == "Size":
                sort_list = Sample.objects.all().order_by("size")
            elif sort_value == "File type":
                sort_list = Sample.objects.all().order_by("file_type")
            elif sort_value == "File size":
                sort_list = Sample.objects.all().order_by("file_size")
            
            c.update({"sample_list": sort_list})
            return render_to_response("browse.html", c)
        else:
            # get all available samples
            sample_list = Sample.objects.all().order_by("name")
            c.update({"sample_list": sample_list})
        return render_to_response("browse.html", c)

    # ===================================== POST =====================================

    if request.method == "POST":
        if "save_loop" in request.POST:
            # save comment
            c.update({"is_loopmessage": True})
            tmp_loopassignment = LoopAssignment.objects.get(id = request.POST["save_loop"])
            new_loop = Loop.objects.get(id = tmp_loopassignment.loop.id)
            new_loop.comment = request.POST["loop_comments"]
            new_loop.save()
            
            # get sample info and reload page
            show_sample = Sample.objects.get(id = tmp_loopassignment.sample.id)
            show_loopassignment = LoopAssignment.objects.filter(sample = tmp_loopassignment.sample.id)
            show_apiassignment = APIAssignment.objects.filter(sample = tmp_loopassignment.sample.id)
            show_hashassignment = HashAssignment.objects.get(sample = tmp_loopassignment.sample.id)
            show_groupassignment = GroupAssignment.objects.filter(member = tmp_loopassignment.sample.id).order_by("-resemblence_rate")

            # get graph picture
            graph_source = show_sample.graph_name
            graph_file = graph_source.rpartition("/")[2]
            graph_dir = STATICFILES_DIRS[0] + "tmp_graphs/"
            if not os.path.exists(graph_dir + graph_file):
                shutil.copy(graph_source, STATICFILES_DIRS[0] + "tmp_graphs")
                os.chmod(graph_dir + graph_file, S_IRWXU | S_IRWXG | S_IRWXO)

            c.update({"show_sample": show_sample,
					  "show_loopassignment": show_loopassignment,
					  "show_apiassignment": show_apiassignment,
					  "show_hashassignment": show_hashassignment,
					  "show_groupassignment": show_groupassignment,
					  "graph_file": graph_file,
					  "loops_tab": "active"})
            return render_to_response("show.html", c)

        if "comments" in request.POST:
            c.update({"is_message": True})
            new_sample = Sample.objects.get(id = request.POST["show_sample"])
            new_sample.comment = request.POST["comments"]
            new_sample.save()

        if "show_sample" in request.POST:
            # get sample info
            show_sample = Sample.objects.get(id = request.POST["show_sample"])
            show_loopassignment = LoopAssignment.objects.filter(sample = request.POST["show_sample"])
            show_apiassignment = APIAssignment.objects.filter(sample = request.POST["show_sample"])
            show_hashassignment = HashAssignment.objects.get(sample = request.POST["show_sample"])
            show_groupassignment = GroupAssignment.objects.filter(member = request.POST["show_sample"]).order_by("-resemblence_rate")
            show_group = SampleGroup.objects.get(leader = request.POST["show_sample"])
            
            # get graph picture
            graph_source = show_sample.graph_name
            graph_file = graph_source.rpartition("/")[2]
            graph_dir = STATICFILES_DIRS[0] + "tmp_graphs/"
            if not os.path.exists(graph_dir + graph_file):
                shutil.copy(graph_source, STATICFILES_DIRS[0] + "tmp_graphs")
                os.chmod(graph_dir + graph_file, S_IRWXU | S_IRWXG | S_IRWXO)

            c.update({"show_sample": show_sample,
					  "show_loopassignment": show_loopassignment,
					  "show_apiassignment": show_apiassignment,
					  "show_hashassignment": show_hashassignment,
					  "show_groupassignment": show_groupassignment,
					  "graph_file": graph_file,
					  "general_tab": "active",
					  "show_group": show_group})
            return render_to_response("show.html", c)
        
        if "show_group" in request.POST:
            # get sample info
            show_group = SampleGroup.objects.get(id = request.POST["show_group"])
            show_groupassignment = GroupAssignment.objects.filter(group_id = request.POST["show_group"]).order_by("-resemblence_rate")
            c.update({"show_group": show_group,
					  "show_groupassignment": show_groupassignment,
					  "browse_groups": True,
					  "browse": False})
            return render_to_response("show_group.html", c)
  
def show_browse_groups(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    info = systemInfo_list[0]
    
    c = RequestContext(request, {"version": info.version, "browse_groups": True})
    c.update(csrf(request))

    # ===================================== GET =====================================

    if request.method == "GET":
        # get all available samples
        group_list = SampleGroup.objects.filter(active = True).order_by("-members_num")
        c.update({"group_list": group_list})
        return render_to_response("browse_groups.html", c)

    # ===================================== POST =====================================

    if request.method == "POST":
        if "save_loop" in request.POST:
            # save comment
            c.update({"is_loopmessage": True, "graph_tab": "active"})
            tmp_loopassignment = LoopAssignment.objects.get(id = request.POST["save_loop"])
            new_loop = Loop.objects.get(id = tmp_loopassignment.loop.id)
            new_loop.comment = request.POST["loop_comments"]
            new_loop.save()
            
            # get sample info and reload page
            show_sample = Sample.objects.get(id = tmp_loopassignment.sample.id)
            show_loopassignment = LoopAssignment.objects.filter(sample = tmp_loopassignment.sample.id)
            show_apiassignment = APIAssignment.objects.filter(sample = tmp_loopassignment.sample.id)
            show_hashassignment = HashAssignment.objects.get(sample = tmp_loopassignment.sample.id)
            show_groupassignment = GroupAssignment.objects.filter(member = tmp_loopassignment.sample.id).order_by("-resemblence_rate")

            # get graph picture
            graph_source = show_sample.graph_name
            graph_file = graph_source.rpartition("/")[2]
            graph_dir = STATICFILES_DIRS[0] + "tmp_graphs/"
            if not os.path.exists(graph_dir + graph_file):
                shutil.copy(graph_source, STATICFILES_DIRS[0] + "tmp_graphs")
                os.chmod(graph_dir + graph_file, S_IRWXU | S_IRWXG | S_IRWXO)

            c.update({"show_sample": show_sample,
					  "show_loopassignment": show_loopassignment,
					  "show_apiassignment": show_apiassignment,
					  "show_hashassignment": show_hashassignment,
					  "show_groupassignment": show_groupassignment,
					  "graph_file": graph_file,
					  "loops_tab": "active"})
            return render_to_response("show.html", c)

        if "comments" in request.POST:
            c.update({"is_message": True})
            new_group = SampleGroup.objects.get(id = request.POST["show_group"])
            new_group.comment = request.POST["comments"]
            new_group.save()

        if "show_group" in request.POST:
            # get sample info
            show_group = SampleGroup.objects.get(id = request.POST["show_group"])
            show_groupassignment = GroupAssignment.objects.filter(group_id = request.POST["show_group"]).order_by("-resemblence_rate")
            c.update({"show_group": show_group,
					  "show_groupassignment": show_groupassignment})
            return render_to_response("show_group.html", c)
           
        if "show_sample" in request.POST:
            # get sample info
            show_sample = Sample.objects.get(id = request.POST["show_sample"])
            show_loopassignment = LoopAssignment.objects.filter(sample = request.POST["show_sample"])
            show_apiassignment = APIAssignment.objects.filter(sample = request.POST["show_sample"])
            show_hashassignment = HashAssignment.objects.get(sample = request.POST["show_sample"])
            show_groupassignment = GroupAssignment.objects.filter(member = request.POST["show_sample"]).order_by("-resemblence_rate")
            
            # get graph picture
            graph_source = show_sample.graph_name
            graph_file = graph_source.rpartition("/")[2]
            graph_dir = STATICFILES_DIRS[0] + "tmp_graphs/"
            if not os.path.exists(graph_dir + graph_file):
                shutil.copy(graph_source, STATICFILES_DIRS[0] + "tmp_graphs")
                os.chmod(graph_dir + graph_file, S_IRWXU | S_IRWXG | S_IRWXO)

            c.update({"show_sample": show_sample,
					  "show_loopassignment": show_loopassignment,
					  "show_apiassignment": show_apiassignment,
					  "show_hashassignment": show_hashassignment,
					  "show_groupassignment": show_groupassignment,
					  "graph_file": graph_file,
					  "general_tab": "active",
					  "browse_groups": False,
					  "browse": True})
            return render_to_response("show.html", c)