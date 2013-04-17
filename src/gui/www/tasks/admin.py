from django.contrib import admin

from models import Task

class TaskAdmin(admin.ModelAdmin):
	list_display = ("name", "type", "start_time", "end_time", "work_time", "errors", "finished", "trait_name", "trait_value", "progress")
	search_fields = ("name", "type")
	list_filter = ("type",)
	ordering = ("name", "type")

admin.site.register(Task, TaskAdmin)