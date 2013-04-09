from django.contrib import admin
from options.models import PendingFile

class PendingFileAdmin(admin.ModelAdmin):
    list_display = ("name",)

admin.site.register(PendingFile, PendingFileAdmin)