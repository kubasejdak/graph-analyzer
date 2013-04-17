from django.contrib import admin

from models import Task

class TaskAdmin(admin.ModelAdmin):
	list_display = ("name", "type", "start_time", "end_time", "work_time", "errors", "finished", "progress")
	search_fields = ("name", "type")
	list_filter = ("type",)
	ordering = ("name", "type")

admin.site.register(Task, TaskAdmin)