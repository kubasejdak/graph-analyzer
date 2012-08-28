from django.contrib import admin
from models import Sample, API, Hash, Loop

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

admin.site.register(Sample, SampleAdmin)
admin.site.register(API, APIAdmin)
admin.site.register(Hash, HashAdmin)
admin.site.register(Loop, LoopAdmin)