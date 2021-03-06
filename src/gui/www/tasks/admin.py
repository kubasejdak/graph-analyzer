from django.contrib import admin

from models import Task, TaskTrait

class TaskAdmin(admin.ModelAdmin):
	list_display = ("name", "type", "start_time", "end_time", "work_time", "errors", "finished", "trait_name", "trait_value", "progress")
	search_fields = ("name", "type")
	list_filter = ("type",)
	ordering = ("name", "type")

class TaskTraitAdmin(admin.ModelAdmin):
	list_display = ("name", "value", "task")
	search_fields = ("name", "value", "task")
	list_filter = ("name",)
	ordering = ("name", "value")

admin.site.register(Task, TaskAdmin)
admin.site.register(TaskTrait, TaskTraitAdmin)