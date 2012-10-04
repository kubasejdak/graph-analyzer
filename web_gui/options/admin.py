from django.contrib import admin
from models import Option, PendingFile

class OptionAdmin(admin.ModelAdmin):
    list_display = ("output_dest", "log_level", "log_file")

class PendingFileAdmin(admin.ModelAdmin):
    list_display = ("name",)

admin.site.register(Option, OptionAdmin)
admin.site.register(PendingFile, PendingFileAdmin)