from django.contrib import admin
from models import Sample, API, APIAssignment, Hash, HashAssignment, Loop, LoopAssignment, Group, GroupAssignment

class SampleAdmin(admin.ModelAdmin):
	list_display = ("name", "extracted_from", "graph_name")
	search_fields = ("name", "extracted_from", "graph_name")
	list_filter = ("extracted_from",)
	ordering = ("extracted_from",)
	
class APIAdmin(admin.ModelAdmin):
	list_display = ("dll", "syscall")
	search_fields = ("dll", "syscall")
	list_filter = ("dll", "syscall")
	ordering = ("syscall",)

class APIAssignmentAdmin(admin.ModelAdmin):
	list_display = ("sample", "api")
	search_fields = ("sample", "api")
	list_filter = ("sample", "api")
	ordering = ("sample",)
	
class HashAdmin(admin.ModelAdmin):
	list_display = ("hash",)
	search_fields = ("hash",)
	list_filter = ("hash",)
	ordering = ("hash",)

class HashAssignmentAdmin(admin.ModelAdmin):
	list_display = ("sample", "hash")
	search_fields = ("sample", "hash")
	list_filter = ("sample", "hash")
	ordering = ("sample",)

class LoopAdmin(admin.ModelAdmin):
	list_display = ("hash", "iterations", "size", "start")
	search_fields = ("hash", "iterations", "size", "start")
	list_filter = ("hash",)
	ordering = ("hash",)

class LoopAssignmentAdmin(admin.ModelAdmin):
	list_display = ("sample", "loop")
	search_fields = ("sample", "loop")
	list_filter = ("sample", "loop")
	ordering = ("sample",)

class GroupAdmin(admin.ModelAdmin):
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
admin.site.register(APIAssignment, APIAssignmentAdmin)
admin.site.register(Hash, HashAdmin)
admin.site.register(HashAssignment, HashAssignmentAdmin)
admin.site.register(Loop, LoopAdmin)
admin.site.register(LoopAssignment, LoopAssignmentAdmin)
admin.site.register(Group, GroupAdmin)
admin.site.register(GroupAssignment, GroupAssignmentAdmin)