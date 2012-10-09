from django.contrib import admin
from options.models import Option, PendingFile, SystemInfo

class SystemInfoAdmin(admin.ModelAdmin):
    list_display = ("version", "status", "error")

class OptionAdmin(admin.ModelAdmin):
    list_display = ("output_dest", "log_level", "log_file")

class PendingFileAdmin(admin.ModelAdmin):
    list_display = ("name", "progress")

admin.site.register(SystemInfo, SystemInfoAdmin)
admin.site.register(Option, OptionAdmin)
admin.site.register(PendingFile, PendingFileAdmin)