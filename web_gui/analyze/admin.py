from django.contrib import admin
from models import Sample, API, Hash, Loop, SampleGroup, GroupAssignment

class SampleAdmin(admin.ModelAdmin):
	list_display = ("name", "extracted_from", "graph_name")
	search_fields = ("name", "extracted_from", "graph_name")
	list_filter = ("extracted_from",)
	ordering = ("-extracted_from",)
	
class APIAdmin(admin.ModelAdmin):
	list_display = ("sample", "dll", "syscall")
	search_fields = ("sample", "dll", "syscall")
	list_filter = ("sample", "dll", "syscall")
	ordering = ("-sample", "-dll")
	
class HashAdmin(admin.ModelAdmin):
	list_display = ("sample", "hash")
	search_fields = ("sample", "hash")
	list_filter = ("sample", "hash")
	ordering = ("-sample", "-hash")

class LoopAdmin(admin.ModelAdmin):
	list_display = ("sample", "hash", "iterations", "size", "start")
	search_fields = ("sample", "hash", "iterations", "size", "start")
	list_filter = ("sample", "hash")
	ordering = ("-sample", "-hash")

class SampleGroupAdmin(admin.ModelAdmin):
	list_display = ("leader", "members_num", "active", "comment")
	search_fields = ("leader", "active", "comment", "members_num")
	list_filter = ("leader", "active", "members_num")
	ordering = ("-active", "-members_num", "-leader")

class GroupAssignmentAdmin(admin.ModelAdmin):
	list_display = ("group", "member", "resemblence_rate")
	search_fields = ("group", "member", "resemblence_rate")
	list_filter = ("group", "member", "resemblence_rate")
	ordering = ("-resemblence_rate", "-group")

admin.site.register(Sample, SampleAdmin)
admin.site.register(API, APIAdmin)
admin.site.register(Hash, HashAdmin)
admin.site.register(Loop, LoopAdmin)
admin.site.register(SampleGroup, SampleGroupAdmin)
admin.site.register(GroupAssignment, GroupAssignmentAdmin)