from django.shortcuts import render_to_response
from django.template import RequestContext
from django.core.context_processors import csrf
from www.settings import STATICFILES_DIRS
import shutil
import os.path
from stat import S_IRWXG, S_IRWXO, S_IRWXU

from options.models import SystemInfo
from analyze.models import Sample, Group, GroupAssignment, LoopAssignment, Loop, APIAssignment, HashAssignment
from django.views.generic.simple import redirect_to

def render_browseSamples(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    info = systemInfo_list[0]
    
    c = RequestContext(request, {"version": info.version, "browse": True})
    c.update(csrf(request))

    # ===================================== GET =====================================

    if request.method == "GET":
        sample_list = Sample.objects.all().order_by("name")
        c.update({"sample_list": sample_list})
        return render_to_response("browse.html", c)

    # ===================================== POST =====================================

    if request.method == "POST":
        if "showSample" in request.POST:
            site = "/jsejdak/sample/?sampleId=%s" % request.POST["showSample"]
            return redirect_to(request, url=site)

        if "search" in request.POST:
            filter_value = request.POST["filter"]
            if filter_value == "Id":
                search_list = Sample.objects.filter(id = request.POST["value"]).order_by("name")
            elif filter_value == "Name":
                search_list = Sample.objects.filter(name = request.POST["value"]).order_by("name")
            elif filter_value == "Extracted from":
                search_list = Sample.objects.filter(extracted_from = request.POST["value"]).order_by("name")
            elif filter_value == "Graph name":
                search_list = Sample.objects.filter(graph_name = request.POST["value"]).order_by("name")
            elif filter_value == "Size":
                search_list = Sample.objects.filter(size = request.POST["value"]).order_by("name")
            elif filter_value == "File type":
                search_list = Sample.objects.filter(file_type = request.POST["value"]).order_by("name")
            elif filter_value == "File size":
                search_list = Sample.objects.filter(file_size = request.POST["value"]).order_by("name")

            c.update({"sample_list": search_list})
            return render_to_response("browse.html", c)

        if "sort" in request.POST:
            sort_value = request.POST["filter"]
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
  
def render_browseGroups(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    info = systemInfo_list[0]
    
    c = RequestContext(request, {"version": info.version, "browse_groups": True})
    c.update(csrf(request))

    # ===================================== GET =====================================

    if request.method == "GET":
        # get all available samples
        group_list = Group.objects.filter(active = True).order_by("-members_num")
        c.update({"group_list": group_list})
        return render_to_response("browse_groups.html", c)

    # ===================================== POST =====================================

    if request.method == "POST":
        if "showGroup" in request.POST:
            site = "/jsejdak/group/?groupId=%s" % request.POST["showGroup"]
            return redirect_to(request, url=site)

def render_showSample(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    info = systemInfo_list[0]

    c = RequestContext(request, {"version": info.version, "browse": True})
    c.update(csrf(request))

    # ===================================== GET =====================================

    if request.method == "GET":
        if "dataSaved" in request.GET:
            c.update({"dataSaved": True})

        if "sampleId" in request.GET:
            # get sample info
            show_sample = Sample.objects.get(id = request.GET["sampleId"])
            show_loopassignment = LoopAssignment.objects.filter(sample = request.GET["sampleId"])
            show_apiassignment = APIAssignment.objects.filter(sample = request.GET["sampleId"])
            show_hashassignment = HashAssignment.objects.get(sample = request.GET["sampleId"])
            show_groupassignment = GroupAssignment.objects.filter(member = request.GET["sampleId"]).order_by("-resemblence_rate")
            show_group = Group.objects.get(leader = request.GET["sampleId"])
            
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
					  "show_group": show_group,
					  "browse": True})
            return render_to_response("show.html", c)

    # ===================================== POST =====================================

    if request.method == "POST":
        if "compare" in request.POST:
            sample1_id = request.POST["compare"]
            sample2_id = request.POST["compareWith"]
            site = "/jsejdak/compare/?sample1Id=%s&sample2Id=%s" % (sample1_id, sample2_id)
            return redirect_to(request, url=site)

        if "saveLoop" in request.POST:
            loop = Loop.objects.get(id = request.POST["saveLoop"])
            loop.comment = request.POST["comment"]
            loop.save()
            site = "%s&dataSaved" % request.get_full_path()
            return redirect_to(request, url=site)
        
        if "saveSample" in request.POST:
            sample = Sample.objects.get(id = request.POST["saveSample"])
            sample.comment = request.POST["comment"]
            sample.save()
            site = "%s&dataSaved" % request.get_full_path()
            return redirect_to(request, url=site)

        if "showGroup" in request.POST:
            site = "/jsejdak/group/?groupId=%s" % request.POST["showGroup"]
            return redirect_to(request, url=site)

def render_showGroup(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    info = systemInfo_list[0]

    c = RequestContext(request, {"version": info.version, "browse_groups": True})
    c.update(csrf(request))

    # ===================================== GET =====================================

    if request.method == "GET":
        if "dataSaved" in request.GET:
            c.update({"dataSaved": True})

        if "groupId" in request.GET:
            show_group = Group.objects.get(id = request.GET["groupId"])
            show_groupassignment = GroupAssignment.objects.filter(group_id = request.GET["groupId"]).order_by("-resemblence_rate")
            c.update({"show_group": show_group,
					  "show_groupassignment": show_groupassignment})
            return render_to_response("show_group.html", c)

    # ===================================== POST =====================================

    if request.method == "POST":
        if "saveGroup" in request.POST:
            group = Group.objects.get(id = request.POST["saveGroup"])
            group.comment = request.POST["comment"]
            group.save()
            site = "%s&dataSaved" % request.get_full_path()
            return redirect_to(request, url=site)

        if "showSample" in request.POST:
            site = "/jsejdak/sample/?sampleId=%s" % request.POST["showSample"]
            return redirect_to(request, url=site)

def render_compareSamples(request):
    # get system info (only one object should exists)
    systemInfo_list = SystemInfo.objects.all()
    info = systemInfo_list[0]
    
    c = RequestContext(request, {"version": info.version, "browse": True})
    c.update(csrf(request))
    
    # ===================================== GET =====================================
    
    if request.method == "GET":
        # get samples
        sample1_id = request.GET["sample1Id"]
        sample2_id = request.GET["sample2Id"]
        sample1 = Sample.objects.get(id = sample1_id)
        sample2 = Sample.objects.get(id = sample2_id)
        
        # get assignments
        loops1_assignments = LoopAssignment.objects.filter(sample = sample1_id)
        loops2_assignments = LoopAssignment.objects.filter(sample = sample2_id)
        
        # get id for each assignment
        loops1_id = []
        loops2_id = []
        
        for assignment in loops1_assignments:
            loops1_id.append(assignment.loop)
   
        for assignment in loops2_assignments:
            loops2_id.append(assignment.loop)
        
        # get shared and unique loops
        shared_loops = []
        unique1_loops = []
        unique2_loops = []
        
        for l1 in loops1_id:
            if(l1 in loops2_id):
                shared_loops.append(l1)
            else:
                unique1_loops.append(l1)
        
        for l2 in loops2_id:
            if not(l2 in shared_loops):
                unique2_loops.append(l2)
        
        c.update({"sample1": sample1,
				  "sample2": sample2,
				  "shared_loops": shared_loops,
				  "unique1_loops": unique1_loops,
				  "unique2_loops": unique2_loops,
				  "loops1_assignments": loops1_assignments,
				  "loops2_assignments": loops2_assignments})
        
        if "dataSaved" in request.GET:
            c.update({"dataSaved": True})

        return render_to_response("compare.html", c)
   
    # ===================================== GET =====================================
   
    if request.method == "POST":
        if "saveLoop" in request.POST:
            loop_id = request.POST["saveLoop"]
            loop = Loop.objects.get(id = loop_id)
            loop.comment = request.POST["comment"]
            loop.save()
            
            site = "%s&dataSaved" % request.get_full_path()
            return redirect_to(request, url=site)
